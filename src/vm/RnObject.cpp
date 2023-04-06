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
#include "RnAnyObject.h"
#include "RnArrayObject.h"
#include "RnBoolObject.h"
#include "RnClassObject.h"
#include "RnFloatObject.h"
#include "RnFunctionObject.h"
#include "RnIntObject.h"
#include "RnNullObject.h"
#include "RnStringObject.h"
#include "RnVirtualMachine.h"

RnInternment<RnObject*>* RnObject::object_internment = new RnInternment<RnObject*>(
    [](RnObject* a, RnObject* b) { return RnObject::ValueCompare(a, b); });

bool RnObject::ValueCompare(RnObject* a, RnObject* b) {
    if (a->GetType() != b->GetType()) {
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
            return true;
        case RnType::RN_VOID:
        case RnType::RN_ANY:
        case RnType::RN_UNKNOWN:
            break;
    }
    return false;
}

/*****************************************************************************/
static RnObject* GetNullObject() {
    return RnObject::null_object;
}

/*****************************************************************************/
std::string RnObject::GetInternedString(InternmentKey key) {
    return object_internment->GetInternedItem(key)->ToString();
}

/*****************************************************************************/
double RnObject::GetInternedFloat(InternmentKey key) {
    return object_internment->GetInternedItem(key)->ToFloat();
}

/*****************************************************************************/
long RnObject::GetInternedInt(InternmentKey key) {
    return object_internment->GetInternedItem(key)->ToInt();
}

/*****************************************************************************/
RnObject* RnObject::GetInternedObject(InternmentKey key) {
    return object_internment->GetInternedItem(key);
}

/*****************************************************************************/
RnIntNative RnObject::InternValue(RnFloatNative x) {

    return object_internment->InternItem(new RnFloatObject(x));
}

/*****************************************************************************/
RnIntNative RnObject::InternValue(RnBoolNative x) {

    return object_internment->InternItem(new RnBoolObject(x));
}

/*****************************************************************************/
RnIntNative RnObject::InternValue(const RnStringNative& x) {
    return object_internment->InternItem(new RnStringObject(x));
}

/*****************************************************************************/
RnIntNative RnObject::InternValue(RnIntNative x) {
    return object_internment->InternItem(new RnIntObject(x));
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
            return RnNullObject::Instance();
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