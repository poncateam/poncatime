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

If you edit the `*_interface` functions in [https://github.com/poncateam/poncatime/blob/main/src/interface.cpp](src/interface.cpp) then you need to re-generate `src/RcppExports.cpp` and `R/RcppExports.R`:

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
