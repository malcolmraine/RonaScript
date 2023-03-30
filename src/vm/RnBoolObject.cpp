/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "RnBoolObject.h"

/*****************************************************************************/
RnBoolObject::RnBoolObject(RnBoolNative data) {
    _data.i_data = data ? 1 : 0;
}

/*****************************************************************************/
RnBoolObject::~RnBoolObject() = default;

/*****************************************************************************/
RnObject* RnBoolObject::operator==(RnObject* obj) {
    return RnObject::Create(
        static_cast<bool>(static_cast<RnBoolNative>(_data.i_data) == obj->ToBool()));
}

/*****************************************************************************/
RnObject* RnBoolObject::operator!=(RnObject* obj) {
    return RnObject::Create(
        static_cast<bool>(static_cast<RnBoolNative>(_data.i_data) != obj->ToBool()));
}

/*****************************************************************************/
RnObject* RnBoolObject::operator|(RnObject* obj) {
    return RnObject::Create(
        static_cast<bool>(static_cast<RnBoolNative>(_data.i_data) | obj->ToBool()));
}

/*****************************************************************************/
RnObject* RnBoolObject::operator||(RnObject* obj) {
    return RnObject::Create(
        static_cast<bool>(static_cast<RnBoolNative>(_data.i_data) || obj->ToBool()));
}

/*****************************************************************************/
RnObject* RnBoolObject::operator&&(RnObject* obj) {
    return RnObject::Create(
        static_cast<bool>(static_cast<RnBoolNative>(_data.i_data) && obj->ToBool()));
}

/*****************************************************************************/
RnObject* RnBoolObject::operator&(RnObject* obj) {
    return RnObject::Create(
        static_cast<bool>(static_cast<RnBoolNative>(_data.i_data) & obj->ToBool()));
}

/*****************************************************************************/
RnObject* RnBoolObject::operator^(RnObject* obj) {
    return RnObject::Create(
        static_cast<bool>(static_cast<RnBoolNative>(_data.i_data) ^ obj->ToBool()));
}

/*****************************************************************************/
RnStringNative RnBoolObject::ToString() const {
    return static_cast<RnBoolNative>(_data.i_data) ? "true" : "false";
}

/*****************************************************************************/
RnIntNative RnBoolObject::ToInt() const {
    return static_cast<RnBoolNative>(_data.i_data) ? 1 : 0;
}

/*****************************************************************************/
RnBoolNative RnBoolObject::ToBool() const {
    return static_cast<RnBoolNative>(_data.i_data);
}

/*****************************************************************************/
RnFloatNative RnBoolObject::ToFloat() const {
    return static_cast<RnBoolNative>(_data.i_data) ? 1.0 : 0.0;
}
