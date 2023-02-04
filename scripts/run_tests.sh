#!/bin/bash

RUN_FUNCTIONAL=0
RUN_UNIT=0
REBUILD=0

while true; do
    case $1 in
    --functional)
        RUN_FUNCTIONAL=1
        shift
        ;;
    --unit)
        RUN_UNIT=1
        shift
        ;;
    --rebuild)
        REBUILD=1
        shift
        ;;
    -h | --help | help)
        echo "RonaScript Test Invocation"
        echo
        echo "Usage: bash run_test.sh [options...]"
        echo "  help,-h,--help            Show help."
        echo "  --functional              Run functional tests"
        echo "  --unit                    Run unit tests"
        echo "  --rebuild                 Rebuild the project before testing"
        exit
        ;;
    *)
        break
        ;;
    esac
    shift
done

RN_EXECUTABLE=$(pwd)/build/RonaScript

if [[ $REBUILD == 1 ]]; then
    echo "Rebuilding..."
    if [[ -d ./scripts ]]; then
        bash ./scripts/build.sh --target rebuild
    else
        bash build.sh --target rebuild
    fi
fi

if [[ $RUN_FUNCTIONAL == 1 ]]; then
    echo "Running functional tests..."
    # Need to check if this is invoked from inside the scripts directory
    if [[ -d "test" ]]; then
        cd "test" || exit 1
    fi

    python3 "$(pwd)/functional_runner.py" "$RN_EXECUTABLE"
fi
