/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#pragma once

#include "../common/RnType.h"
#include "RnBuiltins.h"

#undef RN_BUILTIN_STRING_REGISTRATIONS
#define RN_BUILTIN_STRING_REGISTRATIONS                                 \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_titlecase, RnType::RN_STRING, 1)  \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_lower, RnType::RN_STRING, 1)      \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_upper, RnType::RN_STRING, 1)      \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_snakecase, RnType::RN_STRING, 1)  \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_split, RnType::RN_ARRAY, 1)      \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_substr, RnType::RN_STRING, 1)     \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_startswith, RnType::RN_BOOLEAN, 1) \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_endswith, RnType::RN_BOOLEAN, 1)   \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_join, RnType::RN_STRING, 1)

class RnScope;
class RnObject;

#undef RN_BUILTIN_FUNC
#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DECLARE

class RnBuiltins_String {
public:
    RN_BUILTIN_STRING_REGISTRATIONS
};
