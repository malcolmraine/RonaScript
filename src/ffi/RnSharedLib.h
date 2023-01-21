/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_SANDBOX_RNSHAREDLIB_H_
#define RONASCRIPT_SANDBOX_RNSHAREDLIB_H_

#include <string>
#include <vector>
#include "../vm/RnObject.h"
#include "../vm/RnIntObject.h"
#include "../vm/RnFloatObject.h"
#include "../vm/RnStringObject.h"
#include "../vm/RnArrayObject.h"
#include "../vm/RnBoolObject.h"
#include "../vm/RnNullObject.h"
#include "../vm/RnFunctionObject.h"

#if defined(_WIN32)
#define LIBRARY_API __declspec(dllexport)
#else
#define LIBRARY_API __attribute__ ((visibility("default")))
#endif

#define RONA_EXPORT extern "C" LIBRARY_API

RONA_EXPORT const char* LibraryName();
RONA_EXPORT const char* LibraryVersion();
RONA_EXPORT void LibraryFunctions(std::vector<std::tuple<std::string, RnType::Type>>& functions);



#endif //RONASCRIPT_SANDBOX_RNSHAREDLIB_H_
