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

#include "RnBuiltins_Math.h"
#include <random>
#include "../vm/RnObject.h"
#include "../vm/RnScope.h"

#undef BUILTIN_CLASS
#define BUILTIN_CLASS RnBuiltins_Math

#undef RN_BUILTIN_FUNC
#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DEFINE

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(sum, RnType::RN_FLOAT, 1) {
    assert(ret_val);
    assert(scope);

    RnFloatNative result = 0.0;
    auto operands = args.front()->ToArray();
    for (auto operand : operands) {
        result += operand->ToFloat();
    }

    ret_val->SetData(result);
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(pow, RnType::RN_FLOAT, 2) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(std::pow(args.front()->ToFloat(), args.back()->ToFloat()));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(mod, RnType::RN_INT, 2) {
    assert(ret_val);
    assert(scope);
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(sqrt, RnType::RN_FLOAT, 2) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(std::sqrt(args.front()->ToFloat()));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(cbrt, RnType::RN_FLOAT, 2) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(std::cbrt(args.front()->ToFloat()));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(randf, RnType::RN_FLOAT, 2) {
    assert(ret_val);
    assert(scope);
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(randint, RnType::RN_INT, 2) {
    assert(ret_val);
    assert(scope);
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(normal, RnType::RN_ARRAY, 1) {
    assert(ret_val);
    assert(scope);
}
