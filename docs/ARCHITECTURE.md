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
- Support simultaneous reception of 11-bit standard and 29-bit extended CAN frames on the same router instance
- Support 29-bit extended CAN ID filtering for CANopen FD extended COB-IDs and vendor-specific profiles
- Provide configurable per-protocol CAN filter masks via Kconfig so applications can narrow or broaden reception ranges

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
- Provide `omnican_canopen_nmt_change_cb_t` callback invoked on every NMT state transition (both self-initiated and master-commanded)
- Provide `omnican_canopen_sdo_post_write_cb_t` callback after every successful SDO write so the application can react to OD changes
- Provide `omnican_canopen_rpdo_rx_cb_t` callback when RPDO data is received and mapped into the OD
- Provide `omnican_canopen_emcy_rx_cb_t` callback when an Emergency message from a remote node is received
- Provide `omnican_canopen_sync_cb_t` callback on SYNC message reception for application-level synchronisation
- Provide `omnican_canopen_hb_event_cb_t` callback for heartbeat consumer events distinguishing timeout and recovery
- Provide `omnican_canopen_bootup_rx_cb_t` callback when a remote node Boot-Up message is received

### J1939
- Use 29-bit extended CAN identifiers and reject 11-bit standard ID frames
- Implement the address-claiming procedure per J1939/81 with 250 ms contention window and 64-bit NAME priority
- Prevent PGN transmission until address claiming succeeds
- Return OMNICAN_ERR_BUSY and set addr to NULL when address claiming fails
- Support a static PGN routing table populated via `omnican_j1939_register_pgn()` with callback and user-data
- Invoke PGN callbacks from the system workqueue with source address, data pointer, and length
- Implement J1939 Transport Protocol (TP) for 9-1785 byte messages using BAM and CMDT modes
- Implement Extended Transport Protocol (ETP) for messages larger than 1785 bytes with configurable maximum payload via CONFIG_OMNICAN_J1939_ETP_MAX_PAYLOAD_KB (default 64 KB; range 1–16384 KB) to cap memory usage on embedded targets
- Explicitly implement ETP.CM (PGN 0xC800) and ETP.DT (PGN 0xC700) as separate ETP connection management and data transfer PGNs
- Maintain TP/ETP session state in static context with configurable maximum simultaneous sessions
- Implement RQST (PGN 0xEA00) to request any PGN from a remote device (mandatory for DM request/response)
- Implement ACKM (PGN 0xE800) to send positive, negative, busy, and cannot-respond acknowledgments (mandatory per J1939/21)
- Support Proprietary A (PGN 0xEF00, peer-to-peer) and Proprietary B (PGN 0xFF00, broadcast) via the PGN routing table for manufacturer-specific messages
- Provide `omnican_j1939_address_result_cb_t` callback reporting address claim success, failure, or address-lost events
- Provide `omnican_j1939_tp_progress_cb_t` callback reporting bytes transferred during active TP/ETP sessions
- Provide `omnican_j1939_tp_complete_cb_t` callback on TP/ETP session completion or abort with status
- Support configurable maximum TP/ETP single-transfer payload via CONFIG_OMNICAN_J1939_TP_MAX_PAYLOAD (default 1785 bytes for TP, unlimited for ETP)

### J1939 DTC and SPN
- Implement 4-byte J1939 DTC encode/decode: SPN (19-bit) in bytes 1-3 bits [7:5], FMI (5-bit) in byte 3 bits [4:0], CM (1-bit) in byte 4 bit [7], OC (7-bit) in byte 4 bits [6:0] per J1939/73
- Define all 32 FMI (Failure Mode Identifier) codes 0-31 per J1939/73 §4.3.2
- Provide `omnican_j1939_dtc_encode(uint32_t spn, uint8_t fmi, uint8_t oc, bool cm, uint8_t out[4])` API
- Provide `omnican_j1939_dtc_decode(const uint8_t in[4], uint32_t *spn, uint8_t *fmi, uint8_t *oc, bool *cm)` API
- Provide `omnican_j1939_fmi_string(uint8_t fmi)` to return human-readable FMI description

### J1939 Diagnostics (extended DM set — via CONFIG_OMNICAN_J1939_DM_EXTENDED)
- Implement DM4 (PGN 0xFECD) freeze frame parameters capture and reporting when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- Implement DM13 (PGN 0xDF00) stop/start broadcast control when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- Implement DM20 (PGN 0xC200) monitor performance ratio reporting when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- Implement DM21 (PGN 0xC100) diagnostic readiness 2 when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- Implement DM25 (PGN 0xFDB7) expanded freeze frame with SPN-indexed data when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- Implement DM26 (PGN 0xFDB8) diagnostic readiness 3 when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- Implement DM27 (PGN 0xFD82) all pending DTCs when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- Implement DM28 (PGN 0xFD80) emission-related permanent DTCs when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- Implement DM29 (PGN 0x9E00) DTC counts when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- Implement DM30 (PGN 0xA400) scaled test results by SPN when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- Implement DM31 (PGN 0xA300) DTC-to-lamp association when CONFIG_OMNICAN_J1939_DM_EXTENDED=y

### J1939 SLOT and SPN Decoding (optional — CONFIG_OMNICAN_J1939_SLOT_TABLE)
- Provide a compiled-in SLOT decode table derived from J1939DA DEC2024 (426 SLOTs) when CONFIG_OMNICAN_J1939_SLOT_TABLE=y
- Include a mandatory minimum SPN subset by default covering J1939/71 vehicle application layer required SPNs: engine speed (SPN 190), vehicle speed (SPN 84), engine coolant temperature (SPN 110), fuel rate (SPN 183), throttle position (SPN 91), engine hours (SPN 247), total fuel used (SPN 250)
- Support configurable SPN subset extension via CONFIG_OMNICAN_J1939_SPN_EXTRA_LIST (comma-separated SPN numbers) to add application-specific SPNs beyond the mandatory minimum without including the full 426-SLOT table
- Provide `omnican_j1939_spn_decode(uint32_t spn, const uint8_t *raw, size_t raw_len, double *value, const char **unit)` API for converting raw SPN bytes to engineering units
- Provide `omnican_j1939_spn_register_custom(uint32_t spn, const struct omnican_j1939_slot *slot)` API to register manufacturer-specific SPNs (SPN > 524287) at runtime without Kconfig changes

### J1939 DA Reference Tables (optional)
- Provide compiled-in manufacturer ID lookup table from J1939DA B10 (1,509 entries) when CONFIG_OMNICAN_J1939_MFR_TABLE=y
- Provide `omnican_j1939_mfr_name(uint16_t mfr_id)` returning manufacturer name string
- Provide `omnican_j1939_mfr_register(uint16_t mfr_id, const char *name)` API to register manufacturer-specific IDs not in the DA at runtime
- Provide global preferred source address table from J1939DA B2 when CONFIG_OMNICAN_J1939_IG_ADDRESSES=y
- Provide NAME function lookup from J1939DA B11/B12 when CONFIG_OMNICAN_J1939_IG_ADDRESSES=y

### J1939 Diagnostics (mandatory DM set)
- Implement DM1 (PGN 0xFECA) with correct 2-byte lamp status header: MIL [7:6], Stop Lamp [5:4], Amber Warning [3:2], Protect [1:0] per J1939/73, plus repeated 4-byte DTC records
- Implement DM2 (PGN 0xFECB) previously active DTCs with same frame format as DM1
- Implement DM3 (PGN 0xFECC) clear/reset previously active DTCs (mandatory request handling)
- Implement DM4 (PGN 0xFECD) freeze frame parameters with a configurable SPN capture list: provide CONFIG_OMNICAN_J1939_DM4_SPN_LIST (default: SPN 190, 84, 110, 91 — engine speed, vehicle speed, coolant temp, throttle) and allow application to override via `omnican_j1939_dm4_set_spn_list(uint32_t *spns, uint8_t count)`
- Implement DM5 (PGN 0xFECE) diagnostic readiness with active/previously active DTC counts and supported/completed monitor bits
- Implement DM6 (PGN 0xFECF) emission-related pending DTCs
- Implement DM11 (PGN 0xFED3) clear/reset active DTCs (mandatory request handling)
- Implement DM12 (PGN 0xFED4) emission-related MIL-on DTCs
- Implement DM22 (PGN 0xC300) individual clear/reset of specific active and previously active DTCs by SPN/FMI
- Provide `omnican_j1939_dm1_cb_t` callback delivering lamp status and decoded DTC list when DM1 is received
- Provide `omnican_j1939_dm_request(struct omnican_j1939_node *, uint8_t dm_num, omnican_j1939_addr_t dest)` to request a specific DM from a device

### UDS ISO 14229
- Use Zephyr ISO-TP for all UDS message transport with ISOTP patch when CONFIG_OMNICAN_ISOTP_PATCH=y
- Implement DiagnosticSessionControl (SID 0x10) and enforce service availability per active session
- Maintain an S3 timer (default 5000 ms) and revert to Default session on expiry
- Reset the S3 timer on TesterPresent (SID 0x3E)
- Support SID 0x10 DiagnosticSessionControl
- Support SID 0x11 ECUReset with hardReset, softReset, and keyOffOnReset sub-functions
- Support SID 0x22 ReadDataByIdentifier with multi-DID requests (multiple DIDs in one 0x22 request)
- Support SID 0x27 SecurityAccess with application-supplied seed/key callback
- Support SID 0x28 CommunicationControl with all sub-functions: enableRxAndTx (0x00), enableRxAndDisableTx (0x01), disableRxAndEnableTx (0x02), disableRxAndTx (0x03)
- Support SID 0x2E WriteDataByIdentifier with application-supplied write validation callback before applying data
- Support SID 0x31 RoutineControl with application-supplied start, stop, and requestResults executor callbacks per routine ID
- Support SID 0x34 RequestDownload
- Support SID 0x36 TransferData
- Support SID 0x37 RequestTransferExit
- Support SID 0x85 ControlDTCSetting with application callback to enable or disable DTC storage
- Return NRC 0x11 (serviceNotSupported) for unregistered SIDs
- Never use a hardcoded SecurityAccess seed/key algorithm; always use an application callback
- Lock the session and return NRC 0x36 after exceeding CONFIG_OMNICAN_UDS_MAX_AUTH_ATTEMPTS failed SecurityAccess attempts
- Support suppressPositiveResponse sub-function bit (0x80) per ISO 14229-1 §7.5.2.2
- Implement NRC 0x78 (responsePending) when a service handler cannot respond within p2Server_max and requires p2StarServer_max time
- Support manufacturer-specific DID range (0xF200–0xFEFF) via application-registered DID handler callbacks with read and write hooks
- Implement mandatory ECU identification DIDs: 0xF186 (activeSession), 0xF18B (ECUManufacturingDate), 0xF18C (ECUSerialNumber), 0xF190 (VIN), 0xF195 (SystemSupplierECUSoftwareVersion)
- Support functional addressing via CAN ID 0x7DF for UDS broadcast service requests (TesterPresent, DiagnosticSessionControl)
- Provide `omnican_uds_session_change_cb_t` callback on every session transition with old and new session type
- Provide `omnican_uds_security_change_cb_t` callback on security access level grant or revocation
- Provide `omnican_uds_pre_reset_cb_t` callback before ECUReset execution allowing application to flush state
- Support manufacturer-specific NRC values 0x80–0xFE via application NRC provider callback registered per SID
- Provide `omnican_uds_transfer_progress_cb_t` callback during 0x36 TransferData reporting block sequence and bytes transferred

### OBD-II J1979
- Transmit PID requests on CAN ID 0x7DF and collect responses from 0x7E8-0x7EF within configurable timeout
- Support OBD-II service modes 0x01 through 0x09
- Provide callback-based non-blocking PID request/response model
- Support Mode 0x01 PIDs: 0x00 (supported), 0x01 (monitor status), 0x04 (engine load), 0x05 (coolant temp), 0x0C (RPM), 0x0D (vehicle speed), 0x11 (throttle position)
- Support Mode 0x01 PID support bitmap queries (PIDs 0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0) to enumerate ECU-supported PIDs
- Support Mode 0x01 extended PIDs: 0x14–0x1B (O2 sensor voltage/trim), 0x1C (OBD standard), 0x21 (distance since MIL on), 0x2F (fuel level), 0x31 (distance since codes cleared), 0x33 (barometric pressure), 0x46 (ambient air temperature), 0x49 (accelerator position), 0x51 (fuel type), 0x5C (engine oil temperature)
- Support Mode 0x02 (Freeze Frame) PID requests using the same PID set as Mode 0x01 with frame number sub-byte
- Retrieve Mode 0x03 stored DTCs and Mode 0x07 pending DTCs as decoded fault codes per SAE J2012 (P/C/B/U + 4-digit hex)
- Support Mode 0x04 clear all emission-related DTCs with application completion callback
- Support Mode 0x06 on-board monitoring test results (OBDMID and OBDMID test results) via callback
- Support Mode 0x09 vehicle information: INFOTYPE 0x02 (VIN), 0x04 (calibration ID), 0x06 (CVN), 0x0A (ECU name)
- Aggregate responses from multiple ECUs for broadcast requests and deliver each ECU response individually via per-ECU callback
- Support ECU-specific physical addressing: send to 0x7E0–0x7E7 and receive from the corresponding 0x7E8–0x7EF response CAN ID
- Provide `omnican_obd2_dtc_cb_t` callback delivering decoded DTC records (fault code, status byte) from Mode 0x03/0x07 responses
- Provide `omnican_obd2_timeout_cb_t` callback when a PID request receives no response within the configured timeout

### CANopen FD CiA 1301
- Extend CANopen module with CiA 1301 FD features; require CONFIG_OMNICAN_CANOPEN=y and CONFIG_CAN_FD_MODE=y
- Support CAN FD data payloads of up to 64 bytes for higher-throughput PDO transfers
- Maintain backward compatibility with classical 8-byte CANopen nodes when FD is not negotiated

### Multi-Protocol Co-Existence
- Support simultaneous operation of J1939 (29-bit) and UDS (11-bit) on the same CAN bus without filter conflicts by distinguishing frame types at the Zephyr CAN driver level
- Support simultaneous operation of CANopen (11-bit) and UDS (11-bit) using non-overlapping CAN ID ranges: CANopen 0x000-0x5FF is below UDS 0x7DF/0x7E0-0x7EF; document this co-existence guarantee
- Support simultaneous operation of CANopen (11-bit) and J1939 (29-bit) via frame type distinction with no ID conflict
- Enforce a policy when OBD-II and UDS are both enabled: UDS holds exclusive ownership of CAN ID 0x7DF for functional addressing; OBD-II client shall use physical addressing (0x7E0-0x7E7) when UDS is co-active, or disable OBD-II functional broadcast
- Provide a `docs/MULTIPROTOCOL.md` integration guide documenting supported combinations, CAN ID allocation maps, and Kconfig compatibility matrix for all 4-protocol combinations
- Support cross-protocol error propagation: an `omnican_error_cb_t` global hook that receives errors from any enabled protocol with protocol ID, error code, and context

### ISOTP Patch
- Implement a workaround for Zephyr issue #86025 using separate TX/RX socket contexts with a forwarding shim
- Default CONFIG_OMNICAN_ISOTP_PATCH to y when CONFIG_OMNICAN_UDS or CONFIG_OMNICAN_OBD2 is enabled

### Memory and Threading
- Never use dynamic heap allocation (k_malloc, malloc); all state statically allocated or via net_buf pools
- Handle CAN frame data via Zephyr net_buf from a pool sized by CONFIG_OMNICAN_NET_BUF_COUNT (default 16)
- All protocol context structures shall be opaque to the caller but caller-allocated
- All protocol handlers execute in a Zephyr workqueue context, not the CAN ISR

## Normative References
See `docs/STANDARDS.md` for the complete standards registry with edition numbers.
Primary normative references per protocol:
- **CANopen**: CiA 301 v4.2.0 (Phase 1), CiA 1301 v1.0.0 (Phase 5)
- **J1939**: SAE J1939/21 DEC2010, SAE J1939/81 MAY2003
- **UDS**: ISO 14229-1:2020, ISO 14229-3:2012
- **OBD-II**: SAE J1979 FEB2012, ISO 15765-4:2021
- **ISO-TP**: ISO 15765-2:2016
- **CAN physical**: ISO 11898-1:2015 (via Zephyr CAN driver)

## Design Principles
1. **Zero-overhead when disabled** — every module is behind `IS_ENABLED()` guards; disabled protocols produce no code or data.
2. **Static allocation only** — no heap (`malloc`/`free`). All protocol contexts are caller-allocated. Frame buffers use Zephyr `net_buf` pools.
3. **Zephyr-native** — uses Zephyr CAN driver API, `net_buf`, logging, Kconfig, and the workqueue/thread model throughout. No POSIX shims.
4. **Single node context** — `struct omnican_node` is the root anchor; all protocol sub-contexts hold a pointer back to it.
5. **Standards-faithful** — each protocol module traces directly to the normative standard; any deviation is documented in LEDGER.md.
6. **Apache-2.0** — all source files carry SPDX headers.

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
  docs/STANDARDS.md  Normative standards registry with edition pins
```

## System Architecture Diagram

```mermaid
flowchart TD
  App(["Application code"])
  Node["omnican_node\n(bitrate, fd_mode, protos)"]
  Router["Frame Router\n(src/core/)"]
  CAN["Zephyr CAN Driver\n(ISO 11898)"]
  CO["CANopen CiA 301\n(src/canopen/)"]
  J["J1939\n(src/j1939/)"]
  UDS["UDS ISO 14229\n(src/uds/)"]
  OBD["OBD-II J1979\n(src/obd2/)"]
  COFD["CANopen FD CiA 1301\n(src/canopen_fd/)"]
  ISOTP["Zephyr ISO-TP\n+ patch #86025"]

  App -->|"omnican_node_init()"| Node
  Node -->|"init/start"| Router
  Router <-->|"RX filter\nwork item"| CAN
  Router -->|"dispatch"| CO
  Router -->|"dispatch"| J
  Router -->|"dispatch via ISOTP"| UDS
  Router -->|"dispatch via ISOTP"| OBD
  CO -->|"extends"| COFD
  UDS --- ISOTP
  OBD --- ISOTP
  ISOTP <--> CAN
```

## Core Node Context

`struct omnican_node` (defined in `include/omnican/core.h`) is the shared
anchor for all protocol instances:

```c
struct omnican_node {
    const struct device *can_dev;  /* Zephyr CAN device              */
    uint32_t             bitrate;  /* Nominal bitrate in kbps        */
    bool                 fd_mode;  /* True when CAN FD active        */
    omnican_proto_t      protos;   /* Bitmask of initialised protos  */
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

1. Registers a Zephyr `CAN_MSGQ_DEFINE` or CAN RX callback per enabled protocol.
2. On each received frame, inspects the CAN ID to determine the owning protocol.
3. Dispatches to the appropriate protocol handler via `k_work_submit()`.
4. Runs dispatch in a Zephyr system workqueue callback to keep the CAN ISR short.

### Dispatch Table

| Protocol | CAN filter | ID range | Frame type |
|---|---|---|---|
| CANopen (classical) | Mask filter | 0x000–0x7FF | 11-bit standard |
| CANopen FD / vendor ext | Mask filter | 29-bit extended (configurable) | 29-bit extended |
| J1939 | Accept-all extended | 29-bit extended | 29-bit extended only |
| UDS/OBD-II | ISO-TP layer | 0x7DF, 0x7E0–0x7EF | 11-bit standard |

**11-bit and 29-bit coexistence**: CANopen uses 11-bit IDs; J1939 uses 29-bit IDs.
They can coexist on the same bus without filter conflicts because the Zephyr CAN
driver distinguishes frame type at the filter level. When `CONFIG_OMNICAN_CANOPEN_FD=y`,
an additional 29-bit filter slot is registered for any CANopen FD extended COB-IDs or
vendor-specific profiles that require 29-bit addressing. Applications may narrow or
broaden any filter range via the per-protocol Kconfig mask symbols.

**REQ** The frame router shall register exactly one CAN receive filter per
enabled protocol and dispatch received frames to the correct protocol handler
without blocking the CAN ISR.

**REQ** The frame router shall use a Zephyr system workqueue for all protocol
callback dispatch so that protocol handlers may use kernel primitives (mutexes,
k_sleep, etc.).

## Phase 1 — CANopen (CiA 301) (`src/canopen/`)

**Normative reference**: CiA 301 v4.2.0. See `docs/STANDARDS.md §2`.

### Source
Port and rearchitecture of the `BitConcepts/CANopenNode` fork, incorporating
all fixes from nine iterative audit rounds. The implementation is Zephyr-native
and does not depend on upstream CANopenNode runtime conventions.

### NMT State Machine (CiA 301 §7.3.2.2)

```mermaid
stateDiagram-v2
    [*] --> Initialising
    Initialising --> PreOperational : Boot-Up complete\n(TX CAN ID 0x700+id, data=0x00)
    PreOperational --> Operational : NMT Start (cmd=0x01)
    Operational --> PreOperational : NMT Enter Pre-Op (cmd=0x80)
    Operational --> Stopped : NMT Stop (cmd=0x02)
    PreOperational --> Stopped : NMT Stop (cmd=0x02)
    Stopped --> PreOperational : NMT Enter Pre-Op (cmd=0x80)
    Stopped --> Operational : NMT Start (cmd=0x01)
    PreOperational --> Initialising : NMT Reset Comm (cmd=0x82)
    Operational --> Initialising : NMT Reset Comm (cmd=0x82)
    Stopped --> Initialising : NMT Reset Comm (cmd=0x82)
    PreOperational --> Initialising : NMT Reset Node (cmd=0x81)
    Operational --> Initialising : NMT Reset Node (cmd=0x81)
    Stopped --> Initialising : NMT Reset Node (cmd=0x81)
```

NMT command frames: CAN ID 0x000, 2 bytes: `[cmd, node_id]`. node_id=0 targets all.

**REQ** The CANopen module shall implement the NMT state machine with states
Initialising, Pre-Operational, Operational, and Stopped per CiA 301.

**REQ** The CANopen module shall transmit a Boot-Up message (NMT state 0x00)
on transition from Initialising to Pre-Operational.

**REQ** The CANopen module shall respond to NMT master commands (Start, Stop,
Enter Pre-Operational, Reset Node, Reset Communication) received on CAN ID 0x000.

### SDO Server (CiA 301 §7.2.4)

SDO provides confirmed object dictionary access. Three transfer modes:

| Mode | When | Max data |
|---|---|---|
| Expedited | ≤ 4 bytes | 1–4 bytes in one exchange |
| Segmented | 5–N bytes | 7 bytes per segment, N segments |
| Block transfer | Large data | Up to 127 segments per block, confirmed per block |

SDO frame layout (CAN ID 0x600+id for request, 0x580+id for response):

```
Byte 0: Command specifier (cs)
Byte 1: Index LSB
Byte 2: Index MSB
Byte 3: Sub-index
Bytes 4–7: Data (expedited) or segment count / block size
```

**REQ** The CANopen module shall implement an SDO server supporting expedited,
segmented, and block transfer modes per CiA 301.

**REQ** The SDO server shall return an SDO abort (0x05040000 — SDO timeout)
if no response is received within the configured timeout.

### PDO (CiA 301 §7.2.3)

PDOs carry process data without overhead. Transmission types (OD 0x1800, sub 2):

| Type | Value | Description |
|---|---|---|
| Event-driven (async) | 0xFF | Transmitted on data change or application trigger |
| Synchronous | 0x01–0xF0 | Transmitted every N SYNC messages |
| Synchronous (RTR) | 0xFC | Transmitted after N SYNCs, RTR-triggered |
| Event-driven (RTR) | 0xFD | Transmitted on RTR |
| Manufacturer | 0x00 | Synchronous, acyclic |

PDO mapping (OD 0x1A00/0x1600): each sub-entry encodes `[index(16) | sub(8) | bits(8)]`.

**REQ** The CANopen module shall support Transmit PDO (TPDO) and Receive PDO
(RPDO) with event-driven and synchronisation-triggered transmission modes.

**REQ** PDO mappings shall be configurable at compile time via the Object
Dictionary.

### Emergency (CiA 301 §7.2.7)

Emergency messages: CAN ID `0x80 + node_id`, 8-byte payload:

```
Bytes 0–1: Emergency error code (16-bit)
Byte  2:   Error register (OD 0x1001)
Bytes 3–7: Manufacturer-specific data
```

Error register bits: generic (0), current (1), voltage (2), temperature (3),
communication (4), device profile (5), reserved (6), manufacturer (7).

**REQ** The CANopen module shall produce Emergency messages (CAN ID
`0x80 + node_id`) on internal error events with a 16-bit error code and
8-byte error register per CiA 301.

### Heartbeat (CiA 301 §7.2.8)

Heartbeat producer: CAN ID `0x700 + node_id`, 1-byte NMT state value.
Producer interval: OD 0x1017 (ms). Consumer monitoring: OD 0x1016.

```mermaid
sequenceDiagram
    participant Node
    participant Monitor
    Note over Node: Every CONFIG_OMNICAN_HB_PERIOD_MS
    Node ->> Monitor: 0x700+id [NMT state]
    Note over Monitor: Starts/resets consumer timer
    Note over Monitor: Timer expires (no HB received)
    Monitor ->> Monitor: Raise error event
```

**REQ** The CANopen module shall produce Heartbeat messages at a
Kconfig-configurable interval (default 1000 ms) per CiA 301.

**REQ** The CANopen module shall support Heartbeat consumer monitoring of
configured remote nodes and raise an error event on timeout.

### Object Dictionary Structure

The OD is a static `const struct od_entry od_table[]` array generated from a
descriptor file (compatible with EDS/CiA 306). Each entry:

```c
struct od_entry {
    uint16_t index;
    uint8_t  sub;
    uint8_t  type;      /* UINT8, UINT16, UINT32, ... */
    uint32_t attr;      /* RO, RW, WO, const */
    void    *data;      /* pointer to .data section variable */
    size_t   size;
};
```

**REQ** The Object Dictionary shall be represented as a static C array
generated from an OD descriptor, with no runtime heap allocation.

### Callback Hooks (CANopen)

```c
/* NMT state transition (both self-initiated and master-commanded) */
typedef void (*omnican_canopen_nmt_change_cb_t)(
    struct omnican_node *node,
    uint8_t old_state, uint8_t new_state,
    void *user_data);

/* Called after a successful SDO write to an OD entry */
typedef void (*omnican_canopen_sdo_post_write_cb_t)(
    struct omnican_node *node,
    uint16_t index, uint8_t sub,
    const void *data, size_t len,
    void *user_data);

/* Called when RPDO data is received and mapped into the OD */
typedef void (*omnican_canopen_rpdo_rx_cb_t)(
    struct omnican_node *node,
    uint8_t rpdo_num,
    const uint8_t *data, size_t len,
    void *user_data);

/* Called when Emergency from a remote node is received */
typedef void (*omnican_canopen_emcy_rx_cb_t)(
    struct omnican_node *node,
    uint8_t remote_node_id,
    uint16_t error_code, uint8_t error_register,
    const uint8_t mfr_data[5],
    void *user_data);

/* Called on SYNC reception */
typedef void (*omnican_canopen_sync_cb_t)(
    struct omnican_node *node,
    uint8_t counter,   /* 0 if counter not present */
    void *user_data);

/* Heartbeat consumer event: HB_TIMEOUT or HB_RECOVERED */
typedef void (*omnican_canopen_hb_event_cb_t)(
    struct omnican_node *node,
    uint8_t remote_node_id,
    enum omnican_hb_event event,  /* OMNICAN_HB_TIMEOUT | OMNICAN_HB_RECOVERED */
    void *user_data);

/* Remote node Boot-Up received */
typedef void (*omnican_canopen_bootup_rx_cb_t)(
    struct omnican_node *node,
    uint8_t remote_node_id,
    void *user_data);
```

All callbacks are invoked from the Zephyr system workqueue context.
Registration: `omnican_canopen_set_nmt_cb()`, `omnican_canopen_set_sdo_post_write_cb()`, etc.

### API
```c
int  omnican_canopen_init(struct omnican_node *, uint8_t node_id);
int  omnican_canopen_start(struct omnican_node *);
void omnican_canopen_stop(struct omnican_node *);
void omnican_canopen_process(struct omnican_node *, uint32_t elapsed_us,
                              uint32_t *next_us);
/* Callback registration */
void omnican_canopen_set_nmt_cb(struct omnican_node *,
                                omnican_canopen_nmt_change_cb_t, void *);
void omnican_canopen_set_sdo_post_write_cb(struct omnican_node *,
                                           omnican_canopen_sdo_post_write_cb_t, void *);
void omnican_canopen_set_rpdo_rx_cb(struct omnican_node *,
                                    omnican_canopen_rpdo_rx_cb_t, void *);
void omnican_canopen_set_emcy_rx_cb(struct omnican_node *,
                                    omnican_canopen_emcy_rx_cb_t, void *);
void omnican_canopen_set_sync_cb(struct omnican_node *,
                                  omnican_canopen_sync_cb_t, void *);
void omnican_canopen_set_hb_event_cb(struct omnican_node *,
                                      omnican_canopen_hb_event_cb_t, void *);
void omnican_canopen_set_bootup_rx_cb(struct omnican_node *,
                                       omnican_canopen_bootup_rx_cb_t, void *);
```

`omnican_canopen_process()` drives: heartbeat timer, SDO segmented timeouts,
TPDO event timers, SYNC period counter, Heartbeat consumer watchdogs.

**REQ** `omnican_canopen_process()` shall write the recommended delay to the
next call (in microseconds) into `*next_us` so callers can schedule correctly.

**REQ** The CANopen process loop shall run in a dedicated Zephyr thread with
a configurable stack size (`CONFIG_OMNICAN_CANOPEN_THREAD_STACK_SIZE`,
default 2048 bytes) and priority (`CONFIG_OMNICAN_CANOPEN_THREAD_PRIO`,
default 7).

## Phase 2 — SAE J1939 (`src/j1939/`)

**Normative references**: SAE J1939/21 DEC2010 (TP/ETP), SAE J1939/81 MAY2003
(address claiming). See `docs/STANDARDS.md §3`.

### CAN Identifier Format (J1939/21 §5.2)

J1939 uses 29-bit extended CAN identifiers exclusively.

```
Bits: 28          26 25  24 23       16 15        8 7          0
      ┌─────────────┬──┬──┬───────────┬────────────┬────────────┐
      │ Priority(3) │R │DP│    PF(8)  │   PS(8)    │   SA(8)   │
      └─────────────┴──┴──┴───────────┴────────────┴────────────┘
```

- PF < 0xF0 → peer-to-peer: PS = Destination Address (DA)
- PF ≥ 0xF0 → broadcast: PS = Group Extension (GE); PGN includes GE
- PGN = (DP << 17) | (PF << 8) | (GE if PF ≥ 0xF0 else 0)

**REQ** The J1939 module shall use 29-bit extended CAN identifiers and shall
reject frames with 11-bit standard IDs.

### 64-bit NAME Field (J1939/81 §4.2.2)

```
Bit: 63        56 55      52 51      49 48   42 41     36 35   34 33      21 20       11 10        0
     ┌──────────┬──────────┬──────────┬───────┬─────────┬──────┬────────────┬───────────┬───────────┐
     │ Identity │ Mfr Code │  Ecu     │  Func │  Func   │ Rsv  │   Dev Cls  │ Veh Sys   │ Arb Addr  │
     │ Number   │  (11)    │ Instance │  Inst │  (8)    │  (1) │   Inst(4)  │  (7)      │ Cap (1)   │
     │  (21)    │          │  (3)     │  (5)  │         │      │            │           │           │
     └──────────┴──────────┴──────────┴───────┴─────────┴──────┴────────────┴───────────┴───────────┘
```

Lower NAME value = higher priority in address contention.

### Address Claiming Flow (J1939/81 §4.2)

```mermaid
sequenceDiagram
    participant Node
    participant Bus
    Node ->> Bus: PGN 0xEE00 [NAME] (desired SA)
    Note over Node: Wait 250 ms
    alt No contention
        Note over Node: Address claimed — may transmit PGNs
    else Contender with lower NAME
        Bus ->> Node: PGN 0xEE00 [NAME'] (same SA, lower NAME)
        alt Our NAME is lower
            Node ->> Bus: PGN 0xEE00 [NAME] (same SA) — re-assert
        else Contender wins
            Node ->> Bus: PGN 0xEEFF [NAME] — Cannot Claim
            Note over Node: addr = NULL, return OMNICAN_ERR_BUSY
        end
    end
```

**REQ** The J1939 module shall implement the address-claiming procedure per
J1939/81: transmit a Claim Address message, monitor for contention for 250 ms,
and resolve conflicts using the 64-bit NAME priority.

**REQ** A J1939 node shall not transmit any PGN until address claiming succeeds.

**REQ** If address claiming fails (higher-priority NAME wins), the module shall
report `OMNICAN_ERR_BUSY` and set `addr = OMNICAN_J1939_ADDR_NULL`.

### PGN Routing Table

The routing table is a static array of `{ pgn, callback, user_data }` entries,
limited to `CONFIG_OMNICAN_J1939_MAX_PGN_ENTRIES` (default 16).
Incoming frames are matched by PGN. Unregistered PGNs are silently dropped.

**REQ** The J1939 module shall support a static PGN routing table populated
via `omnican_j1939_register_pgn()` with a callback and user-data pointer.

**REQ** PGN callbacks shall be invoked from the system workqueue with source
address, data pointer, and length.

### Transport Protocol (J1939/21 §5.10)

BAM (Broadcast Announce Message) is used for broadcast PGNs.
CMDT (Connection-Managed Data Transfer) is used for peer-to-peer PGNs.

```mermaid
stateDiagram-v2
    direction LR
    [*] --> Idle
    Idle --> BAM_Tx : send() broadcast
    Idle --> RTS_Tx : send() P2P
    BAM_Tx --> Data_Tx : BAM sent
    RTS_Tx --> CTS_Rx : RTS sent
    CTS_Rx --> Data_Tx : CTS received
    Data_Tx --> ACK_Rx : all packets sent
    ACK_Rx --> Idle : EOM ACK received
    CTS_Rx --> Abort : T2/T3 timeout
    Data_Tx --> Abort : T4 timeout
    Abort --> Idle
```

ETP follows the same pattern with extended RTS/CTS frames for >1785-byte messages.
TP/ETP sessions use statically allocated `struct j1939_tp_session` contexts.

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
                                omnican_j1939_addr_t desired, uint32_t timeout_ms);
int omnican_j1939_send(struct omnican_j1939_node *, omnican_j1939_pgn_t pgn,
                       omnican_j1939_addr_t dst, const uint8_t *data, size_t len);
int omnican_j1939_register_pgn(struct omnican_j1939_node *, omnican_j1939_pgn_t,
                               omnican_j1939_rx_cb_t, void *user_data);
```

### Callback Hooks (J1939)

```c
/* Address claim result: success, failure (BUSY), or address-lost */
typedef void (*omnican_j1939_address_result_cb_t)(
    struct omnican_j1939_node *j1939,
    omnican_j1939_addr_t addr,
    enum omnican_j1939_addr_event event, /* CLAIMED | FAILED | LOST */
    void *user_data);

/* TP/ETP transfer progress (bytes_done / bytes_total) */
typedef void (*omnican_j1939_tp_progress_cb_t)(
    struct omnican_j1939_node *j1939,
    omnican_j1939_pgn_t pgn,
    size_t bytes_done, size_t bytes_total,
    void *user_data);

/* TP/ETP transfer complete or aborted */
typedef void (*omnican_j1939_tp_complete_cb_t)(
    struct omnican_j1939_node *j1939,
    omnican_j1939_pgn_t pgn,
    int status,   /* OMNICAN_OK or error code */
    void *user_data);
```

Registration added to `omnican_j1939_init()` via extended config struct:
```c
struct omnican_j1939_callbacks {
    omnican_j1939_address_result_cb_t address_result;
    omnican_j1939_tp_progress_cb_t    tp_progress;
    omnican_j1939_tp_complete_cb_t    tp_complete;
    void *user_data;
};
int omnican_j1939_set_callbacks(struct omnican_j1939_node *,
                                const struct omnican_j1939_callbacks *);
```

Return codes: `OMNICAN_OK`, `OMNICAN_ERR_BUSY` (address conflict),
`OMNICAN_ERR_TIMEOUT` (TP/ETP timeout), `OMNICAN_ERR_INVAL`, `OMNICAN_ERR_NOMEM`.

## Phase 3 — ISO 14229 / UDS (`src/uds/`)

**Normative references**: ISO 14229-1:2020, ISO 14229-3:2012, ISO 15765-2:2016.
See `docs/STANDARDS.md §4` and `§6`.

### Transport

UDS uses Zephyr's ISO-TP subsystem (ISO 15765-2) as transport.
Physical addressing: tester `0x7E0` → ECU `0x7E8` (configurable via
`struct omnican_uds_server_cfg { .rx_can_id, .tx_can_id, .ecu_addr }`).

**REQ** The UDS module shall use Zephyr ISO-TP for all message transport and
shall apply the ISOTP patch (see §ISOTP Patch) when `CONFIG_OMNICAN_ISOTP_PATCH=y`.

### Session State Machine (ISO 14229-1 §7.5)

```mermaid
stateDiagram-v2
    [*] --> Default
    Default --> Programming : SID 0x10 sub=0x02\n(enters Programming)
    Default --> Extended : SID 0x10 sub=0x03
    Programming --> Default : SID 0x10 sub=0x01\nor S3 timeout (5000 ms)
    Extended --> Default : SID 0x10 sub=0x01\nor S3 timeout (5000 ms)
    Programming --> Extended : SID 0x10 sub=0x03
    note right of Programming : SecurityAccess available\nSID 0x34/0x36/0x37 available
    note right of Extended : SID 0x28, 0x85 available\nSecurityAccess available
```

S3 timer is reset by any correctly received request (including TesterPresent).

**REQ** The UDS server shall implement DiagnosticSessionControl (SID 0x10)
and enforce service availability per active session.

**REQ** The UDS server shall maintain an S3 timer (default 5000 ms) and revert
to Default session on expiry when not in Default session.

**REQ** The TesterPresent service (SID 0x3E) shall reset the S3 timer.

### SecurityAccess Exchange (ISO 14229-1 §10.4)

```mermaid
sequenceDiagram
    participant Tester
    participant ECU
    Tester ->> ECU: SID 0x27, sub=0x01 (requestSeed)
    ECU ->> Tester: SID 0x67, sub=0x01 + seed[N]
    Note over Tester: Computes key = f(seed, secret)
    Tester ->> ECU: SID 0x27, sub=0x02 + key[N]
    alt Key correct
        ECU ->> Tester: SID 0x67, sub=0x02 (unlocked)
    else Key incorrect
        ECU ->> Tester: NRC 0x35 (securityAccessDenied)
        Note over ECU: Increment attempt counter
    else Attempts exceeded
        ECU ->> Tester: NRC 0x36 (exceededNumberOfAttempts)
        Note over ECU: Lock — return NRC 0x37 until delay expires
    end
```

The seed/key algorithm is **never** hardcoded in OmniCAN — it is always
supplied by the application via a callback.

**REQ** The UDS server shall support the following SIDs:
- 0x10 DiagnosticSessionControl
- 0x11 ECUReset (hardReset 0x01, softReset 0x03, keyOffOnReset 0x02)
- 0x22 ReadDataByIdentifier
- 0x27 SecurityAccess (seed/key mechanism, app-provided)
- 0x28 CommunicationControl
- 0x2E WriteDataByIdentifier
- 0x31 RoutineControl (start 0x01, stop 0x02, requestResults 0x03)
- 0x34 RequestDownload
- 0x36 TransferData
- 0x37 RequestTransferExit
- 0x3E TesterPresent
- 0x85 ControlDTCSetting

**REQ** Unregistered SIDs shall return Negative Response Code (NRC) 0x11
(serviceNotSupported).

**REQ** SecurityAccess (SID 0x27) shall use an application-supplied
seed-generation and key-validation callback, never a hardcoded algorithm.

**REQ** After exceeding `CONFIG_OMNICAN_UDS_MAX_AUTH_ATTEMPTS` (default 3)
failed SecurityAccess attempts, the server shall lock the session and return
NRC 0x36 (exceededNumberOfAttempts).

### Data Transfer Flow (ISO 14229-1 §14)

```mermaid
sequenceDiagram
    participant Tester
    participant ECU
    Tester ->> ECU: SID 0x34 RequestDownload\n(addr, len, compression, encrypting)
    ECU ->> Tester: SID 0x74 + maxBlockLen
    loop For each block
        Tester ->> ECU: SID 0x36 TransferData [blockSeq] + data
        ECU ->> Tester: SID 0x76 + blockSeq (ACK)
    end
    Tester ->> ECU: SID 0x37 RequestTransferExit
    ECU ->> Tester: SID 0x77 (complete)
```

### UDS DID Registry

OmniCAN pre-populates mandatory ECU identification DIDs automatically.
Applications extend the DID space by registering handler callbacks:

| DID Range | Description | Who populates |
|---|---|---|
| 0x0000–0x00FF | ISO 14229 reserved | OmniCAN (built-in) |
| 0xF000–0xF0FF | OBD-II mandated | Application |
| 0xF100–0xF1FF | System supplier / PPID | Application |
| 0xF180–0xF1FF | ECU identification (ISO 14229-1 Annex F) | OmniCAN (built-in) |
| 0xF200–0xFEFF | **Manufacturer-specific** | **Application-registered** |
| 0xFF00–0xFFFF | SAE J1979 / OBD-II | OBD-II subsystem |

Mandatory ECU identification DIDs auto-populated by OmniCAN:

| DID | Name | Length | Format |
|---|---|---|---|
| 0xF186 | activeSession | 1 | Current session type |
| 0xF18B | ECUManufacturingDate | 3 | BCD YYYYMMDD trunc |
| 0xF18C | ECUSerialNumber | 17 | ASCII |
| 0xF190 | VIN | 17 | ASCII (app-provided) |
| 0xF195 | SystemSupplierECUSoftwareVersion | 4 | BCD |

### UDS Hook and Callback Reference

```c
/* Session transition: called on every session change */
typedef void (*omnican_uds_session_change_cb_t)(
    struct omnican_uds_server *srv,
    uint8_t old_session, uint8_t new_session,
    void *user_data);

/* Security access level change: called on grant or revocation */
typedef void (*omnican_uds_security_change_cb_t)(
    struct omnican_uds_server *srv,
    uint8_t level,          /* 0 = locked; 1+ = security level */
    bool granted,           /* true = unlocked, false = locked */
    void *user_data);

/* Pre-reset hook: called before ECUReset is executed */
typedef void (*omnican_uds_pre_reset_cb_t)(
    struct omnican_uds_server *srv,
    uint8_t reset_type,     /* 0x01=hard, 0x02=keyOffOn, 0x03=soft */
    void *user_data);

/* TransferData progress: called for each 0x36 block received */
typedef void (*omnican_uds_transfer_progress_cb_t)(
    struct omnican_uds_server *srv,
    uint8_t block_seq,
    size_t bytes_transferred,
    size_t bytes_total,
    void *user_data);

/* WriteDataByIdentifier validation: called before write is applied */
typedef int (*omnican_uds_did_write_validate_cb_t)(
    struct omnican_uds_server *srv,
    uint16_t did,
    const uint8_t *data, size_t len,
    void *user_data);
/* returns 0 = accept; negative = NRC to return */

/* Manufacturer-specific DID handler (read or write) */
typedef int (*omnican_uds_did_handler_cb_t)(
    struct omnican_uds_server *srv,
    uint16_t did,
    bool is_write,
    uint8_t *buf, size_t *buf_len,  /* in/out */
    void *user_data);

/* Routine executor for SID 0x31 RoutineControl */
typedef int (*omnican_uds_routine_cb_t)(
    struct omnican_uds_server *srv,
    uint16_t routine_id,
    uint8_t sub_func,       /* 0x01=start, 0x02=stop, 0x03=results */
    const uint8_t *opt_params, size_t opt_len,
    uint8_t *resp, size_t *resp_len,
    void *user_data);

/* Manufacturer-specific NRC provider per SID */
typedef int (*omnican_uds_nrc_provider_cb_t)(
    struct omnican_uds_server *srv,
    uint8_t sid,
    int internal_error,
    void *user_data);
/* returns NRC value 0x80-0xFE, or 0 to use default */
```

### API
```c
int omnican_uds_server_init(struct omnican_uds_server *srv,
                            struct omnican_node *base,
                            const struct omnican_uds_server_cfg *cfg);
int omnican_uds_register_service(struct omnican_uds_server *srv,
                                 uint8_t sid,
                                 omnican_uds_service_cb_t cb,
                                 void *user_data);
/* Manufacturer DID registration */
int omnican_uds_register_did(struct omnican_uds_server *srv,
                             uint16_t did,
                             omnican_uds_did_handler_cb_t cb,
                             void *user_data);
/* Routine registration */
int omnican_uds_register_routine(struct omnican_uds_server *srv,
                                  uint16_t routine_id,
                                  omnican_uds_routine_cb_t cb,
                                  void *user_data);
/* Hook registration */
void omnican_uds_set_session_change_cb(struct omnican_uds_server *,
                                        omnican_uds_session_change_cb_t, void *);
void omnican_uds_set_security_change_cb(struct omnican_uds_server *,
                                         omnican_uds_security_change_cb_t, void *);
void omnican_uds_set_pre_reset_cb(struct omnican_uds_server *,
                                   omnican_uds_pre_reset_cb_t, void *);
void omnican_uds_set_transfer_progress_cb(struct omnican_uds_server *,
                                           omnican_uds_transfer_progress_cb_t, void *);
void omnican_uds_set_did_write_validate_cb(struct omnican_uds_server *,
                                            omnican_uds_did_write_validate_cb_t, void *);
void omnican_uds_set_nrc_provider_cb(struct omnican_uds_server *,
                                      uint8_t sid,
                                      omnican_uds_nrc_provider_cb_t, void *);
```

Service callback signature:
```c
typedef int (*omnican_uds_service_cb_t)(
    uint8_t sid,
    const uint8_t *req, size_t req_len,
    uint8_t *resp, size_t *resp_len,
    void *user_data
); /* returns 0 = positive response; negative = NRC */
```

## Phase 4 — SAE J1979 / OBD-II (`src/obd2/`)

**Normative references**: SAE J1979 FEB2012, ISO 15765-4:2021.
See `docs/STANDARDS.md §5`.

### Transport

OBD-II uses Zephyr ISO-TP with ISO 15765-4 (OBD on CAN) parameters.
Standard IDs: 0x7DF (broadcast request), 0x7E8–0x7EF (ECU responses).

```mermaid
sequenceDiagram
    participant Scanner
    participant ECU1
    participant ECU2
    Scanner ->> ECU1: 0x7DF [Mode, PID]
    Scanner ->> ECU2: 0x7DF [Mode, PID]
    ECU1 ->> Scanner: 0x7E8 [Mode+0x40, PID, dataA, dataB, ...]
    ECU2 ->> Scanner: 0x7E9 [Mode+0x40, PID, dataA, dataB, ...]
    Note over Scanner: Collect all responses\nwithin timeout
```

**REQ** The OBD-II client shall transmit PID requests on CAN ID 0x7DF and
collect responses from 0x7E8–0x7EF within a configurable timeout.

### Mode 0x01 PID Response Format (J1979 §5.3)

Response bytes: `[Mode+0x40, PID, dataA, dataB, dataC, dataD]`

Key PIDs and decode formulas:

| PID | Bytes | Formula | Unit |
|---|---|---|---|
| 0x04 | 1 | A × 100 / 255 | % load |
| 0x05 | 1 | A − 40 | °C coolant |
| 0x0C | 2 | (256A + B) / 4 | RPM |
| 0x0D | 1 | A | km/h |
| 0x11 | 1 | A × 100 / 255 | % throttle |

**REQ** The OBD-II client shall support service modes 0x01 through 0x09.

**REQ** The OBD-II client shall provide a callback-based response model:
`omnican_obd2_request_pid()` is non-blocking; the registered callback fires
when a response arrives or the request times out.

**REQ** The OBD-II client shall support the following Mode 0x01 PIDs at
minimum: 0x00 (supported PIDs), 0x01 (monitor status), 0x04 (engine load),
0x05 (coolant temp), 0x0C (RPM), 0x0D (vehicle speed), 0x11 (throttle).

### OBD-II Mode and PID Coverage

| Mode | Description | OmniCAN support |
|---|---|---|
| 0x01 | Current data (PIDs) | Full: all supported PIDs enumerated via 0x00/0x20/../0xC0 |
| 0x02 | Freeze frame data | Full: same PID set as Mode 0x01, frame# sub-byte |
| 0x03 | Stored DTCs | Full: decoded P/C/B/U codes per SAE J2012 |
| 0x04 | Clear DTCs | Full: with completion callback |
| 0x05 | O2 sensor monitoring | Supported via mode=0x05 + OBDMID |
| 0x06 | On-board monitoring | Supported: OBDMID test results via callback |
| 0x07 | Pending DTCs | Full: same decode as Mode 0x03 |
| 0x08 | Control on-board system | Supported via generic mode request |
| 0x09 | Vehicle information | Full: VIN (0x02), CalID (0x04), CVN (0x06), ECU name (0x0A) |

### OBD-II DTC Format (SAE J2012)

A DTC is encoded as 2 bytes in Modes 0x03 and 0x07:

```
Byte 0 [7:6]: Type — 00=P (Powertrain), 01=C (Chassis), 10=B (Body), 11=U (Network)
Byte 0 [5:4]: Sub-type (0–3)
Byte 0 [3:0]: Digit 1 (hex)
Byte 1 [7:4]: Digit 2 (hex)
Byte 1 [3:0]: Digit 3 (hex)
```

Example: 0x01 0x34 → P0134 (O2 sensor circuit, no activity)

### OBD-II Callback Hooks

```c
/* Per-ECU PID response (fires once per responding ECU) */
typedef void (*omnican_obd2_response_cb_t)(
    uint8_t mode, uint8_t pid,
    uint8_t ecu_addr,           /* 0x7E8-0x7EF */
    const uint8_t *data, size_t len,
    void *user_data);

/* DTC record from Mode 0x03 / 0x07 */
typedef void (*omnican_obd2_dtc_cb_t)(
    uint8_t mode,               /* 0x03 or 0x07 */
    uint8_t ecu_addr,
    uint16_t dtc_raw,           /* raw 2-byte DTC encoding */
    char dtc_str[6],            /* decoded e.g. "P0134" */
    uint8_t status,             /* SAE J1979 DTC status byte */
    void *user_data);

/* No response received within timeout */
typedef void (*omnican_obd2_timeout_cb_t)(
    uint8_t mode, uint8_t pid,
    void *user_data);
```

### API
```c
int omnican_obd2_client_init(struct omnican_obd2_client *client,
                             struct omnican_node *base);
int omnican_obd2_request_pid(struct omnican_obd2_client *client,
                             uint8_t mode, uint8_t pid,
                             omnican_obd2_response_cb_t cb,
                             void *user_data,
                             uint32_t timeout_ms);
/* Physical addressing to a specific ECU */
int omnican_obd2_request_pid_physical(struct omnican_obd2_client *client,
                                       uint8_t ecu_addr, /* 0x00-0x07 */
                                       uint8_t mode, uint8_t pid,
                                       omnican_obd2_response_cb_t cb,
                                       void *user_data,
                                       uint32_t timeout_ms);
/* Request stored/pending DTCs */
int omnican_obd2_request_dtcs(struct omnican_obd2_client *client,
                               uint8_t mode, /* 0x03 or 0x07 */
                               omnican_obd2_dtc_cb_t cb,
                               void *user_data,
                               uint32_t timeout_ms);
/* Clear emission-related DTCs */
int omnican_obd2_clear_dtcs(struct omnican_obd2_client *client,
                             void (*on_complete)(int status, void *),
                             void *user_data,
                             uint32_t timeout_ms);
/* Set timeout callback */
void omnican_obd2_set_timeout_cb(struct omnican_obd2_client *client,
                                  omnican_obd2_timeout_cb_t cb, void *user_data);
```

## Phase 5 — CANopen FD CiA 1301 (`src/canopen_fd/`)

**Normative reference**: CiA 1301 v1.0.0. See `docs/STANDARDS.md §2.4`.

CANopen FD extends Phase 1 with CAN FD capabilities:

| Feature | Classical CANopen | CANopen FD |
|---|---|---|
| Max PDU size | 8 bytes | 64 bytes |
| Bit rate (data phase) | ≤ 1 Mbit/s | ≤ 8 Mbit/s (hardware-dependent) |
| SDO block transfer | Up to 7 bytes/segment | Up to 60 bytes/segment |
| PDO mapping | Up to 8 × 1-byte signals | Up to 8 × 8-byte signals |
| Backward compat | — | Negotiated; falls back to 8-byte |

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

**Workaround design**:

```mermaid
flowchart LR
    RX_Sock["RX socket\nbind(0x7E0)"] -->|"incoming request"| Shim
    Shim -->|"forward to UDS handler"| UDS_Handler["UDS / OBD-II Handler"]
    UDS_Handler -->|"response"| TX_Sock["TX socket\nbind(0x7E8)"]
    TX_Sock -->|"CAN frame"| Bus(["CAN Bus"])
    Bus -->|"CAN frame"| RX_Sock
```

Two separate ISO-TP socket contexts are used: one bound for receive on the request
ID, one for transmit on the response ID. The shim bridges them.

**REQ** When `CONFIG_OMNICAN_ISOTP_PATCH=y`, the module shall implement a
workaround that uses separate TX and RX socket contexts with a forwarding shim
so that UDS and OBD-II can operate on standard physical addresses.

**REQ** `CONFIG_OMNICAN_ISOTP_PATCH` shall default to `y` when either
`CONFIG_OMNICAN_UDS` or `CONFIG_OMNICAN_OBD2` is enabled.

## Thread and Execution Model

```
CAN ISR  (kernel context — runs at IRQ level)
  └─ Zephyr CAN driver → RX callback
        └─ k_work_submit() → System workqueue (cooperative thread)
              └─ Frame router
                    ├─ CANopen handler  (CAN ID 0x000–0x7FF)
                    ├─ J1939 handler   (extended 29-bit IDs)
                    ├─ ISO-TP RX shim  (0x7DF, 0x7E0–0x7E7)
                    └─ ISO-TP responses (0x7E8–0x7EF)

CANopen thread (dedicated, CONFIG_OMNICAN_CANOPEN_THREAD_PRIO = 7)
  └─ omnican_canopen_process() loop
        ├─ Heartbeat timer tick
        ├─ SDO timeout watchdog
        ├─ TPDO event timers
        └─ Heartbeat consumer watchdogs
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

### Memory Budget (Approximate)

| Component | Static RAM | Stack | Notes |
|---|---|---|---|
| `omnican_node` | ~24 B | — | 1 per CAN bus |
| CANopen context | ~512 B | 2048 B (thread) | OD size adds per entry (~12 B/entry) |
| J1939 node | ~96 B | — | + 64 B × MAX_SESSIONS |
| UDS server | ~256 B | — | + service table (8 B × MAX_SIDS) |
| OBD-II client | ~64 B | — | |
| Frame router | ~128 B | — | + 4 B × enabled protocols |
| net_buf pool | N × 72 B | — | N = CONFIG_OMNICAN_NET_BUF_COUNT |
| **Typical total** | **~2 KB** | **2 KB** | Varies with OD size and session counts |

## Full Kconfig Reference

| Symbol | Type | Default | Range | Description |
|---|---|---|---|---|
| `CONFIG_OMNICAN` | bool | n | — | Master enable. Requires `CAN`, selects `NET_BUF` |
| `CONFIG_OMNICAN_CANOPEN` | bool | n | — | Enable CANopen CiA 301. Requires `OMNICAN` |
| `CONFIG_OMNICAN_CANOPEN_FD` | bool | n | — | Enable CANopen FD CiA 1301. Requires `OMNICAN_CANOPEN`, `CAN_FD_MODE` |
| `CONFIG_OMNICAN_J1939` | bool | n | — | Enable SAE J1939. Requires `OMNICAN`, selects `CAN_ACCEPT_RTR` |
| `CONFIG_OMNICAN_UDS` | bool | n | — | Enable ISO 14229 UDS. Requires `OMNICAN`, `ISOTP` |
| `CONFIG_OMNICAN_OBD2` | bool | n | — | Enable SAE J1979 OBD-II. Requires `OMNICAN`, `ISOTP` |
| `CONFIG_OMNICAN_FRAME_ROUTER` | bool | auto | — | Auto-enabled when any protocol active |
| `CONFIG_OMNICAN_ISOTP_PATCH` | bool | auto | — | Auto-enabled when UDS or OBD2 active |
| `CONFIG_OMNICAN_NET_BUF_COUNT` | int | 16 | 4–64 | CAN frame net_buf pool size |
| `CONFIG_OMNICAN_CANOPEN_THREAD_STACK_SIZE` | int | 2048 | 1024–8192 | CANopen process thread stack (bytes) |
| `CONFIG_OMNICAN_CANOPEN_THREAD_PRIO` | int | 7 | 0–14 | CANopen process thread priority (cooperative) |
| `CONFIG_OMNICAN_HB_PERIOD_MS` | int | 1000 | 10–60000 | Heartbeat producer interval (ms) |
| `CONFIG_OMNICAN_SDO_TIMEOUT_MS` | int | 500 | 100–10000 | SDO server/client timeout (ms) |
| `CONFIG_OMNICAN_J1939_MAX_SESSIONS` | int | 4 | 1–16 | Max simultaneous TP/ETP sessions |
| `CONFIG_OMNICAN_J1939_MAX_PGN_ENTRIES` | int | 16 | 1–64 | Static PGN routing table size |
| `CONFIG_OMNICAN_UDS_S3_TIMEOUT_MS` | int | 5000 | 100–30000 | UDS S3 session timeout (ms) |
| `CONFIG_OMNICAN_UDS_MAX_AUTH_ATTEMPTS` | int | 3 | 1–10 | Max SecurityAccess attempts before lockout |
| `CONFIG_OMNICAN_OBD2_RESPONSE_TIMEOUT_MS` | int | 200 | 50–5000 | OBD-II PID response collection timeout (ms) |
| `CONFIG_OMNICAN_J1939_ETP_MAX_PAYLOAD_KB` | int | 64 | 1–16384 | ETP max single-transfer payload (KB); caps embedded memory use |
| `CONFIG_OMNICAN_J1939_DM_EXTENDED` | bool | n | — | Enable extended DM set (DM4, DM13, DM20–31). Requires `OMNICAN_J1939` |
| `CONFIG_OMNICAN_J1939_DM4_SPN_LIST` | string | "190,84,110,91" | — | Comma-separated SPNs captured in DM4 freeze frame |
| `CONFIG_OMNICAN_J1939_SLOT_TABLE` | bool | n | — | Include J1939DA SLOT decode table (426 SLOTs). Requires `OMNICAN_J1939` |
| `CONFIG_OMNICAN_J1939_SPN_EXTRA_LIST` | string | "" | — | Comma-separated extra SPN numbers to include beyond mandatory minimum |
| `CONFIG_OMNICAN_J1939_MFR_TABLE` | bool | n | — | Include manufacturer ID lookup table (1,509 entries from J1939DA B10) |
| `CONFIG_OMNICAN_J1939_IG_ADDRESSES` | bool | n | — | Include preferred address and NAME function tables (J1939DA B2/B11/B12) |
| `CONFIG_OMNICAN_LOG_LEVEL` | int | 3 (WRN) | 0–4 | Zephyr log level: 0=off 1=err 2=wrn 3=inf 4=dbg |

## Integration Guide

### Minimal Zephyr Application

```c
/* SPDX-License-Identifier: Apache-2.0 */
#include <zephyr/kernel.h>
#include <omnican/omnican.h>   /* pulls in all enabled protocol headers */

static struct omnican_node node;
static struct omnican_j1939_node j1939;

void main(void)
{
    const struct device *can = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));

    /* 1. Init shared node context */
    omnican_node_init(&node, can, 500 /* kbps */, false /* no FD */);

    /* 2. Init J1939 (example: Phase 2) */
    static const uint8_t my_name[8] = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x80, 0x00 };
    omnican_j1939_init(&j1939, &node, my_name);
    omnican_j1939_claim_address(&j1939, 0x28 /* SA=40 */, 500 /* ms */);

    /* 3. Register PGN receive callback */
    omnican_j1939_register_pgn(&j1939, 0xF004 /* EEC1 */, my_eec1_cb, NULL);

    /* 4. Application loop */
    while (1) {
        k_sleep(K_MSEC(100));
    }
}
```

### Kconfig Fragments (prj.conf)

```kconfig
# Enable J1939 only
CONFIG_CAN=y
CONFIG_OMNICAN=y
CONFIG_OMNICAN_J1939=y
CONFIG_OMNICAN_J1939_MAX_SESSIONS=4
CONFIG_OMNICAN_LOG_LEVEL=3
```

```kconfig
# Enable UDS (requires ISOTP)
CONFIG_CAN=y
CONFIG_ISOTP=y
CONFIG_OMNICAN=y
CONFIG_OMNICAN_UDS=y
CONFIG_OMNICAN_UDS_S3_TIMEOUT_MS=5000
CONFIG_OMNICAN_UDS_MAX_AUTH_ATTEMPTS=3
```

### west.yml Integration

Add OmniCAN as a west manifest project:

```yaml
projects:
  - name: omnican
    remote: bitconcepts
    revision: v0.1.0
    path: modules/lib/omnican
```

Then in `CMakeLists.txt`:
```cmake
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
target_sources(app PRIVATE src/main.c)
```

No additional includes needed — `zephyr/module.yml` registers `include/` automatically.

## Dependencies and Compatibility

| Dependency | Version | Notes |
|---|---|---|
| Zephyr RTOS | **v3.7.0** | Pinned in west.yml. See docs/STANDARDS.md §7 |
| Zephyr CAN driver API | (v3.7.0) | `zephyr/drivers/can.h` |
| Zephyr ISO-TP | (v3.7.0) | `zephyr/net/isotp.h`; issue #86025 patched internally |
| Zephyr net_buf | (v3.7.0) | `zephyr/net/buf.h` |
| CANopenNode | master (optional) | Reference only — not vendored |

**REQ** OmniCAN shall be buildable against Zephyr v3.7.0 with no patches to
Zephyr itself (the ISOTP patch lives within OmniCAN's own source tree).

**REQ** OmniCAN shall register as a Zephyr west module via `zephyr/module.yml`
so that `west build` automatically adds `include/` to the build include path.
