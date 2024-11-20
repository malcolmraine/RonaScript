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

#pragma once

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
        return static_cast<RnIntNative>(std::llround(_data.d_data));
    }
    [[nodiscard]] auto ToBool() const -> RnBoolNative override {
        return static_cast<bool>(std::llround(_data.d_data));
    }

    void SetData(RnIntNative data) override {
        ConstInitCheck();
        _data.d_data = static_cast<RnFloatNative>(data);
    }

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_FLOAT;
    }
};
