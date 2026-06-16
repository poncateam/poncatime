set -o errexit
pushd external/ponca
git checkout v1.2
popd
cp wrapper-versions/PoncaV1x2/* .
rm -f *o
R -e "Rcpp::compileAttributes('..')"
R CMD INSTALL ..
R --vanilla < ../tests/testthat/test-CRAN.R
