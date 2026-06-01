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

## 2026-06-01T04:45 — Items 2,5,6,7,8 resolved. ETP Kconfig: CONFIG_OMNICAN_J1939_ETP_MAX_PAYLOAD_KB (default 64KB, range 1-16384KB). DM4 freeze frame: CONFIG_OMNICAN_J1939_DM4_SPN_LIST (default SPNs 190,84,110,91) + dm4_set_spn_list() runtime override. SLOT subset: mandatory minimum 7 SPNs (190,84,110,183,91,247,250) + CONFIG_OMNICAN_J1939_SPN_EXTRA_LIST for additions + spn_register_custom() for mfr SPNs >524287. Mfr IDs: mfr_register() for custom DA entries. Branch protection: main and develop now require PR + 1 approval; tbitcs has bypass. develop branch created. ISOBUS clarification: ISO 11783 != ISO-TP; ISOBUS is J1939-based agricultural app layer (Phase 6 candidate).
- **Author**: agent
- **Type**: task
- **Status**: complete
- **Chain hash**: `dca339275bf5fc26...`

## 2026-06-01T06:49 — J1939/73 alternate sources resolved: Open-SAE-J1939 (MIT C, DM1-DM16 + ISO 11783-7), python-can-j1939 (DTC bit encoding), CSS Electronics guide (lamp bytes, FMI table), EmbeddedFlakes (DM1-DM12). DTC C encode formula documented in STANDARDS.md. ISOBUS ISO 11783 added as Phase 6: AgIsoStack++ (MIT C++, 344 stars) as primary reference + integration point. ISO 11783-1/2/3/5/6/7/10/13/14 standards registered. west.yml: Open-SAE-J1939 (j1939-ref group) + AgIsoStack++ (isobus group) added as optional modules. 154 REQs/154 tests, 29/30 audit.
- **Author**: agent
- **Type**: task
- **Status**: complete
- **Chain hash**: `b3bdfe017ecd294c...`

## 2026-06-01T06:55 — ISOBUS integration decision: native Zephyr C, NOT AgIsoStack++ wrapper. AgIsoStack++ uses C++11 STL (std::vector, std::shared_ptr), POSIX threads (std::thread), dynamic allocation — incompatible with OmniCAN static-allocation model. CAN_STACK_DISABLE_THREADS=ON still requires STL+heap. Bare-metal ARM Cortex-M freestanding not supported (open issue #639, Dec 2025). AgIsoStack++ used as REFERENCE ONLY (protocol state machines, VT upload sequence, TC DDOP format). Phase 6 written from scratch in C with k_thread/k_work, static alloc, zephyr/drivers/can.h — same patterns as Phases 1-5. west.yml group renamed isobus-ref.
- **Author**: agent
- **Type**: task
- **Status**: complete
- **Chain hash**: `f6e014c52e883a62...`

## 2026-06-01T07:02 — J1939/73 open-source coverage analysis complete. PRIMARY SOURCE: Open-SAE-J1939 (MIT C89, no dynamic alloc, has DM1/DM2/DM3/DM14/DM15/DM16 + RQST + ACKM). Porting delta = only hardware shim (CAN_Send_Message -> zephyr/drivers/can.h). Additional: python-can-j1939 (MIT, DTC bits), AgIsoStack++ (MIT C++, DM5/DM12/DM22). NOT usable as code: Linux kernel j1939 (GPL-2.0), MicroControl (commercial). Remaining true gaps requiring J1939/73 purchase: DM5 monitor readiness bit mapping, DM7/8/10 test ID table, DM13/20/25/28 sub-function details. All mandatory DM set (DM1-3,5,6,11,12,22) fully coverable from open sources. J1939DA_GAP.md updated with resolution status for all original 10 items.
- **Author**: agent
- **Type**: task
- **Status**: complete
- **Chain hash**: `c56ed129d68212bb...`
