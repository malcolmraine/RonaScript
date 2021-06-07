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
#include <typeinfo>
#include <typeindex>
#include <cassert>


// https://stackoverflow.com/a/2137056
// every function pointer will be stored as this type
typedef void (*voidFunctionType)();

struct Interface {
    std::map<std::string, voidFunctionType> m1;

    template<typename T>
    void insert(const std::string& s1, T f1){
        auto tt = std::type_index(typeid(f1));
        m1.insert(std::make_pair(s1, (voidFunctionType)f1));
    }

    template<typename... Args>
    RonaObject * searchAndCall(const std::string& s1, Args&&... args){
        auto mapIter = m1.find(s1);
        auto typeCastedFun = (RonaObject *(*)(Args ...))(mapIter->second);

        return typeCastedFun(std::forward<Args>(args)...);
    }
};

class BuiltinMethodManager {
public:
    BuiltinMethodManager();
    ~BuiltinMethodManager();
    bool exists(RonaObject * id);
    RonaObject * call_builtin_function(RonaObject *id, const std::vector<RonaObject *> &args);
protected:
    Interface interface;
};


// Builtins
void call_builtin_function(RonaObject *id, const std::vector<RonaObject *> &args);
RonaObject *rn_builtin_println(std::vector<RonaObject *> args);
RonaObject *rn_builtin_print(std::vector<RonaObject *> args);
RonaObject *rn_builtin_call(std::vector<RonaObject *> args);
RonaObject *rn_builtin_open(std::vector<RonaObject *> args);
RonaObject *rn_builtin_read(std::vector<RonaObject *> args);
RonaObject *rn_builtin_write(std::vector<RonaObject *> args);
RonaObject *rn_builtin_sum(std::vector<RonaObject *> args);
RonaObject *rn_builtin_pow(std::vector<RonaObject *> args);

RonaObject *rn_builtin_clear(std::vector<RonaObject *> args);
RonaObject *rn_builtin_count(std::vector<RonaObject *> args);

// Sys methods
RonaObject *rn_builtin_sys_exec(std::vector<RonaObject *> args);

// Runtime methods
RonaObject *rn_builtin_throw(std::vector<RonaObject *> args);

#endif //RONASCRIPT_BUILTINS_H