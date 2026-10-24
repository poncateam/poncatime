pkg.path <- "~/R/poncatime"
Test <- "asoCurvatureEstimation"
library(data.table)
if(FALSE){
  ## if you want to start the bisect over from scratch, first remove
  ## all historical versions of data.table installed by atime:
  atime::atime_versions_remove("poncatime")
}
## the code below was used to compute the results on
## https://tdhock.github.io/2026-10-16-ponca-git-bisect-asoCurvatureEstimation
atime::bisect(pkg.path, Test, "v1.0", "v1.4")

