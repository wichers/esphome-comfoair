"""Reject inconsistent release metadata before a tag can be published."""

import argparse
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def check_release(version):
    """Validate a stable version and every user-visible copy of that version."""
    if not re.fullmatch(r"(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)", version):
        raise ValueError("Use a stable version such as 1.0.0, without the v prefix")
    if (ROOT / "VERSION").read_text().strip() != version:
        raise ValueError("Requested version must match VERSION")
    for example in ROOT.glob("comfoair*.yaml"):
        source = example.read_text()
        versions = re.findall(r'^\s+version: "([^"]+)"$', source, re.MULTILINE)
        if versions != [version] or f"ref: v{version}\n" not in source:
            raise ValueError(f"Update project version and component ref in {example.name}")
    header = (ROOT / "components/comfoair/comfoair.h").read_text()
    if f"Component version: {version}" not in header:
        raise ValueError("Update the component version in dump_config")
    if f"## [{version}]\n" not in (ROOT / "CHANGELOG.md").read_text():
        raise ValueError("Add this version to CHANGELOG.md")
    if not (ROOT / "RELEASE_NOTES.md").read_text().startswith(f"# v{version}\n"):
        raise ValueError("Update RELEASE_NOTES.md for this version")
    return f"v{version}"


def main():
    """Print the checked tag or exit unsuccessfully with an actionable message."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("version")
    arguments = parser.parse_args()
    try:
        print(check_release(arguments.version))
    except ValueError as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
