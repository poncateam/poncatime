set -o errexit
pushd external/ponca
git checkout v2.0.alpha3
popd
cp wrapper-versions/PoncaV2xalpha3/* .
rm -f *o
R -e "Rcpp::compileAttributes('..')"
R CMD INSTALL ..
R --vanilla < ../tests/testthat/test-CRAN.R
