/*****************************************************************************
* File: RnBuiltins_String.h
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************/

#pragma once

#include "../common/RnType.h"
#include "RnBuiltins.h"

#undef RN_BUILTIN_STRING_REGISTRATIONS
#define RN_BUILTIN_STRING_REGISTRATIONS                                       \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_titlecase, RnType::RN_STRING, 1)   \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_lower, RnType::RN_STRING, 1)       \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_upper, RnType::RN_STRING, 1)       \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_snakecase, RnType::RN_STRING, 1)   \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_split, RnType::RN_ARRAY, 1)        \
    RN_BUILTIN_FUNC(RnBuiltins_String, str_substr, RnType::RN_STRING, 1)      \
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
