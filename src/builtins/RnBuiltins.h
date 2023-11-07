/*****************************************************************************
* File: RnBuiltins.h
* Description:
* Author: Malcolm Hall
* Date: 5/30/22
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

#include <cassert>
#include "../common/RnType.h"

class RnScope;
class RnObject;

#define RN_BUILTIN_REGISTRATIONS     \
    RN_BUILTIN_GENERAL_REGISTRATIONS \
    RN_BUILTIN_TIME_REGISTRATIONS    \
    RN_BUILTIN_MATH_REGISTRATIONS    \
    RN_BUILTIN_IO_REGISTRATIONS      \
    RN_BUILTIN_STRING_REGISTRATIONS  \
    RN_BUILTIN_TYPE_REGISTRATIONS    \
    RN_BUILTIN_ARRAY_REGISTRATIONS   \
    RN_BUILTIN_SYSTEM_REGISTRATIONS

#define RN_BUILTIN_TIME_REGISTRATIONS
#define RN_BUILTIN_MATH_REGISTRATIONS
#define RN_BUILTIN_IO_REGISTRATIONS
#define RN_BUILTIN_STRING_REGISTRATIONS
#define RN_BUILTIN_TYPE_REGISTRATIONS
#define RN_BUILTIN_ARRAY_REGISTRATIONS
#define RN_BUILTIN_SYSTEM_REGISTRATIONS

#define RN_BUILTIN_GENERAL_REGISTRATIONS                        \
    RN_BUILTIN_FUNC(RnBuiltins, unpack, RnType::RN_VOID, 1)     \
    RN_BUILTIN_FUNC(RnBuiltins, system, RnType::RN_ANY, 1)      \
    RN_BUILTIN_FUNC(RnBuiltins, call, RnType::RN_ANY, 2)        \
    RN_BUILTIN_FUNC(RnBuiltins, lload, RnType::RN_OBJECT, 1)    \
    RN_BUILTIN_FUNC(RnBuiltins, bind, RnType::RN_VOID, 2)       \
    RN_BUILTIN_FUNC(RnBuiltins, setenv, RnType::RN_VOID, 2)     \
    RN_BUILTIN_FUNC(RnBuiltins, getenv, RnType::RN_ANY, 1)      \
    RN_BUILTIN_FUNC(RnBuiltins, unsetenv, RnType::RN_VOID, 1)   \
    RN_BUILTIN_FUNC(RnBuiltins, listattr, RnType::RN_ARRAY, 1)  \
    RN_BUILTIN_FUNC(RnBuiltins, attrpairs, RnType::RN_ARRAY, 1) \
    RN_BUILTIN_FUNC(RnBuiltins, hasattr, RnType::RN_VOID, 2)    \
    RN_BUILTIN_FUNC(RnBuiltins, getattr, RnType::RN_VOID, 2)    \
    RN_BUILTIN_FUNC(RnBuiltins, setattr, RnType::RN_VOID, 3)    \
    RN_BUILTIN_FUNC(RnBuiltins, delattr, RnType::RN_VOID, 2)

#define BUILTIN_ASSERTS \
    assert(ret_val);    \
    assert(scope);

#define FIXED_ARG_COUNT_CHECK(name, n)                                              \
    if (args.size() != n) {                                                         \
        throw std::runtime_error("Function '" + std::string(#name) + "' expects " + \
                                 std::to_string(n) + " arguments but received " +   \
                                 std::to_string(args.size()));                      \
    }

#define RN_BUILTIN_FUNC_DECLARE(ns, name, retval, argcnt)                    \
    static void rn_builtin_##name(RnScope* scope, const RnArrayNative& args, \
                                  RnObject* ret_val);

#define RN_BUILTIN_FUNC_DEFINE(name, retval, argcnt)                                 \
    void BUILTIN_CLASS::rn_builtin_##name(RnScope* scope, const RnArrayNative& args, \
                                          RnObject* ret_val)

#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DECLARE

class RnBuiltins {
public:
    RN_BUILTIN_GENERAL_REGISTRATIONS
};
