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

#undef RN_BUILTIN_TYPE_REGISTRATIONS
#define RN_BUILTIN_TYPE_REGISTRATIONS                                    \
    RN_BUILTIN_FUNC(RnBuiltins_Type, to_int, RnType::RN_INT, 1)          \
    RN_BUILTIN_FUNC(RnBuiltins_Type, to_float, RnType::RN_FLOAT, 1)      \
    RN_BUILTIN_FUNC(RnBuiltins_Type, to_string, RnType::RN_STRING, 1)    \
    RN_BUILTIN_FUNC(RnBuiltins_Type, to_bool, RnType::RN_BOOLEAN, 1)     \
    RN_BUILTIN_FUNC(RnBuiltins_Type, is_string, RnType::RN_BOOLEAN, 1)   \
    RN_BUILTIN_FUNC(RnBuiltins_Type, is_int, RnType::RN_BOOLEAN, 1)      \
    RN_BUILTIN_FUNC(RnBuiltins_Type, is_float, RnType::RN_BOOLEAN, 1)    \
    RN_BUILTIN_FUNC(RnBuiltins_Type, is_bool, RnType::RN_BOOLEAN, 1)     \
    RN_BUILTIN_FUNC(RnBuiltins_Type, is_array, RnType::RN_BOOLEAN, 1)    \
    RN_BUILTIN_FUNC(RnBuiltins_Type, is_object, RnType::RN_BOOLEAN, 1)   \
    RN_BUILTIN_FUNC(RnBuiltins_Type, is_callable, RnType::RN_BOOLEAN, 1) \
    RN_BUILTIN_FUNC(RnBuiltins_Type, is_any, RnType::RN_BOOLEAN, 1)

class RnScope;
class RnObject;

#undef RN_BUILTIN_FUNC
#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DECLARE

class RnBuiltins_Type {
public:
    RN_BUILTIN_TYPE_REGISTRATIONS
};
