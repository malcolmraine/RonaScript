//
// Created by Malcolm Hall on 3/29/23.
//

#ifndef RONASCRIPT_SRC_VM_RNNUMERICOBJECT_H_
#define RONASCRIPT_SRC_VM_RNNUMERICOBJECT_H_

#include "RnObjectBase.h"

class RnNumericObject : public RnObjectBase<RnNumericUnion> {
public:
    [[nodiscard]] RnIntNative ToInt() const override;
    [[nodiscard]] RnFloatNative ToFloat() const override;
    [[nodiscard]] RnBoolNative ToBool() const override;
    void SetData(RnIntNative data) override;
    void SetData(RnFloatNative data) override;
    void SetData(RnBoolNative data) override;
    [[nodiscard]] size_t GetByteSize() const override;
    size_t GetBytes(char* buf) override;
    void SetBytes(const char* buf, size_t n) override;
};

#endif  //RONASCRIPT_SRC_VM_RNNUMERICOBJECT_H_