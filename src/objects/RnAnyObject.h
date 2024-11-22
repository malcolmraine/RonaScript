/*****************************************************************************
* File: RnAnyObject.h
* Description:
* Author: Malcolm Hall
* Date: 3/22/23
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
*****************************************************************************/

#pragma once

#include <variant>
#include "RnArrayObject.h"
#include "RnBoolObject.h"
#include "RnClassObject.h"
#include "RnFloatObject.h"
#include "RnFunctionObject.h"
#include "RnIntObject.h"
#include "RnObjectBase.h"
#include "RnStringObject.h"

class RnAnyObject
    : public RnObjectBase<
          std::variant<RnIntObject, RnFloatObject, RnBoolObject, RnClassObject,
                       RnStringObject, RnArrayObject, RnFunctionObject>> {
public:
    RnAnyObject();
    explicit RnAnyObject(RnIntNative data);
    explicit RnAnyObject(RnFloatNative data);
    explicit RnAnyObject(RnBoolNative data);
    explicit RnAnyObject(RnStringNative data);
    explicit RnAnyObject(RnFunction* data);
    explicit RnAnyObject(RnScope* data);
    explicit RnAnyObject(const RnArrayNative& data);

    ~RnAnyObject() override;
    RnObject* operator+(RnObject* obj) override;
    RnObject* operator-(RnObject* obj) override;
    RnObject* operator==(RnObject* obj) override;
    RnObject* operator!=(RnObject* obj) override;
    RnObject* operator/(RnObject* obj) override;
    RnObject* operator|(RnObject* obj) override;
    RnObject* operator||(RnObject* obj) override;
    RnObject* operator&&(RnObject* obj) override;
    RnObject* operator&(RnObject* obj) override;
    RnObject* operator^(RnObject* obj) override;
    RnObject* operator>(RnObject* obj) override;
    RnObject* operator<(RnObject* obj) override;
    RnObject* operator>=(RnObject* obj) override;
    RnObject* operator<=(RnObject* obj) override;
    RnObject* operator%(RnObject* obj) override;
    RnObject* operator>>(RnObject* obj) override;
    RnObject* operator<<(RnObject* obj) override;
    RnObject* operator*(RnObject* obj) override;
    [[nodiscard]] RnIntNative ToInt() const override;
    [[nodiscard]] RnFloatNative ToFloat() const override;
    [[nodiscard]] RnStringNative ToString() const override;
    [[nodiscard]] RnArrayNative ToArray() const override;
    [[nodiscard]] RnBoolNative ToBool() const override;
    [[nodiscard]] RnScope* ToScope() const override;
    [[nodiscard]] RnFunction* ToFunction() const override;
    void SetData(RnIntNative data) override;
    void SetData(RnBoolNative data) override;
    void SetData(RnFloatNative data) override;
    void SetData(const RnArrayNative& data) override;
    void SetData(RnStringNative data) override;
    void SetData(RnFunction* data) override;
    void SetData(RnScope* data) override;
    void CopyFrom(RnObject* obj);
    RnObject* At(RnIntNative index) override;
    RnBoolNative IsActiveDataEqual(const RnObject* obj) const;
    RnBoolNative Contains(RnObject* obj) const;

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_ANY;
    }

    [[nodiscard]] RnType::Type GetActiveType() const override {
        return _active_type;
    }

private:
    RnType::Type _active_type = RnType::RN_NULL;
};
