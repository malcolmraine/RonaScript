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

/*************************************************************************/
size_t RnStringObject::GetByteSize() const {
    // type - 1 byte
    // length = 4 bytes
    // data - length * 1 byte
    return 5 + _data.length();
}

/*************************************************************************/
size_t RnStringObject::GetBytes(char* buf) {
    // This includes the null terminating character. This is important when
    // reading the data back into a string object.
    size_t i = 0;
    union {
        char bytes[4];
        size_t len = 0;
    } size_bytes;
    size_bytes.len = _data.size() + 1;
    buf[i++] = static_cast<char>(RnType::RN_STRING);
    buf[i++] = size_bytes.bytes[0];
    buf[i++] = size_bytes.bytes[1];
    buf[i++] = size_bytes.bytes[2];
    buf[i++] = size_bytes.bytes[3];
    for (auto c : _data) {
        buf[i++] = c;
    }
    buf[i++] = '\0';
    return i;
}

/*************************************************************************/
void RnStringObject::SetBytes(const char* buf, size_t n) {
    _data = std::string(buf + 5);
}
