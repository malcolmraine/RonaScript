/*****************************************************************************
* File: RnOS.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/17/23
* Version: 1
*
*****************************************************************************/

#include "RnOS.h"
#include <cstdlib>

/*****************************************************************************/
std::string RnOS::GetEnv(const std::string& name) {
    return std::getenv(name.c_str());
}

/*****************************************************************************/
void RnOS::SetEnv(const std::string& name, const std::string& value) {
    setenv(name.c_str(), value.c_str(), true);
}
