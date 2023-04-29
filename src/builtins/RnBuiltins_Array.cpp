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

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_filter(RnScope* scope,
                                               const RnArrayNative& args,
                                               RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_union(RnScope* scope, const RnArrayNative& args,
                                              RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_intersect(RnScope* scope,
                                                  const RnArrayNative& args,
                                                  RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_merge(RnScope* scope, const RnArrayNative& args,
                                              RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    RnArrayNative data;
    for (auto arg : args) {
        for (auto& item : arg->ToArray()) {
            data.emplace_back(item);
        }
    }
    ret_val->SetData(data);
}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_count(RnScope* scope, const RnArrayNative& args,
                                        RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    auto obj = args.front();
    if (obj->GetType() == RnType::RN_STRING) {
        ret_val->SetData(static_cast<RnIntNative>(obj->ToString().length()));
    } else {
        ret_val->SetData(static_cast<RnIntNative>(obj->ToArray().size()));
    }
}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_push(RnScope* scope, const RnArrayNative& args,
                                             RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    auto array_obj = dynamic_cast<RnArrayObject*>(args.front());
    for (size_t i = 1; i < args.size(); i++) {
        array_obj->Append(args.at(i));
    }
}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_pop(RnScope* scope, const RnArrayNative& args,
                                            RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_zeros(RnScope* scope, const RnArrayNative& args,
                                              RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    assert(args.size() == 1);
    RnArrayNative data;
    data.reserve(args.front()->ToInt());
    for (RnIntNative i = 0; i < args.front()->ToInt(); i++) {
        // make objects and add to vector
    }
    ret_val->SetData(data);
}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_array_fill(RnScope* scope, const RnArrayNative& args,
                                             RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    assert(args.size() == 2);
}

/*****************************************************************************/
void RnBuiltins_Array::rn_builtin_sequence(RnScope* scope, const RnArrayNative& args,
                                           RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    assert(args.size() > 2 && args.size() < 4);
    /**
	 * arg1: array
	 * arg2: count
	 * arg3: start
	 * arg4: step
	 */
}
