/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
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

#include "RnClassObject.h"
#include <iomanip>
#include <sstream>
#include "../common/RnInternment.h"
#include "../vm/RnFunction.h"
#include "RnVirtualMachine.h"

RnIntNative RnClassObject::MAGIC_METHOD_KEY_STR =
    RnConstStore::InternValue(std::string("__str"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_INT =
    RnConstStore::InternValue(std::string("__int"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_FLOAT =
    RnConstStore::InternValue(std::string("__float"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_ARRAY =
    RnConstStore::InternValue(std::string("__array"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_BOOL =
    RnConstStore::InternValue(std::string("__bool"));

/*****************************************************************************/
RnClassObject::RnClassObject() {
    _data = RnVirtualMachine::GetInstance()->CreateScope();
}

/*****************************************************************************/
RnClassObject::RnClassObject(RnScope* data) {
    _data = data;
}

/*****************************************************************************/
RnClassObject::~RnClassObject() = default;

/*****************************************************************************/
auto RnClassObject::ToString() const -> RnStringNative {
    std::stringstream s;
    s << "{class object @ " << std::hex << _data << "}";
    return {s.str()};
}

/*****************************************************************************/
void RnClassObject::CopySymbols(RnScope* target) {
    auto symbol_table = _data->GetSymbolTable();
    auto target_symbol_table = target->GetSymbolTable();

    for (const auto& symbol : symbol_table->GetSymbols()) {
        auto obj = symbol_table->GetObject(symbol);
        auto newObj = RnObject::Create(obj->GetType());
        newObj->CopyDataFromObject(obj);
        target_symbol_table->SetSymbol(symbol, newObj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::CallFunction(RnIntNative key, RnArrayNative args) {
    auto func_obj = _data->GetObject(key);
    if (func_obj) {
        auto func = func_obj->ToFunction();
        RnVirtualMachine::BindThis(func->GetScope(), this);
        return RnVirtualMachine::GetInstance()->CallFunction(func, args);
    }
    throw std::runtime_error("Calling non-existent function '" +
                             RnConstStore::GetInternedString(key) +
                             "' on instance of class " + GetName());
}

/*****************************************************************************/
RnBoolNative RnClassObject::HasSymbol(RnIntNative key) {
    return _data->GetObject(key) != nullptr;
}

/*****************************************************************************/
bool RnClassObject::TryMagicMethod(RnIntNative key, RnArrayNative args, RnObject* ret_val) {
    if (HasSymbol(key)) {
        auto func_ret_val = CallFunction(key, {});
        ret_val->CopyDataFromObject(func_ret_val);
        return true;
    }
    return false;
}