/*****************************************************************************
* File: RnInternment.cpp
* Description:
* Author: Malcolm Hall
* Date: 4/19/23
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

#include "RnInternment.h"
#include "../objects/RnBoolObject.h"
#include "../objects/RnFloatObject.h"
#include "../objects/RnIntObject.h"
#include "../objects/RnStringObject.h"

RnInternment<RnObject*>* RnConstStore::object_internment = nullptr;

/*****************************************************************************/
RnStringNative RnConstStore::GetInternedString(InternmentKey key) {
    return GetObjectInternment()->GetInternedItem(key)->ToString();
}

/*****************************************************************************/
double RnConstStore::GetInternedFloat(InternmentKey key) {
    return GetObjectInternment()->GetInternedItem(key)->ToFloat();
}

/*****************************************************************************/
long RnConstStore::GetInternedInt(InternmentKey key) {
    return GetObjectInternment()->GetInternedItem(key)->ToInt();
}

/*****************************************************************************/
RnObject* RnConstStore::GetInternedObject(InternmentKey key) {
    return GetObjectInternment()->GetInternedItem(key);
}

/*****************************************************************************/
RnIntNative RnConstStore::InternValue(RnFloatNative x) {
    return GetObjectInternment()->InternItem(new RnFloatObject(x));
}

/*****************************************************************************/
RnIntNative RnConstStore::InternValue(RnBoolNative x) {

    return GetObjectInternment()->InternItem(new RnBoolObject(x));
}

/*****************************************************************************/
RnIntNative RnConstStore::InternValue(const RnStringNative& x) {
    return GetObjectInternment()->InternItem(new RnStringObject(x));
}

/*****************************************************************************/
RnIntNative RnConstStore::InternValue(RnIntNative x) {
    return GetObjectInternment()->InternItem(new RnIntObject(x));
}

/*****************************************************************************/
void RnConstStore::LoadObject(RnObject* object) {
    GetObjectInternment()->LoadObject(object);
}

/*****************************************************************************/
void RnConstStore::Init(size_t size) {
    GetObjectInternment()->Init(size);
}

/*****************************************************************************/
RnInternment<RnObject*>* RnConstStore::GetObjectInternment() {
    if (!object_internment) {
        object_internment = new RnInternment<RnObject*>(
            [](RnObject* a, RnObject* b) { return RnObject::ValueCompare(a, b); });
    }
    return object_internment;
}