/*****************************************************************************
* File: RnBuiltins.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/30/22
* Version: 1
*
******************************************************************************/

#include "RnBuiltins.h"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "../util/String.h"
#include "../vm/RnArrayObject.h"
#include "../vm/RnFunction.h"
#include "../vm/RnFunctionObject.h"
#include "../vm/RnScope.h"

/*****************************************************************************/
void RnBuiltins::rn_builtin_unpack(RnScope* scope, const std::vector<RnObject*>& args,
                                   RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

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
void RnBuiltins::rn_builtin_call(RnScope* scope, const std::vector<RnObject*>& args,
                                 RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    auto obj = scope->GetObject(RnObject::InternValue(args.front()->ToString()));
    auto func_obj = dynamic_cast<RnFunctionObject*>(obj);
    auto func = func_obj->GetData();
    std::vector<RnObject*> callArgs(args.begin() + 1, args.end());
    RnObject* call_ret_val = RnObject::Create(func->GetReturnType());
    func->Call(callArgs, call_ret_val);
    std::vector<RnObject*> ret_vals = {call_ret_val};
    ret_val->SetData(ret_vals);
}

/*****************************************************************************/
void RnBuiltins::rn_builtin_system(RnScope* scope, const std::vector<RnObject*>& args,
                                   RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

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
    std::vector<RnObject*> output = {RnObject::Create(result)};
    ret_val->SetData(output);
}

/*****************************************************************************/
void RnBuiltins::rn_builtin_lload(RnScope* scope, const std::vector<RnObject*>& args,
                                  RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    // TODO: Add file existence check for library
    // TODO: Add check to make sure library actually loaded and return status accordingly
    auto scope_obj = new RnScope(nullptr);
    RnScope::LoadLibraryIntoScope(scope_obj, args.at(0)->ToString(), true);
    ret_val->SetData(scope_obj);
}

/*****************************************************************************/
void RnBuiltins::rn_builtin_bind(RnScope* scope, const std::vector<RnObject*>& args,
                                 RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    // arg 1: object to bind to
    // arg 2: function object

    std::string name = "this";
    auto obj = args[0];
    auto func = dynamic_cast<RnFunctionObject*>(args[1]);

    // User can pass a different name to bind to if they want
    if (args.size() == 3) {
        name = args[2]->ToString();
    }
    func->GetData()->GetScope()->StoreObject(RnObject::InternValue(name), obj);
    ret_val->SetData(func->GetData());
}

/*****************************************************************************/
void RnBuiltins::rn_builtin_setenv(RnScope* scope, const std::vector<RnObject*>& args,
                                   RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(static_cast<RnIntNative>(
        setenv(args[0]->ToString().c_str(), args[1]->ToString().c_str(), 1)));
}

/*****************************************************************************/
void RnBuiltins::rn_builtin_getenv(RnScope* scope, const std::vector<RnObject*>& args,
                                   RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(getenv(args[0]->ToString().c_str()));
}

/*****************************************************************************/
void RnBuiltins::rn_builtin_unsetenv(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    ret_val->SetData(static_cast<RnIntNative>(unsetenv(args[0]->ToString().c_str())));
}

/*****************************************************************************/
void RnBuiltins::rn_builtin_listattr(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    std::vector<RnObject*> attrs;
    for (const auto& attr : args.front()->ToObject()->GetSymbolTable()->GetSymbols()) {
        attrs.push_back(RnObject::Create(RnObject::GetInternedString(attr)));
    }
    ret_val->SetData(attrs);
}

/*****************************************************************************/
void RnBuiltins::rn_builtin_attrpairs(RnScope* scope,
                                      const std::vector<RnObject*>& args,
                                      RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    if (args.front()->GetType() != RnType::RN_OBJECT) {
        return;
    }

    std::vector<RnObject*> attrs;
    auto target_scope = args.front()->ToObject();
    for (const auto& attr : target_scope->GetSymbolTable()->GetSymbols()) {
        auto pair_obj =
            dynamic_cast<RnArrayObject*>(RnObject::Create(RnType::RN_ARRAY));
        std::vector<RnObject*> data = {
            RnObject::Create(RnObject::GetInternedString(attr)),
            target_scope->GetObject(attr)};
        pair_obj->SetData(data);
        attrs.push_back(pair_obj);
    }
    ret_val->SetData(attrs);
}

/*****************************************************************************/
 void RnBuiltins::rn_builtin_hasattr(RnScope* scope, const std::vector<RnObject*>& args,
                               RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    auto obj = args[0]->ToObject();
    auto attr_key = RnObject::InternValue(args[1]->ToString());
    ret_val->SetData(obj->GetSymbolTable()->SymbolExists(attr_key));
 }

 /*****************************************************************************/
 void RnBuiltins::rn_builtin_getattr(RnScope* scope, const std::vector<RnObject*>& args,
                               RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    auto obj = args[0]->ToObject();
    auto attr_key = RnObject::InternValue(args[1]->ToString());
    if (obj->GetSymbolTable()->SymbolExists(attr_key)) {
        auto original = obj->GetObject(attr_key);
        auto result = RnObject::Create(original->GetType());
        result->CopyDataFromObject(original);
        std::vector<RnObject*> array_data = {result};
        ret_val->SetData(array_data);
    }
 }

 /*****************************************************************************/
 void RnBuiltins::rn_builtin_setattr(RnScope* scope, const std::vector<RnObject*>& args,
                               RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    auto obj = args[0]->ToObject();
    auto attr_key = RnObject::InternValue(args[1]->ToString());
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
 void RnBuiltins::rn_builtin_delattr(RnScope* scope, const std::vector<RnObject*>& args,
                               RnObject* ret_val) {
    assert(ret_val);
    assert(scope);

    auto obj = args[0]->ToObject();
    auto attr_key = RnObject::InternValue(args[1]->ToString());
    if (obj->GetSymbolTable()->SymbolExists(attr_key)) {
        obj->RemoveObject(attr_key);
        ret_val->SetData(true);
    } else {
        ret_val->SetData(false);
    }
 }