/*****************************************************************************
* File: RnAnyObject.cpp
* Description:
* Author: Malcolm Hall
* Date: 3/22/23
* Version: 1
*
*****************************************************************************/

#include "RnAnyObject.h"
#include "RnArrayObject.h"
#include "RnBoolObject.h"
#include "RnClassObject.h"
#include "RnFloatObject.h"
#include "RnFunctionObject.h"
#include "RnIntObject.h"
#include "RnNullObject.h"
#include "RnStringObject.h"

/*****************************************************************************/
RnAnyObject::RnAnyObject(RnIntNative data) {}

/*****************************************************************************/
RnAnyObject::~RnAnyObject() = default;

/*****************************************************************************/
RnObject* RnAnyObject::operator+(RnObject* obj) {
    return RnObjectBase::operator+(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator-(RnObject* obj) {
    return RnObjectBase::operator-(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator==(RnObject* obj) {
    return RnObjectBase::operator==(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator!=(RnObject* obj) {
    return RnObjectBase::operator!=(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator/(RnObject* obj) {
    return RnObjectBase::operator/(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator|(RnObject* obj) {
    return RnObjectBase::operator|(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator||(RnObject* obj) {
    return RnObjectBase::operator||(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator&&(RnObject* obj) {
    return RnObjectBase::operator&&(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator&(RnObject* obj) {
    return RnObjectBase::operator&(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator^(RnObject* obj) {
    return RnObjectBase::operator^(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator>(RnObject* obj) {
    return RnObjectBase::operator>(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator<(RnObject* obj) {
    return RnObjectBase::operator<(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator>=(RnObject* obj) {
    return RnObjectBase::operator>=(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator<=(RnObject* obj) {
    return RnObjectBase::operator<=(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator%(RnObject* obj) {
    return RnObjectBase::operator%(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator>>(RnObject* obj) {
    return RnObjectBase::operator>>(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator<<(RnObject* obj) {
    return RnObjectBase::operator<<(obj);
}

/*****************************************************************************/
RnObject* RnAnyObject::operator*(RnObject* obj) {
    return RnObjectBase::operator*(obj);
}

/*****************************************************************************/
RnIntNative RnAnyObject::ToInt() const {
    return std::get<RnIntNative>(_data);
}

/*****************************************************************************/
RnFloatNative RnAnyObject::ToFloat() const {
    return std::get<RnFloatNative>(_data);
}

/*****************************************************************************/
RnStringNative RnAnyObject::ToString() const {
    switch (_active_type) {
        case RnType::RN_BOOLEAN:
            return RnBoolObject(ToBool()).ToString();
        case RnType::RN_STRING:
            return std::get<RnStringNative>(_data);
        case RnType::RN_FLOAT:
            return RnFloatObject(ToFloat()).ToString();
        case RnType::RN_INT:
            return RnIntObject(ToInt()).ToString();
        case RnType::RN_ARRAY:
            return RnArrayObject(ToArray()).ToString();
        case RnType::RN_FUNCTION:
        case RnType::RN_CALLABLE:
            return RnFunctionObject(ToFunction()).ToString();
        case RnType::RN_CLASS_INSTANCE:
        case RnType::RN_OBJECT:
            return RnClassObject(ToObject()).ToString();
        case RnType::RN_NULL:
            return "null";
        case RnType::RN_VOID:
        case RnType::RN_UNKNOWN:
        default:
            assert(false);
            return "";
    }
}

/*****************************************************************************/
std::vector<RnObject*> RnAnyObject::ToArray() const {
    return std::get<std::vector<RnObject*>>(_data);
}

/*****************************************************************************/
RnBoolNative RnAnyObject::ToBool() const {
    return std::get<RnBoolNative>(_data);
}

/*****************************************************************************/
RnScope* RnAnyObject::ToObject() const {
    return std::get<RnScope*>(_data);
}

/*****************************************************************************/
RnFunction* RnAnyObject::ToFunction() const {
    return std::get<RnFunction*>(_data);
}

/*****************************************************************************/
void RnAnyObject::SetData(RnIntNative data) {
    _data = data;
    _active_type = RnType::RN_INT;
}

/*****************************************************************************/
void RnAnyObject::SetData(RnBoolNative data) {
    _data = data;
    _active_type = RnType::RN_BOOLEAN;
}

/*****************************************************************************/
void RnAnyObject::SetData(RnFloatNative data) {
    _data = data;
    _active_type = RnType::RN_FLOAT;
}

/*****************************************************************************/
void RnAnyObject::SetData(std::vector<RnObject*> data) {
    _data = data;
    _active_type = RnType::RN_ARRAY;
}

/*****************************************************************************/
void RnAnyObject::SetData(RnStringNative data) {
    _data = data;
    _active_type = RnType::RN_STRING;
}

/*****************************************************************************/
void RnAnyObject::SetData(RnFunction* data) {
    _data = data;
    _active_type = RnType::RN_CALLABLE;
}

/*****************************************************************************/
void RnAnyObject::SetData(RnScope* data) {
    _data = data;
    _active_type = RnType::RN_OBJECT;
}

/*****************************************************************************/
void RnAnyObject::CopyFrom(RnObject* obj) {
    auto type = dynamic_cast<RnAnyObject*>(obj)->GetActiveType();
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
