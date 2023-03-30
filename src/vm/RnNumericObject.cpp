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
    // data = 8 bytes
    return RN_NUMERIC_DATA_LENGTH + 1
}

/*****************************************************************************/
size_t RnNumericObject::GetBytes(char* buf) {
    buf[0] = static_cast<char>(RnObjectBase<RnNumericUnion>::GetType());
    for (size_t i = 1; i < RN_NUMERIC_DATA_LENGTH + 1; i++) {
        buf[i] = _data.c_data[i - 1];
    }
    return RN_NUMERIC_DATA_LENGTH;
}

/*****************************************************************************/
void RnNumericObject::SetBytes(const char* buf, size_t n) {
    assert(n == RN_NUMERIC_DATA_LENGTH);
    for (size_t i = 0; i < n; i++) {
        _data.c_data[i] = buf[i];
    }
}
