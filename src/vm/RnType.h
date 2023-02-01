/*****************************************************************************
* File: RnType.h
* Description:
* Author: Malcolm Hall
* Date: 5/18/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNTYPE_H
#define RONASCRIPT_RNTYPE_H

#include <string>

typedef long long RnIntNative;
typedef double RnFloatNative;
typedef std::string RnStringNative;
typedef bool RnBoolNative;

class RnType
{
 public:
	enum Type
	{
		RN_BOOLEAN,
		RN_STRING,
		RN_FLOAT,
		RN_INT,
		RN_ARRAY,
		RN_FUNCTION,
		RN_CLASS_INSTANCE,
		RN_OBJECT,
		RN_NULL,
		RN_VOID,
		RN_UNKNOWN,
	};

	/*************************************************************************/
	static std::string TypeToString(RnType::Type type)
	{
		switch (type)
		{
		case RN_BOOLEAN:
			return "bool";
		case RN_STRING:
			return "string";
		case RN_FLOAT:
			return "float";
		case RN_INT:
			return "int";
		case RN_ARRAY:
			return "array";
		case RN_FUNCTION:
			return "function";
		case RN_CLASS_INSTANCE:
			return "object";
		case RN_OBJECT:
			return "object";
		case RN_NULL:
			return "null";
		case RN_UNKNOWN:
			return "unknown";
		default:
			return "unknown";
		}
	}

	/*************************************************************************/
	static Type StringToType(const std::string& type)
	{
		if (type == "string")
		{
			return RN_STRING;
		}
		else if (type == "float")
		{
			return RN_FLOAT;
		}
		else if (type == "array")
		{
			return RN_ARRAY;
		}
		else if (type == "func")
		{
			return RN_FUNCTION;
		}
		else if (type == "int")
		{
			return RN_INT;
		}
		else if (type == "callable")
		{
			return RN_FUNCTION;
		}
		else if (type == "object")
		{
			return RN_OBJECT;
		}
		else if (type == "null" || type == "void")
		{
			return RN_NULL;
		}
		else if (type == "bool")
		{
			return RN_BOOLEAN;
		}
		else
		{
			return RN_UNKNOWN;
		}
	}
};

#endif //RONASCRIPT_RNTYPE_H
