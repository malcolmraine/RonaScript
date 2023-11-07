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

SET( TARGET_RONASCRIPT_MAIN RonaScript )

add_executable(${TARGET_RONASCRIPT_MAIN} src/main.cpp)
target_link_libraries(
        ${TARGET_RONASCRIPT_MAIN}
        RN_ALIB_VM
        RN_ALIB_PARSER
        RN_ALIB_AST
        RN_ALIB_MEMORY_MGMT
        RN_ALIB_LEXER
        RN_ALIB_COMMON
        RN_ALIB_CODEGEN
        RN_ALIB_BUILTINS
        RN_ALIB_UTIL)

if (CMAKE_BUILD_TARGET EQUAL "Release" OR CMAKE_BUILD_TARGET EQUAL "Test")
    target_compile_options(${TARGET_RONASCRIPT_MAIN} PUBLIC -Werror -o1)
elseif(CMAKE_BUILD_TARGET EQUAL "Debug")
    target_compile_options(${TARGET_RONASCRIPT_MAIN} PUBLIC -Werror -o1)
endif()

###############################################################################
###############################################################################
if (CMAKE_BUILD_TYPE EQUAL "Test")
    SET( TARGET_RONASCRIPT_TEST RonaScript_Tests )

    Include(FetchContent)

    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.4.0
    )

    FetchContent_MakeAvailable(Catch2)

    include(src/util/test/CMakeLists.txt)
    include(test/unit/CMakeLists.txt)

    add_executable(${TARGET_RONASCRIPT_TEST}
            test/unit/RnTestRunner.cpp
            ${RN_SOURCES_TEST_UTIL_STRING})
    target_link_libraries(${TARGET_RONASCRIPT_TEST} PRIVATE Catch2::Catch2WithMain RN_ALIB_UTIL)
    target_compile_options(${TARGET_RONASCRIPT_TEST} PUBLIC -Werror)
endif()