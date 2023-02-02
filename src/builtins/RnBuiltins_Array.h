/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_VM_BUILTINS_RNBUILTINS_ARRAY_H_
#define RONASCRIPT_VM_BUILTINS_RNBUILTINS_ARRAY_H_

#include <vector>

class RnScope;
class RnObject;

class RnBuiltins_Array {
public:
    // Array functions
    static void array_merge(RnScope* scope, const std::vector<RnObject*>& args,
                            RnObject* ret_val);
    static void array_filter(RnScope* scope, const std::vector<RnObject*>& args,
                             RnObject* ret_val);
    static void array_union(RnScope* scope, const std::vector<RnObject*>& args,
                            RnObject* ret_val);
    static void array_intersect(RnScope* scope, const std::vector<RnObject*>& args,
                                RnObject* ret_val);
    static void rn_builtin_count(RnScope* scope, const std::vector<RnObject*>& args,
                                 RnObject* ret_val);
    static void rn_builtin_array_merge(RnScope* scope,
                                       const std::vector<RnObject*>& args,
                                       RnObject* ret_val);
    static void rn_builtin_array_push(RnScope* scope,
                                      const std::vector<RnObject*>& args,
                                      RnObject* ret_val);
    static void rn_builtin_array_pop(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val);
    static void rn_builtin_array_zeros(RnScope* scope,
                                       const std::vector<RnObject*>& args,
                                       RnObject* ret_val);
    static void rn_builtin_array_fill(RnScope* scope,
                                      const std::vector<RnObject*>& args,
                                      RnObject* ret_val);
    static void rn_builtin_sequence(RnScope* scope, const std::vector<RnObject*>& args,
                                    RnObject* ret_val);
};

#endif  //RONASCRIPT_VM_BUILTINS_RNBUILTINS_ARRAY_H_