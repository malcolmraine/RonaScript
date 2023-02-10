/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNSHAREDLIB_H_
#define RONASCRIPT_RNSHAREDLIB_H_

#include <string>
#include <vector>
#include "../vm/RnArrayObject.h"
#include "../vm/RnBoolObject.h"
#include "../vm/RnFloatObject.h"
#include "../vm/RnFunctionObject.h"
#include "../vm/RnIntObject.h"
#include "../vm/RnNullObject.h"
#include "../vm/RnObject.h"
#include "../vm/RnStringObject.h"

#if defined(_WIN32)
#define LIBRARY_API __declspec(dllexport)
#else
#define LIBRARY_API __attribute__((visibility("default")))
#endif

#define RONA_EXPORT extern "C" LIBRARY_API

RONA_EXPORT const char* LibraryName();
RONA_EXPORT const char* LibraryVersion();
RONA_EXPORT void LibraryFunctions(
    std::vector<std::tuple<std::string, RnType::Type>>& functions);

#define DEFINE_LIBRARY(name, version) \
    const char* LibraryName() {       \
        return #name;                 \
    }                                 \
    const char* LibraryVersion() {    \
        return #version;              \
    }                                 \
    void LibraryFunctions(std::vector<std::tuple<std::string, RnType::Type>>& functions)

#define REGISTER_LIBRARY_FUNCTION(name, type) functions.emplace_back(#name, type);

#endif  //RONASCRIPT_RNSHAREDLIB_H_
