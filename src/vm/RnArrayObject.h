/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNLISTOBJECT_H
#define RONASCRIPT_RNLISTOBJECT_H

#include <vector>
#include "RnObjectBase.h"

class RnArrayObject : public RnObjectBase<RnArrayNative> {
public:
    RnArrayObject();
    RnArrayObject(const RnArrayNative& data);
    ~RnArrayObject();

    RnObject* operator+(RnObject* obj) override;
    RnObject* operator-(RnObject* obj) override;
    RnObject* operator==(RnObject* obj) override;
    RnObject* operator!=(RnObject* obj) override;
    RnObject* operator/(RnObject* obj) override;
    RnObject* operator|(RnObject* obj) override;
    RnObject* operator&(RnObject* obj) override;
    void Append(RnObject* obj);
    void SetData(RnStringNative data) override;
    void SetData(RnIntNative data) override;
    void SetData(RnFloatNative data) override;
    void SetData(RnArrayNative data) override;
    void SetData(RnObject* data) override;
    [[nodiscard]] RnStringNative ToString() const override;
    [[nodiscard]] RnArrayNative ToArray() const override;
    [[nodiscard]] RnBoolNative ToBool() const override;

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_ARRAY;
    }
};

#endif  //RONASCRIPT_RNLISTOBJECT_H