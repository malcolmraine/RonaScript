/*****************************************************************************
* File: RnBuiltins.h
* Description:
* Author: Malcolm Hall
* Date: 5/30/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNBUILTINS_H
#define RONASCRIPT_RNBUILTINS_H

#include <map>
#include <vector>

class RnScope;
class RnObject;

class RnBuiltins {
public:
    static void rn_builtin_unpack(RnScope* scope, const std::vector<RnObject*>& args,
                                  RnObject* ret_val);
    static void rn_builtin_call(RnScope* scope, const std::vector<RnObject*>& args,
                                RnObject* ret_val);
    static void rn_builtin_system(RnScope* scope, const std::vector<RnObject*>& args,
                                  RnObject* ret_val);
    static void rn_builtin_lload(RnScope* scope, const std::vector<RnObject*>& args,
                                 RnObject* ret_val);
    static void rn_builtin_bind(RnScope* scope, const std::vector<RnObject*>& args,
                                RnObject* ret_val);
    static void rn_builtin_setenv(RnScope* scope, const std::vector<RnObject*>& args,
                                  RnObject* ret_val);
    static void rn_builtin_getenv(RnScope* scope, const std::vector<RnObject*>& args,
                                  RnObject* ret_val);
    static void rn_builtin_unsetenv(RnScope* scope, const std::vector<RnObject*>& args,
                                    RnObject* ret_val);
    static void rn_builtin_listattr(RnScope* scope, const std::vector<RnObject*>& args,
                                    RnObject* ret_val);
    static void rn_builtin_attrpairs(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val);
    static void rn_builtin_hasattr(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val);
    static void rn_builtin_getattr(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val);
    static void rn_builtin_setattr(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val);
    static void rn_builtin_delattr(RnScope* scope, const std::vector<RnObject*>& args,
                                   RnObject* ret_val);
};

#endif  //RONASCRIPT_RNBUILTINS_H