atime::atime_versions_remove("Poncatime")
tinfo <- atime::atime_pkg_test_info("../..")
(tcall <- tinfo$test.call$asoCurvatureEstimation)
tres <- eval(tcall)
tref <- atime::references_best(tres)
pred <- predict(tref)
pred
pred$measurements[N==max(N)]
# pred N=3800 at 0.1 is a good cutoff on my machine.

