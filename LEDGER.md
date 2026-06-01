# Change Ledger

## 2026-05-31 — specsmith import
- Imported project: omnican
- Detected type: cli-python
- Language: unknown
- Build system: 

## 2026-05-31T18:22 — Applied all 6 pending specsmith migrations (v001-v006): governance YAML, slim AGENTS.md, compliance structure, ESDB init, agent-tools.json, session protocol. Fixed: added Apache-2.0 LICENSE, replaced Python CI pipeline with west build + clang-format, corrected scaffold.yml verification_tools (test: west twister), added type_confirmed: true. Created stub .c sources and per-module CMakeLists.txt for all 7 src/ modules (core, canopen, canopen_fd, j1939, uds, obd2, isotp_patch). Audit: 28/29 pass (1 false-positive: specsmith scanner does not recognize Zephyr west module markers as embedded-hardware).
- **Author**: agent
- **Type**: task
- **Status**: complete
- **Chain hash**: `3a80e76418931488...`

## 2026-05-31T18:44 — Architecture, Requirements, and Test Specification phases complete. docs/ARCHITECTURE.md: full 5-phase design (CANopen/J1939/UDS/OBD-II/CANopenFD), thread/memory model, Kconfig summary. 63 requirements (REQ-CORE/ROUTER/CO/JNET/UDS/OBD/COFD/ISOTP/MEM) with 100% test coverage (TEST-001..063). Specsmith audit: 29/30 pass. Phase advanced to Test Specification.
- **Author**: agent
- **Type**: task
- **Status**: complete
- **Chain hash**: `dede1262338dce9c...`

## 2026-05-31T18:48 — Release v0.1.0: all 7 AEE phases complete. CHANGELOG.md, docs/COMPLIANCE.md, SEAL-0003 milestone sealed. Governance baseline ready for protocol implementation.
- **Author**: agent
- **Type**: task
- **Status**: complete
- **Chain hash**: `bab59fbf54bc3beb...`

## 2026-05-31T18:59 — Architecture expanded to completion (921 lines): system Mermaid diagram, frame router dispatch table, CANopen NMT/SDO/PDO/Emergency/Heartbeat state machines, OD structure, J1939 29-bit ID layout, 64-bit NAME field, address claiming sequence, TP/ETP state machine, UDS session state machine, SecurityAccess flow, data transfer flow, OBD-II PID decode table, CANopen FD comparison table, ISOTP patch flow, full Kconfig reference (18 symbols), memory budget, integration guide. docs/STANDARDS.md created: normative registry for ISO 11898, CiA 301/1301, J1939/11/21/81, ISO 14229-1/3, J1979, ISO 15765-2/4 with edition pins.
- **Author**: agent
- **Type**: task
- **Status**: complete
- **Chain hash**: `b5ef52043b0e80c1...`

## 2026-06-01T04:07 — Gap analysis + full protocol expansion: 99 REQs, 99 tests (was 63/63). Added: OBD-II all 9 modes + full PID set + DTC decode (SAE J2012) + physical addressing + timeout/DTC callbacks. UDS manufacturer DID range (0xF200-0xFEFF) + mandatory ECU ID DIDs + suppressPositiveResponse + NRC 0x78 + session/security/reset/transfer/DID-write/routine/NRC hooks. CANopen NMT/SDO/PDO/EMCY/SYNC/HB/bootup callbacks. J1939 address-result/TP-progress/TP-complete callbacks + DM1/DM11 diagnostic PGNs. Frame router 11-bit+29-bit coexistence + CANopen FD 29-bit filter. SAE J2012 added to STANDARDS.md. Audit: 29/30 pass.
- **Author**: agent
- **Type**: task
- **Status**: complete
- **Chain hash**: `c64a9b7484399f7d...`

## 2026-06-01T04:23 — J1939DA DEC2024 gap analysis + multi-protocol expansion: 141 REQs/141 tests (was 99). Added: J1939/73 as normative + J1939DA DEC2024 as normative reference. Multi-protocol co-existence (J1939+UDS, CANopen+UDS, CANopen+J1939, OBD2+UDS conflict policy, cross-protocol error hook). J1939 RQST (0xEA00) + ACKM (0xE800) mandatory. ETP.CM/ETP.DT explicit. PropA/PropB routing. DTC 4-byte SPN/FMI/OC/CM encode/decode with 32 FMI codes. DM mandatory set: DM1 (lamp status corrected), DM2, DM3, DM5, DM6, DM12, DM22 + dm_request(). DM extended set (DM4, DM13, DM20-21, DM25-31) via Kconfig. SLOT decode API (426 SLOTs). Manufacturer ID table (1,509). IG address/NAME function tables. docs/J1939DA_GAP.md: complete gap report. docs/STANDARDS.md: J1939/73 now normative. SEAL-0005 audit gate.
- **Author**: agent
- **Type**: task
- **Status**: complete
- **Chain hash**: `d7c795af9bc803be...`
