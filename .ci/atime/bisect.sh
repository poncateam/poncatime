## git bisect old=equal perf, new=divergent.
cd ../../src/external/ponca
git bisect start
git bisect old v1.0
git bisect new v1.4
git bisect run Rscript ~/R/poncatime/.ci/atime/bisect-test.R
