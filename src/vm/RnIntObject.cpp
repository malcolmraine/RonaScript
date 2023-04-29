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

#include "RnIntObject.h"

/*****************************************************************************/
RnIntObject::RnIntObject(RnIntNative data) {
    _data.i_data = data;
}

/*****************************************************************************/
RnIntObject::~RnIntObject() = default;

/*****************************************************************************/
RnObject* RnIntObject::operator+(RnObject* obj) {
    switch (obj->GetActiveType()) {
        case RnType::RN_INT:
        case RnType::RN_BOOLEAN:
            return RnObject::Create(_data.i_data + obj->ToInt());
        case RnType::RN_FLOAT:
            return RnObject::Create(_data.i_data + obj->ToFloat());
        case RnType::RN_ARRAY:
        case RnType::RN_FUNCTION:
        case RnType::RN_CLASS_INSTANCE:
        case RnType::RN_NULL:
        case RnType::RN_UNKNOWN:
        case RnType::RN_STRING:
        case RnType::RN_OBJECT:
        default:
            break;
    }
    throw std::runtime_error("Operator '+' is not defined for types '" +
                             RnType::TypeToString(GetType()) + "' and '" +
                             RnType::TypeToString(obj->GetActiveType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator-(RnObject* obj) {
    switch (obj->GetActiveType()) {
        case RnType::RN_INT:
        case RnType::RN_BOOLEAN:
            return RnObject::Create(_data.i_data - obj->ToInt());
        case RnType::RN_FLOAT:
            return RnObject::Create(_data.i_data - obj->ToFloat());
        case RnType::RN_ARRAY:
        case RnType::RN_FUNCTION:
        case RnType::RN_CLASS_INSTANCE:
        case RnType::RN_NULL:
        case RnType::RN_UNKNOWN:
        case RnType::RN_STRING:
        case RnType::RN_OBJECT:
        default:
            break;
    }
    throw std::runtime_error("Operator '-' is not defined for types '" +
                             RnType::TypeToString(GetType()) + "' and '" +
                             RnType::TypeToString(obj->GetActiveType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator==(RnObject* obj) {
    switch (obj->GetActiveType()) {
        case RnType::RN_INT:
        case RnType::RN_BOOLEAN:
            return RnObject::Create(_data.i_data == obj->ToInt());
        case RnType::RN_FLOAT:
            return RnObject::Create(_data.i_data == obj->ToFloat());
        case RnType::RN_ARRAY:
        case RnType::RN_FUNCTION:
        case RnType::RN_CLASS_INSTANCE:
        case RnType::RN_NULL:
        case RnType::RN_UNKNOWN:
        case RnType::RN_STRING:
        case RnType::RN_OBJECT:
        default:
            return RnObject::Create(false);
    }
}

/*****************************************************************************/
RnObject* RnIntObject::operator!=(RnObject* obj) {
    switch (obj->GetActiveType()) {
        case RnType::RN_INT:
        case RnType::RN_BOOLEAN:
            return RnObject::Create(_data.i_data == obj->ToInt());
        case RnType::RN_FLOAT:
            return RnObject::Create(_data.i_data == obj->ToFloat());
        case RnType::RN_ARRAY:
        case RnType::RN_FUNCTION:
        case RnType::RN_CLASS_INSTANCE:
        case RnType::RN_NULL:
        case RnType::RN_UNKNOWN:
        case RnType::RN_STRING:
        case RnType::RN_OBJECT:
        default:
            return RnObject::Create(false);
    }
}

/*****************************************************************************/
RnObject* RnIntObject::operator/(RnObject* obj) {
    switch (obj->GetActiveType()) {
        case RnType::RN_BOOLEAN:
        case RnType::RN_FLOAT:
        case RnType::RN_INT:
            return RnObject::Create(static_cast<RnFloatNative>(_data.i_data) /
                                    obj->ToFloat());
        case RnType::RN_ARRAY:
        case RnType::RN_FUNCTION:
        case RnType::RN_CLASS_INSTANCE:
        case RnType::RN_NULL:
        case RnType::RN_UNKNOWN:
        case RnType::RN_STRING:
        case RnType::RN_OBJECT:
        default:
            break;
    }
    throw std::runtime_error("Operator '/' is not defined for types '" +
                             RnType::TypeToString(GetType()) + "' and '" +
                             RnType::TypeToString(obj->GetActiveType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator^(RnObject* obj) {
    switch (obj->GetActiveType()) {
        case RnType::RN_BOOLEAN:
        case RnType::RN_INT:
            return RnObject::Create(_data.i_data ^ obj->ToInt());
        case RnType::RN_FLOAT:
        case RnType::RN_ARRAY:
        case RnType::RN_FUNCTION:
        case RnType::RN_CLASS_INSTANCE:
        case RnType::RN_NULL:
        case RnType::RN_UNKNOWN:
        case RnType::RN_STRING:
        case RnType::RN_OBJECT:
        default:
            break;
    }
    throw std::runtime_error("Operator '+' is not defined for types '" +
                             RnType::TypeToString(GetType()) + "' and '" +
                             RnType::TypeToString(obj->GetActiveType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator%(RnObject* obj) {
    switch (obj->GetActiveType()) {
        case RnType::RN_INT:
            return RnObject::Create(_data.i_data % obj->ToInt());
        case RnType::RN_FLOAT:
            return RnObject::Create(std::fmod(ToFloat(), obj->ToFloat()));
        default:
            throw std::runtime_error("Operator '%' is not defined for types '" +
                                     RnType::TypeToString(GetType()) + "' and '" +
                                     RnType::TypeToString(obj->GetActiveType()) + "'");
    }
}

/*****************************************************************************/
RnObject* RnIntObject::operator*(RnObject* obj) {
    switch (obj->GetActiveType()) {
        case RnType::RN_INT:
        case RnType::RN_BOOLEAN:
            return RnObject::Create(_data.i_data * obj->ToInt());
        case RnType::RN_FLOAT:
            return RnObject::Create(_data.i_data * obj->ToFloat());
        case RnType::RN_ARRAY:
        case RnType::RN_FUNCTION:
        case RnType::RN_CLASS_INSTANCE:
        case RnType::RN_NULL:
        case RnType::RN_UNKNOWN:
        case RnType::RN_STRING:
        case RnType::RN_OBJECT:
        default:
            break;
    }
    throw std::runtime_error("Operator '*' is not defined for types '" +
                             RnType::TypeToString(GetType()) + "' and '" +
                             RnType::TypeToString(obj->GetActiveType()) + "'");
}
