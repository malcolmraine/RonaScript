/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNOBJECTBASE_H
#define RONASCRIPT_RNOBJECTBASE_H

#include "RnObject.h"
#include <vector>
#include "RnScope.h"

#define UNDEFINED_OPERATOR(op)                                       \
    auto operator op (RnObject* obj) -> RnObject* override  {        \
            throw std::runtime_error(                                \
                "Operator '"                                         \
                + std::string(#op)                                   \
                + "' is not defined for types '"                     \
                + RnType::TypeToString(GetType())                    \
                + "' and '"                                          \
                + RnType::TypeToString(obj->GetType()) + "'");       \
    }

#define UNDEFINED_CAST(ret, handle, replacement)                    \
    [[nodiscard]]  ret handle const override {                        \
        throw std::runtime_error(                                    \
            "Cannot convert type "                                \
            + RnType::TypeToString(GetType())                        \
            + " to a "                                                \
            + std::string(#replacement));                            \
    }

#define UNDEFINED_ASSIGNMENT(type, rntype, strval)                               \
    auto SetData(type data) -> void override {                                           \
        throw std::runtime_error(                                               \
            "Cannot assign value " + std::string(#strval) + " of type '"       \
            + RnType::TypeToString(rntype)                                       \
            + "' to symbol of type '"                                       \
            + RnType::TypeToString(GetType()) + "'");                           \
    }

template<class T>
class RnObjectBase : public RnObject
{
 public:
	/*************************************************************************/
	RnObjectBase() = default;

	/*************************************************************************/
	~RnObjectBase() = default;

	/*************************************************************************/
	T GetData() const
	{
		return _data;
	}

//	/*************************************************************************/
//	void SetType(RnType::Type type) override
//	{
//		_type = type;
//	}

	/*************************************************************************/
	[[nodiscard]] RnType::Type GetType() const override
	{
		return RnType::RN_UNKNOWN;
	}

	/*************************************************************************/
	[[nodiscard]] std::vector<char> AsBytes() const override
	{
		return {};
	}

	UNDEFINED_OPERATOR(+)
	UNDEFINED_OPERATOR(-)
	UNDEFINED_OPERATOR(%)
	UNDEFINED_OPERATOR(==)
	UNDEFINED_OPERATOR(!=)
	UNDEFINED_OPERATOR(/)
	UNDEFINED_OPERATOR(||)
	UNDEFINED_OPERATOR(|)
	UNDEFINED_OPERATOR(&&)
	UNDEFINED_OPERATOR(&)
	UNDEFINED_OPERATOR(^)
	UNDEFINED_OPERATOR(>)
	UNDEFINED_OPERATOR(<)
	UNDEFINED_OPERATOR(>=)
	UNDEFINED_OPERATOR(<=)
	UNDEFINED_OPERATOR(>>)
	UNDEFINED_OPERATOR(<<)
	UNDEFINED_OPERATOR(*)

	UNDEFINED_CAST(RnIntNative, ToInt(), "int")
	UNDEFINED_CAST(RnFloatNative, ToFloat(), "float")
	UNDEFINED_CAST(RnStringNative, ToString(), "string")
	UNDEFINED_CAST(std::vector<RnObject*>, ToArray(), "array")
	UNDEFINED_CAST(RnFunction*, ToFunction(), "function")
	UNDEFINED_CAST(RnScope*, ToObject(), "object")
	UNDEFINED_CAST(RnBoolNative, ToBool(), "bool")

	UNDEFINED_ASSIGNMENT(RnStringNative, RnType::RN_STRING, data)
	UNDEFINED_ASSIGNMENT(RnIntNative, RnType::RN_INT, std::to_string(data))
	UNDEFINED_ASSIGNMENT(RnFloatNative, RnType::RN_FLOAT, std::to_string(data))
	UNDEFINED_ASSIGNMENT(RnBoolNative, RnType::RN_BOOLEAN, std::to_string(data))
	UNDEFINED_ASSIGNMENT(std::vector<RnObject*>,
		RnType::RN_ARRAY,
		"array[" + std::to_string(data.size()) + "]")
	UNDEFINED_ASSIGNMENT(RnObject*, data->GetType(), data->ToString())
	UNDEFINED_ASSIGNMENT(RnScope*, RnType::RN_OBJECT, "object")
	UNDEFINED_ASSIGNMENT(RnFunction*, RnType::RN_FUNCTION, "")

	/*************************************************************************/
	void SetConstFlag(bool flag) override
	{
		_is_const = flag;
	}

	/*************************************************************************/
	void CopyDataFromObject(RnObject* obj) override
	{
		switch (obj->GetType())
		{
		case RnType::RN_BOOLEAN:
			SetData(obj->ToInt());
			break;
		case RnType::RN_STRING:
			SetData(obj->ToString());
			break;
		case RnType::RN_FLOAT:
			SetData(obj->ToFloat());
			break;
		case RnType::RN_INT:
			SetData(obj->ToInt());
			break;
		case RnType::RN_ARRAY:
			SetData(obj->ToArray());
			break;
		case RnType::RN_FUNCTION:
			SetData(obj->ToFunction());
			break;
		case RnType::RN_OBJECT:
		case RnType::RN_CLASS_INSTANCE:
			SetData(obj->ToObject());
			break;
		case RnType::RN_NULL:
			break;
		case RnType::RN_UNKNOWN:
			break;
		}
	}

 protected:
	/*************************************************************************/
	void SetDataInternal(T data)
	{
		if (_is_const)
		{
			throw std::runtime_error("Cannot set value of const");
		}
		_data = data;
	}

 protected:
	T _data;
	bool _is_const = false;
};

#endif //RONASCRIPT_RNOBJECTBASE_H
