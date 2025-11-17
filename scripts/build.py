import sys
import subprocess
import argparse
import os


def cmake(*args, **kwargs) -> str:
    cmd = ["cmake", *args]
    for k, v in kwargs.items():
        cmd.append("-D")
        cmd.append(f"{k}={v}")
    result = subprocess.run(cmd)
    if result.returncode != 0:
        print("Build failed.")


def build_project(build_dir: str, build_type: str, proc_count: int) -> None:
    os.makedirs(build_dir, exist_ok=True)
    cmake("-S", ".", "-B", build_dir, CMAKE_BUILD_TYPE=build_type)
    cmake("--build", build_dir, f"-j{proc_count}")
    cmake("--build", build_dir, "-j1")

    if not os.path.exists(f"{build_dir}/RonaScript"):
        print("Error: Failed to build target.")


def clean_project(build_dir: str, proc_count: int) -> None:
    if os.path.exists(build_dir):
        current_dir = os.getcwd()
        os.chdir(build_dir)
        subprocess.run(["make", "clean", f"-j{proc_count}"])
        os.chdir(current_dir)

        try:
            os.remove(build_dir)
        except Exception as e:
            print(f"Failed to remove build directory: {e}")
    else:
        print("Nothing to clean.")


def rebuild_project(build_dir: str, build_type: str, proc_count: int) -> None:
    clean_project(build_dir, proc_count)
    build_project(build_dir, build_type, proc_count)


if __name__ == "__main__":
    parser = argparse.ArgumentParser("RonaScript build script")
    parser.add_argument(
        "--type",
        default="Release",
        choices=["Release", "Debug"],
        help="What type of build to create.",
    )
    parser.add_argument(
        "--jobs", type=int, default=10, help="How many parallel make jobs to run."
    )
    parser.add_argument(
        "--target",
        choices=["clean", "install", "build", "rebuild"],
        help="What make target to build.",
    )
    args = parser.parse_args(sys.argv[1:])
    build_dir = f"./build/{args.type}"

    match args.target:
        case "clean":
            clean_project(build_dir, args.jobs)
        case "build":
            build_project(build_dir, args.type, args.jobs)
        case "rebuild":
            rebuild_project(build_dir, args.type, args.jobs)
        case _:
            print("Invalid target")
