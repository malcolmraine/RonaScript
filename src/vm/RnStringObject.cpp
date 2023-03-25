/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "RnStringObject.h"
#include <utility>

/*****************************************************************************/
RnStringObject::RnStringObject(const RnStringNative& data) {
    _data = std::move(data);
}

/*****************************************************************************/
RnStringObject::~RnStringObject() = default;

/*****************************************************************************/
RnObject* RnStringObject::operator+(RnObject* obj) {
    return RnObject::Create(_data + obj->ToString());
}

/*****************************************************************************/
RnObject* RnStringObject::operator==(RnObject* obj) {
    return RnObject::Create(static_cast<bool>(_data == obj->ToString()));
}

/*****************************************************************************/
RnObject* RnStringObject::operator!=(RnObject* obj) {
    return RnObject::Create(static_cast<bool>(_data != obj->ToString()));
}

/*****************************************************************************/
RnObject* RnStringObject::operator*(RnObject* obj) {
    std::string result;
    for (RnIntNative i = 0; i < obj->ToInt(); i++) {
        result += _data;
    }
    return RnObject::Create(result);
}

/*****************************************************************************/
RnStringNative RnStringObject::ToString() const {
    return _data;
}

/*****************************************************************************/
RnBoolNative RnStringObject::ToBool() const {
    return !_data.empty();
}

/*************************************************************************/
void RnStringObject::SetData(RnStringNative data) {
    SetDataInternal(data);
}