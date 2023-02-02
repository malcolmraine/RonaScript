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
    static void pow(RnScope* scope, const std::vector<RnObject*>& args,
                    RnObject* ret_val);
    static void mod(RnScope* scope, const std::vector<RnObject*>& args,
                    RnObject* ret_val);
    static void sqrt(RnScope* scope, const std::vector<RnObject*>& args,
                     RnObject* ret_val);
    static void cbrt(RnScope* scope, const std::vector<RnObject*>& args,
                     RnObject* ret_val);
    static void randf(RnScope* scope, const std::vector<RnObject*>& args,
                      RnObject* ret_val);
    static void randint(RnScope* scope, const std::vector<RnObject*>& args,
                        RnObject* ret_val);
    static void normal(RnScope* scope, const std::vector<RnObject*>& args,
                       RnObject* ret_val);
};

#endif  //RONASCRIPT_VM_BUILTINS_RNBUILTINS_MATH_H_