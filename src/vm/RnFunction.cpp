/******************************************************************************
* File: RnFunction.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/29/22
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
*******************************************************************************/

#include "RnFunction.h"
#include <utility>
#include "RnObject.h"
#include "RnScope.h"

/*****************************************************************************/
RnFunction::RnFunction(RnStringNative name, long i_start, long i_cnt) {
    _name = std::move(name);
    _i_start = i_start;
    _i_cnt = i_cnt;
    _argument_scope = new RnScope(nullptr);
}

/*****************************************************************************/
RnFunction::~RnFunction() = default;

/*****************************************************************************/
RnStringNative RnFunction::GetName() const {
    return _name;
}

/*****************************************************************************/
void RnFunction::SetName(const RnStringNative& name) {
    _name = name;
}

/*****************************************************************************/
long RnFunction::GetIStart() const {
    return _i_start;
}

/*****************************************************************************/
long RnFunction::GetICnt() const {
    return _i_cnt;
}

/*****************************************************************************/
RnScope* RnFunction::GetScope() {
    return _scope;
}

/*****************************************************************************/
void RnFunction::SetScope(RnScope* scope) {
    _scope = scope;
}

/*****************************************************************************/
bool RnFunction::IsBuiltIn() const {
    return false;
}

/*****************************************************************************/
void RnFunction::Call(const RnArrayNative& args, RnObject* ret_val) {}

/*****************************************************************************/
void RnFunction::CreateArgument(RnIntNative key, RnType::Type type, size_t index) {
    auto obj = RnObject::Create(type);
    obj->Mark();
    _argument_scope->StoreObject(key, obj);
    _argument_index_map[index] = key;
}

/*****************************************************************************/
void RnFunction::PassArguments(const RnArrayNative& args, RnScope* scope) {
    if (args.size() < _argument_index_map.size()) {
        throw std::runtime_error("Expected " +
                                 std::to_string(_argument_index_map.size()) +
                                 " arguments but was passed " +
                                 std::to_string(args.size()) + ": '" + GetName() + "'");
    } else if (args.size() > _argument_index_map.size()) {
        throw std::runtime_error("Too many arguments passed to function '" + GetName() +
                                 "'");
    } else {
        for (size_t i = 0; i < args.size(); i++) {
            scope->StoreObject(_argument_index_map[i], args[i]);
        }
    }
}

/*****************************************************************************/
void RnFunction::InitScope(RnScope* scope) {
    for (const auto& symbol : _argument_scope->GetSymbolTable()->GetSymbols()) {
        scope->StoreObject(
            symbol, RnObject::Create(_argument_scope->GetObject(symbol)->GetType()));
    }
}

/*****************************************************************************/
/*****************************************************************************/
RnBuiltinFunction::RnBuiltinFunction(const RnStringNative& name, BuiltinFunction func)
    : RnFunction(name, -1, -1), _function(func) {}

/*****************************************************************************/
RnBuiltinFunction::~RnBuiltinFunction() = default;

/*****************************************************************************/
bool RnBuiltinFunction::IsBuiltIn() const {
    return true;
}

/*****************************************************************************/
void RnBuiltinFunction::Call(const RnArrayNative& args, RnObject* ret_val) {
    _function(GetScope(), args, ret_val);
}
