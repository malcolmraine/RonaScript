/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "RnArrayObject.h"
#include "../util/RnStringUtil.h"

/*****************************************************************************/
RnArrayObject::RnArrayObject()
{
}

/*****************************************************************************/
RnArrayObject::~RnArrayObject()
{

}

/*****************************************************************************/
RnObject* RnArrayObject::operator+(RnObject* obj)
{
	std::vector<RnObject*> data;
	auto result = RnObject::Create(RnType::RN_ARRAY);

	for (auto& item : obj->ToList())
	{
		data.emplace_back(item);
	}

	for (auto& item : ToList())
	{
		data.emplace_back(item);
	}
	result->SetData(data);

	return result;
}

/*****************************************************************************/
RnObject* RnArrayObject::operator-(RnObject* obj)
{
	return nullptr;
}

/*****************************************************************************/
RnObject* RnArrayObject::operator==(RnObject* obj)
{
	return nullptr;
}

/*****************************************************************************/
RnObject* RnArrayObject::operator!=(RnObject* obj)
{
	return nullptr;
}

/*****************************************************************************/
RnObject* RnArrayObject::operator/(RnObject* obj)
{
	return nullptr;
}

/*****************************************************************************/
RnObject* RnArrayObject::operator|(RnObject* obj)
{
	return nullptr;
}

/*****************************************************************************/
RnObject* RnArrayObject::operator&(RnObject* obj)
{
	return nullptr;
}

/*****************************************************************************/
void RnArrayObject::Append(RnObject* obj)
{
	_data.push_back(obj);
}

/*****************************************************************************/
void RnArrayObject::SetData(RnStringNative data)
{
	RnObjectBase::SetData(data);
}

/*****************************************************************************/
void RnArrayObject::SetData(RnIntNative data)
{
	RnObjectBase::SetData(data);
}

/*****************************************************************************/
void RnArrayObject::SetData(RnFloatNative data)
{
	RnObjectBase::SetData(data);
}

/*****************************************************************************/
void RnArrayObject::SetData(std::vector<RnObject*> data)
{
	SetDataInternal(data);
}

/*****************************************************************************/
void RnArrayObject::SetData(RnObject* data)
{
	if (data->GetType() == RnType::RN_ARRAY)
	{
		auto list_obj = dynamic_cast<RnArrayObject*>(data);
		SetDataInternal(list_obj->GetData());
	}
	else
	{
		throw std::runtime_error(
			"Cannot assign value of type '" + RnType::TypeToString(data->GetType())
				+ "' to symbol of type '" + RnType::TypeToString(GetType()) + "'");
	}
}

/*****************************************************************************/
RnStringNative RnArrayObject::ToString() const
{
	std::string s;
	RnIntNative i = 0;
	for (auto& item : GetData())
	{
		s += item->ToString() + ", ";
		i++;
	}

	return "[" + s.substr(0, s.length() - 2) + "]";
}

/*****************************************************************************/
std::vector<RnObject*> RnArrayObject::ToList() const
{
	return GetData();
}

/*****************************************************************************/
RnBoolNative RnArrayObject::ToBool() const
{
	return !_data.empty();
}