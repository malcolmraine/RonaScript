/*****************************************************************************
* File: RnType.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************/

#pragma once

#include <cassert>
#include <limits>
#include <string>
#include <utility>
#include <vector>
#include "../common/RnString.h"
#include "../vm/RnObjectList.h"

class RnObject;

typedef int64_t RnIntNative;
typedef double RnFloatNative;
typedef std::string RnStringNative;
typedef bool RnBoolNative;
typedef RnObjectList RnArrayNative;

#define RN_NUMERIC_DATA_LENGTH (8)
#define RN_TYPE_BYTES_LENGTH (1)
#define RN_SIZE_BYTES_LENGTH (8)

typedef union {
    char c_data[RN_NUMERIC_DATA_LENGTH];
    RnIntNative i_data;
    RnFloatNative d_data;
} RnNumericUnion;

/*****************************************************************************/
/*****************************************************************************/
class RnType {
public:
    enum Type : uint8_t {
        RN_BOOLEAN = 0,
        RN_STRING,
        RN_FLOAT,
        RN_INT,
        RN_ARRAY,
        RN_FUNCTION,  // Defined routines
        RN_CALLABLE,  // Assigned values
        RN_CLASS_INSTANCE,
        RN_OBJECT,
        RN_NULL,
        RN_VOID,
        RN_ANY,
        RN_UNKNOWN,
    };

    /*************************************************************************/
    static std::string TypeToString(RnType::Type type) {
        switch (type) {
            case RN_BOOLEAN:
                return "bool";
            case RN_STRING:
                return "string";
            case RN_ANY:
                return "any";
            case RN_FLOAT:
                return "float";
            case RN_INT:
                return "int";
            case RN_ARRAY:
                return "array";
            case RN_CALLABLE:
            case RN_FUNCTION:
                return "function";
            case RN_CLASS_INSTANCE:
            case RN_OBJECT:
                return "object";
            case RN_NULL:
                return "null";
            case RN_VOID:
                return "void";
            case RN_UNKNOWN:
            default:
                assert(false);
                return "unknown";
        }
    }

    /*************************************************************************/
    static Type StringToType(const std::string& type) {
        if (type == "string") {
            return RN_STRING;
        } else if (type == "any") {
            return RN_ANY;
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
