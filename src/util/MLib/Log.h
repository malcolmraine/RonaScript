/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef MLIBTEST_LOG_H
#define MLIBTEST_LOG_H

#include <string>

class Log
{
 public:
	static std::string Info(const std::string& msg, bool print = true);
	static std::string Warn(const std::string& msg, bool print = true);
	static std::string Err(const std::string& msg, bool print = true);

 private:
	Log() = default;
	~Log() = default;
};

#endif //MLIBTEST_LOG_H