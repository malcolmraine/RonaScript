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

#include <vector>
#include "RnObject.h"
#include "RnScope.h"

#define UNDEFINED_OPERATOR(op)                                                 \
    auto operator op(RnObject* obj)->RnObject* override {                      \
        throw std::runtime_error("Operator '" + std::string(#op) +             \
                                 "' is not defined for types '" +              \
                                 RnType::TypeToString(GetType()) + "' and '" + \
                                 RnType::TypeToString(obj->GetActiveType()) + "'");  \
    }

#define UNDEFINED_CAST(ret, handle, replacement)                              \
    [[nodiscard]] ret handle const override {                                 \
        throw std::runtime_error("Cannot convert type " +                     \
                                 RnType::TypeToString(GetType()) + " to type " + \
                                 replacement);                  \
    }

#define UNDEFINED_ASSIGNMENT(type, rntype, strval)                              \
    auto SetData(type data)->void override {                                    \
        throw std::runtime_error("Cannot assign value " + std::string(strval) + \
                                 " of type '" + RnType::TypeToString(rntype) +  \
                                 "' to symbol of type '" +                      \
                                 RnType::TypeToString(GetType()) + "'");        \
    }

class RnScope;

template <class T>
class RnObjectBase : public RnObject {
public:
    /*************************************************************************/
    RnObjectBase() = default;

    /*************************************************************************/
    ~RnObjectBase() override = default;

    /*************************************************************************/
    T GetData() const {
        return _data;
    }

    /*************************************************************************/
    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_UNKNOWN;
    }

    /*************************************************************************/
    [[nodiscard]] std::vector<char> AsBytes() const override {
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
    UNDEFINED_CAST(RnArrayNative, ToArray(), "array")
    UNDEFINED_CAST(RnFunction*, ToFunction(), "function")
    UNDEFINED_CAST(RnScope*, ToObject(), "object")
    UNDEFINED_CAST(RnBoolNative, ToBool(), "bool")

    UNDEFINED_ASSIGNMENT(RnStringNative, RnType::RN_STRING, data)
    UNDEFINED_ASSIGNMENT(RnIntNative, RnType::RN_INT, std::to_string(data))
    UNDEFINED_ASSIGNMENT(RnFloatNative, RnType::RN_FLOAT, std::to_string(data))
    UNDEFINED_ASSIGNMENT(RnBoolNative, RnType::RN_BOOLEAN, std::to_string(data))
    UNDEFINED_ASSIGNMENT(RnArrayNative, RnType::RN_ARRAY, "[...]")
    UNDEFINED_ASSIGNMENT(RnObject*, data->GetType(), data->ToString())
    UNDEFINED_ASSIGNMENT(RnScope*, RnType::RN_OBJECT, "object")
    UNDEFINED_ASSIGNMENT(RnFunction*, RnType::RN_FUNCTION, "")

    [[nodiscard]] RnType::Type GetActiveType() const override {
        return GetType();
    }

    /*************************************************************************/
    void SetConstFlag(bool flag) override {
        _is_const = flag;
    }

    /*************************************************************************/
    void CopyDataFromObject(RnObject* obj) override {
        switch (obj->GetType()) {
            case RnType::RN_ANY:
            {
                break;
            }
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
            case RnType::RN_CALLABLE:
                SetData(obj->ToFunction());
                break;
            case RnType::RN_OBJECT:
            case RnType::RN_CLASS_INSTANCE:
                SetData(obj->ToObject());
                break;
            case RnType::RN_NULL:
            case RnType::RN_VOID:
            case RnType::RN_UNKNOWN:
                break;
        }
    }

protected:
    /*************************************************************************/
    void SetDataInternal(T data) {
        if (_is_const && _initialized) {
            throw std::runtime_error("Cannot set value of const");
        }
        _data = data;
        _initialized = true;
    }

protected:
    T _data;
    bool _is_const = false;
    bool _initialized = false;
};

#endif  //RONASCRIPT_RNOBJECTBASE_H
