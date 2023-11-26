/*****************************************************************************
* File: RnObject.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/18/22
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

#include "RnObject.h"
#include "RnAnyObject.h"
#include "RnArrayObject.h"
#include "RnBoolObject.h"
#include "RnClassObject.h"
#include "RnFloatObject.h"
#include "RnFunctionObject.h"
#include "RnIntObject.h"
#include "RnStringObject.h"
#include "RnVirtualMachine.h"

/*****************************************************************************/
bool RnObject::ValueCompare(const RnObject* a, const RnObject* b) {
    if (a->GetActiveType() != b->GetActiveType()) {
        return false;
    }
    switch (a->GetType()) {
        case RnType::RN_BOOLEAN:
            return a->ToBool() == b->ToBool();
        case RnType::RN_STRING:
            return a->ToString() == b->ToString();
        case RnType::RN_FLOAT:
            return a->ToFloat() == b->ToFloat();
        case RnType::RN_INT:
            return a->ToInt() == b->ToInt();
        case RnType::RN_ARRAY:
            return a->ToArray() == b->ToArray();
        case RnType::RN_FUNCTION:
        case RnType::RN_CALLABLE:
            return a->ToFunction() == b->ToFunction();
        case RnType::RN_CLASS_INSTANCE:
        case RnType::RN_OBJECT:
            return a->ToObject() == b->ToObject();
        case RnType::RN_NULL:
            return false;
        case RnType::RN_ANY:
        {
            return dynamic_cast<const RnAnyObject*>(a)->IsActiveDataEqual(b);
        }
        case RnType::RN_VOID:
        case RnType::RN_UNKNOWN:
            break;
    }
    return false;
}

/*****************************************************************************/
RnObject* RnObject::Create(const RnStringNative& data) {
    if (RnVirtualMachine::GetInstance()) {
        return RnVirtualMachine::GetInstance()->CreateObject(data);
    }
    return new RnStringObject(data);
}

/*****************************************************************************/
RnObject* RnObject::Create(RnBoolNative data) {
    if (RnVirtualMachine::GetInstance()) {
        return RnVirtualMachine::GetInstance()->CreateObject(data);
    }
    return new RnBoolObject(data);
}

/*****************************************************************************/
RnObject* RnObject::Create(RnIntNative data) {
    if (RnVirtualMachine::GetInstance()) {
        return RnVirtualMachine::GetInstance()->CreateObject(data);
    }
    return new RnIntObject(data);
}

/*****************************************************************************/
RnObject* RnObject::Create(RnFloatNative data) {
    if (RnVirtualMachine::GetInstance()) {
        return RnVirtualMachine::GetInstance()->CreateObject(data);
    }
    return new RnFloatObject(data);
}

/*****************************************************************************/
RnObject* RnObject::Create(RnType::Type type) {
    if (RnVirtualMachine::GetInstance()) {
        return RnVirtualMachine::GetInstance()->CreateObject(type);
    }

    switch (type) {
        case RnType::RN_BOOLEAN:
            return new RnBoolObject();
        case RnType::RN_STRING:
            return new RnStringObject();
        case RnType::RN_FLOAT:
            return new RnFloatObject();
        case RnType::RN_INT:
            return new RnIntObject();
        case RnType::RN_ANY:
            return new RnAnyObject();
        case RnType::RN_ARRAY:
            return new RnArrayObject();
        case RnType::RN_FUNCTION:
            return new RnFunctionObject();
        case RnType::RN_CLASS_INSTANCE:
        case RnType::RN_OBJECT:
            return new RnClassObject();
        case RnType::RN_UNKNOWN:
        default:
            assert(false);
            return nullptr;
    }
}

/*****************************************************************************/
RnObject* RnObject::Copy(RnObject* obj) {
    auto result = RnObject::Create(obj->GetType());
    switch (obj->GetType()) {
        case RnType::RN_BOOLEAN:
            result->SetData(obj->ToBool());
            break;
        case RnType::RN_ANY:
            dynamic_cast<RnAnyObject*>(result)->CopyFrom(obj);
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

/*****************************************************************************/
void RnObject::ThrowUndefinedOperatorError(const RnStringNative& op, RnObject* obj1,
                                                     RnObject* obj2) {
    throw std::runtime_error("Operator '" + op + "' is not defined for types '" +
                             obj1->GetTypeName() + "' and '" + obj2->GetTypeName() +
                             "'");
}