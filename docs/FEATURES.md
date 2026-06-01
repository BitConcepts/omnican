# OmniCAN — Feature Status & Coverage Reference

**Version**: 0.1.0 | **Updated**: 2026-06-01 | **Phase**: 🚀 Release (governance baseline)

This document is the authoritative feature matrix for OmniCAN. It tracks every
protocol feature across all phases, distinguishes mandatory from optional, notes
known limitations, and credits the open-source stacks used as implementation
references.

> **Implementation status key**
> - ✅ **Architected** — in ARCHITECTURE.md, has requirements + tests
> - 🔧 **Planned** — Kconfig symbol defined, stub in src/; implementation Phase 6+
> - 📋 **Optional** — available via Kconfig flag, not compiled by default
> - ❌ **Not supported** — out of scope or blocked by external dependency
> - ⚠️ **Limitation** — partially supported; see Notes column

---

## Phase 1 — CANopen CiA 301

**Standard**: CiA 301 v4.2.0 | **Kconfig**: `CONFIG_OMNICAN_CANOPEN=y`
**Open-source reference**: CANopenNode (BitConcepts fork, MIT)

| Feature | Mandatory? | Status | Notes |
|---|---|---|---|
| NMT state machine (Init/Pre-Op/Op/Stopped) | ✅ CiA 301 mandatory | ✅ Architected | |
| Boot-Up message on Pre-Op entry | ✅ Mandatory | ✅ Architected | |
| NMT master command reception | ✅ Mandatory | ✅ Architected | Start/Stop/Pre-Op/Reset Node/Reset Comm |
| SDO server — expedited transfer | ✅ Mandatory | ✅ Architected | ≤4 bytes per exchange |
| SDO server — segmented transfer | ✅ Mandatory | ✅ Architected | 7 bytes/segment |
| SDO server — block transfer | Optional | ✅ Architected | Up to 127 segments/block |
| SDO timeout with abort (0x05040000) | ✅ Mandatory | ✅ Architected | CONFIG_OMNICAN_SDO_TIMEOUT_MS |
| TPDO — event-driven (0xFF) | ✅ Mandatory | ✅ Architected | |
| TPDO — synchronous (0x01–0xF0) | ✅ Mandatory | ✅ Architected | |
| RPDO — mapping and reception | ✅ Mandatory | ✅ Architected | |
| PDO compile-time mapping via OD | ✅ Mandatory | ✅ Architected | EDS/CiA 306 compatible |
| Emergency message production | ✅ Mandatory | ✅ Architected | CAN ID 0x80+node_id |
| Heartbeat producer | ✅ Mandatory | ✅ Architected | CONFIG_OMNICAN_HB_PERIOD_MS |
| Heartbeat consumer monitoring | Optional | ✅ Architected | OD 0x1016 |
| Static Object Dictionary (no heap) | ✅ Mandatory | ✅ Architected | Generated from OD descriptor |
| Dedicated process loop thread | Optional | ✅ Architected | CONFIG_OMNICAN_CANOPEN_THREAD_* |
| **Callback hooks** | | | |
| → NMT state change callback | Optional | ✅ Architected | `omnican_canopen_nmt_change_cb_t` |
| → SDO post-write callback | Optional | ✅ Architected | `omnican_canopen_sdo_post_write_cb_t` |
| → RPDO received callback | Optional | ✅ Architected | `omnican_canopen_rpdo_rx_cb_t` |
| → Remote Emergency received | Optional | ✅ Architected | `omnican_canopen_emcy_rx_cb_t` |
| → SYNC received | Optional | ✅ Architected | `omnican_canopen_sync_cb_t` |
| → Heartbeat consumer event | Optional | ✅ Architected | `omnican_canopen_hb_event_cb_t` |
| → Remote Boot-Up received | Optional | ✅ Architected | `omnican_canopen_bootup_rx_cb_t` |

---

## Phase 2 — SAE J1939

**Standards**: J1939/21 DEC2010, J1939/81 MAY2003, J1939DA DEC2024
**Kconfig**: `CONFIG_OMNICAN_J1939=y`
**Open-source reference**: Open-SAE-J1939 (DanielMartensson, MIT C89)

### Core Transport

| Feature | Mandatory? | Status | Notes |
|---|---|---|---|
| 29-bit extended CAN IDs only | ✅ J1939/21 mandatory | ✅ Architected | Rejects 11-bit |
| Address claiming (J1939/81) | ✅ Mandatory | ✅ Architected | 250 ms window, 64-bit NAME priority |
| PGN routing table | ✅ Mandatory | ✅ Architected | CONFIG_OMNICAN_J1939_MAX_PGN_ENTRIES |
| RQST (PGN 0xEA00) — request PGN | ✅ Mandatory | ✅ Architected | Required for all DM request/response |
| ACKM (PGN 0xE800) — acknowledgment | ✅ Mandatory | ✅ Architected | Positive/Negative/Busy/Cannot respond |
| TP (J1939/21) — 9–1785 bytes | ✅ Mandatory | ✅ Architected | BAM + CMDT modes |
| TP.CM (0xEC00) — connection mgmt | ✅ Mandatory | ✅ Architected | RTS/CTS/ACK/Abort |
| TP.DT (0xEB00) — data transfer | ✅ Mandatory | ✅ Architected | 7 bytes/packet |
| ETP (J1939/21) — >1785 bytes | ✅ Mandatory | ✅ Architected | CONFIG_OMNICAN_J1939_ETP_MAX_PAYLOAD_KB (64 KB default) |
| ETP.CM (0xC800) — ETP connection mgmt | ✅ Mandatory | ✅ Architected | Explicit requirement |
| ETP.DT (0xC700) — ETP data transfer | ✅ Mandatory | ✅ Architected | Explicit requirement |
| PropA (0xEF00) — proprietary P2P | Optional | ✅ Architected | Via PGN routing table |
| PropB (0xFF00) — proprietary broadcast | Optional | ✅ Architected | Via PGN routing table |
| **Callback hooks** | | | |
| → Address claim result | Optional | ✅ Architected | `omnican_j1939_address_result_cb_t` |
| → TP/ETP progress | Optional | ✅ Architected | `omnican_j1939_tp_progress_cb_t` |
| → TP/ETP complete | Optional | ✅ Architected | `omnican_j1939_tp_complete_cb_t` |

### J1939 DTC / SPN Encoding (J1939/73)

| Feature | Mandatory? | Status | Notes |
|---|---|---|---|
| 4-byte DTC encode (SPN 19-bit + FMI 5-bit + CM 1-bit + OC 7-bit) | ✅ Mandatory | ✅ Architected | `omnican_j1939_dtc_encode()` |
| 4-byte DTC decode | ✅ Mandatory | ✅ Architected | `omnican_j1939_dtc_decode()` |
| FMI string lookup (FMI 0–31) | Optional | ✅ Architected | `omnican_j1939_fmi_string()` |

### J1939 Diagnostics — Mandatory DM Set (J1939/73)

**Open-source references**: Open-SAE-J1939 (MIT, DM1/2/3/14/15/16),
python-can-j1939 (MIT, DTC bits + lamp byte), CSS Electronics guide (DM1-12),
EmbeddedFlakes (DM1-12 byte maps)

| DM | PGN | Description | Mandatory? | Status | Source |
|---|---|---|---|---|---|
| DM1 | 0xFECA | Active DTCs + lamp status | ✅ J1939/73 mandatory | ✅ Architected | Open-SAE-J1939 (MIT) |
| DM2 | 0xFECB | Previously active DTCs | ✅ Mandatory | ✅ Architected | Open-SAE-J1939 (MIT) |
| DM3 | 0xFECC | Clear previously active DTCs | ✅ Mandatory | ✅ Architected | Open-SAE-J1939 (MIT) |
| DM4 | 0xFECD | Freeze frame parameters | Optional (configurable SPN list) | ✅ Architected | CONFIG_OMNICAN_J1939_DM4_SPN_LIST |
| DM5 | 0xFECE | Diagnostic readiness 1 | ✅ Mandatory | ✅ Architected | CSS Electronics + AgIsoStack++ |
| DM6 | 0xFECF | Emission-related pending DTCs | ✅ Mandatory | ✅ Architected | Same format as DM1/DM2 |
| DM11 | 0xFED3 | Clear active DTCs | ✅ Mandatory | ✅ Architected | Open-SAE-J1939 (MIT) |
| DM12 | 0xFED4 | MIL-on DTCs | ✅ Mandatory | ✅ Architected | Same format as DM1 |
| DM14 | 0x0D900 | Memory access request | Optional | ✅ Architected | Open-SAE-J1939 (MIT) |
| DM15 | 0x0D800 | Memory access response | Optional | ✅ Architected | Open-SAE-J1939 (MIT) |
| DM16 | 0x0D700 | Binary data transfer | Optional | ✅ Architected | Open-SAE-J1939 (MIT) |
| DM22 | 0x0C300 | Individual DTC clear | ✅ Mandatory | ✅ Architected | python-can-j1939 (MIT) |
| `omnican_j1939_dm1_cb_t` | — | DM1 received callback | Optional | ✅ Architected | |
| `omnican_j1939_dm_request()` | — | Request any DM from device | Optional | ✅ Architected | |

### J1939 Diagnostics — Extended DM Set (CONFIG_OMNICAN_J1939_DM_EXTENDED)

> ⚠️ **LIMITATION — SAE J1939/73 purchase required for complete specification**
>
> The extended DM set (DM7–DM10, DM13, DM20, DM21, DM25, DM28) requires
> **SAE J1939/73 APR2011 or later** for normative byte-level definitions.
> Specifically needed: DM5 monitor readiness bit mapping, DM7/DM8/DM10
> test ID-to-procedure table, DM13 service byte sub-functions, DM20 performance
> ratio formula, DM25 expanded freeze frame SPN indexing, DM28 permanent DTC
> qualifier criteria.
>
> **Purchase**: SAE J1939/73_202412 (December 2024 revision, current) from
> [sae.org](https://www.sae.org) — approximately $167 USD.
>
> **Impact**: Without J1939/73 purchase, extended DMs can be implemented as
> generic containers with application-supplied byte handlers; the exact normative
> byte assignments are approximated from public sources.

| DM | PGN | Description | Status | Gap |
|---|---|---|---|---|
| DM4 | 0xFECD | Freeze frame (SPN list configurable) | ✅ Architected | None — app-configurable |
| DM7 | 0x0E300 | Command non-cont. monitored test | 📋 Optional (EXTENDED) | ⚠️ TID/FID encoding needs J1939/73 |
| DM8 | 0xFED0 | Test results (non-cont. monitored) | 📋 Optional (EXTENDED) | ⚠️ Test result format needs J1939/73 |
| DM9 | 0xFED1 | O2 sensor test results | 📋 Optional (EXTENDED) | ⚠️ Needs J1939/73 |
| DM10 | 0xFED2 | Non-cont. monitored test IDs | 📋 Optional (EXTENDED) | ⚠️ TID list needs J1939/73 |
| DM13 | 0x0DF00 | Stop/Start broadcast | 📋 Optional (EXTENDED) | ⚠️ Sub-function bits need J1939/73 |
| DM20 | 0x0C200 | Monitor performance ratio | 📋 Optional (EXTENDED) | ⚠️ Ratio formula needs J1939/73 |
| DM21 | 0x0C100 | Diagnostic readiness 2 | 📋 Optional (EXTENDED) | Implementable from public docs |
| DM25 | 0xFDB7 | Expanded freeze frame | 📋 Optional (EXTENDED) | ⚠️ SPN indexing needs J1939/73 |
| DM26 | 0xFDB8 | Diagnostic readiness 3 | 📋 Optional (EXTENDED) | Implementable from public docs |
| DM27 | 0xFD82 | All pending DTCs | 📋 Optional (EXTENDED) | Same format as DM2 |
| DM28 | 0xFD80 | Permanent DTCs | 📋 Optional (EXTENDED) | ⚠️ Qualifier criteria needs J1939/73 |
| DM29 | 0x9E00 | DTC counts | 📋 Optional (EXTENDED) | Implementable from public docs |
| DM30 | 0xA400 | Scaled test results | 📋 Optional (EXTENDED) | ⚠️ Needs J1939/73 |
| DM31 | 0xA300 | DTC-to-lamp association | 📋 Optional (EXTENDED) | Implementable from public docs |

### J1939 DMs — Not Yet Planned (DA defines, not in architecture)

| DMs | PGN Range | Count | Status |
|---|---|---|---|
| DM17–DM19 | 0xD600–0xD300 | 3 | ❌ Not planned (boot/security/calibration) |
| DM23–DM24 | 0xFDB5–0xFDB6 | 2 | ❌ Not planned (emission monitoring) |
| DM32–DM60 | Various | 29 | ❌ Not planned (harmonized roadworthiness, extended OBD) |

### J1939 DA Reference Tables (CONFIG_OMNICAN_J1939_*)

| Feature | Kconfig | Status | Content |
|---|---|---|---|
| SLOT decode table | `SLOT_TABLE=y` | 📋 Optional | 426 SLOTs from J1939DA DEC2024 |
| Mandatory SPN minimum | (included with SLOT_TABLE) | 📋 Optional | SPNs 190,84,110,183,91,247,250 |
| Extra SPN list | `SPN_EXTRA_LIST="..."` | 📋 Optional | Comma-separated additions |
| Custom SPN registration | `spn_register_custom()` | ✅ Architected | Runtime, no Kconfig needed |
| Manufacturer ID table | `MFR_TABLE=y` | 📋 Optional | 1,509 entries from DA B10 |
| Custom MFR registration | `mfr_register()` | ✅ Architected | Runtime, no Kconfig needed |
| IG preferred address tables | `IG_ADDRESSES=y` | 📋 Optional | B2 global + B3–B7 industry groups |
| NAME function lookup | `IG_ADDRESSES=y` | 📋 Optional | B11/B12 functions |

---

## Phase 3 — ISO 14229 / UDS

**Standards**: ISO 14229-1:2020, ISO 14229-3:2012, ISO 15765-2:2016
**Kconfig**: `CONFIG_OMNICAN_UDS=y` (requires `CONFIG_ISOTP=y`)
**Open-source reference**: None — original implementation

### Sessions and Services

| Feature | Mandatory? | Status | Notes |
|---|---|---|---|
| DiagnosticSessionControl (0x10) | ✅ ISO 14229-1 mandatory | ✅ Architected | Default/Programming/Extended |
| Session availability enforcement | ✅ Mandatory | ✅ Architected | Per-session SID gating |
| S3 timer (session timeout) | ✅ Mandatory | ✅ Architected | CONFIG_OMNICAN_UDS_S3_TIMEOUT_MS |
| ECUReset (0x11) | ✅ Mandatory | ✅ Architected | hardReset/softReset/keyOffOnReset |
| ReadDataByIdentifier (0x22) | ✅ Mandatory | ✅ Architected | Multi-DID in one request |
| SecurityAccess (0x27) | ✅ Mandatory | ✅ Architected | App-supplied seed/key — never hardcoded |
| SecurityAccess lockout | ✅ Mandatory | ✅ Architected | CONFIG_OMNICAN_UDS_MAX_AUTH_ATTEMPTS |
| CommunicationControl (0x28) | ✅ Mandatory | ✅ Architected | All 4 sub-functions |
| WriteDataByIdentifier (0x2E) | ✅ Mandatory | ✅ Architected | With write validation callback |
| RoutineControl (0x31) | ✅ Mandatory | ✅ Architected | Per-routine executor callbacks |
| RequestDownload (0x34) | ✅ Mandatory | ✅ Architected | |
| TransferData (0x36) | ✅ Mandatory | ✅ Architected | With progress callback |
| RequestTransferExit (0x37) | ✅ Mandatory | ✅ Architected | |
| TesterPresent (0x3E) | ✅ Mandatory | ✅ Architected | Resets S3 timer |
| ControlDTCSetting (0x85) | ✅ Mandatory | ✅ Architected | With DTC enable/disable callback |
| suppressPositiveResponse bit (0x80) | ✅ ISO 14229-1 §7.5.2.2 | ✅ Architected | |
| NRC 0x78 responsePending | ✅ Mandatory | ✅ Architected | When handler exceeds p2Server_max |
| NRC 0x11 for unregistered SIDs | ✅ Mandatory | ✅ Architected | |
| Functional addressing (0x7DF) | ✅ Mandatory | ✅ Architected | TesterPresent + DiagSessionCtrl |
| Physical addressing (0x7E0–0x7E7) | ✅ Mandatory | ✅ Architected | Configurable via server_cfg |
| ISO-TP transport (ISO 15765-2) | ✅ Mandatory | ✅ Architected | Via Zephyr ISO-TP subsystem |
| ISOTP #86025 patch | ✅ Required | ✅ Architected | Separate TX/RX socket shim |

### DID Registry

| Feature | Status | Notes |
|---|---|---|
| Manufacturer DID range 0xF200–0xFEFF | ✅ Architected | `omnican_uds_register_did()` |
| ECU identification DID 0xF186 (activeSession) | ✅ Architected | Auto-populated |
| ECU identification DID 0xF18B (ECUMfgDate) | ✅ Architected | Auto-populated |
| ECU identification DID 0xF18C (ECUSerial) | ✅ Architected | Auto-populated |
| ECU identification DID 0xF190 (VIN) | ✅ Architected | App-provided |
| ECU identification DID 0xF195 (SoftwareVersion) | ✅ Architected | Auto-populated |
| Custom routine registration | ✅ Architected | `omnican_uds_register_routine()` |
| Manufacturer-specific NRC (0x80–0xFE) | ✅ Architected | Per-SID NRC provider callback |

### UDS Callback Hooks

| Callback | Status |
|---|---|
| `omnican_uds_session_change_cb_t` | ✅ Architected |
| `omnican_uds_security_change_cb_t` | ✅ Architected |
| `omnican_uds_pre_reset_cb_t` | ✅ Architected |
| `omnican_uds_transfer_progress_cb_t` | ✅ Architected |
| `omnican_uds_did_write_validate_cb_t` | ✅ Architected |
| `omnican_uds_did_handler_cb_t` | ✅ Architected |
| `omnican_uds_routine_cb_t` | ✅ Architected |
| `omnican_uds_nrc_provider_cb_t` | ✅ Architected |

---

## Phase 4 — SAE J1979 / OBD-II

**Standards**: SAE J1979 FEB2012, SAE J2012 MAR2012, ISO 15765-4:2021
**Kconfig**: `CONFIG_OMNICAN_OBD2=y` (requires `CONFIG_ISOTP=y`)

### Mode / PID Coverage

| Mode | Description | Status |
|---|---|---|
| 0x01 | Current powertrain data (PIDs) | ✅ Architected — full PID bitmap enumeration |
| 0x02 | Freeze frame data | ✅ Architected — same PIDs as 0x01 + frame# |
| 0x03 | Stored DTCs | ✅ Architected — decoded P/C/B/U per SAE J2012 |
| 0x04 | Clear DTCs | ✅ Architected — with completion callback |
| 0x05 | O2 sensor monitoring | ✅ Architected — via OBDMID |
| 0x06 | On-board monitoring | ✅ Architected — OBDMID test results |
| 0x07 | Pending DTCs | ✅ Architected — same decode as 0x03 |
| 0x08 | Control on-board system | ✅ Architected — generic mode request |
| 0x09 | Vehicle information | ✅ Architected — VIN/CalID/CVN/ECU name |

### Mode 0x01 PIDs

| PIDs | Status |
|---|---|
| 0x00/0x20/0x40/0x60/0x80/0xA0/0xC0 (support bitmaps) | ✅ Architected |
| 0x01 (monitor status), 0x04 (engine load), 0x05 (coolant) | ✅ Architected |
| 0x0C (RPM), 0x0D (vehicle speed), 0x11 (throttle) | ✅ Architected |
| 0x14–0x1B (O2 sensor), 0x1C (OBD standard) | ✅ Architected |
| 0x21 (distance since MIL), 0x2F (fuel level) | ✅ Architected |
| 0x31 (distance since codes cleared), 0x33 (baro pressure) | ✅ Architected |
| 0x46 (ambient air temp), 0x49 (accelerator), 0x51 (fuel type) | ✅ Architected |
| 0x5C (engine oil temp) | ✅ Architected |

### OBD-II Features

| Feature | Status | Notes |
|---|---|---|
| Broadcast request (0x7DF) | ✅ Architected | Multi-ECU response aggregation |
| Physical addressing (0x7E0–0x7E7) | ✅ Architected | `request_pid_physical()` |
| DTC decode per SAE J2012 | ✅ Architected | P/C/B/U + 4-hex digit |
| `omnican_obd2_dtc_cb_t` | ✅ Architected | |
| `omnican_obd2_timeout_cb_t` | ✅ Architected | |
| `request_dtcs()` (Modes 0x03/0x07) | ✅ Architected | |
| `clear_dtcs()` (Mode 0x04) | ✅ Architected | With completion callback |

---

## Phase 5 — CANopen FD CiA 1301

**Standard**: CiA 1301 v1.0.0
**Kconfig**: `CONFIG_OMNICAN_CANOPEN_FD=y` (requires `CONFIG_OMNICAN_CANOPEN=y` + `CONFIG_CAN_FD_MODE=y`)

| Feature | Status | Notes |
|---|---|---|
| CAN FD payloads up to 64 bytes | ✅ Architected | |
| SDO block transfer up to 60 bytes/segment | ✅ Architected | |
| Higher-throughput PDO transfers | ✅ Architected | |
| Backward compatibility with 8-byte CANopen nodes | ✅ Architected | Negotiated |
| 29-bit extended CAN ID filter slot | ✅ Architected | Via frame router |

---

## Phase 6 — ISOBUS ISO 11783 (Planned)

**Standards**: ISO 11783-1 through ISO 11783-14
**Kconfig**: `CONFIG_OMNICAN_ISOBUS=y` (planned; requires `CONFIG_OMNICAN_J1939=y`)
**Implementation**: Native Zephyr C (NOT a wrapper of AgIsoStack++)
**Reference**: AgIsoStack++ (MIT C++), Open-SAE-J1939 ISO 11783-7 layer (MIT C)

> ⚠️ **LIMITATION — Phase 6 not yet implemented**
>
> ISOBUS Phase 6 is architected and planned but not yet implemented.
> Stubs do not exist in `src/`. Implementation will begin after Phase 2
> (J1939) implementation is complete, as ISOBUS is built on the J1939 foundation.

| Feature | Status | Notes |
|---|---|---|
| Working Set Master election (ISO 11783-5) | 🔧 Planned | Builds on J1939 address claiming |
| Working Set Member announcement | 🔧 Planned | |
| VT client — object pool upload (ISO 11783-6) | 🔧 Planned | Via TP/ETP |
| VT client — soft-key dispatch | 🔧 Planned | |
| VT client — alarm messages | 🔧 Planned | |
| TC client — DDOP exchange (ISO 11783-10) | 🔧 Planned | |
| TC client — process data exchange | 🔧 Planned | |
| TC client — section control | 🔧 Planned | |
| ISB (ISOBUS Shortcut Button, ISO 11783-7 §B.5) | 🔧 Planned | PGN 0xFD02 emergency stop |
| Auxiliary valve commands (ISO 11783-7) | 🔧 Planned | Valves 0–15 |
| General-purpose valve commands | 🔧 Planned | |
| Guidance speed/direction messages | 🔧 Planned | |

---

## Cross-Cutting Features

### Frame Router

| Feature | Status | Notes |
|---|---|---|
| 11-bit CAN frame reception | ✅ Architected | CANopen, UDS, OBD-II |
| 29-bit CAN frame reception | ✅ Architected | J1939, CANopen FD/vendor |
| Simultaneous 11-bit + 29-bit | ✅ Architected | No filter conflicts |
| Workqueue dispatch (non-ISR) | ✅ Architected | All handlers off-ISR |
| Per-protocol configurable filter masks | ✅ Architected | Via Kconfig |

### Multi-Protocol Co-Existence

| Combination | Status | Notes |
|---|---|---|
| J1939 (29-bit) + UDS (11-bit) | ✅ Architected | No filter conflict |
| CANopen (11-bit) + UDS (11-bit) | ✅ Architected | Non-overlapping IDs: CANopen 0x000–0x5FF, UDS 0x7DF+ |
| CANopen (11-bit) + J1939 (29-bit) | ✅ Architected | Frame type distinction |
| OBD-II + UDS | ✅ Architected | UDS owns 0x7DF; OBD-II uses physical addressing when co-active |
| All 4 simultaneously | ✅ Architected | Policy documented in ARCHITECTURE.md |
| Cross-protocol error hook | ✅ Architected | `omnican_error_cb_t` |

### Memory Model

| Feature | Status | Notes |
|---|---|---|
| No heap allocation (no `k_malloc`/`malloc`) | ✅ Architected | All contexts caller-allocated |
| Zephyr `net_buf` pool for frame data | ✅ Architected | CONFIG_OMNICAN_NET_BUF_COUNT |
| Typical RAM usage | ✅ Architected | ~2 KB static (varies with OD + session counts) |

---

## Known Limitations

### J1939/73 Purchase Required for Complete Extended DM Set

**What it is**: SAE J1939/73 — *Application Layer — Diagnostics* — defines
all 35 diagnostic messages (DM1–DM35) including precise byte-level encoding
for test results (DM7–DM10), monitor performance (DM20), expanded freeze frames
(DM25), and permanent DTCs (DM28).

**Current status**: The mandatory DM set (DM1–DM3, DM5, DM6, DM11, DM12, DM22,
DM14–DM16) is **fully implementable** from open-source references (Open-SAE-J1939
MIT, CSS Electronics guide, EmbeddedFlakes). No purchase required for these.

**What's blocked**: Precise normative byte definitions for:
- DM5 monitor readiness bit-to-monitor mapping
- DM7/DM8/DM10 test ID-to-procedure table
- DM13 service byte sub-function bit positions (stop=0x00, start=0x01, no-action=0xFF)
- DM20 monitor performance ratio calculation formula
- DM25 expanded freeze frame SPN indexing format
- DM28 permanent DTC qualifier criteria

**How to unblock**: Purchase **SAE J1939/73_202412** (December 2024 revision)
from [sae.org](https://www.sae.org). Price: approximately $167 USD.
This supersedes J1939/73 APR2011 referenced in STANDARDS.md.

**Workaround**: Extended DMs can be implemented as generic containers with
application-supplied byte handlers. The framework (Kconfig, callbacks, PGN routing)
is already architected — only the byte-level normative details are uncertain for
the 6 items above.

### Other Known Limitations

| Limitation | Impact | Workaround |
|---|---|---|
| Zephyr ISO-TP issue #86025 | UDS/OBD-II cannot bind and TX on same CAN ID | ISOTP patch (separate TX/RX shim) — implemented |
| specsmith scanner detects embedded-hardware as cli-c | Audit reports 1 false-positive (29/30 pass) | `type_confirmed: true` in scaffold.yml |
| ETP practical max ~64 KB (not theoretical 117 MB) | Large ETP sessions constrained by net_buf pool | CONFIG_OMNICAN_J1939_ETP_MAX_PAYLOAD_KB |
| CANopen FD freestanding ARM Cortex-M support | AgIsoStack++ not usable on bare metal | Phase 6 uses native Zephyr C |
| ISOBUS Phase 6 not yet implemented | No VT/TC/ISB until Phase 6 | J1939 Phase 2 provides the foundation |
| DM57 (OBD Info) / DM59 (DR4) / DM60 (Extended DTC) | Not planned | Low priority; application-specific via generic container |

---

## Open-Source Stack Credits

All code adapted from external sources carries an attribution header and is
licensed compatibly with OmniCAN's Apache-2.0 license.

| Stack | License | Used For | Attribution Required |
|---|---|---|---|
| **Open-SAE-J1939** (DanielMartensson) | MIT | J1939/73 DM1/2/3/14/15/16 implementation; RQST/ACKM; TP/ETP reference | Yes — in each ported `.c` file |
| **python-can-j1939** (juergenH87) | MIT | DTC 4-byte encode/decode bit algorithm; DM1 lamp byte parser | Yes — in `dtc.c` |
| **AgIsoStack++** (Open-Agriculture) | MIT | J1939 DM5/DM12/DM22 (translated C++→C); Phase 6 ISOBUS protocol reference | Yes — in adapted files |
| **CANopenNode** (BitConcepts fork) | MIT | Phase 1 CANopen — rearchitected, not vendored | Yes — in `src/canopen/` |
| **CSS Electronics J1939-73 guide** | Public | DM1–DM12 byte maps, FMI table, transmission timing | Reference only |
| **EmbeddedFlakes J1939 Diagnostics** | Public | DM1–DM12 byte-level format, lamp state table | Reference only |
| **Linux kernel j1939** | GPL-2.0 | Protocol behavioral reference ONLY | Not usable as code |
| **MicroControl J1939 Stack** | Commercial | Public header documentation reference ONLY | Not usable as code |

---

## Standards Registry Summary

See `docs/STANDARDS.md` for pinned edition numbers.

| Protocol | Standard | Edition |
|---|---|---|
| CANopen | CiA 301 | v4.2.0 (2011) |
| CANopen FD | CiA 1301 | v1.0.0 (2019) |
| J1939 data link + TP | J1939/21 | DEC2010 |
| J1939 address claiming | J1939/81 | MAY2003 |
| J1939 diagnostics | **J1939/73** | APR2011 (+ _202412 latest) |
| J1939 digital annex | **J1939DA** | DEC2024 r1 |
| UDS application | ISO 14229-1 | 2020 |
| UDS on CAN | ISO 14229-3 | 2012 |
| OBD-II service modes | SAE J1979 | FEB2012 |
| DTC format | SAE J2012 | MAR2012 |
| ISO-TP | ISO 15765-2 | 2016 |
| OBD on CAN | ISO 15765-4 | 2021 |
| CAN physical | ISO 11898-1 | 2015 |
| ISOBUS (Phase 6) | ISO 11783-1/6/7/10 | 2017/2018 |
