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
list(APPEND RN_SOURCES_CODEGEN ${source_files})
list(APPEND RN_INCLUDES_CODEGEN ${include_files})

ADD_LIBRARY(RN_ALIB_CODEGEN STATIC ${RN_SOURCES_CODEGEN})