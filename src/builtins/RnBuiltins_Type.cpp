/*****************************************************************************
* File: RnBuiltins_Type.cpp
* Description:
* Author: Malcolm Hall
* Date: 2/1/22
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

#include "RnBuiltins_Type.h"
#include "../vm/RnClassObject.h"
#include "../vm/RnFunction.h"
#include "../vm/RnObject.h"
#include "../vm/RnScope.h"
#include "../vm/RnVirtualMachine.h"

#undef BUILTIN_CLASS
#define BUILTIN_CLASS RnBuiltins_Type

#undef RN_BUILTIN_FUNC
#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DEFINE

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(stoi, RnType::RN_INT, 1) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(stoi, 1)

    ret_val->SetData(std::stoll(args[0]->ToString()));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(stof, RnType::RN_FLOAT, 1) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(stof, 1)

    ret_val->SetData(std::stod(args[0]->ToString()));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(to_int, RnType::RN_INT, 1) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(to_int, 1)

    RnObject* obj = args[0];
    if (obj->GetActiveType() == RnType::RN_OBJECT) {
        auto instance = dynamic_cast<RnClassObject*>(obj);
        if (!instance->TryMagicMethod(RnClassObject::MAGIC_METHOD_KEY_INT, args,
                                      ret_val)) {
            ret_val->SetData(args[0]->ToInt());
        }
    } else {
        ret_val->SetData(args[0]->ToInt());
    }
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(to_float, RnType::RN_FLOAT, 1) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(to_float, 1)

    RnObject* obj = args[0];
    if (obj->GetActiveType() == RnType::RN_OBJECT) {
        auto instance = dynamic_cast<RnClassObject*>(obj);
        if (!instance->TryMagicMethod(RnClassObject::MAGIC_METHOD_KEY_FLOAT, args,
                                      ret_val)) {
            ret_val->SetData(args[0]->ToFloat());
        }
    } else {
        ret_val->SetData(args[0]->ToFloat());
    }
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(to_string, RnType::RN_STRING, 1) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(to_string, 1)

    RnObject* obj = args[0];
    if (obj->GetActiveType() == RnType::RN_OBJECT) {
        auto instance = dynamic_cast<RnClassObject*>(obj);
        if (!instance->TryMagicMethod(RnClassObject::MAGIC_METHOD_KEY_STR, args,
                                      ret_val)) {
            ret_val->SetData(args[0]->ToString());
        }
    } else {
        ret_val->SetData(args[0]->ToString());
    }
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(to_bool, RnType::RN_BOOLEAN, 1) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(to_bool, 1)

    RnObject* obj = args[0];
    if (obj->GetActiveType() == RnType::RN_OBJECT) {
        auto instance = dynamic_cast<RnClassObject*>(obj);
        if (!instance->TryMagicMethod(RnClassObject::MAGIC_METHOD_KEY_BOOL, args,
                                      ret_val)) {
            ret_val->SetData(args[0]->ToBool());
        }
    } else {
        ret_val->SetData(args[0]->ToBool());
    }
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(to_array, RnType::RN_ARRAY, 1) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(to_array, 1)

    RnObject* obj = args[0];
    if (obj->GetActiveType() == RnType::RN_OBJECT) {
        auto instance = dynamic_cast<RnClassObject*>(obj);
        if (!instance->TryMagicMethod(RnClassObject::MAGIC_METHOD_KEY_ARRAY, args,
                                      ret_val)) {
            ret_val->SetData(args[0]->ToArray());
        }
    } else {
        ret_val->SetData(args[0]->ToArray());
    }
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(is_string, RnType::RN_BOOLEAN, 1) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(static_cast<bool>(args[0]->GetActiveType() == RnType::RN_STRING));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(is_int, RnType::RN_BOOLEAN, 1) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(is_int, 1)

    ret_val->SetData(static_cast<bool>(args[0]->GetActiveType() == RnType::RN_INT));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(is_float, RnType::RN_BOOLEAN, 1) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(is_float, 1)

    ret_val->SetData(static_cast<bool>(args[0]->GetActiveType() == RnType::RN_FLOAT));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(is_bool, RnType::RN_BOOLEAN, 1) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(is_bool, 1)

    ret_val->SetData(static_cast<bool>(args[0]->GetActiveType() == RnType::RN_BOOLEAN));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(is_array, RnType::RN_BOOLEAN, 1) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(is_array, 1)

    ret_val->SetData(static_cast<bool>(args[0]->GetActiveType() == RnType::RN_ARRAY));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(is_object, RnType::RN_BOOLEAN, 1) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(is_object, 1)

    ret_val->SetData(static_cast<bool>(args[0]->GetActiveType() == RnType::RN_OBJECT));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(is_callable, RnType::RN_BOOLEAN, 1) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(is_callable, 1)

    ret_val->SetData(
        static_cast<bool>(args[0]->GetActiveType() == RnType::RN_FUNCTION));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(is_any, RnType::RN_BOOLEAN, 1) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(is_any, 1)

    ret_val->SetData(static_cast<bool>(args[0]->GetType() == RnType::RN_ANY));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(instanceof, RnType::RN_BOOLEAN, 2) {
    assert(ret_val);
    assert(scope);
    FIXED_ARG_COUNT_CHECK(instanceof, 2)

    ret_val->SetData(static_cast<bool>(args[0]->GetTypeName() == args[1]->GetTypeName()));
}



