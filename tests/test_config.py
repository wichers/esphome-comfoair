"""Exercise validation and code generation through the real ESPHome CLI."""

import subprocess
import sys
import tempfile
import unittest
from importlib import import_module
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parents[1]
# components/ is an ESPHome namespace directory, not an installed Python package.
helper_comfoair = import_module("components.comfoair").helper_comfoair


class ConfigTests(unittest.TestCase):
    """Validate optional entities and the UART contract."""

    @staticmethod
    def add_proxy(config, proxy):
        """Add a second UART (uart_proxy) and set proxy_uart.

        None values in proxy remove keys. The key "use" sets proxy_uart directly.
        """
        proxy_uart = {"id": "uart_proxy", "tx_pin": "GPIO17", "rx_pin": "GPIO18",
                      "baud_rate": 9600}
        for key, value in proxy.items():
            if key == "use":
                continue
            if value is None:
                proxy_uart.pop(key, None)
            else:
                proxy_uart[key] = value
        config["uart"] = [config["uart"], proxy_uart]
        config["comfoair"]["proxy_uart"] = proxy.get("use", "uart_proxy")

    def run_config(self, *, uart=None, proxy=None, select=None, entities=None, generate=False):
        """Run an isolated config, optionally returning its generated C++."""
        config = yaml.safe_load((ROOT / "tests/minimal.yaml").read_text())
        config["external_components"][0]["source"]["path"] = str(ROOT / "components")
        if uart:
            for key, value in uart.items():
                if value is None:
                    config["uart"].pop(key, None)
                else:
                    config["uart"][key] = value
        if proxy is not None:
            self.add_proxy(config, proxy)
        if select is not None:
            config["comfoair"]["size_select"] = select
        if entities:
            config["comfoair"].update(entities)
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.yaml"
            path.write_text(yaml.safe_dump(config))
            command = [sys.executable, "-m", "esphome"]
            command += ["compile", "--only-generate"] if generate else ["config"]
            result = subprocess.run(
                command + [str(path)], capture_output=True, text=True, check=False
            )
            generated = ""
            if generate and result.returncode == 0:
                source = Path(directory) / ".esphome/build/comfoair-minimal/src/main.cpp"
                generated = source.read_text()
            return result, generated

    def test_minimal_without_size_select(self):
        """Issue #97: an omitted select must not create an entity."""
        result, generated = self.run_config(generate=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertNotIn("set_size_select", generated)

    def test_explicit_size_select(self):
        """Existing named and ID-only selects still generate code."""
        for select in ({"name": "Unit size"}, {"id": "unit_size"}):
            with self.subTest(select=select):
                result, generated = self.run_config(select=select, generate=True)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.assertIn("set_size_select", generated)

    def test_all_entities(self):
        """Every supported entity must validate and keep its codegen setter."""
        entities = {
            key: {"name": key}
            for keys in helper_comfoair.values()
            for key in keys
        }
        result, generated = self.run_config(entities=entities, generate=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        for key in entities:
            self.assertIn(f"set_{key}(", generated)
        result, _ = self.run_config(entities=entities)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        validated = yaml.safe_load(result.stdout)["comfoair"]
        for key in helper_comfoair["sensor"]:
            self.assertEqual(validated[key]["state_class"], "measurement")
        for key in helper_comfoair["number"]:
            level = key.rsplit("_", 1)[1]
            self.assertEqual(validated[key]["initial_value"],
                             {"absent": 15, "low": 35, "medium": 50, "high": 70}[level])
            self.assertEqual(validated[key]["min_value"], 15)
            self.assertEqual(validated[key]["max_value"], 95)
            self.assertEqual(validated[key]["step"], 1)
            self.assertEqual(validated[key]["unit_of_measurement"], "%")

    def test_invalid_uart(self):
        """Invalid UART settings fail before compilation with useful errors."""
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

    def test_proxy_uart(self):
        """A valid second UART generates the proxy setter."""
        result, generated = self.run_config(proxy={}, generate=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("set_proxy_uart_component(", generated)

    def test_invalid_proxy_uart(self):
        """The display UART needs 9600 8N1 with RX and TX, on its own UART."""
        cases = [
            ({"baud_rate": 115200}, "requires baud rate 9600"),
            ({"data_bits": 7}, "requires 8 data bits"),
            ({"parity": "EVEN"}, "requires parity NONE"),
            ({"stop_bits": 2}, "requires 1 stop bits"),
            ({"rx_pin": None}, "declare a rx_pin"),
            ({"tx_pin": None}, "declare a tx_pin"),
            ({"use": "uart_bus"}, "proxy_uart must use a different UART than uart_id"),
        ]
        for proxy, message in cases:
            with self.subTest(proxy=proxy):
                result, _ = self.run_config(proxy=proxy)
                output = result.stdout + result.stderr
                self.assertNotEqual(result.returncode, 0)
                self.assertIn(message, output)
                if "use" not in proxy:
                    self.assertIn("comfoair proxy_uart", output)


if __name__ == "__main__":
    unittest.main()
