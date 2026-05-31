# OmniCAN — Requirements

**Status:** Draft — parsed from docs/ARCHITECTURE.md via specsmith.

---

## Naming Convention

```
REQ-<COMPONENT>-<NUMBER>
```

Component codes: CORE, ROUTER, CO (CANopen), JNET (J1939), UDS, OBD (OBD-II), COFD, ISOTP, MEM.

---

## Requirements

### REQ-CORE-001
- **Title**: Provide `omnican_node_init()` to bind a Zephyr CAN device, set bitrate, and enab
- **Status**: planned
- **Description**: Provide `omnican_node_init()` to bind a Zephyr CAN device, set bitrate, and enable FD mode
- **Source**: docs/ARCHITECTURE.md ## Core Node
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CORE-002
- **Title**: Return `OMNICAN_ERR_NODEV` from `omnican_node_init()` when the CAN device is not
- **Status**: planned
- **Description**: Return `OMNICAN_ERR_NODEV` from `omnican_node_init()` when the CAN device is not ready
- **Source**: docs/ARCHITECTURE.md ## Core Node
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CORE-003
- **Title**: Define `omnican_proto_t` bitmask with one bit per protocol (CANOPEN, J1939, UDS,
- **Status**: planned
- **Description**: Define `omnican_proto_t` bitmask with one bit per protocol (CANOPEN, J1939, UDS, OBD2)
- **Source**: docs/ARCHITECTURE.md ## Core Node
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CORE-004
- **Title**: Define `omnican_err_t` with codes OK, INVAL, NODEV, NOMEM, TIMEOUT, BUSY, PROTO,
- **Status**: planned
- **Description**: Define `omnican_err_t` with codes OK, INVAL, NODEV, NOMEM, TIMEOUT, BUSY, PROTO, UNSUPPORTED, IO
- **Source**: docs/ARCHITECTURE.md ## Core Node
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CORE-005
- **Title**: Register as a Zephyr west module via `zephyr/module.yml` so `west build` adds in
- **Status**: planned
- **Description**: Register as a Zephyr west module via `zephyr/module.yml` so `west build` adds include/ to the build path
- **Source**: docs/ARCHITECTURE.md ## Core Node
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CORE-006
- **Title**: Be buildable against Zephyr v3.7.0 with no patches to Zephyr itself
- **Status**: planned
- **Description**: Be buildable against Zephyr v3.7.0 with no patches to Zephyr itself
- **Source**: docs/ARCHITECTURE.md ## Core Node
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-ROUTER-001
- **Title**: Register exactly one CAN receive filter per enabled protocol
- **Status**: planned
- **Description**: Register exactly one CAN receive filter per enabled protocol
- **Source**: docs/ARCHITECTURE.md ## Frame Router
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-ROUTER-002
- **Title**: Dispatch received frames to the correct protocol handler without blocking the CA
- **Status**: planned
- **Description**: Dispatch received frames to the correct protocol handler without blocking the CAN ISR
- **Source**: docs/ARCHITECTURE.md ## Frame Router
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-ROUTER-003
- **Title**: Use a Zephyr system workqueue for all protocol callback dispatch
- **Status**: planned
- **Description**: Use a Zephyr system workqueue for all protocol callback dispatch
- **Source**: docs/ARCHITECTURE.md ## Frame Router
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-ROUTER-004
- **Title**: Auto-enable via CONFIG_OMNICAN_FRAME_ROUTER when any protocol is active
- **Status**: planned
- **Description**: Auto-enable via CONFIG_OMNICAN_FRAME_ROUTER when any protocol is active
- **Source**: docs/ARCHITECTURE.md ## Frame Router
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-001
- **Title**: Implement the NMT state machine with states Initialising, Pre-Operational, Opera
- **Status**: planned
- **Description**: Implement the NMT state machine with states Initialising, Pre-Operational, Operational, and Stopped per CiA 301
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-002
- **Title**: Transmit a Boot-Up message (NMT state 0x00) on transition from Initialising to P
- **Status**: planned
- **Description**: Transmit a Boot-Up message (NMT state 0x00) on transition from Initialising to Pre-Operational
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-003
- **Title**: Respond to NMT master commands (Start, Stop, Pre-Operational, Reset Node, Reset 
- **Status**: planned
- **Description**: Respond to NMT master commands (Start, Stop, Pre-Operational, Reset Node, Reset Communication) on CAN ID 0x000
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-004
- **Title**: Implement an SDO server supporting expedited, segmented, and block transfer mode
- **Status**: planned
- **Description**: Implement an SDO server supporting expedited, segmented, and block transfer modes per CiA 301
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-005
- **Title**: Return SDO abort code 0x05040000 on SDO timeout
- **Status**: planned
- **Description**: Return SDO abort code 0x05040000 on SDO timeout
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-006
- **Title**: Support TPDO and RPDO with event-driven and synchronisation-triggered transmissi
- **Status**: planned
- **Description**: Support TPDO and RPDO with event-driven and synchronisation-triggered transmission modes
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-007
- **Title**: Support compile-time PDO mappings via the Object Dictionary
- **Status**: planned
- **Description**: Support compile-time PDO mappings via the Object Dictionary
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-008
- **Title**: Produce Emergency messages (CAN ID 0x80 + node_id) with 16-bit error code and 8-
- **Status**: planned
- **Description**: Produce Emergency messages (CAN ID 0x80 + node_id) with 16-bit error code and 8-byte error register per CiA 301
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-009
- **Title**: Produce Heartbeat messages at a Kconfig-configurable interval (default 1000 ms) 
- **Status**: planned
- **Description**: Produce Heartbeat messages at a Kconfig-configurable interval (default 1000 ms) per CiA 301
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-010
- **Title**: Support Heartbeat consumer monitoring of remote nodes with error event on timeou
- **Status**: planned
- **Description**: Support Heartbeat consumer monitoring of remote nodes with error event on timeout
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-011
- **Title**: Represent the Object Dictionary as a static C array generated from an OD descrip
- **Status**: planned
- **Description**: Represent the Object Dictionary as a static C array generated from an OD descriptor with no heap allocation
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-012
- **Title**: Write recommended delay to next `omnican_canopen_process()` call in microseconds
- **Status**: planned
- **Description**: Write recommended delay to next `omnican_canopen_process()` call in microseconds via `*next_us`
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-013
- **Title**: Run the CANopen process loop in a dedicated Zephyr thread with configurable stac
- **Status**: planned
- **Description**: Run the CANopen process loop in a dedicated Zephyr thread with configurable stack size and priority
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-001
- **Title**: Use 29-bit extended CAN identifiers and reject 11-bit standard ID frames
- **Status**: planned
- **Description**: Use 29-bit extended CAN identifiers and reject 11-bit standard ID frames
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-002
- **Title**: Implement the address-claiming procedure per J1939/81 with 250 ms contention win
- **Status**: planned
- **Description**: Implement the address-claiming procedure per J1939/81 with 250 ms contention window and 64-bit NAME priority
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-003
- **Title**: Prevent PGN transmission until address claiming succeeds
- **Status**: planned
- **Description**: Prevent PGN transmission until address claiming succeeds
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-004
- **Title**: Return OMNICAN_ERR_BUSY and set addr to NULL when address claiming fails
- **Status**: planned
- **Description**: Return OMNICAN_ERR_BUSY and set addr to NULL when address claiming fails
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-005
- **Title**: Support a static PGN routing table populated via `omnican_j1939_register_pgn()` 
- **Status**: planned
- **Description**: Support a static PGN routing table populated via `omnican_j1939_register_pgn()` with callback and user-data
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-006
- **Title**: Invoke PGN callbacks from the system workqueue with source address, data pointer
- **Status**: planned
- **Description**: Invoke PGN callbacks from the system workqueue with source address, data pointer, and length
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-007
- **Title**: Implement J1939 Transport Protocol (TP) for 9-1785 byte messages using BAM and C
- **Status**: planned
- **Description**: Implement J1939 Transport Protocol (TP) for 9-1785 byte messages using BAM and CMDT modes
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-008
- **Title**: Implement Extended Transport Protocol (ETP) for messages larger than 1785 bytes
- **Status**: planned
- **Description**: Implement Extended Transport Protocol (ETP) for messages larger than 1785 bytes
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-009
- **Title**: Maintain TP/ETP session state in static context with configurable maximum simult
- **Status**: planned
- **Description**: Maintain TP/ETP session state in static context with configurable maximum simultaneous sessions
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-001
- **Title**: Use Zephyr ISO-TP for all UDS message transport with ISOTP patch when CONFIG_OMN
- **Status**: planned
- **Description**: Use Zephyr ISO-TP for all UDS message transport with ISOTP patch when CONFIG_OMNICAN_ISOTP_PATCH=y
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-002
- **Title**: Implement DiagnosticSessionControl (SID 0x10) and enforce service availability p
- **Status**: planned
- **Description**: Implement DiagnosticSessionControl (SID 0x10) and enforce service availability per active session
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-003
- **Title**: Maintain an S3 timer (default 5000 ms) and revert to Default session on expiry
- **Status**: planned
- **Description**: Maintain an S3 timer (default 5000 ms) and revert to Default session on expiry
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-004
- **Title**: Reset the S3 timer on TesterPresent (SID 0x3E)
- **Status**: planned
- **Description**: Reset the S3 timer on TesterPresent (SID 0x3E)
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-005
- **Title**: Support SID 0x10 DiagnosticSessionControl
- **Status**: planned
- **Description**: Support SID 0x10 DiagnosticSessionControl
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-006
- **Title**: Support SID 0x11 ECUReset with hardReset, softReset, and keyOffOnReset sub-funct
- **Status**: planned
- **Description**: Support SID 0x11 ECUReset with hardReset, softReset, and keyOffOnReset sub-functions
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-007
- **Title**: Support SID 0x22 ReadDataByIdentifier
- **Status**: planned
- **Description**: Support SID 0x22 ReadDataByIdentifier
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-008
- **Title**: Support SID 0x27 SecurityAccess with application-supplied seed/key callback
- **Status**: planned
- **Description**: Support SID 0x27 SecurityAccess with application-supplied seed/key callback
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-009
- **Title**: Support SID 0x28 CommunicationControl
- **Status**: planned
- **Description**: Support SID 0x28 CommunicationControl
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-010
- **Title**: Support SID 0x2E WriteDataByIdentifier
- **Status**: planned
- **Description**: Support SID 0x2E WriteDataByIdentifier
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-011
- **Title**: Support SID 0x31 RoutineControl with start, stop, and requestResults
- **Status**: planned
- **Description**: Support SID 0x31 RoutineControl with start, stop, and requestResults
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-012
- **Title**: Support SID 0x34 RequestDownload
- **Status**: planned
- **Description**: Support SID 0x34 RequestDownload
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-013
- **Title**: Support SID 0x36 TransferData
- **Status**: planned
- **Description**: Support SID 0x36 TransferData
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-014
- **Title**: Support SID 0x37 RequestTransferExit
- **Status**: planned
- **Description**: Support SID 0x37 RequestTransferExit
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-015
- **Title**: Support SID 0x85 ControlDTCSetting
- **Status**: planned
- **Description**: Support SID 0x85 ControlDTCSetting
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-016
- **Title**: Return NRC 0x11 (serviceNotSupported) for unregistered SIDs
- **Status**: planned
- **Description**: Return NRC 0x11 (serviceNotSupported) for unregistered SIDs
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-017
- **Title**: Never use a hardcoded SecurityAccess seed/key algorithm; always use an applicati
- **Status**: planned
- **Description**: Never use a hardcoded SecurityAccess seed/key algorithm; always use an application callback
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-018
- **Title**: Lock the session and return NRC 0x36 after exceeding CONFIG_OMNICAN_UDS_MAX_AUTH
- **Status**: planned
- **Description**: Lock the session and return NRC 0x36 after exceeding CONFIG_OMNICAN_UDS_MAX_AUTH_ATTEMPTS failed SecurityAccess attempts
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-001
- **Title**: Transmit PID requests on CAN ID 0x7DF and collect responses from 0x7E8-0x7EF wit
- **Status**: planned
- **Description**: Transmit PID requests on CAN ID 0x7DF and collect responses from 0x7E8-0x7EF within configurable timeout
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-002
- **Title**: Support OBD-II service modes 0x01 through 0x09
- **Status**: planned
- **Description**: Support OBD-II service modes 0x01 through 0x09
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-003
- **Title**: Provide callback-based non-blocking PID request/response model
- **Status**: planned
- **Description**: Provide callback-based non-blocking PID request/response model
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-004
- **Title**: Support Mode 0x01 PIDs: 0x00 (supported), 0x01 (monitor status), 0x04 (engine lo
- **Status**: planned
- **Description**: Support Mode 0x01 PIDs: 0x00 (supported), 0x01 (monitor status), 0x04 (engine load), 0x05 (coolant), 0x0C (RPM), 0x0D (speed), 0x11 (throttle)
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-COFD-001
- **Title**: Extend CANopen module with CiA 1301 FD features; require CONFIG_OMNICAN_CANOPEN=
- **Status**: planned
- **Description**: Extend CANopen module with CiA 1301 FD features; require CONFIG_OMNICAN_CANOPEN=y and CONFIG_CAN_FD_MODE=y
- **Source**: docs/ARCHITECTURE.md ## CANopen FD CiA 1301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-COFD-002
- **Title**: Support CAN FD data payloads of up to 64 bytes for higher-throughput PDO transfe
- **Status**: planned
- **Description**: Support CAN FD data payloads of up to 64 bytes for higher-throughput PDO transfers
- **Source**: docs/ARCHITECTURE.md ## CANopen FD CiA 1301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-COFD-003
- **Title**: Maintain backward compatibility with classical 8-byte CANopen nodes when FD is n
- **Status**: planned
- **Description**: Maintain backward compatibility with classical 8-byte CANopen nodes when FD is not negotiated
- **Source**: docs/ARCHITECTURE.md ## CANopen FD CiA 1301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-ISOTP-001
- **Title**: Implement a workaround for Zephyr issue #86025 using separate TX/RX socket conte
- **Status**: planned
- **Description**: Implement a workaround for Zephyr issue #86025 using separate TX/RX socket contexts with a forwarding shim
- **Source**: docs/ARCHITECTURE.md ## ISOTP Patch
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-ISOTP-002
- **Title**: Default CONFIG_OMNICAN_ISOTP_PATCH to y when CONFIG_OMNICAN_UDS or CONFIG_OMNICA
- **Status**: planned
- **Description**: Default CONFIG_OMNICAN_ISOTP_PATCH to y when CONFIG_OMNICAN_UDS or CONFIG_OMNICAN_OBD2 is enabled
- **Source**: docs/ARCHITECTURE.md ## ISOTP Patch
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-MEM-001
- **Title**: Never use dynamic heap allocation (k_malloc, malloc); all state statically alloc
- **Status**: planned
- **Description**: Never use dynamic heap allocation (k_malloc, malloc); all state statically allocated or via net_buf pools
- **Source**: docs/ARCHITECTURE.md ## Memory and Threading
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-MEM-002
- **Title**: Handle CAN frame data via Zephyr net_buf from a pool sized by CONFIG_OMNICAN_NET
- **Status**: planned
- **Description**: Handle CAN frame data via Zephyr net_buf from a pool sized by CONFIG_OMNICAN_NET_BUF_COUNT (default 16)
- **Source**: docs/ARCHITECTURE.md ## Memory and Threading
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-MEM-003
- **Title**: All protocol context structures shall be opaque to the caller but caller-allocat
- **Status**: planned
- **Description**: All protocol context structures shall be opaque to the caller but caller-allocated
- **Source**: docs/ARCHITECTURE.md ## Memory and Threading
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-MEM-004
- **Title**: All protocol handlers execute in a Zephyr workqueue context, not the CAN ISR
- **Status**: planned
- **Description**: All protocol handlers execute in a Zephyr workqueue context, not the CAN ISR
- **Source**: docs/ARCHITECTURE.md ## Memory and Threading
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim
