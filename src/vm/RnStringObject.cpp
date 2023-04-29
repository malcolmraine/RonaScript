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
    return RN_SIZE_BYTES_LENGTH + RN_TYPE_BYTES_LENGTH + _data.length();
}

/*************************************************************************/
size_t RnStringObject::GetBytes(char* buf) {
    // This includes the null terminating character. This is important when
    // reading the data back into a string object.
    size_t i = 0;
    union {
        char bytes[RN_SIZE_BYTES_LENGTH];
        size_t len = 0;
    } size_bytes;
    size_bytes.len = _data.size();
    buf[i++] = static_cast<char>(RnType::RN_STRING);

    for (char byte : size_bytes.bytes) {
        buf[i++] = byte;
    }

    for (auto c : _data) {
        buf[i++] = c;
    }
    return i;
}

/*************************************************************************/
void RnStringObject::SetBytes(const char* buf, size_t n) {
    _data = std::string(buf, n);
}
