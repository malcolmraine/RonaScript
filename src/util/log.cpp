/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "log.h"

/*****************************************************************************/
void Log::INFO(const std::string& msg)
{
#ifdef ENABLE_INFO_LOGGING
	std::cout << msg << "\n";
#endif
}

/*****************************************************************************/
void Log::DEBUG(const std::string& msg)
{
#ifdef ENABLE_DEBUG_LOGGING
	std::cout << msg << "\n";
#endif
}

/*****************************************************************************/
void Log::WARN(const std::string& msg)
{
#ifdef ENABLE_ERROR_LOGGING
	std::cout << "\033[33m" << msg << "\033[0m\n";
#endif
}

/*****************************************************************************/
void Log::ERROR(const std::string& msg)
{
#ifdef ENABLE_ERROR_LOGGING
	std::cout << "\033[31m" << msg << "\033[0m\n";
#endif
}
