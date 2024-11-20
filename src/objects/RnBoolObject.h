/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#pragma once

#include "RnNumericObject.h"

class RnBoolObject : public RnNumericObject {
public:
    explicit RnBoolObject(RnBoolNative data = false);
    ~RnBoolObject() override;

    RnObject* operator==(RnObject* obj) override {
        return RnObject::Create(static_cast<bool>(
            static_cast<RnBoolNative>(_data.i_data) == obj->ToBool()));
    }

    RnObject* operator!=(RnObject* obj) override {
        return RnObject::Create(static_cast<bool>(
            static_cast<RnBoolNative>(_data.i_data) != obj->ToBool()));
    }

    RnObject* operator|(RnObject* obj) override {
        return RnObject::Create(
            static_cast<bool>(static_cast<RnBoolNative>(_data.i_data) | obj->ToBool()));
    }

    RnObject* operator||(RnObject* obj) override {
        return RnObject::Create(static_cast<bool>(
            static_cast<RnBoolNative>(_data.i_data) || obj->ToBool()));
    }

    RnObject* operator&&(RnObject* obj) override {
        return RnObject::Create(static_cast<bool>(
            static_cast<RnBoolNative>(_data.i_data) && obj->ToBool()));
    }
    RnObject* operator&(RnObject* obj) override {
        return RnObject::Create(
            static_cast<bool>(static_cast<RnBoolNative>(_data.i_data) & obj->ToBool()));
    }
    RnObject* operator^(RnObject* obj) override {
        return RnObject::Create(
            static_cast<bool>(static_cast<RnBoolNative>(_data.i_data) ^ obj->ToBool()));
    }

    [[nodiscard]] RnStringNative ToString() const override {
        return static_cast<RnBoolNative>(_data.i_data) ? "true" : "false";
    }

    [[nodiscard]] RnIntNative ToInt() const override {
        return static_cast<RnBoolNative>(_data.i_data) ? 1 : 0;
    }

    [[nodiscard]] RnBoolNative ToBool() const override {
        return static_cast<RnBoolNative>(_data.i_data);
    }

    [[nodiscard]] RnFloatNative ToFloat() const override {
        return static_cast<RnBoolNative>(_data.i_data) ? 1.0 : 0.0;
    }

    void SetData(RnFloatNative data) override {
        ConstInitCheck();
        _data.i_data = static_cast<bool>(data) ? 1 : 0;
    }

    void SetData(RnStringNative data) override {
        ConstInitCheck();
        _data.i_data = !data.empty() ? 1 : 0;
    }

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_BOOLEAN;
    }
};
