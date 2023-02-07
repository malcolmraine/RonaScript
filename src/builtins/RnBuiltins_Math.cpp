/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "RnBuiltins_Math.h"
#include <random>
#include "../vm/RnObject.h"
#include "../vm/RnScope.h"

/*****************************************************************************/
void RnBuiltins_Math::rn_builtin_pow(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val) {
    ret_val->SetData(std::pow(args.front()->ToFloat(), args.back()->ToFloat()));
}

/*****************************************************************************/
void RnBuiltins_Math::rn_builtin_mod(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val) {}

/*****************************************************************************/
void RnBuiltins_Math::rn_builtin_sqrt(RnScope* scope,
                                      const std::vector<RnObject*>& args,
                                      RnObject* ret_val) {
    ret_val->SetData(std::sqrt(args.front()->ToFloat()));
}

/*****************************************************************************/
void RnBuiltins_Math::rn_builtin_cbrt(RnScope* scope,
                                      const std::vector<RnObject*>& args,
                                      RnObject* ret_val) {
    ret_val->SetData(std::cbrt(args.front()->ToFloat()));
}

/*****************************************************************************/
void RnBuiltins_Math::rn_builtin_randf(RnScope* scope,
                                       const std::vector<RnObject*>& args,
                                       RnObject* ret_val) {}

/*****************************************************************************/
void RnBuiltins_Math::rn_builtin_randint(RnScope* scope,
                                         const std::vector<RnObject*>& args,
                                         RnObject* ret_val) {}

/*****************************************************************************/
void RnBuiltins_Math::rn_builtin_normal(RnScope* scope,
                                        const std::vector<RnObject*>& args,
                                        RnObject* ret_val) {}

/*****************************************************************************/
void RnBuiltins_Math::rn_builtin_sum(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val) {
    RnFloatNative result = 0.0;
    auto operands = args.front()->ToArray();
    for (auto operand : operands) {
        result += operand->ToFloat();
    }

    ret_val->SetData(result);
}