//
// Created by Malcolm Hall on 3/22/23.
//

#ifndef RONASCRIPT_SRC_VM_RNANYOBJECT_H_
#define RONASCRIPT_SRC_VM_RNANYOBJECT_H_

#include <variant>
#include "RnObjectBase.h"
#include "RnIntObject.h"
#include "RnFloatObject.h"
#include "RnArrayObject.h"
#include "RnStringObject.h"
#include "RnBoolObject.h"
#include "RnFunctionObject.h"
#include "RnClassObject.h"

class RnAnyObject
    : public RnObjectBase<
          std::variant<RnIntObject, RnFloatObject, RnBoolObject, RnClassObject,
                       RnStringObject, RnArrayObject, RnFunctionObject>> {
public:
    RnAnyObject();
    explicit RnAnyObject(RnIntNative data );
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

#endif  //RONASCRIPT_SRC_VM_RNANYOBJECT_H_