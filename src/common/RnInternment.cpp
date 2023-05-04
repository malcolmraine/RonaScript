/*****************************************************************************
* File: RnInternment.cpp
* Description:
* Author: Malcolm Hall
* Date: 4/19/23
* Version: 1
*
******************************************************************************/

#include "RnInternment.h"
#include "../vm/RnBoolObject.h"
#include "../vm/RnFloatObject.h"
#include "../vm/RnIntObject.h"
#include "../vm/RnStringObject.h"

RnInternment<RnObject*>* RnConstStore::object_internment = new RnInternment<RnObject*>(
    [](RnObject* a, RnObject* b) { return RnObject::ValueCompare(a, b); });

/*****************************************************************************/
std::string RnConstStore::GetInternedString(InternmentKey key) {
    return object_internment->GetInternedItem(key)->ToString();
}

/*****************************************************************************/
double RnConstStore::GetInternedFloat(InternmentKey key) {
    return object_internment->GetInternedItem(key)->ToFloat();
}

/*****************************************************************************/
long RnConstStore::GetInternedInt(InternmentKey key) {
    return object_internment->GetInternedItem(key)->ToInt();
}

/*****************************************************************************/
RnObject* RnConstStore::GetInternedObject(InternmentKey key) {
    return object_internment->GetInternedItem(key);
}

/*****************************************************************************/
RnIntNative RnConstStore::InternValue(RnFloatNative x) {

    return object_internment->InternItem(new RnFloatObject(x));
}

/*****************************************************************************/
RnIntNative RnConstStore::InternValue(RnBoolNative x) {

    return object_internment->InternItem(new RnBoolObject(x));
}

/*****************************************************************************/
RnIntNative RnConstStore::InternValue(const RnStringNative& x) {
    return object_internment->InternItem(new RnStringObject(x));
}

/*****************************************************************************/
RnIntNative RnConstStore::InternValue(RnIntNative x) {
    return object_internment->InternItem(new RnIntObject(x));
}

/*****************************************************************************/
void RnConstStore::LoadObject(RnObject* object) {
    object_internment->LoadObject(object);
}

/*****************************************************************************/
void RnConstStore::Init(size_t size) {
    object_internment->Init(size);
}