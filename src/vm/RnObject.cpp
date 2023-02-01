/*****************************************************************************
* File: RnObject.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/18/22
* Version: 1
*
******************************************************************************/

#include "RnObject.h"

#include <utility>
#include "RnArrayObject.h"
#include "RnStringObject.h"
#include "RnClassObject.h"
#include "RnIntObject.h"
#include "RnFloatObject.h"
#include "RnFunctionObject.h"
#include "RnBoolObject.h"
#include "RnNullObject.h"
#include "RnVirtualMachine.h"

RnInternment<std::string>* string_internment = new RnInternment<std::string>();
RnInternment<double>* float_internment = new RnInternment<double>();
RnInternment<long>* int_internment = new RnInternment<long>();

/*****************************************************************************/
static RnObject* GetNullObject()
{
	return RnObject::null_object;
}

/*****************************************************************************/
std::string RnObject::GetInternedString(InternmentKey key)
{
	return string_internment->GetInternedItem(key);
}

/*****************************************************************************/
double RnObject::GetInternedFloat(InternmentKey key)
{
	return float_internment->GetInternedItem(key);
}

/*****************************************************************************/
long RnObject::GetInternedInt(InternmentKey key)
{
	return int_internment->GetInternedItem(key);
}

/*****************************************************************************/
size_t RnObject::InternValue(RnFloatNative x)
{
	return float_internment->InternItem(x);
}

/*****************************************************************************/
size_t RnObject::InternValue(const RnStringNative& x)
{
	return string_internment->InternItem(x);
}

/*****************************************************************************/
size_t RnObject::InternValue(RnIntNative x)
{
	return int_internment->InternItem(x);
}

/*****************************************************************************/
RnObject* RnObject::Create(RnStringNative data)
{
	return new RnStringObject(std::move(data));
}

/*****************************************************************************/
RnObject* RnObject::Create(RnBoolNative data)
{
	if (RnVirtualMachine::GetInstance())
	{
		return RnVirtualMachine::GetInstance()->CreateObject(data);
	}
	return new RnBoolObject(data);
}

/*****************************************************************************/
RnObject* RnObject::Create(RnIntNative data)
{
	if (RnVirtualMachine::GetInstance())
	{
		return RnVirtualMachine::GetInstance()->CreateObject(data);
	}
	return new RnIntObject(data);
}

/*****************************************************************************/
RnObject* RnObject::Create(RnFloatNative data)
{
	if (RnVirtualMachine::GetInstance())
	{
		return RnVirtualMachine::GetInstance()->CreateObject(data);
	}
	return new RnFloatObject(data);
}

/*****************************************************************************/
RnObject* RnObject::Create(RnType::Type type)
{
	if (RnVirtualMachine::GetInstance())
	{
		return RnVirtualMachine::GetInstance()->CreateObject(type);
	}

	switch (type)
	{
	case RnType::RN_BOOLEAN:
		return new RnBoolObject();
	case RnType::RN_STRING:
		return new RnStringObject();
	case RnType::RN_FLOAT:
		return new RnFloatObject();
	case RnType::RN_INT:
		return new RnIntObject();
	case RnType::RN_ARRAY:
		return new RnArrayObject();
	case RnType::RN_FUNCTION:
		return new RnFunctionObject();
	case RnType::RN_CLASS_INSTANCE:
	case RnType::RN_OBJECT:
		return new RnClassObject();
	case RnType::RN_UNKNOWN:
	default:
		return RnNullObject::Instance();
	}
}

/*****************************************************************************/
RnObject* RnObject::Copy(RnObject* obj)
{
	auto result = RnObject::Create(obj->GetType());
	switch (obj->GetType())
	{
	case RnType::RN_BOOLEAN:
		result->SetData(obj->ToBool());
		break;
	case RnType::RN_STRING:
		result->SetData(obj->ToString());
		break;
	case RnType::RN_FLOAT:
		result->SetData(obj->ToFloat());
		break;
	case RnType::RN_INT:
		result->SetData(obj->ToInt());
		break;
	case RnType::RN_ARRAY:
		result->SetData(obj->ToArray());
		break;
	case RnType::RN_FUNCTION:
		result->SetData(obj->ToFunction());
		break;
	case RnType::RN_CLASS_INSTANCE:
	case RnType::RN_OBJECT:
		result->SetData(obj->ToObject());
		break;
	case RnType::RN_UNKNOWN:
	default:
		throw std::runtime_error("Could not copy object.");
	}

	return result;
}