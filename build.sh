#!/bin/bash

PROC_COUNT=9
TARGET="invalid"

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
function build_project() {
    mkdir ./build
    cmake -S. -B ./build
    cd ./build || exit 1
    make "-j$PROC_COUNT"
    cd ../

    declare -a REMOVALS=(
        "./build/CMakeCache.txt"
        "./build/cmake_install.cmake")

    for item in "${REMOVALS[@]}"; do
        if [[ -d $item ]]; then
            rm -R "$item"
        elif [[ -f $item ]]; then
            rm "$item"
        fi
    done
}

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
function clean_project() {
    if [[ -d ./build ]]; then
        cd ./build || exit 1
        make clean "-j$PROC_COUNT"
        cd ../
        rm -R ./build
    else
        echo "Nothing to clean."
    fi
}

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
function rebuild_project() {
    clean_project
    build_project
}

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
function install_project() {
    rm /usr/local/bin/rona
    cp ./build/RonaScript /usr/local/bin/rona
}

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
function show_help() {
    echo "RonaScript build script"
    echo
    echo "Usage: bash build.sh <command> [options...]"
    echo "  help,-h,--help            Show help."
    echo "  -t, --target <option>     Specify the build target."
    echo "      build                     Build RonaScript."
    echo "      clean                     Clean the project."
    echo "      install                   Install the project."
    echo "      rebuild                   Rebuild the entire project."
}

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
while true; do
    case $1 in
    --target)
        shift
        TARGET=$1
        ;;
    -j)
        shift
        PROC_COUNT=$1
        ;;
    -h | --help | help)
        show_help
        exit
        ;;
    *)
        break
        ;;
    esac
    shift
done

case $TARGET in
clean)
    clean_project
    ;;
install)
    install_project
    exit
    ;;
build)
    build_project
    exit
    ;;
rebuild)
    rebuild_project
    exit
    ;;
invalid)
    echo "No target specified."
    ;;
*)
    echo "Invalid target"
    ;;
esac
