/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
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
