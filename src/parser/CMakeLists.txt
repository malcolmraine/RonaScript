###############################################################################
# File: CMakeLists.txt
# Description:
# Author: Malcolm Hall
# Date: 12/12/22
# Version: 1
#
###############################################################################

file(GLOB source_files ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB include_files ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
list(APPEND RonaScript_SOURCES_PARSER ${source_files})
list(APPEND RonaScript_INCLUDES_AST ${include_files})

ADD_LIBRARY(RonaScript_ALIB_PARSER STATIC ${RonaScript_SOURCES_PARSER})