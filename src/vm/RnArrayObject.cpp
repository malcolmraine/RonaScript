/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
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
void RnArrayObject::SetData(RnArrayNative data) {
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
    std::string s;
    for (auto& item : GetData()) {
        s.append(item->ToString() + ", ");
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
