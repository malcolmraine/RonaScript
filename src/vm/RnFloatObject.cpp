/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "RnFloatObject.h"
#include <sstream>
#include <iomanip>

/*****************************************************************************/
RnFloatObject::RnFloatObject(RnFloatNative data)
{
	_data = data;
}

/*****************************************************************************/
RnFloatObject::~RnFloatObject() = default;

/*****************************************************************************/
auto RnFloatObject::operator+(RnObject* obj) -> RnObject*
{
	return RnObject::Create(static_cast<RnFloatNative>(_data + obj->ToFloat()));
}

/*****************************************************************************/
auto RnFloatObject::operator-(RnObject* obj) -> RnObject*
{
	return RnObject::Create(static_cast<RnFloatNative>(_data - obj->ToFloat()));
}

/*****************************************************************************/
auto RnFloatObject::operator==(RnObject* obj) -> RnObject*
{
	return RnObject::Create(static_cast<bool>(_data == obj->ToFloat()));
}

/*****************************************************************************/
auto RnFloatObject::operator!=(RnObject* obj) -> RnObject*
{
	return RnObject::Create(static_cast<bool>(_data != obj->ToFloat()));
}

/*****************************************************************************/
auto RnFloatObject::operator/(RnObject* obj) -> RnObject*
{
	return RnObject::Create(static_cast<RnFloatNative>(_data / obj->ToFloat()));
}

/*****************************************************************************/
auto RnFloatObject::operator||(RnObject* obj) -> RnObject*
{
	return RnObject::Create(static_cast<bool>(ToBool() || obj->ToBool()));
}

/*****************************************************************************/
auto RnFloatObject::operator&&(RnObject* obj) -> RnObject*
{
	return RnObject::Create(static_cast<bool>(ToBool() && obj->ToBool()));
}

/*****************************************************************************/
auto RnFloatObject::operator>(RnObject* obj) -> RnObject*
{
	return RnObject::Create(static_cast<bool>(_data > obj->ToFloat()));
}

/*****************************************************************************/
auto RnFloatObject::operator<(RnObject* obj) -> RnObject*
{
	return RnObject::Create(static_cast<bool>(_data < obj->ToFloat()));
}

/*****************************************************************************/
auto RnFloatObject::operator>=(RnObject* obj) -> RnObject*
{
	return RnObject::Create(static_cast<bool>(_data >= obj->ToFloat()));
}

/*****************************************************************************/
auto RnFloatObject::operator<=(RnObject* obj) -> RnObject*
{
	return RnObject::Create(static_cast<bool>(_data <= obj->ToFloat()));
}

/*****************************************************************************/
auto RnFloatObject::operator*(RnObject* obj) -> RnObject*
{
	return RnObject::Create(static_cast<RnFloatNative>(_data * obj->ToFloat()));
}

/*****************************************************************************/
auto RnFloatObject::ToString() const -> RnStringNative
{
	std::stringstream ss;
	ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
	ss << std::setprecision(std::numeric_limits<float>::digits10) << _data;
	std::string str = ss.str();
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	if (str.ends_with('.'))
	{
		str.append("0");
	}
	return str;
}

/*****************************************************************************/
auto RnFloatObject::ToInt() const -> RnIntNative
{
	return static_cast<RnIntNative>(_data);
}

/*****************************************************************************/
auto RnFloatObject::ToFloat() const -> RnFloatNative
{
	return _data;
}

/*****************************************************************************/
auto RnFloatObject::ToBool() const -> RnBoolNative
{
	return static_cast<bool>(_data);
}

/*****************************************************************************/
void RnFloatObject::SetData(RnIntNative data)
{
	SetDataInternal(static_cast<RnFloatNative >(data));
}

/*****************************************************************************/
void RnFloatObject::SetData(RnFloatNative data)
{
	SetDataInternal(data);
}
