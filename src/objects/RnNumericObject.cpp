/*****************************************************************************
* File: RnNumericObject.cpp
* Description:
* Author: Malcolm Hall
* Date: 3/29/23
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

#include "RnNumericObject.h"

/*****************************************************************************/
RnIntNative RnNumericObject::ToInt() const {
    return _data.i_data;
}

/*****************************************************************************/
RnFloatNative RnNumericObject::ToFloat() const {
    return _data.d_data;
}

/*****************************************************************************/
RnBoolNative RnNumericObject::ToBool() const {
    return static_cast<RnBoolNative>(_data.i_data);
}

/*****************************************************************************/
void RnNumericObject::SetData(RnIntNative data) {
    ConstInitCheck();
    _data.i_data = data;
}

/*****************************************************************************/
void RnNumericObject::SetData(RnFloatNative data) {
    ConstInitCheck();
    _data.d_data = data;
}

/*****************************************************************************/
void RnNumericObject::SetData(RnBoolNative data) {
    ConstInitCheck();
    _data.i_data = data ? 1 : 0;
}

/*****************************************************************************/
size_t RnNumericObject::GetByteSize() const {
    // type = 1 byte
    // size = 4 bytes
    // data = 8 bytes
    return RN_TYPE_BYTES_LENGTH + RN_SIZE_BYTES_LENGTH + RN_NUMERIC_DATA_LENGTH;
}

/*****************************************************************************/
size_t RnNumericObject::GetBytes(char* buf) {
    size_t i = 0;
    buf[i++] = static_cast<char>(GetType());
    union {
        char bytes[RN_SIZE_BYTES_LENGTH];
        size_t len = 0;
    } size_bytes;
    size_bytes.len = RN_NUMERIC_DATA_LENGTH;
    for (char byte : size_bytes.bytes) {
        buf[i++] = byte;
    }

    for (size_t n = 0; i < RN_NUMERIC_DATA_LENGTH; i++) {
        buf[i++] = _data.c_data[n];
    }
    return RN_NUMERIC_DATA_LENGTH;
}

/*****************************************************************************/
void RnNumericObject::SetBytes(const char* buf, size_t n) {
    for (size_t i = 0; i < n; i++) {
        _data.c_data[i] = buf[i];
    }
}
