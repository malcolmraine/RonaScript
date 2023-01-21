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
RnBoolObject::RnBoolObject(RnBoolNative data)
{
	_data = data;
}

/*****************************************************************************/
RnBoolObject::~RnBoolObject() = default;

/*****************************************************************************/
RnObject* RnBoolObject::operator==(RnObject* obj)
{
	return RnObject::Create(static_cast<bool>(_data == obj->ToBool()));
}

/*****************************************************************************/
RnObject* RnBoolObject::operator!=(RnObject* obj)
{
	return RnObject::Create(static_cast<bool>(_data != obj->ToBool()));
}

/*****************************************************************************/
RnObject* RnBoolObject::operator|(RnObject* obj)
{
	return RnObject::Create(static_cast<bool>(_data | obj->ToBool()));
}

/*****************************************************************************/
RnObject* RnBoolObject::operator||(RnObject* obj)
{
	return RnObject::Create(static_cast<bool>(_data || obj->ToBool()));
}

/*****************************************************************************/
RnObject* RnBoolObject::operator&&(RnObject* obj)
{
	return RnObject::Create(static_cast<bool>(_data && obj->ToBool()));
}

/*****************************************************************************/
RnObject* RnBoolObject::operator&(RnObject* obj)
{
	return RnObject::Create(static_cast<bool>(_data & obj->ToBool()));
}

/*****************************************************************************/
RnObject* RnBoolObject::operator^(RnObject* obj)
{
	return RnObject::Create(static_cast<bool>(_data ^ obj->ToBool()));
}

/*****************************************************************************/
RnStringNative RnBoolObject::ToString() const
{
	return _data ? "true" : "false";
}

/*****************************************************************************/
RnIntNative RnBoolObject::ToInt() const
{
	return _data ? 1 : 0;
}

/*****************************************************************************/
RnBoolNative RnBoolObject::ToBool() const
{
	return _data;
}
