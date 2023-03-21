/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_SRC_UTIL_LOG_H
#define RONASCRIPT_SRC_UTIL_LOG_H

#include <iostream>
#include <string>
#include <vector>

//#define ENABLE_DEBUG_LOGGING
//#define ENABLE_INFO_LOGGING
#define ENABLE_ERROR_LOGGING

class Log {
public:
    static void INFO(const std::string& msg);
    static void WARN(const std::string& msg);
    static void DEBUG(const std::string& msg);
    static void ERROR(const std::string& msg);
};

#endif  //RONASCRIPT_SRC_UTIL_LOG_H
