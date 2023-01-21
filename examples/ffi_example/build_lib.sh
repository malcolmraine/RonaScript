#!/bin/bash

cmake -S -B ./
make all
declare -a REMOVALS=("CMakeFiles/" "cmake_install.cmake" "CMakeCache.txt")

for item in "${REMOVALS[@]}"; do
    if [[ -d $item ]]; then
        rm -R "$item"
    elif [[ -f $item ]]; then
        rm "$item"
    fi
done