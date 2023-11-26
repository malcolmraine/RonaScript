/*****************************************************************************
* File: RnBuiltins_Array.cpp
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

#include "RnBuiltins_Array.h"
#include "../vm/RnArrayObject.h"
#include "../vm/RnScope.h"

#undef BUILTIN_CLASS
#define BUILTIN_CLASS RnBuiltins_Array

//#undef RN_BUILTIN_FUNC
//#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DEFINE

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(filter, RnType::RN_ARRAY, 1){BUILTIN_ASSERTS}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(union, RnType::RN_ARRAY, 1){BUILTIN_ASSERTS}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(intersect, RnType::RN_ARRAY, 1){BUILTIN_ASSERTS}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(count, RnType::RN_INT, 1) {
    BUILTIN_ASSERTS

    auto obj = args.front();
    if (obj->GetActiveType() == RnType::RN_STRING) {
        ret_val->SetData(static_cast<RnIntNative>(obj->ToString().length()));
    } else if (obj->GetActiveType() == RnType::RN_ARRAY) {
        ret_val->SetData(static_cast<RnIntNative>(obj->ToArray().size()));
    } else {
        ret_val->SetData(static_cast<RnIntNative>(1));
    }
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(array_merge, RnType::RN_ARRAY, 1) {
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
RN_BUILTIN_FUNC_DEFINE(array_push, RnType::RN_VOID, 1) {
    BUILTIN_ASSERTS

    auto array_obj = dynamic_cast<RnArrayObject*>(args.front());
    for (size_t i = 1; i < args.size(); i++) {
        array_obj->Append(args.at(i));
    }
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(array_pop, RnType::RN_ANY, 1) {
    BUILTIN_ASSERTS
    auto data = args.front()->ToArray();
    ret_val->SetData(data.back());
    data.pop_back();
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(array_zeros, RnType::RN_ARRAY, 1) {
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
RN_BUILTIN_FUNC_DEFINE(array_fill, RnType::RN_ARRAY, 1) {
    BUILTIN_ASSERTS
    assert(args.size() == 2);
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(sequence, RnType::RN_ARRAY, 1) {
    BUILTIN_ASSERTS
    assert(args.size() > 2 && args.size() < 4);
    /**
	 * arg1: array
	 * arg2: count
	 * arg3: start
	 * arg4: step
	 */
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(slice, RnType::RN_ANY, 3) {
    BUILTIN_ASSERTS
    FIXED_ARG_COUNT_CHECK(slice, 3)
    auto subject = args[0];
    auto start_offset = args[1]->ToInt();
    auto count = args[2]->ToInt();

    if (subject->GetActiveType() == RnType::RN_STRING) {
        RnStringNative data = subject->ToString();
        ret_val->SetData(static_cast<RnStringNative>(
            data.substr(start_offset, count)));
    } else if (subject->GetActiveType() == RnType::RN_ARRAY) {
        RnArrayNative data = subject->ToArray();
        RnArrayNative slice_data;
        slice_data.reserve(count - start_offset);
        ret_val->SetData(RnArrayNative(data.begin() + start_offset,
                                       data.begin() + start_offset + count));

    } else {
        throw std::runtime_error(
            "Routine 'slice' expects a string or array but received a " +
            RnType::TypeToString(subject->GetActiveType()));
    }
}