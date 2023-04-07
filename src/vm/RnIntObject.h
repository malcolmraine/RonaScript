/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNINTOBJECT_H
#define RONASCRIPT_RNINTOBJECT_H

#include "RnNumericObject.h"

class RnIntObject : public RnNumericObject {
public:
    explicit RnIntObject(RnIntNative data = 0);
    ~RnIntObject() override;
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
    [[nodiscard]] RnFloatNative ToFloat() const override;
    [[nodiscard]] RnStringNative ToString() const override;
    [[nodiscard]] RnArrayNative ToArray() const override;
    [[nodiscard]] RnBoolNative ToBool() const override;
    void SetData(RnFloatNative data) override;

    void SetData(RnObject* data) override {
        SetData(data->ToInt());
    }

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_INT;
    }
};

#endif  //RONASCRIPT_RNINTOBJECT_H