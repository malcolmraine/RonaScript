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

class RnBuiltins_Type {
public:
    // Casting
    static void rn_builtin_to_int(RnScope* scope, const RnArrayNative& args,
                                  RnObject* ret_val);
    static void rn_builtin_to_float(RnScope* scope, const RnArrayNative& args,
                                    RnObject* ret_val);
    static void rn_builtin_to_string(RnScope* scope, const RnArrayNative& args,
                                     RnObject* ret_val);
    static void rn_builtin_to_bool(RnScope* scope, const RnArrayNative& args,
                                   RnObject* ret_val);

    // type methods
    static void rn_builtin_is_string(RnScope* scope, const RnArrayNative& args,
                                     RnObject* ret_val);
    static void rn_builtin_is_float(RnScope* scope, const RnArrayNative& args,
                                    RnObject* ret_val);
    static void rn_builtin_is_array(RnScope* scope, const RnArrayNative& args,
                                    RnObject* ret_val);
    static void rn_builtin_is_object(RnScope* scope, const RnArrayNative& args,
                                     RnObject* ret_val);
    static void rn_builtin_is_bool(RnScope* scope, const RnArrayNative& args,
                                   RnObject* ret_val);
    static void rn_builtin_is_callable(RnScope* scope, const RnArrayNative& args,
                                       RnObject* ret_val);
    static void rn_builtin_is_int(RnScope* scope, const RnArrayNative& args,
                                  RnObject* ret_val);
    static void rn_builtin_is_any(RnScope* scope, const RnArrayNative& args,
                                  RnObject* ret_val);
};
