/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "RnFunctionObject.h"
#include <sstream>
#include "RnFunction.h"

/*****************************************************************************/
RnFunctionObject::RnFunctionObject() = default;

/*****************************************************************************/
RnFunctionObject::RnFunctionObject(RnFunction* data) {
    _data = data;
}

/*****************************************************************************/
RnFunctionObject::~RnFunctionObject() = default;

/*****************************************************************************/
RnStringNative RnFunctionObject::ToString() const {
    std::stringstream s;
    s << "{routine @ " << std::hex << _data << "}";
    return s.str();
}

/*****************************************************************************/
void RnFunctionObject::SetData(RnFunction* data) {
    SetDataInternal(data);
}
