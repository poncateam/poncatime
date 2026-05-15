# poncatime
Analysis of Ponca performances using atime

## Fetch the sources
To compile this project, you need to retrieve the dependencies using git submodules:
```bash
git clone https://github.com/poncateam/poncatime.git
cd poncatime
git submodule update --init --recursive
```

## Compile and test

First install R. If you don’t have the Rcpp package, you can install it in R via:

```r
install.packages("Rcpp")
```

If you edit the `*_interface` functions in [src/interface.cpp](https://github.com/poncateam/poncatime/blob/main/src/interface.cpp) then you need to re-generate `src/RcppExports.cpp` and `R/RcppExports.R`:

```sh
R -e "Rcpp::compileAttributes('path/to/poncatime')"
```

After that, you can compile the C++ code in this package, and install it, by running

```sh
R CMD INSTALL path/to/poncatime
```

Finally, you can run tests via

```sh
R --vanilla < tests/testthat/test-CRAN.R
```

Or test under valgrind via

```sh
R -d valgrind --vanilla < tests/testthat/test-small.R
```

# Benchmarking

## performance testing

[.ci/atime/tests.R](https://github.com/poncateam/poncatime/blob/main/.ci/atime/tests.R) contains test cases.

To run the performance testing, I had to make some modifications to atime to handle this use case. To install the updated version,

```r
remotes::install_github("tdhock/atime@poncatime")
```

Then I run the performance test suite via

```r
atime::atime_pkg("path/to/poncatime")
```

which creates result files in `poncatime/.ci/atime`.

## comparative benchmarking

If there are other algorithms (baselines, state-of-the-art) that do a similar computation as asoCurveEstimation, we can add them to the atime code in the [vignette](https://github.com/poncateam/poncatime/blob/main/vignettes/atime.Rmd), to compare performance.
