/*****************************************************************************
* File: RnBuiltins_Array.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/30/22
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

#include "RnBuiltins_Array.h"
#include "../vm/RnArrayObject.h"
#include "../vm/RnScope.h"

#undef BUILTIN_CLASS
#define BUILTIN_CLASS RnBuiltins_Array

#undef RN_BUILTIN_FUNC
#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DEFINE

/*****************************************************************************/
RN_BUILTIN_FUNC(BUILTIN_CLASS, filter, RnType::RN_ARRAY, 1){BUILTIN_ASSERTS}

/*****************************************************************************/
RN_BUILTIN_FUNC(BUILTIN_CLASS, union, RnType::RN_ARRAY, 1){BUILTIN_ASSERTS}

/*****************************************************************************/
RN_BUILTIN_FUNC(BUILTIN_CLASS, intersect, RnType::RN_ARRAY, 1){BUILTIN_ASSERTS}

/*****************************************************************************/
RN_BUILTIN_FUNC(BUILTIN_CLASS, count, RnType::RN_INT, 1) {
    BUILTIN_ASSERTS

    auto obj = args.front();
    if (obj->GetType() == RnType::RN_STRING) {
        ret_val->SetData(static_cast<RnIntNative>(obj->ToString().length()));
    } else if (obj->GetType() == RnType::RN_ARRAY) {
        ret_val->SetData(static_cast<RnIntNative>(obj->ToArray().size()));
    } else {
        ret_val->SetData(static_cast<RnIntNative>(1));
    }
}

/*****************************************************************************/
RN_BUILTIN_FUNC(BUILTIN_CLASS, array_merge, RnType::RN_ARRAY, 1) {
    BUILTIN_ASSERTS

    RnArrayNative data;
    for (auto arg : args) {
        for (auto& item : arg->ToArray()) {
            data.emplace_back(item);
        }
    }
    ret_val->SetData(data);
}

/*****************************************************************************/
RN_BUILTIN_FUNC(BUILTIN_CLASS, array_push, RnType::RN_VOID, 1) {
    BUILTIN_ASSERTS

    auto array_obj = dynamic_cast<RnArrayObject*>(args.front());
    for (size_t i = 1; i < args.size(); i++) {
        array_obj->Append(args.at(i));
    }
}

/*****************************************************************************/
RN_BUILTIN_FUNC(BUILTIN_CLASS, array_pop, RnType::RN_ANY, 1) {
    BUILTIN_ASSERTS
    auto data = args.front()->ToArray();
    ret_val->SetData(data.back());
    data.pop_back();
}

/*****************************************************************************/
RN_BUILTIN_FUNC(BUILTIN_CLASS, array_zeros, RnType::RN_ARRAY, 1) {
    BUILTIN_ASSERTS

    assert(args.size() == 1);
    RnArrayNative data;
    data.reserve(args.front()->ToInt());
    for (RnIntNative i = 0; i < args.front()->ToInt(); i++) {
        data.push_back(RnObject::Create(static_cast<RnIntNative>(0)));
    }
    ret_val->SetData(data);
}

/*****************************************************************************/
RN_BUILTIN_FUNC(BUILTIN_CLASS, array_fill, RnType::RN_ARRAY, 1) {
    BUILTIN_ASSERTS
    assert(args.size() == 2);
}

/*****************************************************************************/
RN_BUILTIN_FUNC(BUILTIN_CLASS, sequence, RnType::RN_ARRAY, 1) {
    BUILTIN_ASSERTS
    assert(args.size() > 2 && args.size() < 4);
    /**
	 * arg1: array
	 * arg2: count
	 * arg3: start
	 * arg4: step
	 */
}