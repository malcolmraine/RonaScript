###############################################################################
# File: CMakeLists.txt
# Description:
# Author: Malcolm Hall
# Date: 12/12/22
# Version: 1
#
###############################################################################
cmake_minimum_required(VERSION 3.20)
project(Hello)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Werror -o3)

include(../../src/vm/CMakeLists.txt)
include(../../src/builtins/CMakeLists.txt)
include(../../src/codegen/CMakeLists.txt)
include(../../src/parser/CMakeLists.txt)
include(../../src/lexer/CMakeLists.txt)
include(../../src/common/CMakeLists.txt)
include(../../src/util/CMakeLists.txt)
set(LIBRARY_OUTPUT_NAME HelloTest)
add_library(Hello SHARED hello.cpp ../../src/ffi/RnSharedLib.h ${RonaScript_SOURCES})
