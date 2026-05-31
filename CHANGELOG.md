# Changelog

All notable changes to OmniCAN will be documented in this file.

The format follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).
This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [0.1.0] — 2026-05-31

### Summary
Initial governed scaffold release of OmniCAN — a unified, multi-protocol CAN stack
for Zephyr RTOS. This release establishes the full AEE governance baseline through the
Test Specification phase and is ready for protocol implementation.

### Added
- **Architecture** (`docs/ARCHITECTURE.md`): full 5-phase design covering CANopen CiA 301,
  SAE J1939, ISO 14229 / UDS, SAE J1979 / OBD-II, and CANopen FD CiA 1301; thread/memory
  model; Kconfig summary; Zephyr v3.7.0 dependency manifest
- **Requirements** (`docs/REQUIREMENTS.md`): 63 requirements across 9 components
  (CORE, ROUTER, CO, JNET, UDS, OBD, COFD, ISOTP, MEM)
- **Test Specification** (`docs/TESTS.md`): 63 unit test specifications with 100%
  requirement traceability (TEST-001 … TEST-063)
- **Public API headers** (`include/omnican/`): complete public C API for all 5 protocols —
  `core.h`, `canopen.h`, `j1939.h`, `uds.h`, `obd2.h`, `version.h`, `omnican.h`
- **Source stubs** (`src/`): compilable stub implementations for all 7 modules —
  `core`, `canopen`, `canopen_fd`, `j1939`, `uds`, `obd2`, `isotp_patch`
- **Kconfig** (`Kconfig`): all `CONFIG_OMNICAN_*` symbols with full dependency graph
- **CMake** (`CMakeLists.txt`): Kconfig-guarded per-protocol subdirectory dispatch
- **West manifest** (`west.yml`): Zephyr v3.7.0 pin + optional CANopenNode reference
- **CI** (`.github/workflows/ci.yml`): west build matrix (5 protocol variants) +
  clang-format lint + west twister test pipeline
- **Governance** (`.specsmith/`, `docs/governance/`, `AGENTS.md`, `LEDGER.md`):
  full specsmith 0.11.8 governance overlay; all 6 migrations applied; trace vault
  sealed (SEAL-0001 architecture, SEAL-0002 verification)
- **Compliance report** (`docs/COMPLIANCE.md`): 63/63 (100%) requirement coverage matrix
- **LICENSE**: Apache-2.0

### Protocol Implementation Status
| Protocol | Phase | Status |
|---|---|---|
| CANopen CiA 301 | Phase 1 | Stub — implementation next |
| SAE J1939 | Phase 2 | Stub — implementation next |
| ISO 14229 / UDS | Phase 3 | Stub — implementation next |
| SAE J1979 / OBD-II | Phase 4 | Stub — implementation next |
| CANopen FD CiA 1301 | Phase 5 | Stub — depends on Phase 1 |

### Governance
- specsmith phase: `🚀 Release`
- Audit: 29/30 checks pass (1 known false-positive: specsmith scanner cannot
  distinguish Zephyr west modules from generic C/C++ CLI projects)
- Trace vault: 2 seals (SEAL-0001 architecture, SEAL-0002 verification)

[0.1.0]: https://github.com/BitConcepts/omnican/releases/tag/v0.1.0
