/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "RnIntObject.h"

/*****************************************************************************/
RnIntObject::RnIntObject(RnIntNative data)
{
	_data = data;
}

/*****************************************************************************/
RnIntObject::~RnIntObject() = default;

/*****************************************************************************/
RnObject* RnIntObject::operator+(RnObject* obj)
{
	switch (obj->GetType())
	{
	case RnType::RN_BOOLEAN:
		return RnObject::Create(GetData() + obj->ToInt());
	case RnType::RN_FLOAT:
		return RnObject::Create(GetData() + obj->ToFloat());
	case RnType::RN_INT:
		return RnObject::Create(GetData() + obj->ToInt());
	case RnType::RN_ARRAY:
	case RnType::RN_FUNCTION:
	case RnType::RN_CLASS_INSTANCE:
	case RnType::RN_NULL:
	case RnType::RN_UNKNOWN:
	case RnType::RN_STRING:
	case RnType::RN_OBJECT:
	default:
		break;
	}
	throw std::runtime_error("Operator '+' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator-(RnObject* obj)
{
	switch (obj->GetType())
	{
	case RnType::RN_BOOLEAN:
		return RnObject::Create(GetData() - obj->ToInt());
	case RnType::RN_FLOAT:
		return RnObject::Create(GetData() - obj->ToFloat());
	case RnType::RN_INT:
		return RnObject::Create(GetData() - obj->ToInt());
	case RnType::RN_ARRAY:
	case RnType::RN_FUNCTION:
	case RnType::RN_CLASS_INSTANCE:
	case RnType::RN_NULL:
	case RnType::RN_UNKNOWN:
	case RnType::RN_STRING:
	case RnType::RN_OBJECT:
	default:
		break;
	}
	throw std::runtime_error("Operator '-' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator==(RnObject* obj)
{
	switch (obj->GetType())
	{
	case RnType::RN_BOOLEAN:
		return RnObject::Create(GetData() == obj->ToInt());
	case RnType::RN_FLOAT:
		return RnObject::Create(GetData() == obj->ToFloat());
	case RnType::RN_INT:
		return RnObject::Create(GetData() == obj->ToInt());
	case RnType::RN_ARRAY:
	case RnType::RN_FUNCTION:
	case RnType::RN_CLASS_INSTANCE:
	case RnType::RN_NULL:
	case RnType::RN_UNKNOWN:
	case RnType::RN_STRING:
	case RnType::RN_OBJECT:
	default:
		break;
	}
	throw std::runtime_error("Operator '==' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator!=(RnObject* obj)
{
	throw std::runtime_error("Operator '!=' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator/(RnObject* obj)
{
	switch (obj->GetType())
	{
	case RnType::RN_BOOLEAN:
		return RnObject::Create(GetData() / obj->ToInt());
	case RnType::RN_FLOAT:
		return RnObject::Create(GetData() / obj->ToFloat());
	case RnType::RN_INT:
		return RnObject::Create(GetData() / obj->ToInt());
	case RnType::RN_ARRAY:
	case RnType::RN_FUNCTION:
	case RnType::RN_CLASS_INSTANCE:
	case RnType::RN_NULL:
	case RnType::RN_UNKNOWN:
	case RnType::RN_STRING:
	case RnType::RN_OBJECT:
	default:
		break;
	}
	throw std::runtime_error("Operator '/' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator|(RnObject* obj)
{
	throw std::runtime_error("Operator '+' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator||(RnObject* obj)
{
	throw std::runtime_error("Operator '+' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator&&(RnObject* obj)
{
	throw std::runtime_error("Operator '+' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator&(RnObject* obj)
{
	throw std::runtime_error("Operator '+' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator^(RnObject* obj)
{
	switch (obj->GetType())
	{
	case RnType::RN_BOOLEAN:
	case RnType::RN_INT:
		return RnObject::Create(GetData() ^ obj->ToInt());
	case RnType::RN_FLOAT:
	case RnType::RN_ARRAY:
	case RnType::RN_FUNCTION:
	case RnType::RN_CLASS_INSTANCE:
	case RnType::RN_NULL:
	case RnType::RN_UNKNOWN:
	case RnType::RN_STRING:
	case RnType::RN_OBJECT:
	default:
		break;
	}
	throw std::runtime_error("Operator '+' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator>(RnObject* obj)
{
	return RnObject::Create(GetData() > obj->ToInt());
}

///*****************************************************************************/
//RnObject* RnIntObject::operator<(RnObject* obj)
//{
//	return RnObject::Create(GetData() < obj->ToInt());
//}

/*****************************************************************************/
RnObject* RnIntObject::operator>=(RnObject* obj)
{
	throw std::runtime_error("Operator '>=' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator<=(RnObject* obj)
{
	return nullptr;
}

/*****************************************************************************/
RnObject* RnIntObject::operator%(RnObject* obj)
{
	throw std::runtime_error("Operator '%' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator>>(RnObject* obj)
{
	throw std::runtime_error("Operator '>>' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator<<(RnObject* obj)
{
	throw std::runtime_error("Operator '<<' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnObject* RnIntObject::operator*(RnObject* obj)
{
	switch (obj->GetType())
	{
	case RnType::RN_BOOLEAN:
		return RnObject::Create(GetData() * obj->ToInt());
	case RnType::RN_FLOAT:
		return RnObject::Create(GetData() * obj->ToFloat());
	case RnType::RN_INT:
		return RnObject::Create(GetData() * obj->ToInt());
	case RnType::RN_ARRAY:
	case RnType::RN_FUNCTION:
	case RnType::RN_CLASS_INSTANCE:
	case RnType::RN_NULL:
	case RnType::RN_UNKNOWN:
	case RnType::RN_STRING:
	case RnType::RN_OBJECT:
	default:
		break;
	}
	throw std::runtime_error("Operator '*' is not defined for types '"
		+ RnType::TypeToString(RnObjectBase<RnIntNative>::GetType()) + "' and '"
		+ RnType::TypeToString(obj->GetType()) + "'");
}

/*****************************************************************************/
RnIntNative RnIntObject::ToInt() const
{
	return static_cast<RnIntNative>(GetData());
}

/*****************************************************************************/
RnFloatNative RnIntObject::ToFloat() const
{
	return static_cast<RnFloatNative>(GetData());
}

/*****************************************************************************/
RnStringNative RnIntObject::ToString() const
{
	return std::to_string(_data);
}

/*****************************************************************************/
std::vector<RnObject*> RnIntObject::ToList() const
{
	return RnObjectBase::ToList();
}

/*****************************************************************************/
RnBoolNative RnIntObject::ToBool() const
{
	return static_cast<bool>(_data);
}

/*****************************************************************************/
void RnIntObject::SetData(RnIntNative data)
{
	SetDataInternal(data);
}

/*****************************************************************************/
void RnIntObject::SetData(RnFloatNative data)
{
	SetDataInternal(static_cast<RnIntNative>(data));
}
