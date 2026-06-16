# Changes in version 2026.6.15 (PR#30)

- New custom `pkg.edit.fun` which works to compile arbitrary historical ponca versions, thus enabling git bisect to find the first commit with a performance regression. See `.ci/atime/bisect.sh` for an example.

