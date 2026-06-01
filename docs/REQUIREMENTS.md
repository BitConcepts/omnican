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

### REQ-ROUTER-005
- **Title**: Support simultaneous reception of 11-bit standard and 29-bit extended CAN frames
- **Status**: planned
- **Description**: Support simultaneous reception of 11-bit standard and 29-bit extended CAN frames on the same router instance
- **Source**: docs/ARCHITECTURE.md ## Frame Router
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-ROUTER-006
- **Title**: Support 29-bit extended CAN ID filtering for CANopen FD extended COB-IDs and ven
- **Status**: planned
- **Description**: Support 29-bit extended CAN ID filtering for CANopen FD extended COB-IDs and vendor-specific profiles
- **Source**: docs/ARCHITECTURE.md ## Frame Router
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-ROUTER-007
- **Title**: Provide configurable per-protocol CAN filter masks via Kconfig so applications c
- **Status**: planned
- **Description**: Provide configurable per-protocol CAN filter masks via Kconfig so applications can narrow or broaden reception ranges
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

### REQ-CO-014
- **Title**: Provide `omnican_canopen_nmt_change_cb_t` callback invoked on every NMT state tr
- **Status**: planned
- **Description**: Provide `omnican_canopen_nmt_change_cb_t` callback invoked on every NMT state transition (both self-initiated and master-commanded)
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-015
- **Title**: Provide `omnican_canopen_sdo_post_write_cb_t` callback after every successful SD
- **Status**: planned
- **Description**: Provide `omnican_canopen_sdo_post_write_cb_t` callback after every successful SDO write so the application can react to OD changes
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-016
- **Title**: Provide `omnican_canopen_rpdo_rx_cb_t` callback when RPDO data is received and m
- **Status**: planned
- **Description**: Provide `omnican_canopen_rpdo_rx_cb_t` callback when RPDO data is received and mapped into the OD
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-017
- **Title**: Provide `omnican_canopen_emcy_rx_cb_t` callback when an Emergency message from a
- **Status**: planned
- **Description**: Provide `omnican_canopen_emcy_rx_cb_t` callback when an Emergency message from a remote node is received
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-018
- **Title**: Provide `omnican_canopen_sync_cb_t` callback on SYNC message reception for appli
- **Status**: planned
- **Description**: Provide `omnican_canopen_sync_cb_t` callback on SYNC message reception for application-level synchronisation
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-019
- **Title**: Provide `omnican_canopen_hb_event_cb_t` callback for heartbeat consumer events d
- **Status**: planned
- **Description**: Provide `omnican_canopen_hb_event_cb_t` callback for heartbeat consumer events distinguishing timeout and recovery
- **Source**: docs/ARCHITECTURE.md ## CANopen CiA 301
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-CO-020
- **Title**: Provide `omnican_canopen_bootup_rx_cb_t` callback when a remote node Boot-Up mes
- **Status**: planned
- **Description**: Provide `omnican_canopen_bootup_rx_cb_t` callback when a remote node Boot-Up message is received
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
- **Title**: Explicitly implement ETP.CM (PGN 0xC800) and ETP.DT (PGN 0xC700) as separate ETP
- **Status**: planned
- **Description**: Explicitly implement ETP.CM (PGN 0xC800) and ETP.DT (PGN 0xC700) as separate ETP connection management and data transfer PGNs
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-010
- **Title**: Maintain TP/ETP session state in static context with configurable maximum simult
- **Status**: planned
- **Description**: Maintain TP/ETP session state in static context with configurable maximum simultaneous sessions
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-011
- **Title**: Implement RQST (PGN 0xEA00) to request any PGN from a remote device (mandatory f
- **Status**: planned
- **Description**: Implement RQST (PGN 0xEA00) to request any PGN from a remote device (mandatory for DM request/response)
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-012
- **Title**: Implement ACKM (PGN 0xE800) to send positive, negative, busy, and cannot-respond
- **Status**: planned
- **Description**: Implement ACKM (PGN 0xE800) to send positive, negative, busy, and cannot-respond acknowledgments (mandatory per J1939/21)
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-013
- **Title**: Support Proprietary A (PGN 0xEF00, peer-to-peer) and Proprietary B (PGN 0xFF00, 
- **Status**: planned
- **Description**: Support Proprietary A (PGN 0xEF00, peer-to-peer) and Proprietary B (PGN 0xFF00, broadcast) via the PGN routing table for manufacturer-specific messages
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-014
- **Title**: Provide `omnican_j1939_address_result_cb_t` callback reporting address claim suc
- **Status**: planned
- **Description**: Provide `omnican_j1939_address_result_cb_t` callback reporting address claim success, failure, or address-lost events
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-015
- **Title**: Provide `omnican_j1939_tp_progress_cb_t` callback reporting bytes transferred du
- **Status**: planned
- **Description**: Provide `omnican_j1939_tp_progress_cb_t` callback reporting bytes transferred during active TP/ETP sessions
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-016
- **Title**: Provide `omnican_j1939_tp_complete_cb_t` callback on TP/ETP session completion o
- **Status**: planned
- **Description**: Provide `omnican_j1939_tp_complete_cb_t` callback on TP/ETP session completion or abort with status
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JNET-017
- **Title**: Support configurable maximum TP/ETP single-transfer payload via CONFIG_OMNICAN_J
- **Status**: planned
- **Description**: Support configurable maximum TP/ETP single-transfer payload via CONFIG_OMNICAN_J1939_TP_MAX_PAYLOAD (default 1785 bytes for TP, unlimited for ETP)
- **Source**: docs/ARCHITECTURE.md ## J1939
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDTC-001
- **Title**: Implement 4-byte J1939 DTC encode/decode: SPN (19-bit) in bytes 1-3 bits [7:5], 
- **Status**: planned
- **Description**: Implement 4-byte J1939 DTC encode/decode: SPN (19-bit) in bytes 1-3 bits [7:5], FMI (5-bit) in byte 3 bits [4:0], CM (1-bit) in byte 4 bit [7], OC (7-bit) in byte 4 bits [6:0] per J1939/73
- **Source**: docs/ARCHITECTURE.md ## J1939 DTC and SPN
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDTC-002
- **Title**: Define all 32 FMI (Failure Mode Identifier) codes 0-31 per J1939/73 §4.3.2
- **Status**: planned
- **Description**: Define all 32 FMI (Failure Mode Identifier) codes 0-31 per J1939/73 §4.3.2
- **Source**: docs/ARCHITECTURE.md ## J1939 DTC and SPN
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDTC-003
- **Title**: Provide `omnican_j1939_dtc_encode(uint32_t spn, uint8_t fmi, uint8_t oc, bool cm
- **Status**: planned
- **Description**: Provide `omnican_j1939_dtc_encode(uint32_t spn, uint8_t fmi, uint8_t oc, bool cm, uint8_t out[4])` API
- **Source**: docs/ARCHITECTURE.md ## J1939 DTC and SPN
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDTC-004
- **Title**: Provide `omnican_j1939_dtc_decode(const uint8_t in[4], uint32_t *spn, uint8_t *f
- **Status**: planned
- **Description**: Provide `omnican_j1939_dtc_decode(const uint8_t in[4], uint32_t *spn, uint8_t *fmi, uint8_t *oc, bool *cm)` API
- **Source**: docs/ARCHITECTURE.md ## J1939 DTC and SPN
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDTC-005
- **Title**: Provide `omnican_j1939_fmi_string(uint8_t fmi)` to return human-readable FMI des
- **Status**: planned
- **Description**: Provide `omnican_j1939_fmi_string(uint8_t fmi)` to return human-readable FMI description
- **Source**: docs/ARCHITECTURE.md ## J1939 DTC and SPN
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDM-001
- **Title**: Implement DM1 (PGN 0xFECA) with correct 2-byte lamp status header: MIL [7:6], St
- **Status**: planned
- **Description**: Implement DM1 (PGN 0xFECA) with correct 2-byte lamp status header: MIL [7:6], Stop Lamp [5:4], Amber Warning [3:2], Protect [1:0] per J1939/73, plus repeated 4-byte DTC records
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (mandatory DM set)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDM-002
- **Title**: Implement DM2 (PGN 0xFECB) previously active DTCs with same frame format as DM1
- **Status**: planned
- **Description**: Implement DM2 (PGN 0xFECB) previously active DTCs with same frame format as DM1
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (mandatory DM set)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDM-003
- **Title**: Implement DM3 (PGN 0xFECC) clear/reset previously active DTCs (mandatory request
- **Status**: planned
- **Description**: Implement DM3 (PGN 0xFECC) clear/reset previously active DTCs (mandatory request handling)
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (mandatory DM set)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDM-004
- **Title**: Implement DM5 (PGN 0xFECE) diagnostic readiness with active/previously active DT
- **Status**: planned
- **Description**: Implement DM5 (PGN 0xFECE) diagnostic readiness with active/previously active DTC counts and supported/completed monitor bits
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (mandatory DM set)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDM-005
- **Title**: Implement DM6 (PGN 0xFECF) emission-related pending DTCs
- **Status**: planned
- **Description**: Implement DM6 (PGN 0xFECF) emission-related pending DTCs
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (mandatory DM set)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDM-006
- **Title**: Implement DM11 (PGN 0xFED3) clear/reset active DTCs (mandatory request handling)
- **Status**: planned
- **Description**: Implement DM11 (PGN 0xFED3) clear/reset active DTCs (mandatory request handling)
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (mandatory DM set)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDM-007
- **Title**: Implement DM12 (PGN 0xFED4) emission-related MIL-on DTCs
- **Status**: planned
- **Description**: Implement DM12 (PGN 0xFED4) emission-related MIL-on DTCs
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (mandatory DM set)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDM-008
- **Title**: Implement DM22 (PGN 0xC300) individual clear/reset of specific active and previo
- **Status**: planned
- **Description**: Implement DM22 (PGN 0xC300) individual clear/reset of specific active and previously active DTCs by SPN/FMI
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (mandatory DM set)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDM-009
- **Title**: Provide `omnican_j1939_dm1_cb_t` callback delivering lamp status and decoded DTC
- **Status**: planned
- **Description**: Provide `omnican_j1939_dm1_cb_t` callback delivering lamp status and decoded DTC list when DM1 is received
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (mandatory DM set)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDM-010
- **Title**: Provide `omnican_j1939_dm_request(struct omnican_j1939_node *, uint8_t dm_num, o
- **Status**: planned
- **Description**: Provide `omnican_j1939_dm_request(struct omnican_j1939_node *, uint8_t dm_num, omnican_j1939_addr_t dest)` to request a specific DM from a device
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (mandatory DM set)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDME-001
- **Title**: Implement DM4 (PGN 0xFECD) freeze frame parameters capture and reporting when CO
- **Status**: planned
- **Description**: Implement DM4 (PGN 0xFECD) freeze frame parameters capture and reporting when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (extended DM set — via CONFIG_OMNICAN_J1939_DM_EXTENDED)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDME-002
- **Title**: Implement DM13 (PGN 0xDF00) stop/start broadcast control when CONFIG_OMNICAN_J19
- **Status**: planned
- **Description**: Implement DM13 (PGN 0xDF00) stop/start broadcast control when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (extended DM set — via CONFIG_OMNICAN_J1939_DM_EXTENDED)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDME-003
- **Title**: Implement DM20 (PGN 0xC200) monitor performance ratio reporting when CONFIG_OMNI
- **Status**: planned
- **Description**: Implement DM20 (PGN 0xC200) monitor performance ratio reporting when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (extended DM set — via CONFIG_OMNICAN_J1939_DM_EXTENDED)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDME-004
- **Title**: Implement DM21 (PGN 0xC100) diagnostic readiness 2 when CONFIG_OMNICAN_J1939_DM_
- **Status**: planned
- **Description**: Implement DM21 (PGN 0xC100) diagnostic readiness 2 when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (extended DM set — via CONFIG_OMNICAN_J1939_DM_EXTENDED)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDME-005
- **Title**: Implement DM25 (PGN 0xFDB7) expanded freeze frame with SPN-indexed data when CON
- **Status**: planned
- **Description**: Implement DM25 (PGN 0xFDB7) expanded freeze frame with SPN-indexed data when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (extended DM set — via CONFIG_OMNICAN_J1939_DM_EXTENDED)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDME-006
- **Title**: Implement DM26 (PGN 0xFDB8) diagnostic readiness 3 when CONFIG_OMNICAN_J1939_DM_
- **Status**: planned
- **Description**: Implement DM26 (PGN 0xFDB8) diagnostic readiness 3 when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (extended DM set — via CONFIG_OMNICAN_J1939_DM_EXTENDED)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDME-007
- **Title**: Implement DM27 (PGN 0xFD82) all pending DTCs when CONFIG_OMNICAN_J1939_DM_EXTEND
- **Status**: planned
- **Description**: Implement DM27 (PGN 0xFD82) all pending DTCs when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (extended DM set — via CONFIG_OMNICAN_J1939_DM_EXTENDED)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDME-008
- **Title**: Implement DM28 (PGN 0xFD80) emission-related permanent DTCs when CONFIG_OMNICAN_
- **Status**: planned
- **Description**: Implement DM28 (PGN 0xFD80) emission-related permanent DTCs when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (extended DM set — via CONFIG_OMNICAN_J1939_DM_EXTENDED)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDME-009
- **Title**: Implement DM29 (PGN 0x9E00) DTC counts when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- **Status**: planned
- **Description**: Implement DM29 (PGN 0x9E00) DTC counts when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (extended DM set — via CONFIG_OMNICAN_J1939_DM_EXTENDED)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDME-010
- **Title**: Implement DM30 (PGN 0xA400) scaled test results by SPN when CONFIG_OMNICAN_J1939
- **Status**: planned
- **Description**: Implement DM30 (PGN 0xA400) scaled test results by SPN when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (extended DM set — via CONFIG_OMNICAN_J1939_DM_EXTENDED)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDME-011
- **Title**: Implement DM31 (PGN 0xA300) DTC-to-lamp association when CONFIG_OMNICAN_J1939_DM
- **Status**: planned
- **Description**: Implement DM31 (PGN 0xA300) DTC-to-lamp association when CONFIG_OMNICAN_J1939_DM_EXTENDED=y
- **Source**: docs/ARCHITECTURE.md ## J1939 Diagnostics (extended DM set — via CONFIG_OMNICAN_J1939_DM_EXTENDED)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JSLOT-001
- **Title**: Provide a compiled-in SLOT decode table derived from J1939DA DEC2024 (426 SLOTs)
- **Status**: planned
- **Description**: Provide a compiled-in SLOT decode table derived from J1939DA DEC2024 (426 SLOTs) when CONFIG_OMNICAN_J1939_SLOT_TABLE=y
- **Source**: docs/ARCHITECTURE.md ## J1939 SLOT and SPN Decoding (optional — CONFIG_OMNICAN_J1939_SLOT_TABLE)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JSLOT-002
- **Title**: Provide `omnican_j1939_spn_decode(uint32_t spn, const uint8_t *raw, size_t raw_l
- **Status**: planned
- **Description**: Provide `omnican_j1939_spn_decode(uint32_t spn, const uint8_t *raw, size_t raw_len, double *value, const char **unit)` API for converting raw SPN bytes to engineering units
- **Source**: docs/ARCHITECTURE.md ## J1939 SLOT and SPN Decoding (optional — CONFIG_OMNICAN_J1939_SLOT_TABLE)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JSLOT-003
- **Title**: Support configurable SPN subset inclusion via CONFIG_OMNICAN_J1939_SPN_INCLUDE l
- **Status**: planned
- **Description**: Support configurable SPN subset inclusion via CONFIG_OMNICAN_J1939_SPN_INCLUDE list to manage flash usage
- **Source**: docs/ARCHITECTURE.md ## J1939 SLOT and SPN Decoding (optional — CONFIG_OMNICAN_J1939_SLOT_TABLE)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDA-001
- **Title**: Provide compiled-in manufacturer ID lookup table from J1939DA B10 (1,509 entries
- **Status**: planned
- **Description**: Provide compiled-in manufacturer ID lookup table from J1939DA B10 (1,509 entries) when CONFIG_OMNICAN_J1939_MFR_TABLE=y
- **Source**: docs/ARCHITECTURE.md ## J1939 DA Reference Tables (optional)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDA-002
- **Title**: Provide `omnican_j1939_mfr_name(uint16_t mfr_id)` returning manufacturer name st
- **Status**: planned
- **Description**: Provide `omnican_j1939_mfr_name(uint16_t mfr_id)` returning manufacturer name string
- **Source**: docs/ARCHITECTURE.md ## J1939 DA Reference Tables (optional)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDA-003
- **Title**: Provide global preferred source address table from J1939DA B2 when CONFIG_OMNICA
- **Status**: planned
- **Description**: Provide global preferred source address table from J1939DA B2 when CONFIG_OMNICAN_J1939_IG_ADDRESSES=y
- **Source**: docs/ARCHITECTURE.md ## J1939 DA Reference Tables (optional)
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-JDA-004
- **Title**: Provide NAME function lookup from J1939DA B11/B12 when CONFIG_OMNICAN_J1939_IG_A
- **Status**: planned
- **Description**: Provide NAME function lookup from J1939DA B11/B12 when CONFIG_OMNICAN_J1939_IG_ADDRESSES=y
- **Source**: docs/ARCHITECTURE.md ## J1939 DA Reference Tables (optional)
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
- **Title**: Support SID 0x22 ReadDataByIdentifier with multi-DID requests (multiple DIDs in 
- **Status**: planned
- **Description**: Support SID 0x22 ReadDataByIdentifier with multi-DID requests (multiple DIDs in one 0x22 request)
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
- **Title**: Support SID 0x28 CommunicationControl with all sub-functions: enableRxAndTx (0x0
- **Status**: planned
- **Description**: Support SID 0x28 CommunicationControl with all sub-functions: enableRxAndTx (0x00), enableRxAndDisableTx (0x01), disableRxAndEnableTx (0x02), disableRxAndTx (0x03)
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-010
- **Title**: Support SID 0x2E WriteDataByIdentifier with application-supplied write validatio
- **Status**: planned
- **Description**: Support SID 0x2E WriteDataByIdentifier with application-supplied write validation callback before applying data
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-011
- **Title**: Support SID 0x31 RoutineControl with application-supplied start, stop, and reque
- **Status**: planned
- **Description**: Support SID 0x31 RoutineControl with application-supplied start, stop, and requestResults executor callbacks per routine ID
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
- **Title**: Support SID 0x85 ControlDTCSetting with application callback to enable or disabl
- **Status**: planned
- **Description**: Support SID 0x85 ControlDTCSetting with application callback to enable or disable DTC storage
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

### REQ-UDS-019
- **Title**: Support suppressPositiveResponse sub-function bit (0x80) per ISO 14229-1 §7.5.2.
- **Status**: planned
- **Description**: Support suppressPositiveResponse sub-function bit (0x80) per ISO 14229-1 §7.5.2.2
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-020
- **Title**: Implement NRC 0x78 (responsePending) when a service handler cannot respond withi
- **Status**: planned
- **Description**: Implement NRC 0x78 (responsePending) when a service handler cannot respond within p2Server_max and requires p2StarServer_max time
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-021
- **Title**: Support manufacturer-specific DID range (0xF200–0xFEFF) via application-register
- **Status**: planned
- **Description**: Support manufacturer-specific DID range (0xF200–0xFEFF) via application-registered DID handler callbacks with read and write hooks
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-022
- **Title**: Implement mandatory ECU identification DIDs: 0xF186 (activeSession), 0xF18B (ECU
- **Status**: planned
- **Description**: Implement mandatory ECU identification DIDs: 0xF186 (activeSession), 0xF18B (ECUManufacturingDate), 0xF18C (ECUSerialNumber), 0xF190 (VIN), 0xF195 (SystemSupplierECUSoftwareVersion)
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-023
- **Title**: Support functional addressing via CAN ID 0x7DF for UDS broadcast service request
- **Status**: planned
- **Description**: Support functional addressing via CAN ID 0x7DF for UDS broadcast service requests (TesterPresent, DiagnosticSessionControl)
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-024
- **Title**: Provide `omnican_uds_session_change_cb_t` callback on every session transition w
- **Status**: planned
- **Description**: Provide `omnican_uds_session_change_cb_t` callback on every session transition with old and new session type
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-025
- **Title**: Provide `omnican_uds_security_change_cb_t` callback on security access level gra
- **Status**: planned
- **Description**: Provide `omnican_uds_security_change_cb_t` callback on security access level grant or revocation
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-026
- **Title**: Provide `omnican_uds_pre_reset_cb_t` callback before ECUReset execution allowing
- **Status**: planned
- **Description**: Provide `omnican_uds_pre_reset_cb_t` callback before ECUReset execution allowing application to flush state
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-027
- **Title**: Support manufacturer-specific NRC values 0x80–0xFE via application NRC provider 
- **Status**: planned
- **Description**: Support manufacturer-specific NRC values 0x80–0xFE via application NRC provider callback registered per SID
- **Source**: docs/ARCHITECTURE.md ## UDS ISO 14229
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-UDS-028
- **Title**: Provide `omnican_uds_transfer_progress_cb_t` callback during 0x36 TransferData r
- **Status**: planned
- **Description**: Provide `omnican_uds_transfer_progress_cb_t` callback during 0x36 TransferData reporting block sequence and bytes transferred
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
- **Description**: Support Mode 0x01 PIDs: 0x00 (supported), 0x01 (monitor status), 0x04 (engine load), 0x05 (coolant temp), 0x0C (RPM), 0x0D (vehicle speed), 0x11 (throttle position)
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-005
- **Title**: Support Mode 0x01 PID support bitmap queries (PIDs 0x00, 0x20, 0x40, 0x60, 0x80,
- **Status**: planned
- **Description**: Support Mode 0x01 PID support bitmap queries (PIDs 0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0) to enumerate ECU-supported PIDs
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-006
- **Title**: Support Mode 0x01 extended PIDs: 0x14–0x1B (O2 sensor voltage/trim), 0x1C (OBD s
- **Status**: planned
- **Description**: Support Mode 0x01 extended PIDs: 0x14–0x1B (O2 sensor voltage/trim), 0x1C (OBD standard), 0x21 (distance since MIL on), 0x2F (fuel level), 0x31 (distance since codes cleared), 0x33 (barometric pressure), 0x46 (ambient air temperature), 0x49 (accelerator position), 0x51 (fuel type), 0x5C (engine oil temperature)
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-007
- **Title**: Support Mode 0x02 (Freeze Frame) PID requests using the same PID set as Mode 0x0
- **Status**: planned
- **Description**: Support Mode 0x02 (Freeze Frame) PID requests using the same PID set as Mode 0x01 with frame number sub-byte
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-008
- **Title**: Retrieve Mode 0x03 stored DTCs and Mode 0x07 pending DTCs as decoded fault codes
- **Status**: planned
- **Description**: Retrieve Mode 0x03 stored DTCs and Mode 0x07 pending DTCs as decoded fault codes per SAE J2012 (P/C/B/U + 4-digit hex)
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-009
- **Title**: Support Mode 0x04 clear all emission-related DTCs with application completion ca
- **Status**: planned
- **Description**: Support Mode 0x04 clear all emission-related DTCs with application completion callback
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-010
- **Title**: Support Mode 0x06 on-board monitoring test results (OBDMID and OBDMID test resul
- **Status**: planned
- **Description**: Support Mode 0x06 on-board monitoring test results (OBDMID and OBDMID test results) via callback
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-011
- **Title**: Support Mode 0x09 vehicle information: INFOTYPE 0x02 (VIN), 0x04 (calibration ID
- **Status**: planned
- **Description**: Support Mode 0x09 vehicle information: INFOTYPE 0x02 (VIN), 0x04 (calibration ID), 0x06 (CVN), 0x0A (ECU name)
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-012
- **Title**: Aggregate responses from multiple ECUs for broadcast requests and deliver each E
- **Status**: planned
- **Description**: Aggregate responses from multiple ECUs for broadcast requests and deliver each ECU response individually via per-ECU callback
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-013
- **Title**: Support ECU-specific physical addressing: send to 0x7E0–0x7E7 and receive from t
- **Status**: planned
- **Description**: Support ECU-specific physical addressing: send to 0x7E0–0x7E7 and receive from the corresponding 0x7E8–0x7EF response CAN ID
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-014
- **Title**: Provide `omnican_obd2_dtc_cb_t` callback delivering decoded DTC records (fault c
- **Status**: planned
- **Description**: Provide `omnican_obd2_dtc_cb_t` callback delivering decoded DTC records (fault code, status byte) from Mode 0x03/0x07 responses
- **Source**: docs/ARCHITECTURE.md ## OBD-II J1979
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-OBD-015
- **Title**: Provide `omnican_obd2_timeout_cb_t` callback when a PID request receives no resp
- **Status**: planned
- **Description**: Provide `omnican_obd2_timeout_cb_t` callback when a PID request receives no response within the configured timeout
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

### REQ-MPROTO-001
- **Title**: Support simultaneous operation of J1939 (29-bit) and UDS (11-bit) on the same CA
- **Status**: planned
- **Description**: Support simultaneous operation of J1939 (29-bit) and UDS (11-bit) on the same CAN bus without filter conflicts by distinguishing frame types at the Zephyr CAN driver level
- **Source**: docs/ARCHITECTURE.md ## Multi-Protocol Co-Existence
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-MPROTO-002
- **Title**: Support simultaneous operation of CANopen (11-bit) and UDS (11-bit) using non-ov
- **Status**: planned
- **Description**: Support simultaneous operation of CANopen (11-bit) and UDS (11-bit) using non-overlapping CAN ID ranges: CANopen 0x000-0x5FF is below UDS 0x7DF/0x7E0-0x7EF; document this co-existence guarantee
- **Source**: docs/ARCHITECTURE.md ## Multi-Protocol Co-Existence
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-MPROTO-003
- **Title**: Support simultaneous operation of CANopen (11-bit) and J1939 (29-bit) via frame 
- **Status**: planned
- **Description**: Support simultaneous operation of CANopen (11-bit) and J1939 (29-bit) via frame type distinction with no ID conflict
- **Source**: docs/ARCHITECTURE.md ## Multi-Protocol Co-Existence
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-MPROTO-004
- **Title**: Enforce a policy when OBD-II and UDS are both enabled: UDS holds exclusive owner
- **Status**: planned
- **Description**: Enforce a policy when OBD-II and UDS are both enabled: UDS holds exclusive ownership of CAN ID 0x7DF for functional addressing; OBD-II client shall use physical addressing (0x7E0-0x7E7) when UDS is co-active, or disable OBD-II functional broadcast
- **Source**: docs/ARCHITECTURE.md ## Multi-Protocol Co-Existence
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-MPROTO-005
- **Title**: Provide a `docs/MULTIPROTOCOL.md` integration guide documenting supported combin
- **Status**: planned
- **Description**: Provide a `docs/MULTIPROTOCOL.md` integration guide documenting supported combinations, CAN ID allocation maps, and Kconfig compatibility matrix for all 4-protocol combinations
- **Source**: docs/ARCHITECTURE.md ## Multi-Protocol Co-Existence
- **Platform**: embedded
- **Boundary**: Zephyr RTOS v3.7.0 — verified via ztest on native_sim

### REQ-MPROTO-006
- **Title**: Support cross-protocol error propagation: an `omnican_error_cb_t` global hook th
- **Status**: planned
- **Description**: Support cross-protocol error propagation: an `omnican_error_cb_t` global hook that receives errors from any enabled protocol with protocol ID, error code, and context
- **Source**: docs/ARCHITECTURE.md ## Multi-Protocol Co-Existence
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
