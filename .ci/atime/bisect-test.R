tinfo <- atime::atime_pkg_test_info("~/R/poncatime")
(tcall <- tinfo$test.call$asoCurvatureEstimation)
tcall$v2.0.alpha1 <- NULL
tcall$sha.vec[["base=master"]] <- NULL
tcall$seconds.limit <- 0.01
tres <- eval(tcall)
plot(tres)
tref <- atime::references_best(tres)
pred <- predict(tref)
library(data.table)
pred$prediction[, version := ifelse(grepl("HEAD", expr.name), "HEAD", expr.name)][]
pwide <- dcast(pred$prediction, . ~ version, value.var="N")
plong <- melt(pwide, measure.vars=c("v1.0","v1.4"))
plong[, diff := abs(HEAD-value)][]
(closer <- plong[which.min(diff), variable])
(status <- ifelse(closer=="v1.0", 0, 1))
q(status=status)
## Note that the script (my_script in the above example) should exit with
## code 0 if the current source code is good/old, and exit with a code
## between 1 and 127 (inclusive), except 125, if the current source code
## is bad/new.

pred$measurements[N==max(N)]
# pred N=3800 at 0.1 is a good cutoff on my machine.

