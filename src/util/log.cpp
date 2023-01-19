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
	std::cout << msg << std::endl;
#endif
}

/*****************************************************************************/
void Log::INFO(int msg)
{
#ifdef ENABLE_INFO_LOGGING
	std::cout << msg << std::endl;
#endif
}

/*****************************************************************************/
void Log::INFO(double msg)
{
#ifdef ENABLE_INFO_LOGGING
	std::cout << msg << std::endl;
#endif
}

/*****************************************************************************/
void Log::INFO(bool msg)
{
#ifdef ENABLE_INFO_LOGGING
	std::cout << (msg ? "true" : "false") << std::endl;
#endif
}

/*****************************************************************************/
void Log::DEBUG(const std::string& msg)
{
#ifdef ENABLE_DEBUG_LOGGING
	std::cout << msg << std::endl;
#endif
}

/*****************************************************************************/
void Log::DEBUG(int msg)
{
#ifdef ENABLE_DEBUG_LOGGING
	std::cout << msg << std::endl;
#endif
}

/*****************************************************************************/
void Log::DEBUG(double msg)
{
#ifdef ENABLE_DEBUG_LOGGING
	std::cout << msg << std::endl;
#endif
}

/*****************************************************************************/
void Log::DEBUG(bool msg)
{
#ifdef ENABLE_DEBUG_LOGGING
	std::cout << (msg ? "true" : "false") << std::endl;
#endif
}

/*****************************************************************************/
void Log::ERROR(const std::string& msg)
{
#ifdef ENABLE_ERROR_LOGGING
	std::cout << msg << std::endl;
#endif
}
