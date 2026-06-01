# OmniCAN — Test Specification

**Status:** Draft — generated from requirements via specsmith.

---

## Tests

### TEST-001
- Covers: REQ-CORE-001
- **Type**: Unit
- **Description**: Verify provide `omnican_node_init()` to bind a zephyr can device, set bitrate, and enable fd mode
- **Status**: Pending

### TEST-002
- Covers: REQ-CORE-002
- **Type**: Unit
- **Description**: Verify return `omnican_err_nodev` from `omnican_node_init()` when the can device is not ready
- **Status**: Pending

### TEST-003
- Covers: REQ-CORE-003
- **Type**: Unit
- **Description**: Verify define `omnican_proto_t` bitmask with one bit per protocol (canopen, j1939, uds, obd2)
- **Status**: Pending

### TEST-004
- Covers: REQ-CORE-004
- **Type**: Unit
- **Description**: Verify define `omnican_err_t` with codes ok, inval, nodev, nomem, timeout, busy, proto, unsupported, io
- **Status**: Pending

### TEST-005
- Covers: REQ-CORE-005
- **Type**: Unit
- **Description**: Verify register as a zephyr west module via `zephyr/module.yml` so `west build` adds include/ to the build path
- **Status**: Pending

### TEST-006
- Covers: REQ-CORE-006
- **Type**: Unit
- **Description**: Verify be buildable against zephyr v3.7.0 with no patches to zephyr itself
- **Status**: Pending

### TEST-007
- Covers: REQ-ROUTER-001
- **Type**: Unit
- **Description**: Verify register exactly one can receive filter per enabled protocol
- **Status**: Pending

### TEST-008
- Covers: REQ-ROUTER-002
- **Type**: Unit
- **Description**: Verify dispatch received frames to the correct protocol handler without blocking the can isr
- **Status**: Pending

### TEST-009
- Covers: REQ-ROUTER-003
- **Type**: Unit
- **Description**: Verify use a zephyr system workqueue for all protocol callback dispatch
- **Status**: Pending

### TEST-010
- Covers: REQ-ROUTER-004
- **Type**: Unit
- **Description**: Verify auto-enable via config_omnican_frame_router when any protocol is active
- **Status**: Pending

### TEST-011
- Covers: REQ-ROUTER-005
- **Type**: Unit
- **Description**: Verify support simultaneous reception of 11-bit standard and 29-bit extended can frames on the same router instance
- **Status**: Pending

### TEST-012
- Covers: REQ-ROUTER-006
- **Type**: Unit
- **Description**: Verify support 29-bit extended can id filtering for canopen fd extended cob-ids and vendor-specific profiles
- **Status**: Pending

### TEST-013
- Covers: REQ-ROUTER-007
- **Type**: Unit
- **Description**: Verify provide configurable per-protocol can filter masks via kconfig so applications can narrow or broaden reception ranges
- **Status**: Pending

### TEST-014
- Covers: REQ-CO-001
- **Type**: Unit
- **Description**: Verify implement the nmt state machine with states initialising, pre-operational, operational, and stopped per cia 301
- **Status**: Pending

### TEST-015
- Covers: REQ-CO-002
- **Type**: Unit
- **Description**: Verify transmit a boot-up message (nmt state 0x00) on transition from initialising to pre-operational
- **Status**: Pending

### TEST-016
- Covers: REQ-CO-003
- **Type**: Unit
- **Description**: Verify respond to nmt master commands (start, stop, pre-operational, reset node, reset communication) on can id 0x000
- **Status**: Pending

### TEST-017
- Covers: REQ-CO-004
- **Type**: Unit
- **Description**: Verify implement an sdo server supporting expedited, segmented, and block transfer modes per cia 301
- **Status**: Pending

### TEST-018
- Covers: REQ-CO-005
- **Type**: Unit
- **Description**: Verify return sdo abort code 0x05040000 on sdo timeout
- **Status**: Pending

### TEST-019
- Covers: REQ-CO-006
- **Type**: Unit
- **Description**: Verify support tpdo and rpdo with event-driven and synchronisation-triggered transmission modes
- **Status**: Pending

### TEST-020
- Covers: REQ-CO-007
- **Type**: Unit
- **Description**: Verify support compile-time pdo mappings via the object dictionary
- **Status**: Pending

### TEST-021
- Covers: REQ-CO-008
- **Type**: Unit
- **Description**: Verify produce emergency messages (can id 0x80 + node_id) with 16-bit error code and 8-byte error register per cia 301
- **Status**: Pending

### TEST-022
- Covers: REQ-CO-009
- **Type**: Unit
- **Description**: Verify produce heartbeat messages at a kconfig-configurable interval (default 1000 ms) per cia 301
- **Status**: Pending

### TEST-023
- Covers: REQ-CO-010
- **Type**: Unit
- **Description**: Verify support heartbeat consumer monitoring of remote nodes with error event on timeout
- **Status**: Pending

### TEST-024
- Covers: REQ-CO-011
- **Type**: Unit
- **Description**: Verify represent the object dictionary as a static c array generated from an od descriptor with no heap allocation
- **Status**: Pending

### TEST-025
- Covers: REQ-CO-012
- **Type**: Unit
- **Description**: Verify write recommended delay to next `omnican_canopen_process()` call in microseconds via `*next_us`
- **Status**: Pending

### TEST-026
- Covers: REQ-CO-013
- **Type**: Unit
- **Description**: Verify run the canopen process loop in a dedicated zephyr thread with configurable stack size and priority
- **Status**: Pending

### TEST-027
- Covers: REQ-CO-014
- **Type**: Unit
- **Description**: Verify provide `omnican_canopen_nmt_change_cb_t` callback invoked on every nmt state transition (both self-initiated and master-commanded)
- **Status**: Pending

### TEST-028
- Covers: REQ-CO-015
- **Type**: Unit
- **Description**: Verify provide `omnican_canopen_sdo_post_write_cb_t` callback after every successful sdo write so the application can react to od changes
- **Status**: Pending

### TEST-029
- Covers: REQ-CO-016
- **Type**: Unit
- **Description**: Verify provide `omnican_canopen_rpdo_rx_cb_t` callback when rpdo data is received and mapped into the od
- **Status**: Pending

### TEST-030
- Covers: REQ-CO-017
- **Type**: Unit
- **Description**: Verify provide `omnican_canopen_emcy_rx_cb_t` callback when an emergency message from a remote node is received
- **Status**: Pending

### TEST-031
- Covers: REQ-CO-018
- **Type**: Unit
- **Description**: Verify provide `omnican_canopen_sync_cb_t` callback on sync message reception for application-level synchronisation
- **Status**: Pending

### TEST-032
- Covers: REQ-CO-019
- **Type**: Unit
- **Description**: Verify provide `omnican_canopen_hb_event_cb_t` callback for heartbeat consumer events distinguishing timeout and recovery
- **Status**: Pending

### TEST-033
- Covers: REQ-CO-020
- **Type**: Unit
- **Description**: Verify provide `omnican_canopen_bootup_rx_cb_t` callback when a remote node boot-up message is received
- **Status**: Pending

### TEST-034
- Covers: REQ-JNET-001
- **Type**: Unit
- **Description**: Verify use 29-bit extended can identifiers and reject 11-bit standard id frames
- **Status**: Pending

### TEST-035
- Covers: REQ-JNET-002
- **Type**: Unit
- **Description**: Verify implement the address-claiming procedure per j1939/81 with 250 ms contention window and 64-bit name priority
- **Status**: Pending

### TEST-036
- Covers: REQ-JNET-003
- **Type**: Unit
- **Description**: Verify prevent pgn transmission until address claiming succeeds
- **Status**: Pending

### TEST-037
- Covers: REQ-JNET-004
- **Type**: Unit
- **Description**: Verify return omnican_err_busy and set addr to null when address claiming fails
- **Status**: Pending

### TEST-038
- Covers: REQ-JNET-005
- **Type**: Unit
- **Description**: Verify support a static pgn routing table populated via `omnican_j1939_register_pgn()` with callback and user-data
- **Status**: Pending

### TEST-039
- Covers: REQ-JNET-006
- **Type**: Unit
- **Description**: Verify invoke pgn callbacks from the system workqueue with source address, data pointer, and length
- **Status**: Pending

### TEST-040
- Covers: REQ-JNET-007
- **Type**: Unit
- **Description**: Verify implement j1939 transport protocol (tp) for 9-1785 byte messages using bam and cmdt modes
- **Status**: Pending

### TEST-041
- Covers: REQ-JNET-008
- **Type**: Unit
- **Description**: Verify implement extended transport protocol (etp) for messages larger than 1785 bytes
- **Status**: Pending

### TEST-042
- Covers: REQ-JNET-009
- **Type**: Unit
- **Description**: Verify maintain tp/etp session state in static context with configurable maximum simultaneous sessions
- **Status**: Pending

### TEST-043
- Covers: REQ-JNET-010
- **Type**: Unit
- **Description**: Verify provide `omnican_j1939_address_result_cb_t` callback reporting address claim success, failure, or address-lost events
- **Status**: Pending

### TEST-044
- Covers: REQ-JNET-011
- **Type**: Unit
- **Description**: Verify provide `omnican_j1939_tp_progress_cb_t` callback reporting bytes transferred during active tp/etp sessions
- **Status**: Pending

### TEST-045
- Covers: REQ-JNET-012
- **Type**: Unit
- **Description**: Verify provide `omnican_j1939_tp_complete_cb_t` callback on tp/etp session completion or abort with status
- **Status**: Pending

### TEST-046
- Covers: REQ-JNET-013
- **Type**: Unit
- **Description**: Verify support j1939 diagnostic pgn reception: dm1 (pgn 0xfeca, active dtcs) and dm11 (pgn 0xfed3, clear active dtcs) via the pgn routing table
- **Status**: Pending

### TEST-047
- Covers: REQ-JNET-014
- **Type**: Unit
- **Description**: Verify support configurable maximum tp/etp single-transfer payload via config_omnican_j1939_tp_max_payload (default 1785 bytes for tp, unlimited for etp)
- **Status**: Pending

### TEST-048
- Covers: REQ-UDS-001
- **Type**: Unit
- **Description**: Verify use zephyr iso-tp for all uds message transport with isotp patch when config_omnican_isotp_patch=y
- **Status**: Pending

### TEST-049
- Covers: REQ-UDS-002
- **Type**: Unit
- **Description**: Verify implement diagnosticsessioncontrol (sid 0x10) and enforce service availability per active session
- **Status**: Pending

### TEST-050
- Covers: REQ-UDS-003
- **Type**: Unit
- **Description**: Verify maintain an s3 timer (default 5000 ms) and revert to default session on expiry
- **Status**: Pending

### TEST-051
- Covers: REQ-UDS-004
- **Type**: Unit
- **Description**: Verify reset the s3 timer on testerpresent (sid 0x3e)
- **Status**: Pending

### TEST-052
- Covers: REQ-UDS-005
- **Type**: Unit
- **Description**: Verify support sid 0x10 diagnosticsessioncontrol
- **Status**: Pending

### TEST-053
- Covers: REQ-UDS-006
- **Type**: Unit
- **Description**: Verify support sid 0x11 ecureset with hardreset, softreset, and keyoffonreset sub-functions
- **Status**: Pending

### TEST-054
- Covers: REQ-UDS-007
- **Type**: Unit
- **Description**: Verify support sid 0x22 readdatabyidentifier with multi-did requests (multiple dids in one 0x22 request)
- **Status**: Pending

### TEST-055
- Covers: REQ-UDS-008
- **Type**: Unit
- **Description**: Verify support sid 0x27 securityaccess with application-supplied seed/key callback
- **Status**: Pending

### TEST-056
- Covers: REQ-UDS-009
- **Type**: Unit
- **Description**: Verify support sid 0x28 communicationcontrol with all sub-functions: enablerxandtx (0x00), enablerxanddisabletx (0x01), disablerxandenabletx (0x02), disablerxandtx (0x03)
- **Status**: Pending

### TEST-057
- Covers: REQ-UDS-010
- **Type**: Unit
- **Description**: Verify support sid 0x2e writedatabyidentifier with application-supplied write validation callback before applying data
- **Status**: Pending

### TEST-058
- Covers: REQ-UDS-011
- **Type**: Unit
- **Description**: Verify support sid 0x31 routinecontrol with application-supplied start, stop, and requestresults executor callbacks per routine id
- **Status**: Pending

### TEST-059
- Covers: REQ-UDS-012
- **Type**: Unit
- **Description**: Verify support sid 0x34 requestdownload
- **Status**: Pending

### TEST-060
- Covers: REQ-UDS-013
- **Type**: Unit
- **Description**: Verify support sid 0x36 transferdata
- **Status**: Pending

### TEST-061
- Covers: REQ-UDS-014
- **Type**: Unit
- **Description**: Verify support sid 0x37 requesttransferexit
- **Status**: Pending

### TEST-062
- Covers: REQ-UDS-015
- **Type**: Unit
- **Description**: Verify support sid 0x85 controldtcsetting with application callback to enable or disable dtc storage
- **Status**: Pending

### TEST-063
- Covers: REQ-UDS-016
- **Type**: Unit
- **Description**: Verify return nrc 0x11 (servicenotsupported) for unregistered sids
- **Status**: Pending

### TEST-064
- Covers: REQ-UDS-017
- **Type**: Unit
- **Description**: Verify never use a hardcoded securityaccess seed/key algorithm; always use an application callback
- **Status**: Pending

### TEST-065
- Covers: REQ-UDS-018
- **Type**: Unit
- **Description**: Verify lock the session and return nrc 0x36 after exceeding config_omnican_uds_max_auth_attempts failed securityaccess attempts
- **Status**: Pending

### TEST-066
- Covers: REQ-UDS-019
- **Type**: Unit
- **Description**: Verify support suppresspositiveresponse sub-function bit (0x80) per iso 14229-1 §7.5.2.2
- **Status**: Pending

### TEST-067
- Covers: REQ-UDS-020
- **Type**: Unit
- **Description**: Verify implement nrc 0x78 (responsepending) when a service handler cannot respond within p2server_max and requires p2starserver_max time
- **Status**: Pending

### TEST-068
- Covers: REQ-UDS-021
- **Type**: Unit
- **Description**: Verify support manufacturer-specific did range (0xf200–0xfeff) via application-registered did handler callbacks with read and write hooks
- **Status**: Pending

### TEST-069
- Covers: REQ-UDS-022
- **Type**: Unit
- **Description**: Verify implement mandatory ecu identification dids: 0xf186 (activesession), 0xf18b (ecumanufacturingdate), 0xf18c (ecuserialnumber), 0xf190 (vin), 0xf195 (systemsupplierecusoftwareversion)
- **Status**: Pending

### TEST-070
- Covers: REQ-UDS-023
- **Type**: Unit
- **Description**: Verify support functional addressing via can id 0x7df for uds broadcast service requests (testerpresent, diagnosticsessioncontrol)
- **Status**: Pending

### TEST-071
- Covers: REQ-UDS-024
- **Type**: Unit
- **Description**: Verify provide `omnican_uds_session_change_cb_t` callback on every session transition with old and new session type
- **Status**: Pending

### TEST-072
- Covers: REQ-UDS-025
- **Type**: Unit
- **Description**: Verify provide `omnican_uds_security_change_cb_t` callback on security access level grant or revocation
- **Status**: Pending

### TEST-073
- Covers: REQ-UDS-026
- **Type**: Unit
- **Description**: Verify provide `omnican_uds_pre_reset_cb_t` callback before ecureset execution allowing application to flush state
- **Status**: Pending

### TEST-074
- Covers: REQ-UDS-027
- **Type**: Unit
- **Description**: Verify support manufacturer-specific nrc values 0x80–0xfe via application nrc provider callback registered per sid
- **Status**: Pending

### TEST-075
- Covers: REQ-UDS-028
- **Type**: Unit
- **Description**: Verify provide `omnican_uds_transfer_progress_cb_t` callback during 0x36 transferdata reporting block sequence and bytes transferred
- **Status**: Pending

### TEST-076
- Covers: REQ-OBD-001
- **Type**: Unit
- **Description**: Verify transmit pid requests on can id 0x7df and collect responses from 0x7e8-0x7ef within configurable timeout
- **Status**: Pending

### TEST-077
- Covers: REQ-OBD-002
- **Type**: Unit
- **Description**: Verify support obd-ii service modes 0x01 through 0x09
- **Status**: Pending

### TEST-078
- Covers: REQ-OBD-003
- **Type**: Unit
- **Description**: Verify provide callback-based non-blocking pid request/response model
- **Status**: Pending

### TEST-079
- Covers: REQ-OBD-004
- **Type**: Unit
- **Description**: Verify support mode 0x01 pids: 0x00 (supported), 0x01 (monitor status), 0x04 (engine load), 0x05 (coolant temp), 0x0c (rpm), 0x0d (vehicle speed), 0x11 (throttle position)
- **Status**: Pending

### TEST-080
- Covers: REQ-OBD-005
- **Type**: Unit
- **Description**: Verify support mode 0x01 pid support bitmap queries (pids 0x00, 0x20, 0x40, 0x60, 0x80, 0xa0, 0xc0) to enumerate ecu-supported pids
- **Status**: Pending

### TEST-081
- Covers: REQ-OBD-006
- **Type**: Unit
- **Description**: Verify support mode 0x01 extended pids: 0x14–0x1b (o2 sensor voltage/trim), 0x1c (obd standard), 0x21 (distance since mil on), 0x2f (fuel level), 0x31 (distance since codes cleared), 0x33 (barometric pressure), 0x46 (ambient air temperature), 0x49 (accelerator position), 0x51 (fuel type), 0x5c (engine oil temperature)
- **Status**: Pending

### TEST-082
- Covers: REQ-OBD-007
- **Type**: Unit
- **Description**: Verify support mode 0x02 (freeze frame) pid requests using the same pid set as mode 0x01 with frame number sub-byte
- **Status**: Pending

### TEST-083
- Covers: REQ-OBD-008
- **Type**: Unit
- **Description**: Verify retrieve mode 0x03 stored dtcs and mode 0x07 pending dtcs as decoded fault codes per sae j2012 (p/c/b/u + 4-digit hex)
- **Status**: Pending

### TEST-084
- Covers: REQ-OBD-009
- **Type**: Unit
- **Description**: Verify support mode 0x04 clear all emission-related dtcs with application completion callback
- **Status**: Pending

### TEST-085
- Covers: REQ-OBD-010
- **Type**: Unit
- **Description**: Verify support mode 0x06 on-board monitoring test results (obdmid and obdmid test results) via callback
- **Status**: Pending

### TEST-086
- Covers: REQ-OBD-011
- **Type**: Unit
- **Description**: Verify support mode 0x09 vehicle information: infotype 0x02 (vin), 0x04 (calibration id), 0x06 (cvn), 0x0a (ecu name)
- **Status**: Pending

### TEST-087
- Covers: REQ-OBD-012
- **Type**: Unit
- **Description**: Verify aggregate responses from multiple ecus for broadcast requests and deliver each ecu response individually via per-ecu callback
- **Status**: Pending

### TEST-088
- Covers: REQ-OBD-013
- **Type**: Unit
- **Description**: Verify support ecu-specific physical addressing: send to 0x7e0–0x7e7 and receive from the corresponding 0x7e8–0x7ef response can id
- **Status**: Pending

### TEST-089
- Covers: REQ-OBD-014
- **Type**: Unit
- **Description**: Verify provide `omnican_obd2_dtc_cb_t` callback delivering decoded dtc records (fault code, status byte) from mode 0x03/0x07 responses
- **Status**: Pending

### TEST-090
- Covers: REQ-OBD-015
- **Type**: Unit
- **Description**: Verify provide `omnican_obd2_timeout_cb_t` callback when a pid request receives no response within the configured timeout
- **Status**: Pending

### TEST-091
- Covers: REQ-COFD-001
- **Type**: Unit
- **Description**: Verify extend canopen module with cia 1301 fd features; require config_omnican_canopen=y and config_can_fd_mode=y
- **Status**: Pending

### TEST-092
- Covers: REQ-COFD-002
- **Type**: Unit
- **Description**: Verify support can fd data payloads of up to 64 bytes for higher-throughput pdo transfers
- **Status**: Pending

### TEST-093
- Covers: REQ-COFD-003
- **Type**: Unit
- **Description**: Verify maintain backward compatibility with classical 8-byte canopen nodes when fd is not negotiated
- **Status**: Pending

### TEST-094
- Covers: REQ-ISOTP-001
- **Type**: Unit
- **Description**: Verify implement a workaround for zephyr issue #86025 using separate tx/rx socket contexts with a forwarding shim
- **Status**: Pending

### TEST-095
- Covers: REQ-ISOTP-002
- **Type**: Unit
- **Description**: Verify default config_omnican_isotp_patch to y when config_omnican_uds or config_omnican_obd2 is enabled
- **Status**: Pending

### TEST-096
- Covers: REQ-MEM-001
- **Type**: Unit
- **Description**: Verify never use dynamic heap allocation (k_malloc, malloc); all state statically allocated or via net_buf pools
- **Status**: Pending

### TEST-097
- Covers: REQ-MEM-002
- **Type**: Unit
- **Description**: Verify handle can frame data via zephyr net_buf from a pool sized by config_omnican_net_buf_count (default 16)
- **Status**: Pending

### TEST-098
- Covers: REQ-MEM-003
- **Type**: Unit
- **Description**: Verify all protocol context structures shall be opaque to the caller but caller-allocated
- **Status**: Pending

### TEST-099
- Covers: REQ-MEM-004
- **Type**: Unit
- **Description**: Verify all protocol handlers execute in a zephyr workqueue context, not the can isr
- **Status**: Pending
