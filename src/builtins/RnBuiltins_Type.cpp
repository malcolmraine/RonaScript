/*****************************************************************************
* File: RnBuiltins_Type.cpp
* Description:
* Author: Malcolm Hall
* Date: 2/1/22
* Version: 1
*
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
