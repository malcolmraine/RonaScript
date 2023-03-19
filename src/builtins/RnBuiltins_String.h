/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_VM_BUILTINS_RNBUILTINS_STRING_H_
#define RONASCRIPT_VM_BUILTINS_RNBUILTINS_STRING_H_

#include <vector>

class RnScope;
class RnObject;

class RnBuiltins_String {
public:
    // String methods
    static void rn_builtin_str_titlecase(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val);
    static void rn_builtin_str_lower(RnScope* scope, const std::vector<RnObject*>& args,
                                 RnObject* ret_val);
    static void rn_builtin_str_upper(RnScope* scope, const std::vector<RnObject*>& args,
                                 RnObject* ret_val);
    static void rn_builtin_str_snakecase(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val);
    static void rn_builtin_str_split(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val);
    static void rn_builtin_str_substr(RnScope* scope,
                                      const std::vector<RnObject*>& args,
                                      RnObject* ret_val);
    static void rn_builtin_str_startswith(RnScope* scope,
                                         const std::vector<RnObject*>& args,
                                         RnObject* ret_val);
    static void rn_builtin_str_endswith(RnScope* scope,
                                        const std::vector<RnObject*>& args,
                                        RnObject* ret_val);
    static void rn_builtin_str_join(RnScope* scope, const std::vector<RnObject*>& args,
                                    RnObject* ret_val);
};

#endif  //RONASCRIPT_VM_BUILTINS_RNBUILTINS_STRING_H_