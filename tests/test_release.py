"""Exercise the release metadata gate without publishing or accessing GitHub."""

import subprocess
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class ReleaseTests(unittest.TestCase):
    """Check valid metadata and reject inconsistent or unsafe tag inputs."""

    def test_release_gate(self):
        """Only the repository's declared version may be released."""
        version = (ROOT / "VERSION").read_text().strip()
        for requested in (version, "99.0.0", "v1.0.0", "1.0.0;echo bad", "01.0.0"):
            with self.subTest(version=requested):
                result = subprocess.run(
                    [sys.executable, str(ROOT / ".github/scripts/check_release.py"), requested],
                    capture_output=True, text=True, check=False,
                )
                self.assertEqual(result.returncode == 0, requested == version,
                                 result.stdout + result.stderr)
