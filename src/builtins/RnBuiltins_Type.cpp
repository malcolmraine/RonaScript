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
void RnBuiltins_Type::rn_builtin_to_int(RnScope* scope, const std::vector<RnObject*>& args,
                             RnObject* ret_val) {
    ret_val->SetData(args[0]->ToInt());
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_to_float(RnScope* scope, const std::vector<RnObject*>& args,
                               RnObject* ret_val) {
    ret_val->SetData(args[0]->ToFloat());
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_to_string(RnScope* scope, const std::vector<RnObject*>& args,
                                RnObject* ret_val) {
    ret_val->SetData(args[0]->ToString());
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_to_bool(RnScope* scope, const std::vector<RnObject*>& args,
                                           RnObject* ret_val) {
    ret_val->SetData(args[0]->ToBool());
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_string(RnScope* scope,
                                           const std::vector<RnObject*>& args,
                                           RnObject* ret_val) {
    ret_val->SetData(static_cast<bool>(args[0]->GetType() == RnType::RN_STRING));
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_float(RnScope* scope,
                                          const std::vector<RnObject*>& args,
                                          RnObject* ret_val) {
    ret_val->SetData(static_cast<bool>(args[0]->GetType() == RnType::RN_FLOAT));
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_array(RnScope* scope,
                                          const std::vector<RnObject*>& args,
                                          RnObject* ret_val) {
    ret_val->SetData(static_cast<bool>(args[0]->GetType() == RnType::RN_ARRAY));
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_object(RnScope* scope,
                                           const std::vector<RnObject*>& args,
                                           RnObject* ret_val) {
    ret_val->SetData(static_cast<bool>(args[0]->GetType() == RnType::RN_OBJECT));
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_bool(RnScope* scope,
                                         const std::vector<RnObject*>& args,
                                         RnObject* ret_val) {
    ret_val->SetData(static_cast<bool>(args[0]->GetType() == RnType::RN_BOOLEAN));
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_callable(RnScope* scope,
                                             const std::vector<RnObject*>& args,
                                             RnObject* ret_val) {
    ret_val->SetData(static_cast<bool>(args[0]->GetType() == RnType::RN_FUNCTION));
}

/*****************************************************************************/
void RnBuiltins_Type::rn_builtin_is_int(RnScope* scope,
                                        const std::vector<RnObject*>& args,
                                        RnObject* ret_val) {
    ret_val->SetData(static_cast<bool>(args[0]->GetType() == RnType::RN_INT));
}
