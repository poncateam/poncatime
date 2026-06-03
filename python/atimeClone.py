from git import Repo
import shutil
import os
import json

repo    = Repo("..")
repoUrl = repo.remote().url
buiddDir = "build/"


sha= [ "PoncaV0x3"
    , "PoncaV1x0"
    , "PoncaV1x2"
    , "PoncaV1x3"
    , "PoncaV1x4"
    , "PoncaV2xalpha0"
    , "PoncaV2xalpha1"
    , "PoncaV2xalpha2"
       ]

config_command = "cmake -B build -DCMAKE_BUILD_TYPE=Release src/ "
build_command  = "cmake --build build -j 4"
run_command    = "cd build && ./poncatime-test"

print("Processing repository ", repoUrl, " in ", buiddDir)

os.makedirs(buiddDir, exist_ok=True)









def prepareRepository(buildDir, s, copy_src, clone = False):
    targetDir = os.path.join(buildDir, s)

    subRepo = None

    if not os.path.isdir(targetDir):
        if clone:
            print("Clone repository")
            subRepo = Repo.clone_from(repoUrl, targetDir)
        else:
            print("Copy repository")
            shutil.copytree(os.path.join(copy_src, ".git"), os.path.join(targetDir,".git"), dirs_exist_ok=True)

    if subRepo is None:
        print("Init Git")
        subRepo = Repo(targetDir)

    print("Switch active branch to ", s)
    subRepo.git.checkout(s, force=True)
    print("Update submodules")
    subRepo.git.submodule('update', '--init', '--recursive')


for s in sha:
    print("**** REPOSITORY PREPARATION ****")
    prepareRepository(buiddDir, s, "..")

    print("Overwrite folder `cpp` with current version")
    targetCPPDir = os.path.join(buiddDir, s, "src", "cpp")
    sourceCPPDir = os.path.join("..", "src", "cpp")
    shutil.rmtree(targetCPPDir)
    shutil.copytree(sourceCPPDir, targetCPPDir, ignore=shutil.ignore_patterns('*.git'))
    shutil.copy(os.path.join("..", "src", "CMakeLists.txt"), os.path.join(buiddDir, s, "src"))


    print("**** CONFIGURE ****")
    os.system("cd " + os.path.join(buiddDir, s) + " && " + config_command)
    print("**** BUILD ****")
    os.system("cd " + os.path.join(buiddDir, s) + " && " + build_command)

# json file used to store the results experiments
resJson = {}

for s in sha:
    print("run ", os.path.join(buiddDir, s))
    status = os.system("cd " + os.path.join(buiddDir, s) + " && " + run_command)

    jsonFile = os.path.join(buiddDir, s, "build", "run_output.json")
    with open(jsonFile, "r") as file:
        data = json.load(file)
        resJson[s] = data

with open("results.json", "w") as file:
    json.dump(resJson, file, indent=4)