"""Generate docs/REQUIREMENTS.md and docs/TESTS.md in specsmith sync format."""
from pathlib import Path
import json

root = Path(__file__).parent.parent
reqs_raw = json.loads((root / ".specsmith" / "reqs.json").read_text())
tests_raw = json.loads((root / ".specsmith" / "tests.json").read_text())

# Map component names -> short REQ prefix codes
COMP_MAP = {
    "Core Node":           "CORE",
    "Frame Router":        "ROUTER",
    "CANopen CiA 301":     "CO",
    "J1939":               "JNET",
    "UDS ISO 14229":       "UDS",
    "OBD-II J1979":        "OBD",
    "CANopen FD CiA 1301": "COFD",
    "ISOTP Patch":         "ISOTP",
    "Memory and Threading":"MEM",
}

# Build normalised ID map: old parse-reqs ID -> new REQ-COMP-NNN
counters = {}
id_map = {}
for r in reqs_raw:
    comp = r["component"]
    code = COMP_MAP.get(comp, comp.upper().replace(" ", "").replace("-", "")[:6])
    counters[code] = counters.get(code, 0) + 1
    new_id = f"REQ-{code}-{counters[code]:03d}"
    id_map[r["id"]] = new_id

# ── REQUIREMENTS.md ────────────────────────────────────────────────────────
lines = [
    "# OmniCAN — Requirements",
    "",
    "**Status:** Draft — parsed from docs/ARCHITECTURE.md via specsmith.",
    "",
    "---",
    "",
    "## Naming Convention",
    "",
    "```",
    "REQ-<COMPONENT>-<NUMBER>",
    "```",
    "",
    "Component codes: CORE, ROUTER, CO (CANopen), JNET (J1939), UDS, OBD (OBD-II), COFD, ISOTP, MEM.",
    "",
    "---",
    "",
    "## Requirements",
    "",
]

for r in reqs_raw:
    new_id = id_map[r["id"]]
    lines += [
        f"### {new_id}",
        f"- **Title**: {r['description'][:80]}",
        f"- **Status**: planned",
        f"- **Description**: {r['description']}",
        f"- **Source**: docs/ARCHITECTURE.md ## {r['component']}",
        "",
    ]

(root / "docs" / "REQUIREMENTS.md").write_text("\n".join(lines), encoding="utf-8")
print(f"Wrote docs/REQUIREMENTS.md ({len(reqs_raw)} requirements)")

# ── TESTS.md ───────────────────────────────────────────────────────────────
tlines = [
    "# OmniCAN — Test Specification",
    "",
    "**Status:** Draft — generated from requirements via specsmith.",
    "",
    "---",
    "",
    "## Tests",
    "",
]

for t in tests_raw:
    req_new = id_map.get(t["requirement_id"], t["requirement_id"])
    tlines += [
        f"### {t['id']}",
        f"- Covers: {req_new}",
        f"- **Type**: {t['type']}",
        f"- **Description**: {t['description']}",
        f"- **Status**: Pending",
        "",
    ]

(root / "docs" / "TESTS.md").write_text("\n".join(tlines), encoding="utf-8")
print(f"Wrote docs/TESTS.md ({len(tests_raw)} test cases)")

# Save updated id_map for reference
(root / ".specsmith" / "id_map.json").write_text(
    json.dumps(id_map, indent=2), encoding="utf-8"
)
print("Saved .specsmith/id_map.json")
