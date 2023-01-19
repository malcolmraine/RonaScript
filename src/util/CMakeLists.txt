###############################################################################
# File: CMakeLists.txt
# Description:
# Author: Malcolm Hall
# Date: 12/12/22
# Version: 1
#
###############################################################################
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_CURRENT_LIST_DIR}/MLib/CMakeLists.txt)
file(GLOB files ${CMAKE_CURRENT_LIST_DIR}/*.cpp ${CMAKE_CURRENT_LIST_DIR}/*.h)
list(APPEND RonaScript_SOURCES ${files})
