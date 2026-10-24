# Changes in version 2026.10.23 (PR#36)

- Deleted `bisect.sh` and added `bisect-2026-10-16.R` for computing the bisect results using the new version of atime, and added `bisect-animint.R` for creating the corresponding interactive graphics.
- `.ci/atime/tests.R` uses new `ci.commits` argument instead of for loop, to define the historical versions to show in each of the CI performance tests.

# Changes in version 2026.6.15 (PR#30)

- New custom `pkg.edit.fun` which works to compile arbitrary historical ponca versions, thus enabling git bisect to find the first commit with a performance regression. See `.ci/atime/bisect.sh` for an example.

