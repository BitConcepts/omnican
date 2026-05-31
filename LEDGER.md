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
