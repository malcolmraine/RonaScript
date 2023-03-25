/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNFUNCTIONOBJECT_H
#define RONASCRIPT_RNFUNCTIONOBJECT_H

#include "RnObjectBase.h"

class RnFunction;

class RnFunctionObject : public RnObjectBase<RnFunction*> {
public:
    RnFunctionObject();
    RnFunctionObject(RnFunction* data);
    ~RnFunctionObject() override;
    [[nodiscard]] RnStringNative ToString() const override;
    void SetData(RnFunction* data) override;

    [[nodiscard]] RnFunction* ToFunction() const override {
        return _data;
    }

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_FUNCTION;
    }

private:
    RnType::Type _return_type = RnType::RN_NULL;
};

#endif  //RONASCRIPT_RNFUNCTIONOBJECT_H