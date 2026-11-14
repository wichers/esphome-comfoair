"""Exercise the release metadata gate without publishing or accessing GitHub."""

import runpy
import subprocess
import sys
import unittest
from pathlib import Path
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]


class ReleaseTests(unittest.TestCase):
    """Check valid metadata and reject inconsistent or unsafe tag inputs."""

    def test_version_change_gate(self):
        """Skip unchanged versions, validate changed ones, and fail on bad refs."""
        script = runpy.run_path(str(ROOT / ".github/scripts/check_release.py"))
        main = script["main"]
        version = (ROOT / "VERSION").read_text().strip()
        for previous in (version, "0.0.0"):
            with (
                self.subTest(previous=previous),
                patch.object(sys, "argv", ["check_release.py", "--previous-ref", "before"]),
                patch(
                    "subprocess.run",
                    return_value=subprocess.CompletedProcess([], 0, stdout=f"{previous}\n"),
                ),
                patch("builtins.print") as output,
            ):
                main()
                if previous == version:
                    output.assert_not_called()
                else:
                    output.assert_called_once_with(f"v{version}")
        with (
            patch.object(sys, "argv", ["check_release.py", "--previous-ref", "bad"]),
            patch("subprocess.run", side_effect=subprocess.CalledProcessError(1, "git")),
            patch("sys.stderr"),
            self.assertRaises(SystemExit) as raised,
        ):
            main()
        self.assertEqual(raised.exception.code, 2)

    def test_release_gate(self):
        """Only the repository's declared version may be released."""
        version = (ROOT / "VERSION").read_text().strip()
        for requested in (version, "99.0.0", "v1.0.0", "1.0.0;echo bad", "01.0.0"):
            with self.subTest(version=requested):
                result = subprocess.run(
                    [sys.executable, str(ROOT / ".github/scripts/check_release.py"), requested],
                    capture_output=True,
                    text=True,
                    check=False,
                )
                self.assertEqual(
                    result.returncode == 0, requested == version, result.stdout + result.stderr
                )
