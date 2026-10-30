"""Validate the shipped installation examples against the local component."""

import base64
import os
import re
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parents[1]


class ExampleTests(unittest.TestCase):
    """Use disposable credentials; never load installation secrets or contact devices."""

    def test_examples(self):
        """All examples validate, including the documented optional proxy pins."""
        for example in sorted(ROOT.glob("comfoair*.yaml")):
            for proxy in (False, True) if "# - id: uart_proxy" in example.read_text() else (False,):
                with self.subTest(example=example.name, proxy=proxy):
                    source = example.read_text()
                    # Keep the public example; override only source and credentials.
                    source = re.sub(
                        r"      type: git\n      url: [^\n]+\n      ref: [^\n]+",
                        f"      type: local\n      path: {ROOT / 'components'}", source,
                    )
                    if proxy:
                        source = re.sub(
                            r"(?m)^  # (- id: uart_proxy|  (?:baud_rate|tx_pin|rx_pin):.*)$",
                                        r"  \1", source)
                        source = source.replace(
                            "  # proxy_uart: uart_proxy", "  proxy_uart: uart_proxy"
                        )
                    names = set(re.findall(r"!secret (\w+)", source))
                    secrets = {name: "test-" + os.urandom(12).hex() for name in names}
                    secrets["comfoair_api_encryption_key"] = (
                        base64.b64encode(os.urandom(32)).decode()
                    )
                    secrets["mqtt_host"] = "192.0.2.1"
                    with tempfile.TemporaryDirectory() as directory:
                        path = Path(directory) / example.name
                        path.write_text(source)
                        (Path(directory) / "secrets.yaml").write_text(yaml.safe_dump(secrets))
                        result = subprocess.run(
                            [sys.executable, "-m", "esphome", "config", str(path)],
                            capture_output=True, text=True, check=False,
                        )
                        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
