/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "Log.h"
#include <iostream>

/*****************************************************************************/
std::string Log::Info(const std::string& msg, bool print) {
    std::cout << "[INFO]  " << msg << std::endl;
    return msg;
}

/*****************************************************************************/
std::string Log::Warn(const std::string& msg, bool print) {
    std::cout << "[WARN]  " << msg << std::endl;
    return msg;
}

/*****************************************************************************/
std::string Log::Err(const std::string& msg, bool print) {
    std::cout << "[ERROR]  " << msg << std::endl;
    return msg;
}
