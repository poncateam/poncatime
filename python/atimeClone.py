from git import Repo
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
status = os.system("cd " + buiddDir)
for s in sha:
    print("fetch", s)
    subRepo = Repo.clone_from(repoUrl, os.path.join(buiddDir, s))
    print("switching active branch to ", s)
    subRepo.git.checkout(s)
    print("updating submodules")
    output = subRepo.git.submodule('update', '--init', '--recursive')
    print("configure")
    print("cd " + os.path.join(buiddDir, s) + " && " + config_command)
    os.system("cd " + os.path.join(buiddDir, s) + " && " + config_command)
    print("build")
    print("cd " + os.path.join(buiddDir, s) + " && " + build_command)
    os.system("cd " + os.path.join(buiddDir, s) + " && " + build_command)

for s in sha:
    print("run ", os.path.join(buiddDir, s))
    status = os.system("cd " + os.path.join(buiddDir, s) + " && " + run_command)

status = os.system("cd ..")
