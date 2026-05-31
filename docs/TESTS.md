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
- Covers: REQ-CO-001
- **Type**: Unit
- **Description**: Verify implement the nmt state machine with states initialising, pre-operational, operational, and stopped per cia 301
- **Status**: Pending

### TEST-012
- Covers: REQ-CO-002
- **Type**: Unit
- **Description**: Verify transmit a boot-up message (nmt state 0x00) on transition from initialising to pre-operational
- **Status**: Pending

### TEST-013
- Covers: REQ-CO-003
- **Type**: Unit
- **Description**: Verify respond to nmt master commands (start, stop, pre-operational, reset node, reset communication) on can id 0x000
- **Status**: Pending

### TEST-014
- Covers: REQ-CO-004
- **Type**: Unit
- **Description**: Verify implement an sdo server supporting expedited, segmented, and block transfer modes per cia 301
- **Status**: Pending

### TEST-015
- Covers: REQ-CO-005
- **Type**: Unit
- **Description**: Verify return sdo abort code 0x05040000 on sdo timeout
- **Status**: Pending

### TEST-016
- Covers: REQ-CO-006
- **Type**: Unit
- **Description**: Verify support tpdo and rpdo with event-driven and synchronisation-triggered transmission modes
- **Status**: Pending

### TEST-017
- Covers: REQ-CO-007
- **Type**: Unit
- **Description**: Verify support compile-time pdo mappings via the object dictionary
- **Status**: Pending

### TEST-018
- Covers: REQ-CO-008
- **Type**: Unit
- **Description**: Verify produce emergency messages (can id 0x80 + node_id) with 16-bit error code and 8-byte error register per cia 301
- **Status**: Pending

### TEST-019
- Covers: REQ-CO-009
- **Type**: Unit
- **Description**: Verify produce heartbeat messages at a kconfig-configurable interval (default 1000 ms) per cia 301
- **Status**: Pending

### TEST-020
- Covers: REQ-CO-010
- **Type**: Unit
- **Description**: Verify support heartbeat consumer monitoring of remote nodes with error event on timeout
- **Status**: Pending

### TEST-021
- Covers: REQ-CO-011
- **Type**: Unit
- **Description**: Verify represent the object dictionary as a static c array generated from an od descriptor with no heap allocation
- **Status**: Pending

### TEST-022
- Covers: REQ-CO-012
- **Type**: Unit
- **Description**: Verify write recommended delay to next `omnican_canopen_process()` call in microseconds via `*next_us`
- **Status**: Pending

### TEST-023
- Covers: REQ-CO-013
- **Type**: Unit
- **Description**: Verify run the canopen process loop in a dedicated zephyr thread with configurable stack size and priority
- **Status**: Pending

### TEST-024
- Covers: REQ-JNET-001
- **Type**: Unit
- **Description**: Verify use 29-bit extended can identifiers and reject 11-bit standard id frames
- **Status**: Pending

### TEST-025
- Covers: REQ-JNET-002
- **Type**: Unit
- **Description**: Verify implement the address-claiming procedure per j1939/81 with 250 ms contention window and 64-bit name priority
- **Status**: Pending

### TEST-026
- Covers: REQ-JNET-003
- **Type**: Unit
- **Description**: Verify prevent pgn transmission until address claiming succeeds
- **Status**: Pending

### TEST-027
- Covers: REQ-JNET-004
- **Type**: Unit
- **Description**: Verify return omnican_err_busy and set addr to null when address claiming fails
- **Status**: Pending

### TEST-028
- Covers: REQ-JNET-005
- **Type**: Unit
- **Description**: Verify support a static pgn routing table populated via `omnican_j1939_register_pgn()` with callback and user-data
- **Status**: Pending

### TEST-029
- Covers: REQ-JNET-006
- **Type**: Unit
- **Description**: Verify invoke pgn callbacks from the system workqueue with source address, data pointer, and length
- **Status**: Pending

### TEST-030
- Covers: REQ-JNET-007
- **Type**: Unit
- **Description**: Verify implement j1939 transport protocol (tp) for 9-1785 byte messages using bam and cmdt modes
- **Status**: Pending

### TEST-031
- Covers: REQ-JNET-008
- **Type**: Unit
- **Description**: Verify implement extended transport protocol (etp) for messages larger than 1785 bytes
- **Status**: Pending

### TEST-032
- Covers: REQ-JNET-009
- **Type**: Unit
- **Description**: Verify maintain tp/etp session state in static context with configurable maximum simultaneous sessions
- **Status**: Pending

### TEST-033
- Covers: REQ-UDS-001
- **Type**: Unit
- **Description**: Verify use zephyr iso-tp for all uds message transport with isotp patch when config_omnican_isotp_patch=y
- **Status**: Pending

### TEST-034
- Covers: REQ-UDS-002
- **Type**: Unit
- **Description**: Verify implement diagnosticsessioncontrol (sid 0x10) and enforce service availability per active session
- **Status**: Pending

### TEST-035
- Covers: REQ-UDS-003
- **Type**: Unit
- **Description**: Verify maintain an s3 timer (default 5000 ms) and revert to default session on expiry
- **Status**: Pending

### TEST-036
- Covers: REQ-UDS-004
- **Type**: Unit
- **Description**: Verify reset the s3 timer on testerpresent (sid 0x3e)
- **Status**: Pending

### TEST-037
- Covers: REQ-UDS-005
- **Type**: Unit
- **Description**: Verify support sid 0x10 diagnosticsessioncontrol
- **Status**: Pending

### TEST-038
- Covers: REQ-UDS-006
- **Type**: Unit
- **Description**: Verify support sid 0x11 ecureset with hardreset, softreset, and keyoffonreset sub-functions
- **Status**: Pending

### TEST-039
- Covers: REQ-UDS-007
- **Type**: Unit
- **Description**: Verify support sid 0x22 readdatabyidentifier
- **Status**: Pending

### TEST-040
- Covers: REQ-UDS-008
- **Type**: Unit
- **Description**: Verify support sid 0x27 securityaccess with application-supplied seed/key callback
- **Status**: Pending

### TEST-041
- Covers: REQ-UDS-009
- **Type**: Unit
- **Description**: Verify support sid 0x28 communicationcontrol
- **Status**: Pending

### TEST-042
- Covers: REQ-UDS-010
- **Type**: Unit
- **Description**: Verify support sid 0x2e writedatabyidentifier
- **Status**: Pending

### TEST-043
- Covers: REQ-UDS-011
- **Type**: Unit
- **Description**: Verify support sid 0x31 routinecontrol with start, stop, and requestresults
- **Status**: Pending

### TEST-044
- Covers: REQ-UDS-012
- **Type**: Unit
- **Description**: Verify support sid 0x34 requestdownload
- **Status**: Pending

### TEST-045
- Covers: REQ-UDS-013
- **Type**: Unit
- **Description**: Verify support sid 0x36 transferdata
- **Status**: Pending

### TEST-046
- Covers: REQ-UDS-014
- **Type**: Unit
- **Description**: Verify support sid 0x37 requesttransferexit
- **Status**: Pending

### TEST-047
- Covers: REQ-UDS-015
- **Type**: Unit
- **Description**: Verify support sid 0x85 controldtcsetting
- **Status**: Pending

### TEST-048
- Covers: REQ-UDS-016
- **Type**: Unit
- **Description**: Verify return nrc 0x11 (servicenotsupported) for unregistered sids
- **Status**: Pending

### TEST-049
- Covers: REQ-UDS-017
- **Type**: Unit
- **Description**: Verify never use a hardcoded securityaccess seed/key algorithm; always use an application callback
- **Status**: Pending

### TEST-050
- Covers: REQ-UDS-018
- **Type**: Unit
- **Description**: Verify lock the session and return nrc 0x36 after exceeding config_omnican_uds_max_auth_attempts failed securityaccess attempts
- **Status**: Pending

### TEST-051
- Covers: REQ-OBD-001
- **Type**: Unit
- **Description**: Verify transmit pid requests on can id 0x7df and collect responses from 0x7e8-0x7ef within configurable timeout
- **Status**: Pending

### TEST-052
- Covers: REQ-OBD-002
- **Type**: Unit
- **Description**: Verify support obd-ii service modes 0x01 through 0x09
- **Status**: Pending

### TEST-053
- Covers: REQ-OBD-003
- **Type**: Unit
- **Description**: Verify provide callback-based non-blocking pid request/response model
- **Status**: Pending

### TEST-054
- Covers: REQ-OBD-004
- **Type**: Unit
- **Description**: Verify support mode 0x01 pids: 0x00 (supported), 0x01 (monitor status), 0x04 (engine load), 0x05 (coolant), 0x0c (rpm), 0x0d (speed), 0x11 (throttle)
- **Status**: Pending

### TEST-055
- Covers: REQ-COFD-001
- **Type**: Unit
- **Description**: Verify extend canopen module with cia 1301 fd features; require config_omnican_canopen=y and config_can_fd_mode=y
- **Status**: Pending

### TEST-056
- Covers: REQ-COFD-002
- **Type**: Unit
- **Description**: Verify support can fd data payloads of up to 64 bytes for higher-throughput pdo transfers
- **Status**: Pending

### TEST-057
- Covers: REQ-COFD-003
- **Type**: Unit
- **Description**: Verify maintain backward compatibility with classical 8-byte canopen nodes when fd is not negotiated
- **Status**: Pending

### TEST-058
- Covers: REQ-ISOTP-001
- **Type**: Unit
- **Description**: Verify implement a workaround for zephyr issue #86025 using separate tx/rx socket contexts with a forwarding shim
- **Status**: Pending

### TEST-059
- Covers: REQ-ISOTP-002
- **Type**: Unit
- **Description**: Verify default config_omnican_isotp_patch to y when config_omnican_uds or config_omnican_obd2 is enabled
- **Status**: Pending

### TEST-060
- Covers: REQ-MEM-001
- **Type**: Unit
- **Description**: Verify never use dynamic heap allocation (k_malloc, malloc); all state statically allocated or via net_buf pools
- **Status**: Pending

### TEST-061
- Covers: REQ-MEM-002
- **Type**: Unit
- **Description**: Verify handle can frame data via zephyr net_buf from a pool sized by config_omnican_net_buf_count (default 16)
- **Status**: Pending

### TEST-062
- Covers: REQ-MEM-003
- **Type**: Unit
- **Description**: Verify all protocol context structures shall be opaque to the caller but caller-allocated
- **Status**: Pending

### TEST-063
- Covers: REQ-MEM-004
- **Type**: Unit
- **Description**: Verify all protocol handlers execute in a zephyr workqueue context, not the can isr
- **Status**: Pending
