"""Batch-import parsed requirements and tests into specsmith ESDB."""
import json
import subprocess
import sys
from pathlib import Path

root = Path(__file__).parent.parent

reqs = json.loads((root / ".specsmith" / "reqs.json").read_text())
tests = json.loads((root / ".specsmith" / "tests.json").read_text())

ok = 0
fail = 0

print(f"Importing {len(reqs)} requirements...")
for r in reqs:
    result = subprocess.run(
        [
            "specsmith", "req", "add",
            "--id", r["id"],
            "--title", r["description"][:80],
            "--description", r["description"],
            "--status", "planned",
            "--source", f"docs/ARCHITECTURE.md ## {r['component']}",
        ],
        capture_output=True,
        text=True,
        cwd=root,
    )
    if result.returncode != 0:
        print(f"  FAIL {r['id']}: {result.stderr.strip()}", file=sys.stderr)
        fail += 1
    else:
        ok += 1

print(f"  {ok} OK, {fail} failed")

ok = 0
fail = 0

print(f"\nImporting {len(tests)} test cases...")
for t in tests:
    result = subprocess.run(
        [
            "specsmith", "test", "add",
            "--id", t["id"],
            "--title", t["description"][:80],
            "--description", t["description"],
            "--req", t["requirement_id"],
        ],
        capture_output=True,
        text=True,
        cwd=root,
    )
    if result.returncode != 0:
        print(f"  FAIL {t['id']}: {result.stderr.strip()}", file=sys.stderr)
        fail += 1
    else:
        ok += 1

print(f"  {ok} OK, {fail} failed")
print("\nDone.")
