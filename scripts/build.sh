#!/bin/bash

PROC_COUNT=12
TARGET="invalid"
BUILD_DIR="./build"
BUILD_TYPE=Release

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
function build_project() {
    mkdir "$BUILD_DIR"
    SETUP_CMD="cmake -S. -B $BUILD_DIR -D CMAKE_BUILD_TYPE=$BUILD_TYPE"
    echo "$SETUP_CMD"
    eval "$SETUP_CMD"

    BUILD_CMD="cmake --build $BUILD_DIR -j$PROC_COUNT"
    echo "$BUILD_CMD"
    eval "$BUILD_CMD"

    BUILD_CMD="cmake --build $BUILD_DIR -j1"
    echo "$BUILD_CMD"
    eval "$BUILD_CMD"

    if [[ ! -f "$BUILD_DIR/RonaScript" ]];
    then
        echo "Error: Failed to build target."
        exit 1
    fi
}

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
function clean_project() {
    if [[ -d $BUILD_DIR ]]; then
        cd "$BUILD_DIR" || exit 1
        make clean "-j$PROC_COUNT"
        cd ../
        rm -R "$BUILD_DIR"
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
    cp "$BUILD_DIR/RonaScript" /usr/local/bin/rona
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
if [[ -d ../build ]]; then
    BUILD_DIR=../build
elif [[ -d ./build ]]; then
    BUILD_DIR=./build
fi

while true; do
    case $1 in
    --target)
        shift
        TARGET=$1
        ;;
    --type)
        shift
        BUILD_TYPE=$1;
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
