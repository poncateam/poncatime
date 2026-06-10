set -o errexit
pushd external/ponca
git checkout v2.0.alpha0
popd
cp wrapper-versions/PoncaV2xalpha0/* .
rm -f *o
R -e "Rcpp::compileAttributes('..')"
R CMD INSTALL ..
R --vanilla < ../tests/testthat/test-CRAN.R
