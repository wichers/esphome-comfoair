## Problem and resulting behavior

<!-- Explain the concrete trigger and what this change does. -->

## Validation

<!-- Regression test, checks run, and hardware/model evidence if available.
Separate software tests from physical-device validation. -->

## User impact

<!-- YAML/entity/unit changes, migration steps, or “None”. Update the changelog
for the next release; never move a published release tag. -->

## Release

- [ ] No release: `VERSION` is unchanged.
- [ ] Release: updated `VERSION`, component log version, all example project
      versions and source refs, `CHANGELOG.md`, and `RELEASE_NOTES.md` together.
- [ ] For a release, reviewed hardware evidence and explicitly listed unverified
      scenarios in `RELEASE_NOTES.md`; software tests are not hardware validation.

<!-- Choose no release or release. Merging a VERSION change into master
  automatically builds, tags and publishes after the release checks pass.
  Validate release metadata with: python .github/scripts/check_release.py -->
