/*****************************************************************************
* File: RnOS.h
* Description:
* Author: Malcolm Hall
* Date: 5/17/23
* Version: 1
*
*****************************************************************************/

#pragma once

#include <string>

class RnOS {
public:
    static std::string GetEnv(const std::string& name);
    static void SetEnv(const std::string& name, const std::string& value);

};