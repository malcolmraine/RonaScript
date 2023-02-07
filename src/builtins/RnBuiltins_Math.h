/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_VM_BUILTINS_RNBUILTINS_MATH_H_
#define RONASCRIPT_VM_BUILTINS_RNBUILTINS_MATH_H_

#include <vector>

class RnScope;
class RnObject;

class RnBuiltins_Math {
public:
    static void rn_builtin_sum(RnScope* scope, const std::vector<RnObject*>& args,
                               RnObject* ret_val);
    static void rn_builtin_pow(RnScope* scope, const std::vector<RnObject*>& args,
                               RnObject* ret_val);
    static void rn_builtin_mod(RnScope* scope, const std::vector<RnObject*>& args,
                               RnObject* ret_val);
    static void rn_builtin_sqrt(RnScope* scope, const std::vector<RnObject*>& args,
                                RnObject* ret_val);
    static void rn_builtin_cbrt(RnScope* scope, const std::vector<RnObject*>& args,
                                RnObject* ret_val);
    static void rn_builtin_randf(RnScope* scope, const std::vector<RnObject*>& args,
                                 RnObject* ret_val);
    static void rn_builtin_randint(RnScope* scope, const std::vector<RnObject*>& args,
                                   RnObject* ret_val);
    static void rn_builtin_normal(RnScope* scope, const std::vector<RnObject*>& args,
                                  RnObject* ret_val);
};

#endif  //RONASCRIPT_VM_BUILTINS_RNBUILTINS_MATH_H_