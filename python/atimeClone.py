from git import Repo
import shutil
import os

repo    = Repo("..")
repoUrl = repo.remote().url
buiddDir = "build/"


sha= [ "PoncaV0x3"
    , "PoncaV1x0"
    , "PoncaV1x2"
    , "PoncaV1x3"
    , "PoncaV1x4"
    , "PoncaV2xalpha0"
       ]

config_command = "cmake -B build -DCMAKE_BUILD_TYPE=Release src/ "
build_command  = "cmake --build build -j 4"
run_command    = "cd build && time ./poncatime-test"

print("Processing repository ", repoUrl, " in ", buiddDir)

os.makedirs(buiddDir, exist_ok=True)









def prepareRepository(buildDir, s, copy_src, clone = False):
    print("Prepare", s)
    targetDir = os.path.join(buildDir, s)

    print("get repository", s)
    subRepo = None

    if not os.path.isdir(targetDir):
        if clone:
            subRepo = Repo.clone_from(repoUrl, targetDir)
        else:
            shutil.copytree(os.path.join(copy_src, ".git"), os.path.join(targetDir,".git"), dirs_exist_ok=True)

    if subRepo is None:
        subRepo = Repo(targetDir)

    print("switching active branch to ", s)
    subRepo.git.checkout(s, force=True)
    print("updating submodules")
    subRepo.git.submodule('update', '--init', '--recursive')


for s in sha:
    prepareRepository(buiddDir, s, "..")

    print("**** CONFIGURE ****")
    os.system("cd " + os.path.join(buiddDir, s) + " && " + config_command)
    print("**** BUILD ****")
    os.system("cd " + os.path.join(buiddDir, s) + " && " + build_command)

for s in sha:
    print("run ", os.path.join(buiddDir, s))
    status = os.system("cd " + os.path.join(buiddDir, s) + " && " + run_command)

status = os.system("cd ..")
