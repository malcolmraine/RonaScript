/*****************************************************************************
* File: RnBuiltins_Type.cpp
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

#include "RnBuiltins_Type.h"
#include "../vm/RnObject.h"
#include "../vm/RnScope.h"

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_to_int(RnScope* scope, const RnArrayNative& args,
                                        RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(args[0]->ToInt());
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_to_float(RnScope* scope, const RnArrayNative& args,
                                          RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(args[0]->ToFloat());
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_to_string(RnScope* scope, const RnArrayNative& args,
                                           RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(args[0]->ToString());
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_to_bool(RnScope* scope, const RnArrayNative& args,
                                         RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(args[0]->ToBool());
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_string(RnScope* scope, const RnArrayNative& args,
                                           RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(static_cast<bool>(args[0]->GetActiveType() == RnType::RN_STRING));
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_float(RnScope* scope, const RnArrayNative& args,
                                          RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(static_cast<bool>(args[0]->GetActiveType() == RnType::RN_FLOAT));
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_array(RnScope* scope, const RnArrayNative& args,
                                          RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(static_cast<bool>(args[0]->GetActiveType() == RnType::RN_ARRAY));
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_object(RnScope* scope, const RnArrayNative& args,
                                           RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(static_cast<bool>(args[0]->GetActiveType() == RnType::RN_OBJECT));
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_bool(RnScope* scope, const RnArrayNative& args,
                                         RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(static_cast<bool>(args[0]->GetActiveType() == RnType::RN_BOOLEAN));
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_callable(RnScope* scope, const RnArrayNative& args,
                                             RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(
        static_cast<bool>(args[0]->GetActiveType() == RnType::RN_FUNCTION));
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_int(RnScope* scope, const RnArrayNative& args,
                                        RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(static_cast<bool>(args[0]->GetActiveType() == RnType::RN_INT));
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_any(RnScope* scope, const RnArrayNative& args,
                                        RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(static_cast<bool>(args[0]->GetType() == RnType::RN_ANY));
}
