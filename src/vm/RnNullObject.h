/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNNULLOBJECT_H
#define RONASCRIPT_RNNULLOBJECT_H

#include "RnObjectBase.h"

class RnNullObject : public RnObjectBase<void*> {
public:
    static RnNullObject* Instance();
    RnNullObject();
    ~RnNullObject() override;
    [[nodiscard]] auto ToString() const -> RnStringNative override;
    [[nodiscard]] auto ToBool() const -> RnBoolNative override;
    [[nodiscard]] auto ToFloat() const -> RnFloatNative override;
    [[nodiscard]] auto ToInt() const -> RnIntNative override;

    [[nodiscard]] RnType::Type GetType() const override { return RnType::RN_NULL; }
};

#endif  //RONASCRIPT_RNNULLOBJECT_H