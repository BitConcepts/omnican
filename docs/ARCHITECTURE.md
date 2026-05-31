# Architecture — OmniCAN

**Version**: 0.1.0 | **License**: Apache-2.0 | **Target**: Zephyr RTOS v3.7.0

## Overview
OmniCAN is a unified, multi-protocol CAN stack delivered as a Zephyr west
module. It provides a single shared node context and central frame router
that dispatches to independently selectable protocol modules. Every protocol
is guarded by a Kconfig symbol so unused modules compile to zero.

Protocol implementation is phased:
- **Phase 1** — CANopen CiA 301 (ported from BitConcepts/CANopenNode fork)
- **Phase 2** — SAE J1939 (address claiming, PGN routing, TP/ETP)
- **Phase 3** — ISO 14229 / UDS (over Zephyr ISO-TP + issue #86025 patch)
- **Phase 4** — SAE J1979 / OBD-II (over Zephyr ISO-TP)
- **Phase 5** — CANopen FD CiA 1301 (extends Phase 1)

## Components

### Core Node
- Provide `omnican_node_init()` to bind a Zephyr CAN device, set bitrate, and enable FD mode
- Return `OMNICAN_ERR_NODEV` from `omnican_node_init()` when the CAN device is not ready
- Define `omnican_proto_t` bitmask with one bit per protocol (CANOPEN, J1939, UDS, OBD2)
- Define `omnican_err_t` with codes OK, INVAL, NODEV, NOMEM, TIMEOUT, BUSY, PROTO, UNSUPPORTED, IO
- Register as a Zephyr west module via `zephyr/module.yml` so `west build` adds include/ to the build path
- Be buildable against Zephyr v3.7.0 with no patches to Zephyr itself

### Frame Router
- Register exactly one CAN receive filter per enabled protocol
- Dispatch received frames to the correct protocol handler without blocking the CAN ISR
- Use a Zephyr system workqueue for all protocol callback dispatch
- Auto-enable via CONFIG_OMNICAN_FRAME_ROUTER when any protocol is active

### CANopen CiA 301
- Implement the NMT state machine with states Initialising, Pre-Operational, Operational, and Stopped per CiA 301
- Transmit a Boot-Up message (NMT state 0x00) on transition from Initialising to Pre-Operational
- Respond to NMT master commands (Start, Stop, Pre-Operational, Reset Node, Reset Communication) on CAN ID 0x000
- Implement an SDO server supporting expedited, segmented, and block transfer modes per CiA 301
- Return SDO abort code 0x05040000 on SDO timeout
- Support TPDO and RPDO with event-driven and synchronisation-triggered transmission modes
- Support compile-time PDO mappings via the Object Dictionary
- Produce Emergency messages (CAN ID 0x80 + node_id) with 16-bit error code and 8-byte error register per CiA 301
- Produce Heartbeat messages at a Kconfig-configurable interval (default 1000 ms) per CiA 301
- Support Heartbeat consumer monitoring of remote nodes with error event on timeout
- Represent the Object Dictionary as a static C array generated from an OD descriptor with no heap allocation
- Write recommended delay to next `omnican_canopen_process()` call in microseconds via `*next_us`
- Run the CANopen process loop in a dedicated Zephyr thread with configurable stack size and priority

### J1939
- Use 29-bit extended CAN identifiers and reject 11-bit standard ID frames
- Implement the address-claiming procedure per J1939/81 with 250 ms contention window and 64-bit NAME priority
- Prevent PGN transmission until address claiming succeeds
- Return OMNICAN_ERR_BUSY and set addr to NULL when address claiming fails
- Support a static PGN routing table populated via `omnican_j1939_register_pgn()` with callback and user-data
- Invoke PGN callbacks from the system workqueue with source address, data pointer, and length
- Implement J1939 Transport Protocol (TP) for 9-1785 byte messages using BAM and CMDT modes
- Implement Extended Transport Protocol (ETP) for messages larger than 1785 bytes
- Maintain TP/ETP session state in static context with configurable maximum simultaneous sessions

### UDS ISO 14229
- Use Zephyr ISO-TP for all UDS message transport with ISOTP patch when CONFIG_OMNICAN_ISOTP_PATCH=y
- Implement DiagnosticSessionControl (SID 0x10) and enforce service availability per active session
- Maintain an S3 timer (default 5000 ms) and revert to Default session on expiry
- Reset the S3 timer on TesterPresent (SID 0x3E)
- Support SID 0x10 DiagnosticSessionControl
- Support SID 0x11 ECUReset with hardReset, softReset, and keyOffOnReset sub-functions
- Support SID 0x22 ReadDataByIdentifier
- Support SID 0x27 SecurityAccess with application-supplied seed/key callback
- Support SID 0x28 CommunicationControl
- Support SID 0x2E WriteDataByIdentifier
- Support SID 0x31 RoutineControl with start, stop, and requestResults
- Support SID 0x34 RequestDownload
- Support SID 0x36 TransferData
- Support SID 0x37 RequestTransferExit
- Support SID 0x85 ControlDTCSetting
- Return NRC 0x11 (serviceNotSupported) for unregistered SIDs
- Never use a hardcoded SecurityAccess seed/key algorithm; always use an application callback
- Lock the session and return NRC 0x36 after exceeding CONFIG_OMNICAN_UDS_MAX_AUTH_ATTEMPTS failed SecurityAccess attempts

### OBD-II J1979
- Transmit PID requests on CAN ID 0x7DF and collect responses from 0x7E8-0x7EF within configurable timeout
- Support OBD-II service modes 0x01 through 0x09
- Provide callback-based non-blocking PID request/response model
- Support Mode 0x01 PIDs: 0x00 (supported), 0x01 (monitor status), 0x04 (engine load), 0x05 (coolant), 0x0C (RPM), 0x0D (speed), 0x11 (throttle)

### CANopen FD CiA 1301
- Extend CANopen module with CiA 1301 FD features; require CONFIG_OMNICAN_CANOPEN=y and CONFIG_CAN_FD_MODE=y
- Support CAN FD data payloads of up to 64 bytes for higher-throughput PDO transfers
- Maintain backward compatibility with classical 8-byte CANopen nodes when FD is not negotiated

### ISOTP Patch
- Implement a workaround for Zephyr issue #86025 using separate TX/RX socket contexts with a forwarding shim
- Default CONFIG_OMNICAN_ISOTP_PATCH to y when CONFIG_OMNICAN_UDS or CONFIG_OMNICAN_OBD2 is enabled

### Memory and Threading
- Never use dynamic heap allocation (k_malloc, malloc); all state statically allocated or via net_buf pools
- Handle CAN frame data via Zephyr net_buf from a pool sized by CONFIG_OMNICAN_NET_BUF_COUNT (default 16)
- All protocol context structures shall be opaque to the caller but caller-allocated
- All protocol handlers execute in a Zephyr workqueue context, not the CAN ISR

## Design Principles
1. **Zero-overhead when disabled** — every module is behind `IS_ENABLED()` guards; disabled protocols produce no code or data.
2. **Static allocation only** — no heap (`malloc`/`free`). All protocol contexts are caller-allocated. Frame buffers use Zephyr `net_buf` pools.
3. **Zephyr-native** — uses Zephyr CAN driver API, `net_buf`, logging, Kconfig, and the workqueue/thread model throughout. No POSIX shims.
4. **Single node context** — `struct omnican_node` is the root anchor; all protocol sub-contexts hold a pointer back to it.
5. **Apache-2.0** — all source files carry SPDX headers.

## Repository Structure
```
omnican/
  include/omnican/   Public C headers — one per module
  src/core/          Frame router (auto-enabled by any protocol)
  src/canopen/       CANopen CiA 301
  src/canopen_fd/    CANopen FD CiA 1301
  src/j1939/         SAE J1939
  src/uds/           ISO 14229 / UDS
  src/obd2/          SAE J1979 / OBD-II
  src/isotp_patch/   Zephyr ISOTP #86025 workaround
  CMakeLists.txt     Module root — dispatches to src/ subdirs via Kconfig
  Kconfig            All CONFIG_OMNICAN_* symbols
  west.yml           Manifest: Zephyr v3.7.0, optional CANopenNode ref
  zephyr/module.yml  West module descriptor
```

## Core Node Context

`struct omnican_node` (defined in `include/omnican/core.h`) is the shared
anchor for all protocol instances:

```c
struct omnican_node {
    const struct device *can_dev;  /* Zephyr CAN device              */
    uint32_t             bitrate;  /* Nominal bitrate in kbps         */
    bool                 fd_mode;  /* True when CAN FD active         */
    omnican_proto_t      protos;   /* Bitmask of initialised protocols */
};
```

**REQ** The system shall provide `omnican_node_init()` to bind a Zephyr CAN
device, set bitrate, and enable FD mode, returning `OMNICAN_ERR_NODEV` if
the device is not ready.

**REQ** The system shall define a bitmask type `omnican_proto_t` with one bit
per protocol (`CANOPEN`, `J1939`, `UDS`, `OBD2`) so the active protocol set
can be inspected at runtime.

**REQ** The system shall define `omnican_err_t` with the codes `OMNICAN_OK`,
`OMNICAN_ERR_INVAL`, `OMNICAN_ERR_NODEV`, `OMNICAN_ERR_NOMEM`,
`OMNICAN_ERR_TIMEOUT`, `OMNICAN_ERR_BUSY`, `OMNICAN_ERR_PROTO`,
`OMNICAN_ERR_UNSUPPORTED`, `OMNICAN_ERR_IO`.

## Frame Router (`src/core/`)

The frame router is enabled automatically when any protocol is active
(`CONFIG_OMNICAN_FRAME_ROUTER`, defaulting to `y`). It:

1. Registers a CAN receive callback (or message queue) with the Zephyr CAN driver.
2. On each received frame, inspects the CAN ID to determine the owning protocol.
3. Dispatches to the appropriate protocol handler.
4. Runs dispatch in a Zephyr system workqueue callback to keep the CAN ISR short.

**REQ** The frame router shall register exactly one CAN receive filter per
enabled protocol and dispatch received frames to the correct protocol handler
without blocking the CAN ISR.

**REQ** The frame router shall use a Zephyr system workqueue for all protocol
callback dispatch so that protocol handlers may use kernel primitives (mutexes,
k_sleep, etc.).

## Phase 1 — CANopen (CiA 301) (`src/canopen/`)

### Source
Port and rearchitecture of the `BitConcepts/CANopenNode` fork, incorporating
all fixes from nine iterative audit rounds. The implementation is Zephyr-native
and does not depend on upstream CANopenNode runtime conventions.

### NMT State Machine
State transitions: `Initialising → Pre-Operational → Operational → Stopped`.

**REQ** The CANopen module shall implement the NMT state machine with states
Initialising, Pre-Operational, Operational, and Stopped per CiA 301.

**REQ** The CANopen module shall transmit a Boot-Up message (NMT state 0x00)
on transition from Initialising to Pre-Operational.

**REQ** The CANopen module shall respond to NMT master commands (Start, Stop,
Enter Pre-Operational, Reset Node, Reset Communication) received on CAN ID 0x000.

### SDO Server
**REQ** The CANopen module shall implement an SDO server supporting expedited,
segmented, and block transfer modes per CiA 301.

**REQ** The SDO server shall return an SDO abort (0x05040000 — SDO timeout)
if no response is received within the configured timeout.

### PDO
**REQ** The CANopen module shall support Transmit PDO (TPDO) and Receive PDO
(RPDO) with event-driven and synchronisation-triggered transmission modes.

**REQ** PDO mappings shall be configurable at compile time via the Object
Dictionary.

### Emergency
**REQ** The CANopen module shall produce Emergency messages (CAN ID
`0x80 + node_id`) on internal error events with a 16-bit error code and
8-byte error register per CiA 301.

### Heartbeat
**REQ** The CANopen module shall produce Heartbeat messages at a
Kconfig-configurable interval (default 1000 ms) per CiA 301.

**REQ** The CANopen module shall support Heartbeat consumer monitoring of
configured remote nodes and raise an error event on timeout.

### Object Dictionary
**REQ** The Object Dictionary shall be represented as a static C array
generated from an OD descriptor, with no runtime heap allocation.

### API
```c
int  omnican_canopen_init(struct omnican_node *, uint8_t node_id);
int  omnican_canopen_start(struct omnican_node *);
void omnican_canopen_stop(struct omnican_node *);
void omnican_canopen_process(struct omnican_node *, uint32_t elapsed_us,
                              uint32_t *next_us);
```
`omnican_canopen_process()` shall be called periodically (from a dedicated
thread or the main loop) to drive timers, heartbeat, and SDO timeouts.

**REQ** `omnican_canopen_process()` shall write the recommended delay to the
next call (in microseconds) into `*next_us` so callers can schedule correctly.

## Phase 2 — SAE J1939 (`src/j1939/`)

### CAN Identifier Format
J1939 uses 29-bit extended CAN identifiers exclusively.
Format: `[Priority(3)] [Reserved(1)] [DataPage(1)] [PF(8)] [PS(8)] [SA(8)]`

**REQ** The J1939 module shall use 29-bit extended CAN identifiers and shall
reject frames with 11-bit standard IDs.

### Address Claiming (J1939/81)
**REQ** The J1939 module shall implement the address-claiming procedure per
J1939/81: transmit a Claim Address message, monitor for contention for 250 ms,
and resolve conflicts using the 64-bit NAME priority.

**REQ** A J1939 node shall not transmit any PGN until address claiming succeeds.

**REQ** If address claiming fails (higher-priority NAME wins), the module shall
report `OMNICAN_ERR_BUSY` and set `addr = OMNICAN_J1939_ADDR_NULL`.

### PGN Routing
**REQ** The J1939 module shall support a static PGN routing table populated
via `omnican_j1939_register_pgn()` with a callback and user-data pointer.

**REQ** PGN callbacks shall be invoked from the system workqueue with source
address, data pointer, and length.

### Transport Protocol (J1939/21 TP and ETP)
**REQ** The J1939 module shall implement J1939 Transport Protocol (TP) for
multi-packet messages of 9–1785 bytes using BAM and CMDT modes.

**REQ** The J1939 module shall implement Extended Transport Protocol (ETP) for
messages larger than 1785 bytes (up to ~117 MB), subject to Zephyr memory
constraints.

**REQ** TP/ETP session state shall be maintained in static context with a
configurable maximum number of simultaneous sessions (`CONFIG_OMNICAN_J1939_MAX_SESSIONS`).

### API
```c
int omnican_j1939_init(struct omnican_j1939_node *, struct omnican_node *,
                       const uint8_t name[8]);
int omnican_j1939_claim_address(struct omnican_j1939_node *,
                                omnican_j1939_addr_t, uint32_t timeout_ms);
int omnican_j1939_send(struct omnican_j1939_node *, omnican_j1939_pgn_t,
                       omnican_j1939_addr_t, const uint8_t *, size_t);
int omnican_j1939_register_pgn(struct omnican_j1939_node *,
                               omnican_j1939_pgn_t,
                               omnican_j1939_rx_cb_t, void *);
```

## Phase 3 — ISO 14229 / UDS (`src/uds/`)

### Transport
UDS uses Zephyr's ISO-TP subsystem (ISO 15765-2) as transport.
Physical addressing: tester → ECU on configurable CAN IDs (default 0x7E0/0x7E8).

**REQ** The UDS module shall use Zephyr ISO-TP for all message transport and
shall apply the ISOTP patch (see §ISOTP Patch) when `CONFIG_OMNICAN_ISOTP_PATCH=y`.

### Session State Machine
Sessions: Default (0x01) → Programming (0x02) → Extended Diagnostic (0x03).

**REQ** The UDS server shall implement DiagnosticSessionControl (SID 0x10)
and enforce service availability per active session.

**REQ** The UDS server shall maintain an S3 timer (default 5000 ms) and revert
to Default session on expiry when not in Default session.

**REQ** The TesterPresent service (SID 0x3E) shall reset the S3 timer.

### Supported Services
**REQ** The UDS server shall support the following SIDs:
- 0x10 DiagnosticSessionControl
- 0x11 ECUReset (hardReset, softReset, keyOffOnReset)
- 0x22 ReadDataByIdentifier
- 0x27 SecurityAccess (seed/key mechanism, app-provided)
- 0x28 CommunicationControl
- 0x2E WriteDataByIdentifier
- 0x31 RoutineControl (start, stop, requestResults)
- 0x34 RequestDownload
- 0x36 TransferData
- 0x37 RequestTransferExit
- 0x3E TesterPresent
- 0x85 ControlDTCSetting

**REQ** Unregistered SIDs shall return Negative Response Code (NRC) 0x11
(serviceNotSupported).

### SecurityAccess
**REQ** SecurityAccess (SID 0x27) shall use an application-supplied
seed-generation and key-validation callback, never a hardcoded algorithm.

**REQ** After exceeding `CONFIG_OMNICAN_UDS_MAX_AUTH_ATTEMPTS` (default 3)
failed SecurityAccess attempts, the server shall lock the session and return
NRC 0x36 (exceededNumberOfAttempts).

### API
```c
int omnican_uds_server_init(struct omnican_uds_server *,
                            struct omnican_node *,
                            const struct omnican_uds_server_cfg *);
int omnican_uds_register_service(struct omnican_uds_server *, uint8_t sid,
                                 omnican_uds_service_cb_t, void *);
```

## Phase 4 — SAE J1979 / OBD-II (`src/obd2/`)

### Transport
OBD-II uses Zephyr ISO-TP with ISO 15765-4 (OBD on CAN) parameters.
Standard IDs: 0x7DF (broadcast request), 0x7E8–0x7EF (ECU responses).

**REQ** The OBD-II client shall transmit PID requests on CAN ID 0x7DF and
collect responses from 0x7E8–0x7EF within a configurable timeout.

### Service Mode Support
**REQ** The OBD-II client shall support service modes 0x01 through 0x09.

**REQ** The OBD-II client shall provide a callback-based response model:
`omnican_obd2_request_pid()` is non-blocking; the registered callback fires
when a response arrives or the request times out.

**REQ** The OBD-II client shall support the following Mode 0x01 PIDs at
minimum: 0x00 (supported PIDs), 0x01 (monitor status), 0x04 (engine load),
0x05 (coolant temp), 0x0C (RPM), 0x0D (vehicle speed), 0x11 (throttle).

### API
```c
int omnican_obd2_client_init(struct omnican_obd2_client *,
                             struct omnican_node *);
int omnican_obd2_request_pid(struct omnican_obd2_client *,
                             uint8_t mode, uint8_t pid,
                             omnican_obd2_response_cb_t, void *user_data,
                             uint32_t timeout_ms);
```

## Phase 5 — CANopen FD CiA 1301 (`src/canopen_fd/`)

**REQ** The CANopen FD module shall extend the CANopen module (Phase 1) with
CiA 1301 FD-specific features and shall require `CONFIG_OMNICAN_CANOPEN=y` and
`CONFIG_CAN_FD_MODE=y`.

**REQ** CANopen FD frames shall support data payloads of up to 64 bytes,
enabling higher-throughput PDO transfers than classical CANopen.

**REQ** The CANopen FD module shall remain backward-compatible: a node
configured for FD shall be able to communicate with classical CANopen nodes
using 8-byte frames when FD is not negotiated.

## ISOTP Patch (`src/isotp_patch/`)

Zephyr issue [#86025](https://github.com/zephyrproject-rtos/zephyr/issues/86025):
the Zephyr ISO-TP subsystem does not allow binding and transmitting on the same
CAN ID within a single socket context. Standard UDS physical addressing
(e.g. tester 0x7E0 / ECU 0x7E8) requires both.

**REQ** When `CONFIG_OMNICAN_ISOTP_PATCH=y`, the module shall implement a
workaround that uses separate TX and RX socket contexts with a forwarding shim
so that UDS and OBD-II can operate on standard physical addresses.

**REQ** `CONFIG_OMNICAN_ISOTP_PATCH` shall default to `y` when either
`CONFIG_OMNICAN_UDS` or `CONFIG_OMNICAN_OBD2` is enabled.

## Thread and Execution Model

```
CAN ISR
  └─ Zephyr CAN driver → receive callback
        └─ Submit work item → System workqueue
              └─ Frame router dispatch
                    ├─ CANopen handler
                    ├─ J1939 handler
                    ├─ UDS ISO-TP handler
                    └─ OBD-II ISO-TP handler
```

**REQ** All protocol handlers shall execute in a Zephyr workqueue context (not
the CAN ISR) so they may safely use kernel synchronisation primitives.

**REQ** The CANopen process loop shall run in a dedicated Zephyr thread with
a configurable stack size (`CONFIG_OMNICAN_CANOPEN_THREAD_STACK_SIZE`,
default 2048 bytes) and priority (`CONFIG_OMNICAN_CANOPEN_THREAD_PRIO`,
default 7).

## Memory Model

**REQ** OmniCAN shall not use dynamic heap allocation (`k_malloc`, `malloc`).
All persistent state shall be statically allocated by the caller or via
Zephyr `net_buf` pools.

**REQ** CAN frame data shall be handled via Zephyr `net_buf` from a pool
sized by `CONFIG_OMNICAN_NET_BUF_COUNT` (default 16).

**REQ** All protocol context structures (`omnican_node`, `omnican_j1939_node`,
`omnican_uds_server`, `omnican_obd2_client`) shall be opaque to the caller
but caller-allocated (declared with `struct omnican_xxx;` forward declarations
with a separately published size constant or `sizeof` helper).

## Kconfig Summary

| Symbol | Meaning |
|---|---|
| `CONFIG_OMNICAN` | Master enable; requires `CAN`, selects `NET_BUF` |
| `CONFIG_OMNICAN_CANOPEN` | Enable CANopen CiA 301 |
| `CONFIG_OMNICAN_CANOPEN_FD` | Enable CANopen FD CiA 1301 (requires CANOPEN + CAN_FD_MODE) |
| `CONFIG_OMNICAN_J1939` | Enable SAE J1939 |
| `CONFIG_OMNICAN_UDS` | Enable ISO 14229 UDS (requires ISOTP) |
| `CONFIG_OMNICAN_OBD2` | Enable SAE J1979 OBD-II (requires ISOTP) |
| `CONFIG_OMNICAN_FRAME_ROUTER` | Auto-enabled when any protocol active |
| `CONFIG_OMNICAN_ISOTP_PATCH` | Auto-enabled for UDS/OBD2 |

## Dependencies

| Dependency | Version | Notes |
|---|---|---|
| Zephyr RTOS | v3.7.0 | Pinned in west.yml |
| Zephyr CAN driver API | — | `zephyr/drivers/can.h` |
| Zephyr ISO-TP subsystem | — | Required by UDS and OBD-II |
| Zephyr net_buf | — | Frame buffer pool |
| CANopenNode | master (optional) | Reference only; not vendored |

**REQ** OmniCAN shall be buildable against Zephyr v3.7.0 with no patches to
Zephyr itself (the ISOTP patch lives within OmniCAN's own source tree).

**REQ** OmniCAN shall register as a Zephyr west module via `zephyr/module.yml`
so that `west build` automatically adds `include/` to the build include path.
