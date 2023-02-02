###############################################################################
# File: CMakeLists.txt
# Description:
# Author: Malcolm Hall
# Date: 12/12/22
# Version: 1
#
###############################################################################
cmake_minimum_required(VERSION 3.20)
project(RonaScript)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang)
add_compile_options(-Werror -o3)

include(src/lexer/CMakeLists.txt)
include(src/parser/CMakeLists.txt)
include(src/parser/ast/CMakeLists.txt)
include(src/vm/CMakeLists.txt)
include(src/builtins/CMakeLists.txt)
include(src/util/CMakeLists.txt)
include(src/codegen/CMakeLists.txt)

add_executable(RonaScript src/main.cpp ${RonaScript_SOURCES} src/RnVersionInfo.h)
