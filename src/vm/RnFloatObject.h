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

#include "RnObjectBase.h"

class RnFloatObject : public RnObjectBase<RnFloatNative> {
public:
    explicit RnFloatObject(RnFloatNative data = 0.0);
    ~RnFloatObject() override;
    auto operator+(RnObject* obj) -> RnObject* override;
    auto operator-(RnObject* obj) -> RnObject* override;
    auto operator==(RnObject* obj) -> RnObject* override;
    auto operator!=(RnObject* obj) -> RnObject* override;
    auto operator/(RnObject* obj) -> RnObject* override;
    auto operator||(RnObject* obj) -> RnObject* override;
    auto operator&&(RnObject* obj) -> RnObject* override;
    auto operator>(RnObject* obj) -> RnObject* override;
    auto operator<(RnObject* obj) -> RnObject* override;
    auto operator>=(RnObject* obj) -> RnObject* override;
    auto operator<=(RnObject* obj) -> RnObject* override;
    auto operator*(RnObject* obj) -> RnObject* override;
    auto operator%(RnObject* obj) -> RnObject* override;
    [[nodiscard]] auto ToString() const -> RnStringNative override;
    [[nodiscard]] auto ToInt() const -> RnIntNative override;
    [[nodiscard]] auto ToFloat() const -> RnFloatNative override;
    [[nodiscard]] auto ToBool() const -> RnBoolNative override;
    void SetData(RnIntNative data) override;
    void SetData(RnFloatNative data) override;

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_FLOAT;
    }
};

#endif  //RONASCRIPT_RNFLOATOBJECT_H