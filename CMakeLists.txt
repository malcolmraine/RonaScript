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

###############################################################################
###############################################################################
include(src/common/CMakeLists.txt)
include(src/lexer/CMakeLists.txt)
include(src/parser/CMakeLists.txt)
include(src/parser/ast/CMakeLists.txt)
include(src/vm/CMakeLists.txt)
include(src/builtins/CMakeLists.txt)
include(src/util/CMakeLists.txt)
include(src/codegen/CMakeLists.txt)
include(src/memory_mgmt/CMakeLists.txt)

add_executable(RonaScript src/main.cpp ${RonaScript_SOURCES} ${RonaScript_util} src/RnBuildInfo.h)
target_compile_options(RonaScript PUBLIC -Werror)

###############################################################################
###############################################################################
Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(RonaScript_Tests
        test/unit/RnTestRunner.cpp
        src/util/test/Test_String.cpp
        ${RonaScript_util}
        )
target_link_libraries(RonaScript_Tests PRIVATE Catch2::Catch2WithMain)
target_compile_options(RonaScript_Tests PUBLIC -Werror)