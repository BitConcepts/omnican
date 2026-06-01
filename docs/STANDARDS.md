# OmniCAN — Standards Reference

This document lists every normative standard referenced by OmniCAN, pinned to
the specific edition used during design. Implementations shall conform to the
version listed here unless a superseding version is explicitly approved and the
LEDGER.md is updated with the change.

**Maintained by**: BitConcepts, LLC
**Last reviewed**: 2026-05-31
**Governing rule**: docs/governance/RULES.md H1 — governance files require proposal + approval to change.

---

## 1. CAN Physical and Data-Link Layer

These standards define the electrical and frame-level behaviour that underpins
all OmniCAN protocols. OmniCAN does not implement these layers directly —
Zephyr's CAN driver abstracts them — but conformance is required of the
hardware and Zephyr BSP in use.

| Standard | Title | Edition | Key scope for OmniCAN |
|---|---|---|---|
| ISO 11898-1:2015 | Road vehicles — Controller area network (CAN) — Part 1: Data link layer and physical signalling | 2015 | CAN frame structure, bit stuffing, error handling, 11-bit and 29-bit identifiers |
| ISO 11898-2:2016 | Road vehicles — CAN — Part 2: High-speed medium access unit | 2016 | Classical CAN physical layer, 1 Mbit/s maximum |
| ISO 11898-3:2006 | Road vehicles — CAN — Part 3: Low-speed, fault-tolerant, medium-dependent interface | 2006 | Fault-tolerant transceivers (informative only for OmniCAN) |
| ISO 11898-5:2007 | Road vehicles — CAN — Part 5: High-speed medium access unit with low-power mode | 2007 | Selective wake-up (informative) |
| Bosch CAN FD Spec 1.0 | CAN with Flexible Data-Rate — Specification | Rev. 1.0 (2012) | CAN FD frame format, BRS/ESI bits, DLC extensions to 64 bytes |
| ISO 11898-1:2015/Amd 1:2021 | CAN FD addendum | 2021 | CAN FD incorporated into ISO 11898-1 (normative for hardware; Zephyr CAN driver implements) |

**OmniCAN dependency**: The Zephyr CAN driver API (`zephyr/drivers/can.h`) encapsulates
all ISO 11898 compliance. OmniCAN selects FD mode via `omnican_node_init(..., fd_mode=true)`
and relies on the BSP for physical conformance.

---

## 2. CANopen (Phase 1 and Phase 5)

CANopen is defined by the CAN in Automation (CiA) trade association.
Specifications are available at [can-cia.org](https://www.can-cia.org).

### 2.1 Core Application Layer

| Standard | Title | Edition | Key scope for OmniCAN |
|---|---|---|---|
| **CiA 301 v4.2.0** | CANopen Application Layer and Communication Profile | v4.2.0 (2011) | **Primary normative reference**. NMT, SDO server/client, PDO, Heartbeat, Emergency, Sync, Time stamp. All Phase 1 requirements trace to this document. |
| CiA 302-1 v4.1.0 | CANopen — Additional application layer functions — Part 1: Network Management | v4.1.0 (2013) | NMT master behaviour (OmniCAN implements slave only in v0.x) |
| CiA 302-2 v4.1.0 | CANopen — Additional application layer functions — Part 2: SDO Manager | v4.1.0 (2013) | SDO manager (not in scope for v0.x) |
| CiA 303-1 v1.9.0 | CANopen Recommendation — Part 1: Units and prefixes | v1.9.0 (2013) | SI units for OD entries (informative) |
| CiA 306-1 v1.3.0 | CANopen Electronic Data Sheet Specification — Part 1: Common objects | v1.3.0 (2007) | EDS/DCF format for OD generation tooling |

### 2.2 CAN ID Allocation (CiA 301 §7.3.3)

| Communication Object | CAN ID (11-bit) | Notes |
|---|---|---|
| NMT (master→node) | 0x000 | Broadcast, 2-byte payload |
| SYNC | 0x080 | Broadcast, 0 or 8 bytes |
| Emergency (node) | 0x080 + node_id | 8-byte payload |
| TPDO1 | 0x180 + node_id | Default mapping |
| RPDO1 | 0x200 + node_id | Default mapping |
| TPDO2 | 0x280 + node_id | Default mapping |
| RPDO2 | 0x300 + node_id | Default mapping |
| TPDO3 | 0x380 + node_id | Default mapping |
| RPDO3 | 0x400 + node_id | Default mapping |
| TPDO4 | 0x480 + node_id | Default mapping |
| RPDO4 | 0x500 + node_id | Default mapping |
| SDO (server response) | 0x580 + node_id | Server-to-client |
| SDO (client request) | 0x600 + node_id | Client-to-server |
| NMT Error Control | 0x700 + node_id | Heartbeat / Node Guarding |

node_id range: 1–127. OmniCAN validates this range in `omnican_canopen_init()`.

### 2.3 Object Dictionary Index Ranges (CiA 301 §7.4)

| Index Range | Description |
|---|---|
| 0x0000 | Not used |
| 0x0001–0x001F | Static data types |
| 0x0020–0x003F | Complex data types |
| 0x0040–0x005F | Manufacturer-specific complex data types |
| 0x0060–0x007F | Device profile data types |
| 0x0080–0x009F | Reserved |
| 0x1000–0x1FFF | Communication profile area (CiA 301) |
| 0x2000–0x5FFF | Manufacturer-specific profile area |
| 0x6000–0x9FFF | Standardised device profile area |
| 0xA000–0xFFFF | Reserved |

Key OD entries used by OmniCAN:

| Index | Sub | Name | Type |
|---|---|---|---|
| 0x1000 | 0 | Device type | UINT32 |
| 0x1001 | 0 | Error register | UINT8 |
| 0x1002 | 0 | Manufacturer status register | UINT32 |
| 0x1003 | 0 | Pre-defined error field (count) | UINT8 |
| 0x1003 | 1–8 | Pre-defined error field (entries) | UINT32 |
| 0x1005 | 0 | COB-ID SYNC | UINT32 |
| 0x1006 | 0 | Communication cycle period | UINT32 |
| 0x1014 | 0 | COB-ID Emergency | UINT32 |
| 0x1015 | 0 | Inhibit time Emergency | UINT16 |
| 0x1016 | 1–127 | Consumer heartbeat time | UINT32 |
| 0x1017 | 0 | Producer heartbeat time (ms) | UINT16 |
| 0x1018 | 0–4 | Identity object | — |
| 0x1200–0x127F | — | SDO server parameters | — |
| 0x1400–0x15FF | — | RPDO communication parameters | — |
| 0x1600–0x17FF | — | RPDO mapping parameters | — |
| 0x1800–0x19FF | — | TPDO communication parameters | — |
| 0x1A00–0x1BFF | — | TPDO mapping parameters | — |

### 2.4 CANopen FD

| Standard | Title | Edition | Key scope for OmniCAN |
|---|---|---|---|
| **CiA 1301 v1.0.0** | CANopen FD Application Layer and Communication Profile | v1.0.0 (2019) | **Primary normative reference for Phase 5**. CAN FD frame support in CANopen, new SDO FD, PDO-FD |

---

## 3. SAE J1939 (Phase 2)

J1939 is defined by SAE International. Documents are available at
[sae.org](https://www.sae.org). The J1939 standard family consists of many
parts; only those directly implemented by OmniCAN are marked **normative**.

### 3.1 Standard Parts

| Standard | Title | Edition | Key scope for OmniCAN |
|---|---|---|---|
| **SAE J1939 APR2009** | Serial Control and Communications Heavy Duty Vehicle Network — Top Level Document | APR2009 | **Overview and roadmap**. Defines the J1939 family structure. |
| **SAE J1939/11 MAY2006** | Physical Layer — 250Kbps, Shielded Twisted Pair | MAY2006 | Physical layer at 250 kbps; implemented by hardware/BSP |
| SAE J1939/15 AUG2008 | Reduced Physical Layer — 250Kbps, Unshielded Twisted Pair | AUG2008 | Alternate physical layer (informative) |
| **SAE J1939/21 DEC2010** | Data Link Layer | DEC2010 | **Primary normative reference**. 29-bit extended CAN ID structure, PGN definition, TP (9–1785 bytes), ETP (>1785 bytes), RQST (0xEA00), ACKM (0xE800), broadcast/peer-to-peer addressing |
| SAE J1939/31 APR2004 | Network Layer | APR2004 | Multi-segment network bridging (not in scope for v0.x) |
| **SAE J1939/73 APR2011** | Application Layer — Diagnostics | APR2011 | **Normative**. DM1–DM35 diagnostic messages; SPN/FMI/OC/CM 4-byte DTC encoding; lamp status byte; FMI code table (32 codes); diagnostic readiness; freeze frame; memory access (DM14–16) |
| **SAE J1939/81 MAY2003** | Network Management | MAY2003 | **Normative**. Address claiming algorithm (J1939/81 §4.2), 64-bit NAME priority, 250 ms contention window |
| SAE J1939/71 MAR2011 | Vehicle Application Layer | MAR2011 | PGN definitions for common vehicle signals (informative for OmniCAN routing) |
| **SAE J1939DA DEC2024** | Digital Annex of Serial Control and Communications Heavy Duty Vehicle Network Data | DEC2024 | **Normative reference for DA**. 3,193 PGNs, ~8,000 SPNs, 426 SLOTs, 62 DM messages (DM1–DM60), 1,509 manufacturer IDs, industry group preferred addresses (B2–B7), NAME functions (B11/B12). See `docs/J1939DA_GAP.md` for full gap analysis. |

### 3.2 29-bit CAN Identifier Layout (J1939/21 §5.2)

```
Bits: 28          26 25  24 23       16 15        8 7          0
      ┌─────────────┬──┬──┬───────────┬────────────┬────────────┐
      │ Priority(3) │R │DP│    PF(8)  │   PS(8)    │   SA(8)   │
      └─────────────┴──┴──┴───────────┴────────────┴────────────┘
```

| Field | Bits | Description |
|---|---|---|
| Priority | [28:26] | 0 (highest) – 7 (lowest); default 6 for most PGNs |
| Reserved (R) | [25] | Always 0 |
| Data Page (DP) | [24] | Selects PGN data page; 0=standard, 1=extended |
| PDU Format (PF) | [23:16] | 0x00–0xEF = peer-to-peer (PS=DA); 0xF0–0xFF = broadcast (PS=GE) |
| PDU Specific (PS) | [15:8] | Destination address (peer-to-peer) or Group Extension (broadcast) |
| Source Address (SA) | [7:0] | 0x00–0xFD node addresses; 0xFE = null; 0xFF = global |

**PGN** = (DP << 17) | (PF << 8) | (PS if PF ≥ 0xF0 else 0)

### 3.3 J1939 Transport Protocol Timing (J1939/21 §5.10)

| Parameter | Value | Description |
|---|---|---|
| T1 | 750 ms | Max time to receive CTS after RTS/BAM |
| T2 | 1250 ms | Max time between data packets |
| T3 | 1250 ms | Max time to receive ACK after last data packet |
| T4 | 1050 ms | Timeout waiting for next packet |
| Tr | 200 ms | Max response time (CTS, ACK, Abort) |

---

## 4. ISO 14229 / UDS (Phase 3)

UDS is defined by ISO. Documents available at [iso.org](https://www.iso.org).

| Standard | Title | Edition | Key scope for OmniCAN |
|---|---|---|---|
| **ISO 14229-1:2020** | Road vehicles — Unified diagnostic services (UDS) — Part 1: Application layer | 2020 | **Primary normative reference**. All SIDs, NRCs, session types, SecurityAccess, data download/upload |
| ISO 14229-2:2013 | UDS — Part 2: Session layer services | 2013 | Session and timing parameters (S3 timer = 5000 ms default) |
| **ISO 14229-3:2012** | UDS — Part 3: Unified diagnostic services on CAN implementation | 2012 | **Normative**. CAN-specific addressing modes (physical/functional), N_PDU mapping |
| ISO 14229-5:2013 | UDS — Part 5: Unified diagnostic services on Internet Protocol implementation | 2013 | DoIP (not in scope for v0.x) |
| ISO 14229-7:2015 | UDS — Part 7: Unified diagnostic services on Local Interconnect Network | 2015 | LIN (not in scope) |

### 4.1 UDS Service Availability by Session (ISO 14229-1 Annex C)

| SID | Service | Default (0x01) | Programming (0x02) | Extended (0x03) |
|---|---|---|---|---|
| 0x10 | DiagnosticSessionControl | ✓ | ✓ | ✓ |
| 0x11 | ECUReset | — | ✓ | ✓ |
| 0x22 | ReadDataByIdentifier | ✓ | ✓ | ✓ |
| 0x27 | SecurityAccess | — | ✓ | ✓ |
| 0x28 | CommunicationControl | — | — | ✓ |
| 0x2E | WriteDataByIdentifier | — | ✓ | ✓ |
| 0x31 | RoutineControl | — | ✓ | ✓ |
| 0x34 | RequestDownload | — | ✓ | — |
| 0x36 | TransferData | — | ✓ | — |
| 0x37 | RequestTransferExit | — | ✓ | — |
| 0x3E | TesterPresent | ✓ | ✓ | ✓ |
| 0x85 | ControlDTCSetting | — | — | ✓ |

Note: OmniCAN enforces per-session availability via the service dispatch table.
Application callbacks registered via `omnican_uds_register_service()` supplement this.

### 4.2 UDS Negative Response Codes (ISO 14229-1 §A.1)

| NRC | Code | Description |
|---|---|---|
| generalReject | 0x10 | Server could not process the request |
| serviceNotSupported | 0x11 | SID not supported |
| subFunctionNotSupported | 0x12 | Sub-function not supported |
| incorrectMessageLengthOrInvalidFormat | 0x13 | Malformed request |
| responseTooLong | 0x14 | Response exceeds max transport size |
| conditionsNotCorrect | 0x22 | Server conditions not met |
| requestOutOfRange | 0x31 | Parameter or ID out of range |
| securityAccessDenied | 0x33 | Security locked |
| authenticationRequired | 0x34 | Authentication required |
| requestSequenceError | 0x24 | Sequence violation |
| exceededNumberOfAttempts | 0x36 | Too many failed SecurityAccess attempts |
| requiredTimeDelayNotExpired | 0x37 | Must wait before retrying SecurityAccess |
| uploadDownloadNotAccepted | 0x70 | Transfer cannot be started |
| transferDataSuspended | 0x71 | Transfer aborted |
| generalProgrammingFailure | 0x72 | Programming error |
| wrongBlockSequenceCounter | 0x73 | Block counter mismatch |
| requestCorrectlyReceivedResponsePending | 0x78 | NRC pending — more time needed |
| subFunctionNotSupportedInActiveSession | 0x7E | Available in different session |
| serviceNotSupportedInActiveSession | 0x7F | Available in different session |

### 4.3 UDS Timing Parameters (ISO 14229-2 §6.2)

| Parameter | Default | Description |
|---|---|---|
| p2Server_max | 50 ms | Max response time before NRC 0x78 |
| p2\*Server_max | 5000 ms | Max time after NRC 0x78 |
| S3Server | 5000 ms | Session timeout (OmniCAN default) |
| p4Server_max | 5000 ms | TesterPresent interval |

---

## 5. SAE J1979 / OBD-II (Phase 4)

| Standard | Title | Edition | Key scope for OmniCAN |
|---|---|---|---|
| **SAE J1979 FEB2012** | E/E Diagnostic Test Modes | FEB2012 | **Primary normative reference**. OBD service modes 0x01–0x09, PID definitions, response formats |
| SAE J1979-2:2022 | E/E Diagnostic Test Modes — OBDonUDS | 2022 | OBD over UDS (future scope) |
| **SAE J2012 MAR2012** | Diagnostic Trouble Code Definitions | MAR2012 | **Normative**. DTC format: P/C/B/U prefix, sub-type bits [5:4], 4-hex-digit fault code. Used by OmniCAN `omnican_obd2_dtc_cb_t` for decoded `dtc_str[6]` output |
| SAE J1978 APR1992 | OBD II Scan Tool | APR1992 | Scan tool interface (informative) |
| SAE J1962 JAN2012 | Diagnostic Connector | JAN2012 | OBD-II connector (hardware; informative) |

### 5.1 OBD-II Service Mode Summary (J1979 §5)

| Mode | Description |
|---|---|
| 0x01 | Request current powertrain diagnostic data (PIDs) |
| 0x02 | Request powertrain freeze frame data |
| 0x03 | Request emission-related DTC |
| 0x04 | Clear/reset emission-related diagnostic information |
| 0x05 | Request oxygen sensor monitoring test results |
| 0x06 | Request on-board monitoring test results for non-CAN systems |
| 0x07 | Request emission-related DTC detected during current/last driving cycle |
| 0x08 | Request control of on-board system/test/component |
| 0x09 | Request vehicle information |

### 5.2 Standard OBD-II CAN IDs (ISO 15765-4)

| ID | Direction | Description |
|---|---|---|
| 0x7DF | Tester → ECUs | Functional broadcast request (all ECUs) |
| 0x7E0 | Tester → ECU 1 | Physical request to ECU 1 |
| 0x7E8 | ECU 1 → Tester | Physical response from ECU 1 |
| 0x7E1–0x7E7 | Tester → ECU 2–8 | Physical requests |
| 0x7E9–0x7EF | ECU 2–8 → Tester | Physical responses |

---

## 6. ISO 15765 / ISO-TP (Transport for UDS and OBD-II)

| Standard | Title | Edition | Key scope for OmniCAN |
|---|---|---|---|
| **ISO 15765-2:2016** | Road vehicles — Diagnostic communication over CAN (DoCAN) — Part 2: Transport protocol and network layer services | 2016 | **Primary normative reference for ISO-TP**. Single Frame (SF), First Frame (FF), Consecutive Frame (CF), Flow Control (FC) frame types; block-size and STmin parameters |
| **ISO 15765-4:2021** | Road vehicles — DoCAN — Part 4: Requirements for emission-related systems | 2021 | **Normative for OBD-II**. OBD CAN IDs, functional addressing, timing |
| ISO 15765-3:2004 | Road vehicles — DoCAN — Part 3: Implementation of unified diagnostic services | 2004 | UDS-over-CAN addressing (superseded by ISO 14229-3; informative) |

### 6.1 ISO-TP Frame Types (ISO 15765-2 §9.6)

| Frame Type | PCI Byte | Description |
|---|---|---|
| Single Frame (SF) | 0x0N (N=length 1–7) | Complete PDU ≤ 7 bytes in one CAN frame |
| First Frame (FF) | 0x1H + 0xLL (length) | Start of multi-frame PDU |
| Consecutive Frame (CF) | 0x2N (N=sequence 0–F) | Subsequent frames after FF |
| Flow Control (FC) | 0x3N (N=flag) | CTS (0), Wait (1), Overflow (2) |

---

## 7. Zephyr RTOS Dependency

| Dependency | Version | Source | OmniCAN usage |
|---|---|---|---|
| Zephyr RTOS | **v3.7.0** | github.com/zephyrproject-rtos/zephyr | Kernel, CAN driver API, ISO-TP subsystem, net_buf, workqueue, logging, Kconfig |
| Zephyr CAN driver API | (part of v3.7.0) | `zephyr/drivers/can.h` | Frame TX/RX, filter registration, FD mode |
| Zephyr ISO-TP | (part of v3.7.0) | `zephyr/net/isotp.h` | UDS and OBD-II transport; affected by issue #86025 |
| Zephyr net_buf | (part of v3.7.0) | `zephyr/net/buf.h` | Zero-copy CAN frame data handling |
| Zephyr logging | (part of v3.7.0) | `zephyr/logging/log.h` | `CONFIG_OMNICAN_LOG_LEVEL` |
| CANopenNode | master | github.com/CANopenNode/CANopenNode | **Reference only** — not vendored. OD generation tooling guidance. |

**Version pinning**: `west.yml` pins Zephyr at `v3.7.0`. Any upgrade must be
approved via the proposal workflow (docs/governance/RULES.md H2) and validated
against the ISOTP patch applicability.

---

## 8. Compliance and Safety Standards (Informative)

These standards are **not** normative for OmniCAN v0.x but are documented here
for future safety-critical adoption planning.

| Standard | Title | Relevance |
|---|---|---|
| ISO 26262 | Road vehicles — Functional safety | ASIL classification for automotive ECU firmware |
| IEC 62443 | Industrial automation and control systems security | CAN network security hardening |
| MISRA C:2012 | Guidelines for the use of the C language in critical systems | Coding standard for safety-critical OmniCAN deployments |
| AUTOSAR | Automotive Open System Architecture | OBD-II and UDS stack integration patterns |

---

## 9. Change Log

| Date | Change | Author |
|---|---|---|
| 2026-05-31 | Initial standards registry created | Oz / BitConcepts |
