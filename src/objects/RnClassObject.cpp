/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
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

#include "RnClassObject.h"
#include <iomanip>
#include <sstream>
#include "../vm/RnFunction.h"
#include "../vm/RnVirtualMachine.h"
#include "RnIntObject.h"

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
RnIntNative RnClassObject::MAGIC_METHOD_KEY_PLUS =
    RnConstStore::InternValue(std::string("__plus"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_MINUS =
    RnConstStore::InternValue(std::string("__minus"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_MOD =
    RnConstStore::InternValue(std::string("__mod"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_EQ =
    RnConstStore::InternValue(std::string("__eq"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_NEQ =
    RnConstStore::InternValue(std::string("__neq"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_DIV =
    RnConstStore::InternValue(std::string("__div"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_OR =
    RnConstStore::InternValue(std::string("__or"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_AND =
    RnConstStore::InternValue(std::string("__and"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_XOR =
    RnConstStore::InternValue(std::string("__xor"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_GT =
    RnConstStore::InternValue(std::string("__gt"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_LT =
    RnConstStore::InternValue(std::string("__lt"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_GEQ =
    RnConstStore::InternValue(std::string("__geq"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_LEQ =
    RnConstStore::InternValue(std::string("__leq"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_RSHIFT =
    RnConstStore::InternValue(std::string("__rshift"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_LSHIFT =
    RnConstStore::InternValue(std::string("__lshift"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_MUL =
    RnConstStore::InternValue(std::string("__mul"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_BINAND =
    RnConstStore::InternValue(std::string("__binand"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_BINOR =
    RnConstStore::InternValue(std::string("__binor"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_GET_INDEX =
    RnConstStore::InternValue(std::string("__getindex"));
RnIntNative RnClassObject::MAGIC_METHOD_KEY_MEMBERSHIP =
    RnConstStore::InternValue(std::string("__in"));

/*****************************************************************************/
RnClassObject::RnClassObject() {
    _data = nullptr;
}

/*****************************************************************************/
RnClassObject::RnClassObject(RnScope* data) {
    _data = data;
}

/*****************************************************************************/
RnClassObject::~RnClassObject() = default;

/*****************************************************************************/
auto RnClassObject::ToString() const -> RnStringNative {
    if (!_data) {
        return "null";
    } else {
        std::stringstream s;
        s << "{class object @ " << std::hex << _data << "}";
        return s.str();
    }
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
RnObject* RnClassObject::CallFunction(RnIntNative key, const RnArrayNative& args) {
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
bool RnClassObject::TryMagicMethod(RnIntNative key, const RnArrayNative& args,
                                   RnObject* ret_val) {
    if (HasSymbol(key)) {
        auto func_ret_val = CallFunction(key, {});
        ret_val->CopyDataFromObject(func_ret_val);
        return true;
    }
    return false;
}

/*****************************************************************************/
RnObject* RnClassObject::operator*(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_MUL)) {
        return CallFunction(MAGIC_METHOD_KEY_MUL, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator*(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator^(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_XOR)) {
        return CallFunction(MAGIC_METHOD_KEY_XOR, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator^(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator%(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_MOD)) {
        return CallFunction(MAGIC_METHOD_KEY_MOD, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator%(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator+(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_PLUS)) {
        return CallFunction(MAGIC_METHOD_KEY_PLUS, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator+(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator-(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_MINUS)) {
        return CallFunction(MAGIC_METHOD_KEY_MINUS, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator-(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator==(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_EQ)) {
        return CallFunction(MAGIC_METHOD_KEY_EQ, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator==(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator!=(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_NEQ)) {
        return CallFunction(MAGIC_METHOD_KEY_NEQ, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator!=(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator/(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_DIV)) {
        return CallFunction(MAGIC_METHOD_KEY_DIV, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator/(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator|(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_BINOR)) {
        return CallFunction(MAGIC_METHOD_KEY_BINOR, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator|(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator||(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_OR)) {
        return CallFunction(MAGIC_METHOD_KEY_OR, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator||(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator&&(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_AND)) {
        return CallFunction(MAGIC_METHOD_KEY_AND, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator&&(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator&(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_BINAND)) {
        return CallFunction(MAGIC_METHOD_KEY_BINAND, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator*(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator>(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_GT)) {
        return CallFunction(MAGIC_METHOD_KEY_GT, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator>(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator<(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_LT)) {
        return CallFunction(MAGIC_METHOD_KEY_LT, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator<(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator>=(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_GEQ)) {
        return CallFunction(MAGIC_METHOD_KEY_GEQ, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator>=(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator<=(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_LEQ)) {
        return CallFunction(MAGIC_METHOD_KEY_LEQ, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator<=(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator>>(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_RSHIFT)) {
        return CallFunction(MAGIC_METHOD_KEY_RSHIFT, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator>>(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::operator<<(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_LSHIFT)) {
        return CallFunction(MAGIC_METHOD_KEY_LSHIFT, {obj});
    } else {
        return RnObjectBase<RnScope*>::operator<<(obj);
    }
}

/*****************************************************************************/
RnObject* RnClassObject::At(RnIntNative index) {
    if (HasSymbol(MAGIC_METHOD_KEY_GET_INDEX)) {
        RnIntObject indexObject;
        indexObject.SetData(index);
        return CallFunction(MAGIC_METHOD_KEY_GET_INDEX, {&indexObject});
    } else {
        return RnObjectBase<RnScope*>::At(index);
    }
}

/*****************************************************************************/
RnBoolNative RnClassObject::Contains(RnObject* obj) {
    if (HasSymbol(MAGIC_METHOD_KEY_MEMBERSHIP)) {
        return CallFunction(MAGIC_METHOD_KEY_MEMBERSHIP, {obj})->ToBool();
    } else {
        ThrowUndefinedOperatorError("in", this, obj);
    }
}

/*****************************************************************************/
void RnClassObject::CopyDataFromObject(RnObject* obj) {
    if (obj->GetActiveType() == RnType::RN_CLASS_INSTANCE ||
        obj->GetActiveType() == RnType::RN_OBJECT) {
        SetDefinition(dynamic_cast<RnClassObject*>(obj)->GetDefinition());
    }
    RnObjectBase<RnScope*>::CopyDataFromObject(obj);
}

/*****************************************************************************/
void RnClassObject::SetNull() {
    SetName("null");
    _data = nullptr;
}

/*****************************************************************************/
std::string RnClassObject::GetTypeName() const {
    return GetName();
}