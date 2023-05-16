/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
* MIT License
*
* Copyright (c) 2021 Malcolm Hall
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

#include "RnBuiltins.h"

#undef RN_BUILTIN_ARRAY_REGISTRATIONS
#define RN_BUILTIN_ARRAY_REGISTRATIONS \
    RN_BUILTIN_FUNC(RnBuiltins_Array, filter, RnType::RN_ARRAY, 1) \
    RN_BUILTIN_FUNC(RnBuiltins_Array, union, RnType::RN_ARRAY, 1) \
    RN_BUILTIN_FUNC(RnBuiltins_Array, intersect, RnType::RN_ARRAY, 1) \
    RN_BUILTIN_FUNC(RnBuiltins_Array, count, RnType::RN_INT, 1)     \
    RN_BUILTIN_FUNC(RnBuiltins_Array, array_merge, RnType::RN_ARRAY, 1) \
    RN_BUILTIN_FUNC(RnBuiltins_Array, array_push, RnType::RN_VOID, 1) \
    RN_BUILTIN_FUNC(RnBuiltins_Array, array_pop, RnType::RN_ANY, 1) \
    RN_BUILTIN_FUNC(RnBuiltins_Array, array_zeros, RnType::RN_ARRAY, 1)     \
    RN_BUILTIN_FUNC(RnBuiltins_Array, array_fill, RnType::RN_ARRAY, 1) \
    RN_BUILTIN_FUNC(RnBuiltins_Array, sequence, RnType::RN_ARRAY, 1)

class RnScope;
class RnObject;

#undef RN_BUILTIN_FUNC
#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DECLARE

class RnBuiltins_Array {
public:
    RN_BUILTIN_ARRAY_REGISTRATIONS
};
