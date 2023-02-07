/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_VM_BUILTINS_RNBUILTINS_TYPE_H_
#define RONASCRIPT_VM_BUILTINS_RNBUILTINS_TYPE_H_

#include <vector>

class RnScope;
class RnObject;

class RnBuiltins_Type {
public:
    // Casting
    static void rn_builtin_to_int(RnScope* scope, const std::vector<RnObject*>& args,
                                  RnObject* ret_val);
    static void rn_builtin_to_float(RnScope* scope, const std::vector<RnObject*>& args,
                                    RnObject* ret_val);
    static void rn_builtin_to_string(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val);
    static void rn_builtin_to_bool(RnScope* scope, const std::vector<RnObject*>& args,
                                   RnObject* ret_val);

    // type methods
    static void rn_builtin_is_string(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val);
    static void rn_builtin_is_float(RnScope* scope, const std::vector<RnObject*>& args,
                                    RnObject* ret_val);
    static void rn_builtin_is_array(RnScope* scope, const std::vector<RnObject*>& args,
                                    RnObject* ret_val);
    static void rn_builtin_is_object(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val);
    static void rn_builtin_is_bool(RnScope* scope, const std::vector<RnObject*>& args,
                                   RnObject* ret_val);
    static void rn_builtin_is_callable(RnScope* scope,
                                       const std::vector<RnObject*>& args,
                                       RnObject* ret_val);
    static void rn_builtin_is_int(RnScope* scope, const std::vector<RnObject*>& args,
                                  RnObject* ret_val);
};

#endif  //RONASCRIPT_VM_BUILTINS_RNBUILTINS_TYPE_H_