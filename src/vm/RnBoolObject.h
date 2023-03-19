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

#include "RnObjectBase.h"

class RnBoolObject : public RnObjectBase<RnBoolNative> {
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
    void SetData(RnBoolNative data) override {
        SetDataInternal(data);
    }

    void SetData(RnIntNative data) override {
        SetDataInternal(data);
    }

    void SetData(RnFloatNative data) override {
        SetDataInternal(static_cast<bool>(data));
    }

    void SetData(RnStringNative data) override {
        SetDataInternal(static_cast<bool>(!data.empty()));
    }

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_BOOLEAN;
    }
};

#endif  //RONASCRIPT_RNBOOLOBJECT_H