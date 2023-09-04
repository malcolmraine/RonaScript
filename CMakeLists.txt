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

add_executable(${TARGET_RONASCRIPT_MAIN}
        src/main.cpp
        ${RonaScript_INCLUDES_UTIL}
        ${RonaScript_INCLUDES_VM}
        ${RonaScript_INCLUDES_PARSER}
        ${RonaScript_INCLUDES_AST}
        ${RonaScript_INCLUDES_MEMORY_MGMT}
        ${RonaScript_INCLUDES_LEXER}
        ${RonaScript_INCLUDES_COMMON}
        ${RonaScript_INCLUDES_CODEGEN}
        ${RonaScript_INCLUDES_BUILTINS}
        ${RonaScript_INCLUDES_UTIL})
target_link_libraries(
        ${TARGET_RONASCRIPT_MAIN}
        RonaScript_ALIB_VM
        RonaScript_ALIB_PARSER
        RonaScript_ALIB_AST
        RonaScript_ALIB_MEMORY_MGMT
        RonaScript_ALIB_LEXER
        RonaScript_ALIB_COMMON
        RonaScript_ALIB_CODEGEN
        RonaScript_ALIB_BUILTINS
        RonaScript_ALIB_UTIL)
target_compile_options(${TARGET_RONASCRIPT_MAIN} PUBLIC -Werror)

###############################################################################
###############################################################################
SET( TARGET_RONASCRIPT_TEST RonaScript_Tests )

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.4.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(${TARGET_RONASCRIPT_TEST}
        test/unit/RnTestRunner.cpp
        src/util/test/Test_String.cpp
        ${RonaScript_INCLUDE_UTIL})
target_link_libraries(${TARGET_RONASCRIPT_TEST} PRIVATE Catch2::Catch2WithMain RonaScript_ALIB_UTIL)
target_compile_options(${TARGET_RONASCRIPT_TEST} PUBLIC -Werror)