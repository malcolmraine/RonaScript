###############################################################################
# File: CMakeLists.txt
# Description:
# Author: Malcolm Hall
# Date: 12/12/22
# Version: 1
#
###############################################################################

file(GLOB source_files ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB include_files ${CMAKE_CURRENT_LIST_DIR}/*.h)
list(APPEND RN_SOURCES_LEXER ${source_files})
list(APPEND RN_INCLUDES_LEXER ${include_files})

ADD_LIBRARY(RN_ALIB_LEXER STATIC ${RN_SOURCES_LEXER})