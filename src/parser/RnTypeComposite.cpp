/*****************************************************************************
* File: RnTypeComposite.cpp
* Description:
* Author: Malcolm Hall
* Date: 7/11/22
* Version: 1
*
******************************************************************************/

#include "RnTypeComposite.h"

/*****************************************************************************/
RnTypeComposite::RnTypeComposite(const std::string& type)
{
	_type = RnType::StringToType(type);
	_bounds.lower = std::numeric_limits<double>::infinity();
	_bounds.upper = std::numeric_limits<double>::infinity();
}

/*****************************************************************************/
RnTypeComposite::RnTypeComposite(RnType::Type type)
{
	_type = type;
	_bounds.lower = std::numeric_limits<double>::infinity();
	_bounds.upper = std::numeric_limits<double>::infinity();
}

/*****************************************************************************/
RnTypeComposite::~RnTypeComposite() = default;

/*****************************************************************************/
[[nodiscard]] bool RnTypeComposite::IsWithinRange(RnTypeComposite type) const
{
	auto other_bounds = type.GetFloatBounds();
	return _bounds.lower >= other_bounds.lower && _bounds.upper <= other_bounds.upper;
}

///*****************************************************************************/
//bool RnTypeComposite::CanAssignFrom(const RnTypeComposite& type)
//{
//	if (type._type == _type)
//	{
//		return true;
//	}
//
//	switch (type._type)
//	{
//	case RnType::RN_FLOAT:
//	{
//		switch (_type)
//		{
//		case RnType::RN_FLOAT:
//		case RnType::RN_INT:
//			break;
//		default:
//			return false;
//		}
//		break;
//	}
//	case RnType::RN_INT:
//	{
//		switch (_type)
//		{
//		case RnType::RN_FLOAT:
//		case RnType::RN_INT:
//			break;
//		default:
//			return false;
//		}
//		break;
//	}
//	case RnType::RN_ARRAY:
//	{
//		switch (_type)
//		{
//		case RnType::RN_BOOLEAN:
//			break;
//		case RnType::RN_STRING:
//			break;
//		case RnType::RN_FLOAT:
//			break;
//		case RnType::RN_INT:
//			break;
//		case RnType::RN_ARRAY:
//			break;
//		case RnType::RN_FUNCTION:
//			break;
//		case RnType::RN_CLASS_INSTANCE:
//			break;
//		case RnType::RN_OBJECT:
//			break;
//		case RnType::RN_NULL:
//			break;
//		case RnType::RN_UNKNOWN:
//			break;
//		default:
//			return false;
//		}
//		break;
//	}
//	case RnType::RN_FUNCTION:
//	{
//		switch (_type)
//		{
//		case RnType::RN_BOOLEAN:
//			break;
//		case RnType::RN_STRING:
//			break;
//		case RnType::RN_FLOAT:
//			break;
//		case RnType::RN_INT:
//			break;
//		case RnType::RN_ARRAY:
//			break;
//		case RnType::RN_FUNCTION:
//			break;
//		case RnType::RN_CLASS_INSTANCE:
//			break;
//		case RnType::RN_OBJECT:
//			break;
//		case RnType::RN_NULL:
//			break;
//		case RnType::RN_UNKNOWN:
//			break;
//		default:
//			return false;
//		}
//		break;
//	}
//	case RnType::RN_CLASS_INSTANCE:
//	{
//		switch (_type)
//		{
//		case RnType::RN_BOOLEAN:
//			break;
//		case RnType::RN_STRING:
//			break;
//		case RnType::RN_FLOAT:
//			break;
//		case RnType::RN_INT:
//			break;
//		case RnType::RN_ARRAY:
//			break;
//		case RnType::RN_FUNCTION:
//			break;
//		case RnType::RN_CLASS_INSTANCE:
//			break;
//		case RnType::RN_OBJECT:
//			break;
//		case RnType::RN_NULL:
//			break;
//		case RnType::RN_UNKNOWN:
//			break;
//		default:
//			return false;
//		}
//		break;
//	}
//	case RnType::RN_OBJECT:
//	{
//		switch (_type)
//		{
//		case RnType::RN_BOOLEAN:
//			break;
//		case RnType::RN_STRING:
//			break;
//		case RnType::RN_FLOAT:
//			break;
//		case RnType::RN_INT:
//			break;
//		case RnType::RN_ARRAY:
//			break;
//		case RnType::RN_FUNCTION:
//			break;
//		case RnType::RN_CLASS_INSTANCE:
//			break;
//		case RnType::RN_OBJECT:
//			break;
//		case RnType::RN_NULL:
//			break;
//		case RnType::RN_UNKNOWN:
//			break;
//		default:
//			return false;
//		}
//		break;
//	}
//	case RnType::RN_NULL:
//	{
//		switch (_type)
//		{
//		case RnType::RN_BOOLEAN:
//			break;
//		case RnType::RN_STRING:
//			break;
//		case RnType::RN_FLOAT:
//			break;
//		case RnType::RN_INT:
//			break;
//		case RnType::RN_ARRAY:
//			break;
//		case RnType::RN_FUNCTION:
//			break;
//		case RnType::RN_CLASS_INSTANCE:
//			break;
//		case RnType::RN_OBJECT:
//			break;
//		case RnType::RN_NULL:
//			break;
//		case RnType::RN_UNKNOWN:
//			break;
//		default:
//			return false;
//		}
//		break;
//	}
//	case RnType::RN_UNKNOWN:
//	{
//		return false;
//	}
//	default:
//		return false;
//	}
//	return false;
//}

///*****************************************************************************/
//bool RnTypeComposite::CanAssignTo(RnTypeComposite type)
//{
//	return true;
//}

/*****************************************************************************/
Bounds<RnIntNative> RnTypeComposite::GetIntBounds() const
{
	Bounds<RnIntNative> bounds{};
	bounds.lower = static_cast<RnIntNative>(_bounds.lower);
	bounds.upper = static_cast<RnIntNative>(_bounds.upper);

	return bounds;
}

/*****************************************************************************/
Bounds<RnFloatNative> RnTypeComposite::GetFloatBounds()
{
	return _bounds;
}

/*****************************************************************************/
void RnTypeComposite::SetBounds(RnIntNative lower, RnIntNative upper)
{
	_bounds.lower = static_cast<RnFloatNative>(lower);
	_bounds.upper = static_cast<RnFloatNative>(upper);
}

/*****************************************************************************/
void RnTypeComposite::SetBounds(RnFloatNative lower, RnFloatNative upper)
{
	_bounds.lower = lower;
	_bounds.upper = upper;
}

/*****************************************************************************/
std::string RnTypeComposite::ToString()
{
	switch (_type)
	{
	case RnType::RN_INT:
	case RnType::RN_STRING:
	case RnType::RN_ARRAY:
	{
		auto bounds = GetIntBounds();
		return _str_type + "<" + std::to_string(bounds.lower) + ".."
			+ std::to_string(bounds.upper) + ">";
	}
	default:
	{
		return _str_type + "<" + std::to_string(_bounds.lower) + ".."
			+ std::to_string(_bounds.upper) + ">";
	}
	}

}