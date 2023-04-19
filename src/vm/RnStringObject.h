/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNSTRINGOBJECT_H
#define RONASCRIPT_RNSTRINGOBJECT_H

#include <string>
#include "RnObjectBase.h"

class RnStringObject : public RnObjectBase<RnStringNative> {
public:
    explicit RnStringObject(const RnStringNative& data = "");
    ~RnStringObject() override;

    RnObject* operator+(RnObject* obj) override;
    RnObject* operator==(RnObject* obj) override;
    RnObject* operator!=(RnObject* obj) override;
    RnObject* operator*(RnObject* obj) override;
    [[nodiscard]] RnStringNative ToString() const override;
    [[nodiscard]] RnBoolNative ToBool() const override;
    void SetData(RnStringNative data) override;
    [[nodiscard]] size_t GetByteSize() const override;
    size_t GetBytes(char* buf) override;
    void SetBytes(const char* buf, size_t n) override;

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_STRING;
    }
};

#endif  //RONASCRIPT_RNSTRINGOBJECT_H