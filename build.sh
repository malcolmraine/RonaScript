#!/bin/bash

INSTALL=0
CLEAN=0
PROC_COUNT=9

while true;
do
    case $1 in
        --clean)
            CLEAN=1
            ;;
        --install)
            INSTALL=1
            ;;
        -j)
            shift
            PROC_COUNT=$1
            ;;
        *)
            break
            ;;
    esac
    shift
done

if (( CLEAN == 1 ));
then
    rm -R ./build
fi

mkdir ./build
cmake -S. -B ./build
cd ./build || exit 1
make "-j$PROC_COUNT"
rm Makefile
rm CMakeCache.txt
rm cmake_install.cmake

if (( INSTALL == 1 ))
then
    rm /usr/local/bin/rona
    cp RonaScript /usr/local/bin/rona
fi