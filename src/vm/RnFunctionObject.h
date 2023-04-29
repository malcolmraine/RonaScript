/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#pragma once

#include "RnObjectBase.h"

class RnFunction;

class RnFunctionObject : public RnObjectBase<RnFunction*> {
public:
    RnFunctionObject();
    explicit RnFunctionObject(RnFunction* data);
    ~RnFunctionObject() override;
    [[nodiscard]] RnStringNative ToString() const override;
    void SetData(RnFunction* data) override;

    [[nodiscard]] RnFunction* ToFunction() const override {
        return _data;
    }

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_FUNCTION;
    }
};
