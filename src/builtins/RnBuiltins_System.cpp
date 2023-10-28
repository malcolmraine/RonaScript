/*****************************************************************************
* File: RnBuiltins_IO.cpp
* Description:
* Author: Malcolm Hall
* Date: 2/1/22
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

#include "RnBuiltins_System.h"
#include "../vm/RnObject.h"
#include "../vm/RnScope.h"
#include "../common/RnConfig.h"

#undef BUILTIN_CLASS
#define BUILTIN_CLASS RnBuiltins_System

#undef RN_BUILTIN_FUNC
#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DEFINE

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(__set_recursion_limit, RnType::RN_INT, 1) {
    BUILTIN_ASSERTS
    FUNCTION_ARG_COUNT_CHECK(__set_recursion_limit, 1)

    RnConfig::SetCallStackMaxDepth(args.at(0)->ToInt());
    ret_val->SetData(static_cast<RnIntNative>(RnConfig::GetCallStackMaxDepth()));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(__argv, RnType::RN_ARRAY, 0) {
    FUNCTION_ARG_COUNT_CHECK(__argv, 0)
    ret_val->SetData(RnConfig::GetArgv());
}

