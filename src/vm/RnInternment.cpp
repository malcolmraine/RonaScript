/*****************************************************************************
* File: RnInternment.cpp
* Description:
* Author: Malcolm Hall
* Date: 4/19/23
* Version: 1
*
******************************************************************************/

#include "RnInternment.h"
#include "RnObject.h"
#include "RnFloatObject.h"
#include "RnStringObject.h"
#include "RnIntObject.h"
#include "RnBoolObject.h"

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
