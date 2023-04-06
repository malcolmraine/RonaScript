/*****************************************************************************
* File: RnNumericObject.cpp
* Description:
* Author: Malcolm Hall
* Date: 3/29/23
* Version: 1
*
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
    buf[i++] = static_cast<char>(RnObjectBase<RnNumericUnion>::GetType());
    union {
        char bytes[RN_SIZE_BYTES_LENGTH];
        size_t len = 0;
    } size_bytes;
    size_bytes.len = RN_NUMERIC_DATA_LENGTH;
    buf[i++] = size_bytes.bytes[0];
    buf[i++] = size_bytes.bytes[1];
    buf[i++] = size_bytes.bytes[2];
    buf[i++] = size_bytes.bytes[3];

    for (size_t n = 0; i < RN_NUMERIC_DATA_LENGTH; i++) {
        buf[i++] = _data.c_data[n];
    }
    return RN_NUMERIC_DATA_LENGTH;
}

/*****************************************************************************/
void RnNumericObject::SetBytes(const char* buf, size_t n) {
    for (size_t i = RN_TYPE_BYTES_LENGTH + RN_SIZE_BYTES_LENGTH; i < n; i++) {
        _data.c_data[i] = buf[i];
    }
}
