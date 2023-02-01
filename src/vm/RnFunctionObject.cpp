/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "RnFunctionObject.h"
#include "RnFunction.h"
#include <sstream>

/*****************************************************************************/
RnFunctionObject::RnFunctionObject() = default;

/*****************************************************************************/
RnFunctionObject::~RnFunctionObject() = default;

/*****************************************************************************/
RnStringNative RnFunctionObject::ToString() const
{
	std::stringstream s;
	s << "{routine @ " << std::hex << _data << "}";
	return s.str();
}

/*****************************************************************************/
void RnFunctionObject::SetData(RnFunction* data)
{
	SetDataInternal(data);
}
