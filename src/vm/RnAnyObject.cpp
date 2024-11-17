/*****************************************************************************
* File: RnAnyObject.cpp
* Description:
* Author: Malcolm Hall
* Date: 3/22/23
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
*****************************************************************************/

#include "RnAnyObject.h"
#include "RnArrayObject.h"
#include "RnBoolObject.h"
#include "RnClassObject.h"
#include "RnFloatObject.h"
#include "RnFunctionObject.h"
#include "RnIntObject.h"
#include "RnStringObject.h"

/*****************************************************************************/
RnAnyObject::RnAnyObject() {
    _data = RnIntObject(0);
    _active_type = RnType::RN_INT;
}

/*****************************************************************************/
RnAnyObject::RnAnyObject(RnFloatNative data) {
    _data = RnFloatObject(data);
    _active_type = RnType::RN_FLOAT;
}

/*****************************************************************************/
RnAnyObject::RnAnyObject(RnBoolNative data) {
    _data = RnBoolObject(data);
    _active_type = RnType::RN_BOOLEAN;
}

/*****************************************************************************/
RnAnyObject::RnAnyObject(RnStringNative data) {
    _data = RnStringObject(data);
    _active_type = RnType::RN_STRING;
}

/*****************************************************************************/
RnAnyObject::RnAnyObject(RnFunction* data) {
    _data = RnFunctionObject(data);
    _active_type = RnType::RN_CALLABLE;
}

/*****************************************************************************/
RnAnyObject::RnAnyObject(RnScope* data) {
    _data = RnClassObject(data);
    _active_type = RnType::RN_OBJECT;
}

/*****************************************************************************/
RnAnyObject::RnAnyObject(const RnArrayNative& data) {
    _data = RnArrayObject();
    _active_type = RnType::RN_ARRAY;
}

/*****************************************************************************/
RnAnyObject::~RnAnyObject() = default;

#define ANY_BINARY_OP(op)                                    \
    switch (_active_type) {                                  \
        case RnType::RN_BOOLEAN:                             \
            return std::get<RnBoolObject>(_data) op obj;     \
        case RnType::RN_STRING:                              \
            return std::get<RnStringObject>(_data) op obj;   \
        case RnType::RN_FLOAT:                               \
            return std::get<RnFloatObject>(_data) op obj;    \
        case RnType::RN_INT:                                 \
            return std::get<RnIntObject>(_data) op obj;      \
        case RnType::RN_ARRAY:                               \
            return std::get<RnArrayObject>(_data) op obj;    \
        case RnType::RN_FUNCTION:                            \
        case RnType::RN_CALLABLE:                            \
            return std::get<RnFunctionObject>(_data) op obj; \
        case RnType::RN_CLASS_INSTANCE:                      \
        case RnType::RN_OBJECT:                              \
            return std::get<RnClassObject>(_data) op obj;    \
        case RnType::RN_NULL:                                \
        case RnType::RN_VOID:                                \
        case RnType::RN_ANY:                                 \
        case RnType::RN_UNKNOWN:                             \
            assert(false);                                   \
            return nullptr;                                  \
    }

#define ANY_TO_NATIVE(fn, default_value)                   \
    switch (_active_type) {                                \
        case RnType::RN_BOOLEAN:                           \
            return std::get<RnBoolObject>(_data).fn();     \
        case RnType::RN_STRING:                            \
            return std::get<RnStringObject>(_data).fn();   \
        case RnType::RN_FLOAT:                             \
            return std::get<RnFloatObject>(_data).fn();    \
        case RnType::RN_INT:                               \
            return std::get<RnIntObject>(_data).fn();      \
        case RnType::RN_ARRAY:                             \
            return std::get<RnArrayObject>(_data).fn();    \
        case RnType::RN_FUNCTION:                          \
        case RnType::RN_CALLABLE:                          \
            return std::get<RnFunctionObject>(_data).fn(); \
        case RnType::RN_CLASS_INSTANCE:                    \
        case RnType::RN_OBJECT:                            \
            return std::get<RnClassObject>(_data).fn();    \
        case RnType::RN_NULL:                              \
        case RnType::RN_VOID:                              \
        case RnType::RN_ANY:                               \
        case RnType::RN_UNKNOWN:                           \
            assert(false);                                 \
            return default_value;                          \
    }

/*****************************************************************************/
RnObject* RnAnyObject::operator+(RnObject* obj){ANY_BINARY_OP(+)}

/*****************************************************************************/
RnObject* RnAnyObject::operator-(RnObject* obj){ANY_BINARY_OP(-)}

/*****************************************************************************/
RnObject* RnAnyObject::operator==(RnObject* obj){ANY_BINARY_OP(==)}

/*****************************************************************************/
RnObject* RnAnyObject::operator!=(RnObject* obj){ANY_BINARY_OP(!=)}

/*****************************************************************************/
RnObject* RnAnyObject::operator/(RnObject* obj){ANY_BINARY_OP(/)}

/*****************************************************************************/
RnObject* RnAnyObject::operator|(RnObject* obj){ANY_BINARY_OP(|)}

/*****************************************************************************/
RnObject* RnAnyObject::operator||(RnObject* obj){ANY_BINARY_OP(||)}

/*****************************************************************************/
RnObject* RnAnyObject::operator&&(RnObject* obj){ANY_BINARY_OP(&&)}

/*****************************************************************************/
RnObject* RnAnyObject::operator&(RnObject* obj){ANY_BINARY_OP(&)}

/*****************************************************************************/
RnObject* RnAnyObject::operator^(RnObject* obj){ANY_BINARY_OP(^)}

/*****************************************************************************/
RnObject* RnAnyObject::operator>(RnObject* obj){ANY_BINARY_OP(>)}

/*****************************************************************************/
RnObject* RnAnyObject::operator<(RnObject* obj){ANY_BINARY_OP(<)}

/*****************************************************************************/
RnObject* RnAnyObject::operator>=(RnObject* obj){ANY_BINARY_OP(>=)}

/*****************************************************************************/
RnObject* RnAnyObject::operator<=(RnObject* obj){ANY_BINARY_OP(<=)}

/*****************************************************************************/
RnObject* RnAnyObject::operator%(RnObject* obj){ANY_BINARY_OP(%)}

/*****************************************************************************/
RnObject* RnAnyObject::operator>>(RnObject* obj){ANY_BINARY_OP(>>)}

/*****************************************************************************/
RnObject* RnAnyObject::operator<<(RnObject* obj){ANY_BINARY_OP(<<)}

/*****************************************************************************/
RnObject* RnAnyObject::operator*(RnObject* obj){ANY_BINARY_OP(*)}

/*****************************************************************************/
RnIntNative RnAnyObject::ToInt() const {ANY_TO_NATIVE(ToInt, 0)}

/*****************************************************************************/
RnFloatNative RnAnyObject::ToFloat() const {ANY_TO_NATIVE(ToFloat, 0.0)}

/*****************************************************************************/
RnStringNative RnAnyObject::ToString() const {ANY_TO_NATIVE(ToString, "")}

/*****************************************************************************/
RnArrayNative RnAnyObject::ToArray() const {ANY_TO_NATIVE(ToArray, {})}

/*****************************************************************************/
RnBoolNative RnAnyObject::ToBool() const {ANY_TO_NATIVE(ToBool, false)}

/*****************************************************************************/
RnScope* RnAnyObject::ToObject() const {ANY_TO_NATIVE(ToObject, nullptr)}

/*****************************************************************************/
RnFunction* RnAnyObject::ToFunction() const {
    ANY_TO_NATIVE(ToFunction, nullptr)
}

/*****************************************************************************/
void RnAnyObject::SetData(RnIntNative data) {
    _data = RnIntObject(data);
    _active_type = RnType::RN_INT;
}

/*****************************************************************************/
void RnAnyObject::SetData(RnBoolNative data) {
    _data = RnBoolObject(data);
    _active_type = RnType::RN_BOOLEAN;
}

/*****************************************************************************/
void RnAnyObject::SetData(RnFloatNative data) {
    _data = RnFloatObject(data);
    _active_type = RnType::RN_FLOAT;
}

/*****************************************************************************/
void RnAnyObject::SetData(const RnArrayNative& data) {
    _data = RnArrayObject(data);
    _active_type = RnType::RN_ARRAY;
}

/*****************************************************************************/
void RnAnyObject::SetData(RnStringNative data) {
    _data = RnStringObject(data);
    _active_type = RnType::RN_STRING;
}

/*****************************************************************************/
void RnAnyObject::SetData(RnFunction* data) {
    _data = RnFunctionObject(data);
    _active_type = RnType::RN_CALLABLE;
}

/*****************************************************************************/
void RnAnyObject::SetData(RnScope* data) {
    _data = RnClassObject(data);
    _active_type = RnType::RN_OBJECT;
}

/*****************************************************************************/
void RnAnyObject::CopyFrom(RnObject* obj) {
    auto type = obj->GetActiveType();
    _active_type = type;
    switch (type) {
        case RnType::RN_BOOLEAN:
            SetData(obj->ToBool());
            break;
        case RnType::RN_STRING:
            SetData(obj->ToString());
            break;
        case RnType::RN_FLOAT:
            SetData(obj->ToFloat());
            break;
        case RnType::RN_INT:
            SetData(obj->ToInt());
            break;
        case RnType::RN_ARRAY:
            SetData(obj->ToArray());
            break;
        case RnType::RN_FUNCTION:
        case RnType::RN_CALLABLE:
            SetData(obj->ToFunction());
            break;
        case RnType::RN_CLASS_INSTANCE:
        case RnType::RN_OBJECT:
            SetData(obj->ToObject());
            break;
        case RnType::RN_NULL:
        case RnType::RN_UNKNOWN:
        case RnType::RN_VOID:
        case RnType::RN_ANY:
        default:
            assert(false);
            break;
    }
}

/*****************************************************************************/
RnObject* RnAnyObject::At(RnIntNative index) {
    switch (_active_type) {
        case RnType::RN_ARRAY:
            return std::get<RnArrayObject>(_data).At(index);
        case RnType::RN_STRING:
            return std::get<RnStringObject>(_data).At(index);
        default:
            throw std::runtime_error(RnType::TypeToString(GetType()) +
                                     " type does not indices");
    }
}

/*****************************************************************************/
RnBoolNative RnAnyObject::IsActiveDataEqual(const RnObject* obj) const {
    switch (_active_type) {
        case RnType::RN_BOOLEAN: {
            RnBoolObject data = std::get<RnBoolObject>(_data);
            return RnObject::ValueCompare(&data, obj);
        }
        case RnType::RN_STRING: {
            RnStringObject data = std::get<RnStringObject>(_data);
            return RnObject::ValueCompare(&data, obj);
        }
        case RnType::RN_FLOAT: {
            RnFloatObject data = std::get<RnFloatObject>(_data);
            return RnObject::ValueCompare(&data, obj);
        }
        case RnType::RN_INT: {
            RnIntObject data = std::get<RnIntObject>(_data);
            return RnObject::ValueCompare(&data, obj);
        }
        case RnType::RN_ARRAY: {
            RnArrayObject data = std::get<RnArrayObject>(_data);
            return RnObject::ValueCompare(&data, obj);
        }
        case RnType::RN_FUNCTION:
        case RnType::RN_CALLABLE: {
            RnFunctionObject data = std::get<RnFunctionObject>(_data);
            return RnObject::ValueCompare(&data, obj);
        }
        case RnType::RN_CLASS_INSTANCE:
        case RnType::RN_OBJECT: {
            RnClassObject data = std::get<RnClassObject>(_data);
            return RnObject::ValueCompare(&data, obj);
        }
        case RnType::RN_NULL: {
            return obj->GetType() == RnType::RN_NULL;
        }
        case RnType::RN_UNKNOWN:
        case RnType::RN_VOID:
        case RnType::RN_ANY:
        default:
            assert(false);
            break;
    }
}

/*****************************************************************************/
RnBoolNative RnAnyObject::Contains(RnObject* obj) const {
    switch (_active_type) {
        case RnType::RN_ARRAY:
            return std::get<RnArrayObject>(_data).Contains(obj);
        default:
            RnObject::ThrowUndefinedOperatorError("in", this, obj);
    }
}