# Versioned releases

`VERSION` starts at **1.0.0**. The earlier `1.1.0` example strings were not
published releases and do not establish a release history. Tags and GitHub
Releases are the public component versions; ESPHome has its own version.

Use patch releases for fixes, minor releases for compatible additions, and major
releases for breaking YAML/entity contracts. Never move or overwrite a published
tag. Support reports should include both the component tag/commit and ESPHome
version. `dump_config` logs the component version; examples set matching project
metadata. For untagged development builds also record the Git commit.

## Prepare and publish

1. Make a focused PR updating `VERSION`, the component version in `dump_config`,
   every example's project version and source `ref`, `CHANGELOG.md`,
   `RELEASE_NOTES.md`, and the Release workflow's default version. Run
   `python .github/scripts/check_release.py 1.0.0` with the actual version.
2. Require all ESPHome workflow jobs to pass, review migration notes and merge
   the release's fixes. A repository administrator should configure a branch
   rule requiring these checks and PR review; this cannot be configured by a
   contributor without administration permission.
3. Test the merged commit using the installation's own YAML with a local source.
   Confirm the board, flash/PSRAM, UART pins, network target and secrets before
   any upload. Record model/firmware, entity identities and baseline readings;
   then observe multiple complete polling cycles after reboot. Aim for a
   90-second capture and record its actual duration. Do not operate installer/reset
   controls as a smoke test. Record communication status/response age and any
   automation-driven control commands. Mark a serial-disconnect recovery test as
   unverified unless it was actually performed and recovery was observed.
4. Track wall-controller validation separately: display updates and ordinary
   fan-mode changes from both sides, ACK/reply order, timeout recovery, and lost
   commands. Record model/firmware and relative serial timestamps if tested.
   Software fixtures do not establish hardware verification. A direct-UART
   release may proceed with proxy/recovery/installer scenarios explicitly marked
   hardware-unverified in the public release notes.
5. From Actions → Release, select **master**, enter the version, confirm the
   evidence review and provide the actual hardware results plus a separate list
   of unverified scenarios. The workflow validates metadata,
   reruns the full baseline CI and latest-stable check, verifies master has not
   advanced, and creates the tag and release using `gh` only if all checks pass.
   Existing tags are rejected. No universal binary is uploaded.

The workflow does not publish on PR merge, tag push or schedule. A manual
dispatch is the deliberate publication step. There is no release before the
required PRs merge and the recorded hardware evidence and limits are reviewed.
The confirmation attests the accuracy of the evidence; it does not attest that
every scenario has been tested. Never mark an unperformed test as verified.

## Installation and rollback

Normal installations use `source.ref: v1.0.0` and `refresh: never`, replacing
the version only when deliberately upgrading. Development uses a local source
or an explicitly chosen branch. Source changes do not upload firmware by
themselves. To roll back after a later release, select the previous published
tag, rebuild with its tested ESPHome version, and upload using the same device
settings. Before the first release there is no previous published tag; retain
your known-good installation YAML, source revision and firmware backup.

Read unit/statistics and control behavior changes before upgrades. Binary/text
readings remain last-known during outages, so automations must gate them on the
serial `connected` diagnostic. MQTT consumers must treat non-finite numeric
measurements as unavailable.
