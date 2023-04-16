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