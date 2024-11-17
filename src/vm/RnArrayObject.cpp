/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
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

#include "RnArrayObject.h"
#include "../util/String.h"

/*****************************************************************************/
RnArrayObject::RnArrayObject() = default;

/*****************************************************************************/
RnArrayObject::RnArrayObject(const RnArrayNative& data) {
    _data = data;
}

/*****************************************************************************/
RnArrayObject::~RnArrayObject() = default;

/*****************************************************************************/
RnObject* RnArrayObject::operator+(RnObject* obj) {
    RnArrayNative data;
    auto result = RnObject::Create(RnType::RN_ARRAY);

    for (auto& item : obj->ToArray()) {
        data.emplace_back(item);
    }

    for (auto& item : ToArray()) {
        data.emplace_back(item);
    }
    result->SetData(data);

    return result;
}

/*****************************************************************************/
RnObject* RnArrayObject::operator-(RnObject* obj) {
    return nullptr;
}

/*****************************************************************************/
RnObject* RnArrayObject::operator==(RnObject* obj) {
    auto result = RnObject::Create(RnType::RN_BOOLEAN);
    if (obj->GetType() != RnType::RN_ARRAY) {
        result->SetData(false);
    } else {
        result->SetData(dynamic_cast<RnArrayObject*>(obj)->ContentsEqual(_data));
    }
    return result;
}

/*****************************************************************************/
RnObject* RnArrayObject::operator!=(RnObject* obj) {
    auto result = RnObject::Create(RnType::RN_BOOLEAN);
    if (obj->GetType() != RnType::RN_ARRAY) {
        result->SetData(true);
    } else {
        result->SetData(!dynamic_cast<RnArrayObject*>(obj)->ContentsEqual(_data));
    }
    return result;
}

/*****************************************************************************/
RnObject* RnArrayObject::operator/(RnObject* obj) {
    return nullptr;
}

/*****************************************************************************/
RnObject* RnArrayObject::operator|(RnObject* obj) {
    return nullptr;
}

/*****************************************************************************/
RnObject* RnArrayObject::operator&(RnObject* obj) {
    return nullptr;
}

/*****************************************************************************/
void RnArrayObject::Append(RnObject* obj) {
    _data.push_back(obj);
}

/*****************************************************************************/
void RnArrayObject::SetData(RnStringNative data) {
    RnObjectBase::SetData(data);
}

/*****************************************************************************/
void RnArrayObject::SetData(RnIntNative data) {
    RnObjectBase::SetData(data);
}

/*****************************************************************************/
void RnArrayObject::SetData(RnFloatNative data) {
    RnObjectBase::SetData(data);
}

/*****************************************************************************/
void RnArrayObject::SetData(const RnArrayNative& data) {
    SetDataInternal(data);
}

/*****************************************************************************/
void RnArrayObject::SetData(RnObject* data) {
    if (data->GetType() == RnType::RN_ARRAY) {
        auto list_obj = dynamic_cast<RnArrayObject*>(data);
        SetDataInternal(list_obj->GetData());
    } else {
        throw std::runtime_error(
            "Cannot assign value of type '" + RnType::TypeToString(data->GetType()) +
            "' to symbol of type '" + RnType::TypeToString(GetType()) + "'");
    }
}

/*****************************************************************************/
RnStringNative RnArrayObject::ToString() const {
    RnStringNative s;
    for (auto item : GetData()) {
        if (item->GetActiveType() == RnType::RN_STRING) {
            s.append("\'" + item->ToString() + "\', ");
        } else {
            s.append(item->ToString() + ", ");
        }
    }

    return "[" + s.substr(0, s.length() - 2) + "]";
}

/*****************************************************************************/
RnArrayNative RnArrayObject::ToArray() const {
    return GetData();
}

/*****************************************************************************/
RnBoolNative RnArrayObject::ToBool() const {
    return !_data.empty();
}

/*****************************************************************************/
RnBoolNative RnArrayObject::Contains(RnObject* obj) const {
    for (auto item : _data) {
        if (ValueCompare(item, obj)) {
            return true;
        }
    }
    return false;
}

/*****************************************************************************/
bool RnArrayObject::ContentsEqual(const RnArrayNative& data) {
    if (_data.size() != data.size()) {
        return false;
    } else {
        for (size_t i = 0; i < _data.size(); i++) {
            if (_data[i] != data[i]) {
                return false;
            }
        }
    }
    return true;
}

/*****************************************************************************/
RnObject* RnArrayObject::At(RnIntNative index) {
    return _data.at(index);
}

/*****************************************************************************/
void RnArrayObject::Mark() {
    RnObject::Mark();
    for (auto obj : _data) {
        obj->Mark();
    }
}