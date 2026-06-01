# OmniCAN — J1939DA Gap Analysis & Multi-Protocol Gap Report

**DA Version**: J1939DA DEC2024 r1 (December 2024)
**Analysis date**: 2026-06-01
**Analyst**: Oz / BitConcepts

This document is the authoritative gap report between OmniCAN's current
architecture (docs/ARCHITECTURE.md) and the normative requirements of the
J1939 Digital Annex DEC2024, plus multi-protocol co-existence requirements.

---

## 1. J1939DA DEC2024 — What's In It

| Sheet | Rows | Description |
|---|---|---|
| SPs & PGs | 33,313 | ALL SP (Suspect Parameter) and PG (Parameter Group) definitions |
| SLOTs | 426 | Scaling, Length, Offset, Transfer function for every SP |
| Manufacturer IDs (B10) | 1,509 | All manufacturer ID codes for J1939 NAME field |
| Global Source Addresses (B2) | 112 | Preferred addresses for all industry groups |
| Global NAME Functions (B11) | 99 | Universal NAME function codes |
| IG Specific NAME Function (B12) | 289 | Industry-group-specific NAME functions |
| Industry Groups (B1) | 11 | IG 0–8 definitions |
| IG1 Source Addresses (B3) | 77 | On-Highway preferred addresses |
| IG2 Source Addresses (B4) | 15 | Agricultural/Forestry preferred addresses |
| IG3–IG5 Source Addresses | 29/25/23 | Construction, Marine, Industrial |

**DA totals**: 3,193 unique PGNs, 62 diagnostic messages (DM1–DM60+),
~8,000+ unique SPNs.

---

## 2. Diagnostic Messages (DM) — Complete List

The DA defines **62 DM messages**. OmniCAN currently mentions only DM1 and DM11.
Status: **COVERED** = in OmniCAN requirements; **MISSING** = not yet in architecture.

| PGN | Hex | Acronym | Description | J1939/73 § | OmniCAN |
|---|---|---|---|---|---|
| 65226 | 0xFECA | **DM1** | Active Diagnostic Trouble Codes | §5.6 | COVERED (partial — lamp status missing) |
| 65227 | 0xFECB | **DM2** | Previously Active DTCs | §5.7 | MISSING |
| 65228 | 0xFECC | **DM3** | Clear/Reset Previously Active DTCs | §5.8 | MISSING |
| 65229 | 0xFECD | **DM4** | Freeze Frame Parameters | §5.9 | MISSING |
| 65230 | 0xFECE | **DM5** | Diagnostic Readiness 1 | §5.10 | MISSING |
| 65231 | 0xFECF | **DM6** | Emission-Related Pending DTCs | §5.11 | MISSING |
| 65232 | 0xFED0 | **DM8** | Test Results (Non-cont. Monitored) | §5.12 | MISSING |
| 65233 | 0xFED1 | **DM9** | Oxygen Sensor Test Results | §5.13 | MISSING |
| 65234 | 0xFED2 | **DM10** | Non-Cont. Monitored Test IDs Support | §5.14 | MISSING |
| 65235 | 0xFED3 | **DM11** | Clear/Reset Active DTCs | §5.15 | COVERED |
| 65236 | 0xFED4 | **DM12** | MIL-On DTCs | §5.16 | MISSING |
| 58112 | 0x0E300 | **DM7** | Command Non-cont. Monitored Test | §5.12 | MISSING |
| 57088 | 0x0DF00 | **DM13** | Stop/Start Broadcast | §5.17 | MISSING |
| 55552 | 0x0D900 | **DM14** | Memory Access Request | §5.18 | MISSING |
| 55296 | 0x0D800 | **DM15** | Memory Access Response | §5.19 | MISSING |
| 55040 | 0x0D700 | **DM16** | Binary Data Transfer | §5.20 | MISSING |
| 54784 | 0x0D600 | **DM17** | Boot Load Data | §5.21 | MISSING |
| 54272 | 0x0D400 | **DM18** | Data Security | §5.22 | MISSING |
| 54016 | 0x0D300 | **DM19** | Calibration Information | §5.23 | MISSING |
| 49664 | 0x0C200 | **DM20** | Monitor Performance Ratio | §5.24 | MISSING |
| 49408 | 0x0C100 | **DM21** | Diagnostic Readiness 2 | §5.25 | MISSING |
| 49920 | 0x0C300 | **DM22** | Individual Clear/Reset of Active and Previously Active DTC | §5.26 | MISSING |
| 64949 | 0x0FDB5 | **DM23** | Previously MIL-On DTCs | §5.27 | MISSING |
| 64950 | 0x0FDB6 | **DM24** | SPN Support | §5.28 | MISSING |
| 64951 | 0x0FDB7 | **DM25** | Expanded Freeze Frame | §5.29 | MISSING |
| 64952 | 0x0FDB8 | **DM26** | Diagnostic Readiness 3 | §5.30 | MISSING |
| 64898 | 0x0FD82 | **DM27** | All Pending DTCs | §5.31 | MISSING |
| 64896 | 0x0FD80 | **DM28** | Permanent DTCs | §5.32 | MISSING |
| 40448 | 0x09E00 | **DM29** | DTC Counts | §5.33 | MISSING |
| 41984 | 0x0A400 | **DM30** | Scaled Test Results | §5.34 | MISSING |
| 41728 | 0x0A300 | **DM31** | DTC To Lamp Association | §5.35 | MISSING |
| 41472 | 0x0A200 | **DM32** | Regulated Exhaust Emission Level Exceedance | §5.36 | MISSING |
| 41216 | 0x0A100 | **DM33** | Emission Increasing AECD Active | §5.37 | MISSING |
| 40960 | 0x0A000 | **DM34** | NTE Status | §5.38 | MISSING |
| 40704 | 0x09F00 | **DM35** | Immediate Fault Status | §5.39 | MISSING |
| 64868 | 0x0FD64 | **DM36** | Harmonized Roadworthiness - Vehicle | — | MISSING |
| 64867 | 0x0FD63 | **DM37** | Harmonized Roadworthiness – System | — | MISSING |
| 64866 | 0x0FD62 | **DM38** | Harmonized Global Regulation Description | — | MISSING |
| 64865 | 0x0FD61 | **DM39** | Harmonized Cumulative CMI – System | — | MISSING |
| 64864 | 0x0FD60 | **DM40** | Harmonized B1 Failure Counts | — | MISSING |
| 64863 | 0x0FD5F | **DM41** | DTCs - A, Pending | — | MISSING |
| 64862 | 0x0FD5E | **DM42** | DTCs - A, Confirmed and Active | — | MISSING |
| 64861 | 0x0FD5D | **DM43** | DTCs - A, Previously Active | — | MISSING |
| 64860 | 0x0FD5C | **DM44** | DTCs - B1, Pending | — | MISSING |
| 64859 | 0x0FD5B | **DM45** | DTCs - B1, Confirmed and Active | — | MISSING |
| 64858 | 0x0FD5A | **DM46** | DTCs - B1, Previously Active | — | MISSING |
| 64857 | 0x0FD59 | **DM47** | DTCs - B2, Pending | — | MISSING |
| 64856 | 0x0FD58 | **DM48** | DTCs - B2, Confirmed and Active | — | MISSING |
| 64855 | 0x0FD57 | **DM49** | DTCs - B2, Previously Active | — | MISSING |
| 64854 | 0x0FD56 | **DM50** | DTCs - C, Pending | — | MISSING |
| 64853 | 0x0FD55 | **DM51** | DTCs - C, Confirmed and Active | — | MISSING |
| 64852 | 0x0FD54 | **DM52** | DTCs - C, Previously Active | — | MISSING |
| 64723 | 0x0FCD3 | **DM55** | Clear/Reset Service Only DTCs | — | MISSING |
| 64722 | 0x0FCD2 | **DM54** | Previously Active Service Only DTCs | — | MISSING |
| 64721 | 0x0FCD1 | **DM53** | Active Service Only DTCs | — | MISSING |
| 64711 | 0x0FCC7 | **DM56** | Model Year and Certification Engine Family | — | MISSING |
| 64710 | 0x0FCC6 | **DM57** | OBD Information | — | MISSING |
| 64475 | 0x0FBDB | **DM58** | Reserved for Diagnostic Message | — | MISSING |
| 64300 | 0x0FB2C | **DM59** | Diagnostic Readiness 4 | — | MISSING |
| 64074 | 0x0FA4A | **DM60** | Extended DTC Information | — | MISSING |

**Summary**: 2 covered (DM1 partial, DM11), 60 missing from OmniCAN.

---

## 3. J1939 DTC / SPN Encoding — NOT in OmniCAN Architecture

The J1939 DTC format (used in DM1, DM2, DM6, DM12, etc.) is a **4-byte structure**:

```
Bytes 1–2: SPN bits [7:0] and [15:8]   (lower 16 bits of 19-bit SPN)
Byte  3:   SPN [18:16] in bits [7:5]
            FMI [4:0]  (Failure Mode Identifier)
Byte  4:   CM  [7]     (Conversion Method: 0=J1939-73, 1=manufacturer-specific)
            OC  [6:0]  (Occurrence Count, 0–126)
```

OmniCAN must implement:
- `omnican_j1939_dtc_encode()` — pack SPN + FMI + CM + OC into 4 bytes
- `omnican_j1939_dtc_decode()` — unpack 4 bytes into SPN + FMI + CM + OC
- FMI lookup: 0=Above normal, 1=Below normal, 2=Data erratic, 3=Voltage above, 4=Voltage below, 5=Current low, 6=Current high, 7=Mechanical, 8=Abnormal freq, 9=Abnormal update, 10=Abnormal rate, 11=Root cause unknown, 12=Bad intelligent device, 13=Out of calibration, 14=Special, 15=Data valid but above normal (least severe), 16=Data valid but above normal (moderately severe), 17=Data valid but below normal (least severe), 18=Data valid but below normal (moderately severe), 19=Received network data in error, 20-30=Reserved, 31=Not available

---

## 4. DM1 Lamp Status Byte — MISSING from OmniCAN

DM1 (PGN 0xFECA) has a **1-byte lamp status header** per DTC group:

```
Bits [7:6]: MIL (Malfunction Indicator Lamp)       00=off, 01=on, 10=flash, 11=n/a
Bits [5:4]: Stop Lamp                               00=off, 01=on, 10=flash, 11=n/a
Bits [3:2]: Amber Warning Lamp                      00=off, 01=on, 10=flash, 11=n/a
Bits [1:0]: Protect Lamp                            00=off, 01=on, 10=flash, 11=n/a
```

DM1 frame format:
```
Byte 1:  Lamp status (as above)
Byte 2:  SPN Flash / Flash State bits (flash rate for each lamp)
Bytes 3+: Repeated 4-byte DTC records (SPN + FMI + CM + OC)
```

OmniCAN's `omnican_j1939_dtc_cb_t` must include lamp status per DM1 message.

---

## 5. Network Management PGNs — Partially Missing

| PGN | Hex | Acronym | Status |
|---|---|---|---|
| 59392 | 0xE800 | ACKM | **MISSING** — J1939 Acknowledgment (positive, negative, busy, cannot respond) |
| 59904 | 0xEA00 | RQST | **MISSING** — Request any PGN from a device |
| 60160 | 0xEB00 | TP.DT | Covered via TP implementation |
| 60416 | 0xEC00 | TP.CM | Covered via TP implementation |
| 51456 | 0xC800 | ETP.CM | **MISSING** — ETP connection management (needs explicit requirement) |
| 51200 | 0xC700 | ETP.DT | **MISSING** — ETP data transfer (needs explicit requirement) |
| 60928 | 0xEE00 | AC | Covered — address claiming |
| 61184 | 0xEF00 | PropA | **MISSING** — Proprietary A (peer-to-peer manufacturer messages) |
| 65280 | 0xFF00 | PropB | **MISSING** — Proprietary B (broadcast manufacturer messages) |

---

## 6. SLOT Definitions — MISSING

The DA defines **426 SLOTs** for decoding SPN values. Each SLOT defines:
- SLOT Name (e.g. SAEbs01, SAEec01)
- Scaling (e.g. "0.125 rpm/bit")
- Range (e.g. "0–8031.875 rpm")
- Offset (e.g. "0 rpm")
- Length (e.g. "2 bytes")
- Unit (e.g. "rpm")

OmniCAN **does not** currently define:
- A SLOT lookup table or API
- An `omnican_j1939_slot_decode()` function
- Any SPN scaling/unit resolution

This means applications cannot decode raw SPN values to engineering units
without implementing SLOT lookup themselves. OmniCAN should provide:
- A compiled-in SLOT table (or Kconfig-selectable subset)
- `omnican_j1939_spn_decode(uint32_t spn, const uint8_t *raw, size_t len, double *value, const char **unit)`

---

## 7. Manufacturer IDs — MISSING

The DA contains **1,509 manufacturer ID codes** for the NAME field.
OmniCAN does not provide a lookup table or API.

Missing: `omnican_j1939_mfr_name(uint16_t mfr_id)` → manufacturer string.

---

## 8. Source Address Tables — MISSING

The DA defines preferred source addresses for 5 industry groups:
- B2: 112 global preferred addresses (e.g. SA=0 = Engine #1)
- B3: 77 on-highway preferred addresses
- B4–B7: Agricultural, Construction, Marine, Industrial

OmniCAN does not provide:
- Preferred address lookup by function/industry group
- `omnican_j1939_preferred_addr(uint8_t industry_group, uint8_t function)` → preferred SA

---

## 9. NAME Functions — MISSING

The DA defines **99 global NAME functions** (B11) + **289 IG-specific** (B12).
These define what a ECU's J1939 NAME identifies it as (e.g. Function=0 = Engine).

OmniCAN does not provide:
- NAME function lookup API
- Validation of NAME field function values against DA

---

## 10. Multi-Protocol Co-Existence — MISSING from Architecture

### 10.1 J1939 + UDS (most common real-world use case)

A single ECU running both J1939 (29-bit, 250 kbps) and UDS (11-bit ISO-TP):
- J1939 physical: 29-bit extended IDs on the same CAN bus
- UDS physical: 11-bit standard IDs (0x7E0/0x7E8) on the same CAN bus
- No filter conflict: different frame types
- **Gap**: No shared `omnican_node` configuration example or explicit gate checking
- **Gap**: J1939 diagnostic DM14/DM15/DM16 messages overlap conceptually with UDS
  memory access — need disambiguation/routing policy

### 10.2 CANopen + UDS

CANopen (11-bit) and UDS (11-bit) on the same bus — POTENTIAL CONFLICT:
- CANopen SDO server: CAN ID 0x580–0x5FF (for node IDs 1–127)
- UDS request: CAN ID 0x7E0 (and responses 0x7E8–0x7EF)
- No overlap for node IDs 1–127 since 0x580+127=0x5FF < 0x7E0
- BUT: CANopen Emergency range is 0x080–0x0FF; UDS is 0x7E0+; safe.
- **Gap**: Explicit co-existence policy and filter mask documentation missing
- **Gap**: CANopen NMT (0x000) and UDS functional address (0x7DF) both broadcast — need
  filter logic to ensure NMT doesn't interfere

### 10.3 CANopen + J1939

11-bit (CANopen) + 29-bit (J1939) — CLEAN SEPARATION:
- Frame router already handles this via frame type distinction
- **Gap**: Explicit requirement for simultaneous operation not yet in arch
- **Gap**: Priority interaction: J1939 29-bit IDs with priority bits vs CANopen 11-bit
  COB-ID arbitration — documentation missing

### 10.4 All Four Protocols Simultaneously

J1939 + UDS + CANopen + OBD-II:
- OBD-II uses 0x7DF + 0x7E8–0x7EF (same range as UDS physical)
- **CONFLICT**: OBD-II broadcast (0x7DF) and UDS functional addressing (0x7DF) overlap
- **Gap**: Policy for OBD-II + UDS co-existence when both use 0x7DF

---

## 11. Open-Source Coverage of J1939/73 (Updated)

After searching all available open-source implementations, the following MIT/Apache-2.0
licensed stacks cover J1939/73 sufficiently to implement OmniCAN's DM set:

### Available open-source implementations (MIT/Apache-2.0 — usable by OmniCAN)

| Source | License | J1939/73 coverage |
|---|---|---|
| **Open-SAE-J1939** (DanielMartensson) | MIT | DM1, DM2, DM3, DM14, DM15, DM16 — full C89 implementation. RQST (0xEA00) + ACKM (0xE800) complete. No dynamic allocation. PRIMARY SOURCE for OmniCAN porting. |
| **python-can-j1939** (juergenH87) | MIT | DTC encode/decode bit operations; DM1 lamp byte parser; DM11, DM22 |
| **AgIsoStack++** (Open-Agriculture) | MIT | C++ diagnostic_protocol: DM5, DM12, DM22 — translatable to C |
| **CSS Electronics guide** | Public | DM1–DM12 byte maps, FMI 32-code table, transmission timing |
| **EmbeddedFlakes J1939 Diagnostics** | Public | DM1–DM12 byte-level format, FMI 0–21+31 table |

### NOT usable as code (wrong license)

| Source | License | Why not | How to use |
|---|---|---|---|
| Linux kernel j1939 | GPL-2.0 | Copyleft, incompatible with Apache-2.0 | Protocol reference only |
| MicroControl J1939 Stack | Commercial | Proprietary | Public header docs as reference |

### Porting strategy for Open-SAE-J1939 → Zephyr

The only delta from Open-SAE-J1939 to OmniCAN is the hardware shim:
```c
/* Replace: CAN_Send_Message(ID, data) */
/* With: can_send(node->can_dev, &frame, K_MSEC(10), NULL, NULL) */
```
All DM byte logic, DTC encoding, request/response state machines port directly.
Attribution header required in each ported file.

## 12. What Cannot Be Found / Requires Clarification (Updated)

1. **J1939/73 full text** — RESOLVED for mandatory DM set via Open-SAE-J1939 (MIT),
   CSS Electronics guide, and EmbeddedFlakes articles. Extended DMs (DM7–DM10,
   DM20+) still require J1939/73 for precise service byte definitions.

2. **SPN 1213–1215 (Lamp Status SPNs)** — RESOLVED. Lamp byte format (MIL [7:6],
   RSL [5:4], AWL [3:2], PL [1:0]) documented in CSS Electronics guide and
   EmbeddedFlakes. Values: 00=off, 01=on, 10=flash, 11=N/A.

3. **DM4 Freeze Frame SPN list** — RESOLVED as application-configurable.
   OmniCAN uses CONFIG_OMNICAN_J1939_DM4_SPN_LIST (default: 190,84,110,91).
   Application can override via `omnican_j1939_dm4_set_spn_list()`.

4. **DM8/DM30 Test Result TID/FID format** — PARTIALLY resolved via DM10 in DA
   (lists supported test IDs). Full test procedure definitions remain in J1939/73
   text. Implement as generic container with app-supplied callbacks.

5. **DM14/DM15/DM16 memory access handshake** — RESOLVED via Open-SAE-J1939
   (MIT). Complete multi-step state machine (`Send_Request_DM14` →
   `Send_Response_DM15` → `Send_Binary_Data_DM16`) is fully implemented
   and portable to Zephyr.

6. **ETP session limits** — RESOLVED. CONFIG_OMNICAN_J1939_ETP_MAX_PAYLOAD_KB
   (default 64 KB) caps this appropriately for embedded targets.

7. **SLOT flash budget** — RESOLVED. Kconfig subset selection (mandatory minimum
   7 SPNs + CONFIG_OMNICAN_J1939_SPN_EXTRA_LIST) manages flash usage.

8. **Manufacturer-specific SPNs (>524287)** — RESOLVED. `spn_register_custom()`
   API allows runtime registration without Kconfig changes.

9. **DA revision columns** — Informational only. OmniCAN does not version-gate
   on revision codes.

10. **ISOBUS (ISO 11783)** — RESOLVED as Phase 6. AgIsoStack++ (MIT C++) serves
    as protocol reference. Native Zephyr C implementation planned.

### Remaining true gaps (require SAE J1939/73 purchase for completeness)

- DM5 monitor readiness bits exact normative mapping (which bit = which monitor)
- DM7/DM8/DM10 test ID-to-procedure table
- DM13 service byte sub-function encoding (stop/start/no-action bit positions)
- DM20 monitor performance ratio calculation formula
- DM25 expanded freeze frame SPN indexing format
- DM28 permanent DTC qualifier criteria

All other DM messages in the mandatory set (DM1-3, DM5, DM6, DM11, DM12, DM22)
are fully specified by open public documentation and the MIT-licensed Open-SAE-J1939
implementation.

---

## 12. Priority Classification

### Mandatory (must implement for J1939 spec compliance)
- DM1 with correct lamp status byte (J1939/73 mandatory)
- DM11 (already covered — clear active DTCs)
- DM3 (clear previously active DTCs)
- DTC SPN/FMI/OC encoding/decoding
- PGN 0xEA00 (RQST — required for any PGN request/response)
- PGN 0xE800 (ACKM — required for TP and DM responses)
- ETP.CM (0xC800) and ETP.DT (0xC700) explicit requirements

### High priority (strongly recommended for practical use)
- DM2 (previously active DTCs)
- DM5 (diagnostic readiness)
- DM6 (emission pending DTCs)
- DM12 (MIL-on DTCs)
- DM22 (individual DTC clear)
- SLOT decode API (without it, SPN values are raw bytes)
- PropA (0xEF00) and PropB (0xFF00) routing

### Optional via Kconfig (enable per use case)
- DM4, DM7–DM10, DM13–DM21, DM23–DM60 (via CONFIG_OMNICAN_J1939_DM_EXTENDED)
- Manufacturer ID lookup table (CONFIG_OMNICAN_J1939_MFR_TABLE)
- SLOT lookup table (CONFIG_OMNICAN_J1939_SLOT_TABLE)
- Preferred address tables B3–B7 (CONFIG_OMNICAN_J1939_IG_ADDRESSES)

### Multi-protocol (all require explicit arch coverage)
- J1939 + UDS co-existence (mandatory for any OBD-on-J1939 application)
- CANopen + UDS co-existence filter policy
- OBD-II + UDS 0x7DF arbitration policy
- Cross-protocol error propagation model

---

## 13. Summary Gap Count

| Category | Total in DA | OmniCAN | Gap |
|---|---|---|---|
| DM messages | 62 | 2 (DM1 partial, DM11) | **60 missing** |
| PGNs total | 3,193 | Routing only (no decode) | All decode missing |
| SLOTs | 426 | 0 | **426 missing** |
| SPNs (unique) | ~8,000+ | 0 | **All decode missing** |
| Manufacturer IDs | 1,509 | 0 | **1,509 missing** |
| NAME functions | 99 global + 289 IG | 0 | **388 missing** |
| IG source addresses | 4 tables | 0 | **4 tables missing** |
| Multi-protocol configs | 4 combinations | 0 explicit | **All missing** |
| Network mgmt PGNs | 9 key PGNs | 3 (TP, AC) | **6 missing** |

---

## 14. Recommended Implementation Order

1. **DTC encode/decode** (SPN+FMI+OC+CM 4-byte format)
2. **DM1 lamp status byte** — fix existing DM1 implementation
3. **RQST (0xEA00) and ACKM (0xE800)** — needed for DM request/response
4. **DM2, DM3, DM5, DM6, DM11, DM12, DM22** — core diagnostic set
5. **ETP.CM / ETP.DT explicit requirements**
6. **PropA / PropB routing**
7. **SLOT decode API** (Kconfig-selectable subset)
8. **Multi-protocol co-existence** arch and requirements
9. **Extended DM messages** (DM7–DM60 via Kconfig)
10. **Manufacturer ID and NAME function tables** (optional Kconfig)
