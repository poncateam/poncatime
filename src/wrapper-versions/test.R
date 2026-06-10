remotes::install_github("tdhock/atime@poncatime")

edit_wrapper <- function(pkg.path){
  src.dir <- file.path(pkg.path, "src")
  h.vec <- Sys.glob(file.path(
    src.dir, "wrapper-versions", "*", "curvatureEstimation.h"))
  (version.dirs <- c("", dirname(h.vec)))
  for(vdir in version.dirs){
    print(vdir)
    if(vdir != ""){
      system(paste("cp", file.path(vdir, "*"), src.dir))
    }
    unlink(file.path(src.dir, "*o"))
    INSTALL.cmd <- paste(
      shQuote(file.path(
        Sys.getenv("R_HOME"),
        "bin",
        "R")),
      'CMD INSTALL -l',
      shQuote(.libPaths()[1]),
      shQuote(pkg.path))
    print(status.int <- system(INSTALL.cmd))
    if(status.int == 0)break
  }
  status.int
  vdir
}
system("cd ../external/ponca && git checkout v1.4")
system("cd ../external/ponca && git checkout v2.0.alpha1")
edit_wrapper("../..")

vres <- atime::atime_versions(
  pkg.path="../..",
  checkout="../external/ponca", #TODO
  verbose=TRUE,
  pkg.edit.fun=function(old.Package, new.Package, sha, new.pkg.path){
    pkg_find_replace <- function(glob, FIND, REPLACE, warn=TRUE){
      glob_find_replace(file.path(new.pkg.path, glob), FIND, REPLACE, warn)
    }
    pkg_find_replace(
      "DESCRIPTION", 
      paste0("Package:\\s+", old.Package),
      paste("Package:", new.Package))
    Package_ <- gsub(".", "_", old.Package, fixed=TRUE)
    new.Package_ <- paste0(Package_, "_", sha)
    pkg_find_replace(
      file.path("src", "RcppExports.cpp"),
      paste0("R_init_", Package_),
      paste0("R_init_", new.Package_))
    pkg_find_replace(
      file.path("R", "RcppExports.R"),
      sprintf("PACKAGE = '%s'", old.Package),
      sprintf("PACKAGE = '%s'", new.Package),
      warn=FALSE)#does not appear if registration used.
    pkg_find_replace(
      "NAMESPACE",
      sprintf('useDynLib\\("?%s"?', Package_),
      paste0('useDynLib(', new.Package))
  },
  setup={
    N_points <- N
    N_queries <- 0
    dataScale <- 10
    N_list <- Poncatime:::generatePointClouds_interface(N_points, N_queries, dataScale)
  },
  main="e20b31406626bf916da3990d7c64b820ebf9ca58",
  expr=Poncatime:::buildKdTree_interface(N_list$points))
