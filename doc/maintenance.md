# Maintenance priorities

## Implemented fixes

- UART final validation replaces the deprecated C++ runtime check. The
  installed ESPHome 2026.10.0 API accepts all specified keywords. Its
  `uart.h` marks the old function deprecated since 2026.9.0, with removal in
  2027.3.0. Incorrect UART YAML now intentionally fails before compilation.
- Issue #97: removing `size_select`'s implicit empty default prevents creation
  of an entity without a name or ID. Existing codegen skips absent entities,
  and C++ initializes the select pointer to null and checks it before use.
- Configuration/codegen regression tests, native tests of the real C++ serial
  implementation, and a version/hardware firmware CI matrix.

API references:

- [ESPHome final validation](https://developers.esphome.io/architecture/components/)
- [UART component architecture](https://developers.esphome.io/architecture/components/uart/)
- [UART Python API source](https://github.com/esphome/esphome/blob/2026.10.0/esphome/components/uart/__init__.py)

## Follow-up work (not mixed into the fixes)

| Priority | Improvement | Risk / verification needed |
| --- | --- | --- |
| P1 | Run the new GitHub CI matrix before merging | Local success does not establish Linux runner/toolchain compatibility. Version pins need maintenance. |
| P2 | Add per-command decode fixtures and climate command tests | Current protocol tests cover framing, not every published sensor value or control action. Verify against captured unit traffic without changing wire semantics. |
| P2 | Physical UART and Home Assistant smoke test | Software tests cannot establish RS232 electrical behavior, timing, fan control, or entity behavior on a real installation. |
| P3 | Convert `to_code` from `yield` to `async`/`await` | ESPHome 2026.10.0 still explicitly supports old-style coroutines. A separate mechanical change can be checked by comparing generated C++ and running both version pins; no urgency for this fix. |
| P3 | Consolidate repeated sensor/number schemas | Avoid changing defaults, units, ranges, IDs or entity metadata. Prefer a separate reviewable refactor with exhaustive schema fixtures; no new helper is needed for the current fixes. |
| P3 | More specific serial receive error logs | The loop currently calls every invalid frame an invalid checksum, including length errors. Improve diagnostics separately, without changing acceptance of legacy frames. |

The standard UART validator already identifies the component, required setting
and referenced UART. Custom validation/error wrappers would duplicate ESPHome
behavior and are not needed.

## Open pull requests checked before implementation

- #96: optional second UART/ComfoSense proxy; intentionally excluded. It modifies
  the same Python and C++ files, so rebasing and rerunning protocol tests is
  necessary if it merges.
- #95: fault bits and reset buttons; separate features, overlapping files.
- #94: Auto ventilation level and zero fan-period handling; separate C++ fix.

No remote branches or pull requests are created by these local commits, and no
open PR is merged or modified.
