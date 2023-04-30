

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
    explicit RnAnyObject(RnArrayNative data);

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
    [[nodiscard]] RnScope* ToObject() const override;
    [[nodiscard]] RnFunction* ToFunction() const override;
    void SetData(RnIntNative data) override;
    void SetData(RnBoolNative data) override;
    void SetData(RnFloatNative data) override;
    void SetData(RnArrayNative data) override;
    void SetData(RnStringNative data) override;
    void SetData(RnFunction* data) override;
    void SetData(RnScope* data) override;
    void CopyFrom(RnObject* obj);

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_ANY;
    }

    [[nodiscard]] RnType::Type GetActiveType() const override {
        return _active_type;
    }

private:
    RnType::Type _active_type = RnType::RN_NULL;
};
