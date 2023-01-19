/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "RnNullObject.h"

RnNullObject* instance = nullptr;

/*****************************************************************************/
auto RnNullObject::Instance() -> RnNullObject*
{
	if (!instance)
	{
		instance = new RnNullObject();
	}
	return instance;
}

/*****************************************************************************/
RnNullObject::RnNullObject() = default;

/*****************************************************************************/
RnNullObject::~RnNullObject() = default;

/*****************************************************************************/
auto RnNullObject::ToString() const -> RnStringNative
{
	return "null";
}

/*****************************************************************************/
auto RnNullObject::ToBool() const -> RnBoolNative
{
	return false;
}

/*****************************************************************************/
auto RnNullObject::ToFloat() const -> RnFloatNative
{
	return 0.0;
}

/*****************************************************************************/
auto RnNullObject::ToInt() const -> RnIntNative
{
	return 0;
}