/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNBOOLOBJECT_H
#define RONASCRIPT_RNBOOLOBJECT_H

#include "RnNumericObject.h"

class RnBoolObject : public RnNumericObject {
public:
    explicit RnBoolObject(RnBoolNative data = false);
    ~RnBoolObject() override;
    RnObject* operator==(RnObject* obj) override;
    RnObject* operator!=(RnObject* obj) override;
    RnObject* operator|(RnObject* obj) override;
    RnObject* operator||(RnObject* obj) override;
    RnObject* operator&&(RnObject* obj) override;
    RnObject* operator&(RnObject* obj) override;
    RnObject* operator^(RnObject* obj) override;
    [[nodiscard]] RnStringNative ToString() const override;
    [[nodiscard]] RnIntNative ToInt() const override;
    [[nodiscard]] RnBoolNative ToBool() const override;
    [[nodiscard]] RnFloatNative ToFloat() const override;

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

#endif  //RONASCRIPT_RNBOOLOBJECT_H