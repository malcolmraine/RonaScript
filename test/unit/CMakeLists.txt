###############################################################################
# File: CMakeLists.txt
# Description:
# Author: Malcolm Hall
# Date: 9/4/23
# Version: 1
#
###############################################################################

file(GLOB source_files ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB include_files ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
list(APPEND RN_SOURCES_UNIT_TESTS${source_files})
list(APPEND RN_INCLUDES_UNIT_TESTS ${include_files})