"""Exercise validation and code generation through the real ESPHome CLI."""

import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parents[1]


class ConfigTests(unittest.TestCase):
    def run_config(self, *, uart=None, select=None, generate=False):
        config = yaml.safe_load((ROOT / "tests/minimal.yaml").read_text())
        config["external_components"][0]["source"]["path"] = str(ROOT / "components")
        if uart:
            for key, value in uart.items():
                if value is None:
                    config["uart"].pop(key, None)
                else:
                    config["uart"][key] = value
        if select is not None:
            config["comfoair"]["size_select"] = select
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.yaml"
            path.write_text(yaml.safe_dump(config))
            command = [sys.executable, "-m", "esphome"]
            command += ["compile", "--only-generate"] if generate else ["config"]
            result = subprocess.run(command + [str(path)], capture_output=True, text=True, check=False)
            generated = ""
            if generate and result.returncode == 0:
                generated = (Path(directory) / ".esphome/build/comfoair-minimal/src/main.cpp").read_text()
            return result, generated

    def test_minimal_without_size_select(self):
        result, generated = self.run_config(generate=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertNotIn("set_size_select", generated)

    def test_explicit_size_select(self):
        for select in ({"name": "Unit size"}, {"id": "unit_size"}):
            with self.subTest(select=select):
                result, generated = self.run_config(select=select, generate=True)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.assertIn("set_size_select", generated)

    def test_invalid_uart(self):
        cases = [
            ({"baud_rate": 19200}, "requires baud rate 9600"),
            ({"data_bits": 7}, "requires 8 data bits"),
            ({"parity": "EVEN"}, "requires parity NONE"),
            ({"stop_bits": 2}, "requires 1 stop bits"),
            ({"rx_pin": None}, "declare a rx_pin"),
            ({"tx_pin": None}, "declare a tx_pin"),
        ]
        for uart, message in cases:
            with self.subTest(uart=uart):
                result, _ = self.run_config(uart=uart)
                self.assertNotEqual(result.returncode, 0)
                self.assertIn(message, result.stdout + result.stderr)


if __name__ == "__main__":
    unittest.main()
