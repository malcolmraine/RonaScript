/*****************************************************************************
* File: RnBuiltins.cpp
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

#include "RnBuiltins.h"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include "../util/String.h"
#include "../vm/RnArrayObject.h"
#include "../vm/RnFunction.h"
#include "../vm/RnFunctionObject.h"
#include "../vm/RnScope.h"
#include "../vm/RnVirtualMachine.h"

#undef RN_BUILTIN_FUNC
#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DEFINE
#define BUILTIN_CLASS RnBuiltins

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(unpack, RnType::RN_VOID, 1) {
    BUILTIN_ASSERTS

    auto parent_scope = scope->GetParent();
    RnScope* unpack_scope = parent_scope;
    if (!unpack_scope) {
        unpack_scope = scope;
    }

    for (auto obj : args.front()->ToArray()) {
        scope->GetStack().push_back(obj);
    }
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(system, RnType::RN_ANY, 1) {
    BUILTIN_ASSERTS

    std::array<char, 128> buffer{};
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(
        popen(args.front()->ToString().c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    result = String::Replace(result, "\n", "\\n");
    RnArrayNative output = {RnObject::Create(result)};
    ret_val->SetData(output);
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(call, RnType::RN_ANY, 2) {
    BUILTIN_ASSERTS

    auto obj = scope->GetObject(RnConstStore::InternValue(args.front()->ToString()));
    auto func_obj = dynamic_cast<RnFunctionObject*>(obj);
    auto func = func_obj->GetData();
    ret_val->CopyDataFromObject(RnVirtualMachine::GetInstance()->CallFunction(func, args));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(lload, RnType::RN_OBJECT, 1) {
    BUILTIN_ASSERTS

    // TODO: Add file existence check for library
    // TODO: Add check to make sure library actually loaded and return status accordingly
    auto scope_obj = new RnScope(nullptr);
    RnScope::LoadLibraryIntoScope(scope_obj, args.at(0)->ToString(), true);
    ret_val->SetData(scope_obj);
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(bind, RnType::RN_VOID, 2) {
    BUILTIN_ASSERTS
    // arg 1: object to bind to
    // arg 2: function object

    std::string name = "this";
    auto obj = args[0];
    auto func = dynamic_cast<RnFunctionObject*>(args[1]);

    // User can pass a different name to bind to if they want
    if (args.size() == 3) {
        name = args[2]->ToString();
    }
    func->GetData()->GetScope()->StoreObject(RnConstStore::InternValue(name), obj);
    ret_val->SetData(func->GetData());
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(setenv, RnType::RN_VOID, 2) {
    BUILTIN_ASSERTS

    ret_val->SetData(static_cast<RnIntNative>(
        setenv(args[0]->ToString().c_str(), args[1]->ToString().c_str(), 1)));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(getenv, RnType::RN_ANY, 1) {
    BUILTIN_ASSERTS

    ret_val->SetData(getenv(args[0]->ToString().c_str()));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(unsetenv, RnType::RN_VOID, 1) {
    BUILTIN_ASSERTS

    ret_val->SetData(static_cast<RnIntNative>(unsetenv(args[0]->ToString().c_str())));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(listattr, RnType::RN_ARRAY, 1) {
    BUILTIN_ASSERTS

    RnArrayNative attrs;
    for (const auto& attr : args.front()->ToObject()->GetSymbolTable()->GetSymbols()) {
        attrs.push_back(RnObject::Create(RnConstStore::GetInternedString(attr)));
    }
    ret_val->SetData(attrs);
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(attrpairs, RnType::RN_ARRAY, 1) {
    BUILTIN_ASSERTS

    if (args.front()->GetType() != RnType::RN_OBJECT) {
        return;
    }

    RnArrayNative attrs;
    auto target_scope = args.front()->ToObject();
    for (const auto& attr : target_scope->GetSymbolTable()->GetSymbols()) {
        auto pair_obj =
            dynamic_cast<RnArrayObject*>(RnObject::Create(RnType::RN_ARRAY));
        RnArrayNative data = {RnObject::Create(RnConstStore::GetInternedString(attr)),
                              target_scope->GetObject(attr)};
        pair_obj->SetData(data);
        attrs.push_back(pair_obj);
    }
    ret_val->SetData(attrs);
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(hasattr, RnType::RN_VOID, 2) {
    BUILTIN_ASSERTS

    auto obj = args[0]->ToObject();
    auto attr_key = RnConstStore::InternValue(args[1]->ToString());
    ret_val->SetData(obj->GetSymbolTable()->SymbolExists(attr_key));
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(getattr, RnType::RN_VOID, 2) {
    BUILTIN_ASSERTS

    auto obj = args[0]->ToObject();
    auto attr_key = RnConstStore::InternValue(args[1]->ToString());
    if (obj->GetSymbolTable()->SymbolExists(attr_key)) {
        auto original = obj->GetObject(attr_key);
        auto result = RnObject::Create(original->GetType());
        result->CopyDataFromObject(original);
        RnArrayNative array_data = {result};
        ret_val->SetData(array_data);
    }
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(setattr, RnType::RN_VOID, 3) {
    BUILTIN_ASSERTS

    auto obj = args[0]->ToObject();
    auto attr_key = RnConstStore::InternValue(args[1]->ToString());
    if (obj->GetSymbolTable()->SymbolExists(attr_key)) {
        auto original = obj->GetObject(attr_key);
        original->CopyDataFromObject(args[2]);
    } else {
        auto copy = RnObject::Create(args[2]->GetType());
        copy->CopyDataFromObject(args[2]);
        obj->StoreObject(attr_key, copy);
    }
    ret_val->SetData(true);
}

/*****************************************************************************/
RN_BUILTIN_FUNC_DEFINE(delattr, RnType::RN_VOID, 2) {
    BUILTIN_ASSERTS

    auto obj = args[0]->ToObject();
    auto attr_key = RnConstStore::InternValue(args[1]->ToString());
    if (obj->GetSymbolTable()->SymbolExists(attr_key)) {
        obj->RemoveObject(attr_key);
        ret_val->SetData(true);
    } else {
        ret_val->SetData(false);
    }
}