/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#pragma once

#include "../common/RnType.h"

class RnScope;
class RnObject;

class RnBuiltins_Math {
public:
    static void rn_builtin_sum(RnScope* scope, const RnArrayNative& args,
                               RnObject* ret_val);
    static void rn_builtin_pow(RnScope* scope, const RnArrayNative& args,
                               RnObject* ret_val);
    static void rn_builtin_mod(RnScope* scope, const RnArrayNative& args,
                               RnObject* ret_val);
    static void rn_builtin_sqrt(RnScope* scope, const RnArrayNative& args,
                                RnObject* ret_val);
    static void rn_builtin_cbrt(RnScope* scope, const RnArrayNative& args,
                                RnObject* ret_val);
    static void rn_builtin_randf(RnScope* scope, const RnArrayNative& args,
                                 RnObject* ret_val);
    static void rn_builtin_randint(RnScope* scope, const RnArrayNative& args,
                                   RnObject* ret_val);
    static void rn_builtin_normal(RnScope* scope, const RnArrayNative& args,
                                  RnObject* ret_val);
};
