/*****************************************************************************
* File: RnBuiltins.h
* Description:
* Author: Malcolm Hall
* Date: 5/30/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2021 Malcolm Hall
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************/

#pragma once

#include "../common/RnType.h"

class RnScope;
class RnObject;

class RnBuiltins {
public:
    static void rn_builtin_unpack(RnScope* scope, const RnArrayNative& args,
                                  RnObject* ret_val);
    static void rn_builtin_call(RnScope* scope, const RnArrayNative& args,
                                RnObject* ret_val);
    static void rn_builtin_system(RnScope* scope, const RnArrayNative& args,
                                  RnObject* ret_val);
    static void rn_builtin_lload(RnScope* scope, const RnArrayNative& args,
                                 RnObject* ret_val);
    static void rn_builtin_bind(RnScope* scope, const RnArrayNative& args,
                                RnObject* ret_val);
    static void rn_builtin_setenv(RnScope* scope, const RnArrayNative& args,
                                  RnObject* ret_val);
    static void rn_builtin_getenv(RnScope* scope, const RnArrayNative& args,
                                  RnObject* ret_val);
    static void rn_builtin_unsetenv(RnScope* scope, const RnArrayNative& args,
                                    RnObject* ret_val);
    static void rn_builtin_listattr(RnScope* scope, const RnArrayNative& args,
                                    RnObject* ret_val);
    static void rn_builtin_attrpairs(RnScope* scope, const RnArrayNative& args,
                                     RnObject* ret_val);
    static void rn_builtin_hasattr(RnScope* scope, const RnArrayNative& args,
                                   RnObject* ret_val);
    static void rn_builtin_getattr(RnScope* scope, const RnArrayNative& args,
                                   RnObject* ret_val);
    static void rn_builtin_setattr(RnScope* scope, const RnArrayNative& args,
                                   RnObject* ret_val);
    static void rn_builtin_delattr(RnScope* scope, const RnArrayNative& args,
                                   RnObject* ret_val);
};
