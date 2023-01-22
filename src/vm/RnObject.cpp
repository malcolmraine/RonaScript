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
size_t RnObject::InternValue(float x)
{
	return float_internment->InternItem(x);
}

/*****************************************************************************/
size_t RnObject::InternValue(double x)
{
	return float_internment->InternItem(x);
}

/*****************************************************************************/
size_t RnObject::InternValue(const std::string& x)
{
	return string_internment->InternItem(x);
}

/*****************************************************************************/
size_t RnObject::InternValue(long x)
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
	return new RnBoolObject(data);
}

/*****************************************************************************/
RnObject* RnObject::Create(RnIntNative data)
{
	return new RnIntObject(data);
}

/*****************************************************************************/
RnObject* RnObject::Create(RnFloatNative data)
{
	return new RnFloatObject(data);
}

/*****************************************************************************/
RnObject* RnObject::Create(RnType::Type type)
{
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