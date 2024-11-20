/*****************************************************************************
* File: RnScope.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
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

#include "RnScope.h"
#include <dlfcn.h>
#include "../objects/RnAnyObject.h"
#include "../objects/RnArrayObject.h"
#include "../objects/RnBoolObject.h"
#include "../objects/RnClassObject.h"
#include "../objects/RnFloatObject.h"
#include "../objects/RnFunctionObject.h"
#include "../objects/RnIntObject.h"
#include "../objects/RnObject.h"
#include "../objects/RnStringObject.h"
#include "../util/log.h"
#include "RnFunction.h"
#include "RnMemoryManager.h"
#include "RnSymbolTable.h"

std::unordered_map<RnStringNative, void*> RnScope::_handles;

/*****************************************************************************/
RnScope::RnScope(RnScope* parent) : _memory_group(nullptr) {
    _parent = parent;
    if (parent) {
        _symbolTable.SetParent(parent->GetSymbolTable());
        _memory_group.SetParent(parent->GetMemoryGroup());
    }
}

/*****************************************************************************/
RnScope::~RnScope() {
    if (_memory_group.GetParent()) {
        _memory_group.GetParent()->RemoveChildGroup(&_memory_group);
    }
}

/*****************************************************************************/
void RnScope::StoreObject(RnIntNative key, RnObject* value) {
    _symbolTable.SetSymbol(key, value);
}

/*****************************************************************************/
RnObject* RnScope::GetObject(RnIntNative key) {
    if (key == UINT32_MAX) {
        return RnObject::Create(RnType::RN_NULL);
    }
    return _symbolTable.GetObject(key);
}

/*****************************************************************************/
void RnScope::RemoveObject(RnIntNative key) {
    _symbolTable.RemoveSymbol(key);
}

/*****************************************************************************/
RnSymbolTable* RnScope::GetSymbolTable() {
    return &_symbolTable;
}

/*****************************************************************************/
void RnScope::SetParent(RnScope* scope) {
    _parent = scope;
    if (_parent) {
        _symbolTable.SetParent(_parent->GetSymbolTable());
        _memory_group.SetParent(_parent->GetMemoryGroup());
    }
}

/*****************************************************************************/
RnScope* RnScope::GetParent() const {
    return _parent;
}

/*****************************************************************************/
RnMemoryGroup* RnScope::GetMemoryGroup() {
    return &_memory_group;
}

/*****************************************************************************/
bool RnScope::LoadLibraryIntoScope(RnScope* scope, const RnStringNative& library,
                                   bool add_data) {
    // This function should really be moved somewhere more appropriate and
    // should do something other than just load the names into the parent scope.
    // It would be ideal to return an object containing the scope
    void* handle = dlopen(library.c_str(), RTLD_LOCAL);
    _handles[library] = handle;

    if (handle) {
        //		void (* ListExports)(
        //			std::vector<std::tuple<RnStringNative, RnType::Type>>&) = nullptr;
        auto ListExports =
            (void (*)(std::vector<std::tuple<RnStringNative, RnType::Type>>&))dlsym(
                handle, "LibraryFunctions");

        auto LibraryName = (const char* (*)())dlsym(handle, "LibraryName");
        auto LibraryVersion = (const char* (*)())dlsym(handle, "LibraryVersion");
        std::vector<std::tuple<RnStringNative, RnType::Type>> functions;
        ListExports(functions);
        RnArrayNative function_names;
        function_names.reserve(functions.size());
        for (const auto& info : functions) {
            function_names.push_back(RnObject::Create(std::get<0>(info)));
            auto name = std::get<0>(info);
            Log::DEBUG("Loading external function: " + std::get<0>(info));

            auto func =
                new RnBuiltinFunction(name, CastToBuiltin(dlsym(handle, name.c_str())));
            func->SetScope(scope);
            func->SetReturnType(std::get<1>(info));
            auto obj =
                dynamic_cast<RnFunctionObject*>(RnObject::Create(RnType::RN_FUNCTION));
            obj->SetData(func);
            scope->StoreObject(RnConstStore::InternValue(name), obj);
        }

        if (add_data) {
            auto name_list = RnObject::Create(RnType::RN_ARRAY);
            name_list->SetData(function_names);
            scope->StoreObject(RnConstStore::InternValue("__exports__"), name_list);

            auto name_obj = RnObject::Create(RnType::RN_STRING);
            name_obj->SetData(static_cast<RnStringNative>(LibraryName()));
            scope->StoreObject(RnConstStore::InternValue("__name__"), name_obj);

            auto version_obj = RnObject::Create(RnType::RN_STRING);
            version_obj->SetData(static_cast<RnStringNative>(LibraryVersion()));
            scope->StoreObject(RnConstStore::InternValue("__version__"), version_obj);
        }
    } else {
        Log::DEBUG("Unable to load external library: " + library);
        return false;
    }
    return true;
}

/*****************************************************************************/
RnObject* RnScope::MakeLocal(RnType::Type type) {
    switch (type) {
        case RnType::RN_BOOLEAN:
            _locals.emplace_back(RnBoolObject());
            return &std::get<RnBoolObject>(_locals.back());
        case RnType::RN_STRING:
            _locals.emplace_back(RnStringObject());
            return &std::get<RnStringObject>(_locals.back());
        case RnType::RN_FLOAT:
            _locals.emplace_back(RnFloatObject());
            return &std::get<RnFloatObject>(_locals.back());
        case RnType::RN_INT:
            _locals.emplace_back(RnIntObject());
            return &std::get<RnIntObject>(_locals.back());
        case RnType::RN_ANY:
            _locals.emplace_back(RnAnyObject());
            return &std::get<RnAnyObject>(_locals.back());
        case RnType::RN_ARRAY:
            _locals.emplace_back(RnArrayObject());
            return &std::get<RnArrayObject>(_locals.back());
        case RnType::RN_FUNCTION:
        case RnType::RN_CALLABLE:
            _locals.emplace_back(RnFunctionObject());
            return &std::get<RnFunctionObject>(_locals.back());
        case RnType::RN_CLASS_INSTANCE:
        case RnType::RN_OBJECT:
            _locals.emplace_back(RnClassObject());
            return &std::get<RnClassObject>(_locals.back());
        case RnType::RN_NULL:
        case RnType::RN_VOID:
        case RnType::RN_UNKNOWN:
        default:
            break;
    }
    return nullptr;
}

/*****************************************************************************/
void RnScope::Reset() {
    GetSymbolTable()->Clear();
    GetMemoryGroup()->Clear();
    _locals.clear();
}
