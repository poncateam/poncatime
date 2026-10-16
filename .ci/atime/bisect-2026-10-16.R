pkg.path <- "~/R/poncatime"
Test <- "asoCurvatureEstimation"
library(data.table)
if(FALSE){
  ## atime functions for git bisect were added in this branch:
  remotes::install_github("tdhock/atime@bisect")
  ## if you want to start the bisect over from scratch, first remove
  ## all historical versions of data.table installed by atime:
  atime::atime_versions_remove("poncatime")
}
(bisect.dt <- rowwiseDT(
  old=,     new=, label=, expected=, stage=,
  "v1.0", "v1.4", "?", "?", "final"))
for(bisect.i in 1:nrow(bisect.dt)){
  print(bisect.row <- bisect.dt[bisect.i])
  bisect.row[, atime::bisect(pkg.path, Test, old, new)]
}
