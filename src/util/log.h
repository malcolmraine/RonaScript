/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef VARSTACKTEST_LOG_H
#define VARSTACKTEST_LOG_H

#include <iostream>
#include <string>
#include <vector>
//#include "../vm/rona_object.h"
//#include "../vm/rona_class.h"
//#include "../vm/rona_function.h"
//#include "../vm/instruction.h"

#define ENABLE_DEBUG_LOGGING
#define ENABLE_INFO_LOGGING
#define ENABLE_ERROR_LOGGING

class Log
{
 public:
	static void INFO(const std::string& msg);
	static void INFO(int value);
	static void INFO(double value);
	static void INFO(bool value);
	static void DEBUG(const std::string& msg);
	static void DEBUG(int value);
	static void DEBUG(double value);
	static void DEBUG(bool value);
	static void ERROR(const std::string& msg);

};

#endif //VARSTACKTEST_LOG_H
