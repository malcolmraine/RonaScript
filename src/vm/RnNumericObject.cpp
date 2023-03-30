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
