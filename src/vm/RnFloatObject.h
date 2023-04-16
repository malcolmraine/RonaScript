/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNFLOATOBJECT_H
#define RONASCRIPT_RNFLOATOBJECT_H

#include "RnNumericObject.h"

class RnFloatObject : public RnNumericObject {
public:
    explicit RnFloatObject(RnFloatNative data = 0.0);
    ~RnFloatObject() override;

    auto operator+(RnObject* obj) -> RnObject* override {
        return RnObject::Create(
            static_cast<RnFloatNative>(_data.d_data + obj->ToFloat()));
    }

    auto operator-(RnObject* obj) -> RnObject* override {
        return RnObject::Create(
            static_cast<RnFloatNative>(_data.d_data - obj->ToFloat()));
    }

    auto operator==(RnObject* obj) -> RnObject* override {
        return RnObject::Create(static_cast<bool>(_data.d_data == obj->ToFloat()));
    }

    auto operator!=(RnObject* obj) -> RnObject* override {
        return RnObject::Create(static_cast<bool>(_data.d_data != obj->ToFloat()));
    }

    auto operator/(RnObject* obj) -> RnObject* override {
        return RnObject::Create(
            static_cast<RnFloatNative>(_data.d_data / obj->ToFloat()));
    }

    auto operator||(RnObject* obj) -> RnObject* override {
        return RnObject::Create(static_cast<bool>(ToBool() || obj->ToBool()));
    }

    auto operator&&(RnObject* obj) -> RnObject* override {
        return RnObject::Create(static_cast<bool>(ToBool() && obj->ToBool()));
    }

    auto operator>(RnObject* obj) -> RnObject* override {
        return RnObject::Create(static_cast<bool>(_data.d_data > obj->ToFloat()));
    }

    auto operator<(RnObject* obj) -> RnObject* override {
        return RnObject::Create(static_cast<bool>(_data.d_data < obj->ToFloat()));
    }

    auto operator>=(RnObject* obj) -> RnObject* override {
        return RnObject::Create(static_cast<bool>(_data.d_data >= obj->ToFloat()));
    }

    auto operator<=(RnObject* obj) -> RnObject* override {
        return RnObject::Create(static_cast<bool>(_data.d_data <= obj->ToFloat()));
    }

    auto operator*(RnObject* obj) -> RnObject* override {
        return RnObject::Create(
            static_cast<RnFloatNative>(_data.d_data * obj->ToFloat()));
    }

    auto operator%(RnObject* obj) -> RnObject* override;
    [[nodiscard]] auto ToString() const -> RnStringNative override;

    [[nodiscard]] auto ToInt() const -> RnIntNative override {
        return static_cast<RnIntNative>(_data.d_data);
    }
    [[nodiscard]] auto ToBool() const -> RnBoolNative override {
        return static_cast<bool>(_data.d_data);
    }

    void SetData(RnIntNative data) override {
        ConstInitCheck();
        _data.d_data = static_cast<RnFloatNative>(data);
    }

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_FLOAT;
    }
};

#endif  //RONASCRIPT_RNFLOATOBJECT_H