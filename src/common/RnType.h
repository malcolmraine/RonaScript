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

#include <limits>
#include <string>
#include <utility>

typedef long long RnIntNative;
typedef double RnFloatNative;
typedef std::string RnStringNative;
typedef bool RnBoolNative;

class RnType {
public:
    enum Type : int {
        RN_BOOLEAN = 0,
        RN_STRING,
        RN_FLOAT,
        RN_INT,
        RN_ARRAY,
        RN_FUNCTION,    // Defined routines
        RN_CALLABLE, // Assigned values
        RN_CLASS_INSTANCE,
        RN_OBJECT,
        RN_NULL,
        RN_VOID,
        RN_UNKNOWN,
    };

    /*************************************************************************/
    static std::string TypeToString(RnType::Type type) {
        switch (type) {
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
    static Type StringToType(const std::string& type) {
        if (type == "string") {
            return RN_STRING;
        } else if (type == "float") {
            return RN_FLOAT;
        } else if (type == "array") {
            return RN_ARRAY;
        } else if (type == "routine") {
            return RN_FUNCTION;
        } else if (type == "int") {
            return RN_INT;
        } else if (type == "callable") {
            return RN_CALLABLE;
        } else if (type == "object") {
            return RN_OBJECT;
        } else if (type == "null" || type == "void") {
            return RN_NULL;
        } else if (type == "bool") {
            return RN_BOOLEAN;
        } else {
            return RN_UNKNOWN;
        }
    }
};

template <typename T>
struct Bounds {
    T lower;
    T upper;
};

class RnTypeComposite {
public:
    explicit RnTypeComposite(const std::string& type);
    explicit RnTypeComposite(RnType::Type type);
    ~RnTypeComposite();
    [[nodiscard]] bool IsWithinRange(RnTypeComposite type) const;  // TODO: Unit test
    [[nodiscard]] Bounds<RnIntNative> GetIntBounds() const;
    Bounds<RnFloatNative> GetFloatBounds();
    void SetBounds(RnIntNative lower, RnIntNative upper);
    void SetBounds(RnFloatNative lower, RnFloatNative upper);
    std::string ToString();  // TODO: Unit test

    [[nodiscard]] RnType::Type GetType() const {
        return _type;
    }

private:
    Bounds<RnFloatNative> _bounds{};
    RnType::Type _type;
};

#endif  //RONASCRIPT_RNTYPE_H
