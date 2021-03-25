/******************************************************************************
* File:
* Description:
* Author: Malcolm Hall
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
*******************************************************************************/

#ifndef RONASCRIPT_BUILTINS_H
#define RONASCRIPT_BUILTINS_H


#include "rona_object.h"
#include <string>
#include <map>
#include <vector>
#include "scope.h"
#include "iostream"

// Declarations
void call_builtin_function(Scope *scope, RonaObject *id, const std::vector<RonaObject *> &args);
void rn_builtin_println(Scope *scope, std::vector<RonaObject *> args);
void rn_builtin_print(Scope *scope, std::vector<RonaObject *> args);
void rn_builtin_call(Scope *scope, std::vector<RonaObject *> args);
void rn_builtin_sys(Scope *scope, std::vector<RonaObject *> args);
void rn_builtin_exit(Scope *scope, std::vector<RonaObject *> args);
void rn_builtin_open(Scope *scope, std::vector<RonaObject *> args);
void rn_builtin_read(Scope *scope, std::vector<RonaObject *> args);
void rn_builtin_write(Scope *scope, std::vector<RonaObject *> args);
void rn_builtin_sum(Scope *scope, std::vector<RonaObject *> args);
void rn_builtin_pow(Scope *scope, std::vector<RonaObject *> args);


#endif //RONASCRIPT_BUILTINS_H