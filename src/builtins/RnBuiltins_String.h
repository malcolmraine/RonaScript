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
    static void titlecase(RnScope* scope, const std::vector<RnObject*>& args,
                          RnObject* ret_val);
    static void lower(RnScope* scope, const std::vector<RnObject*>& args,
                      RnObject* ret_val);
    static void upper(RnScope* scope, const std::vector<RnObject*>& args,
                      RnObject* ret_val);
    static void snakecase(RnScope* scope, const std::vector<RnObject*>& args,
                          RnObject* ret_val);
    static void str_split(RnScope* scope, const std::vector<RnObject*>& args,
                          RnObject* ret_val);
    static void str_merge(RnScope* scope, const std::vector<RnObject*>& args,
                          RnObject* ret_val);
    static void str_substr(RnScope* scope, const std::vector<RnObject*>& args,
                           RnObject* ret_val);
    static void str_startwith(RnScope* scope, const std::vector<RnObject*>& args,
                              RnObject* ret_val);
    static void str_endswith(RnScope* scope, const std::vector<RnObject*>& args,
                             RnObject* ret_val);
    static void str_join(RnScope* scope, const std::vector<RnObject*>& args,
                         RnObject* ret_val);
    static void rn_builtin_str_merge(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val);
};

#endif  //RONASCRIPT_VM_BUILTINS_RNBUILTINS_STRING_H_