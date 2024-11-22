/*****************************************************************************
* File: RnMetaDataObject.h
* Description:
* Author: Malcolm Hall
* Date: 11/22/24
* Version: 1
*
*****************************************************************************/

#pragma once

#include "RnObjectBase.h"

class RnPackedObject : public RnObjectBase<RnObject*> {
public:
    RnPackedObject();
    ~RnPackedObject() override;
    void UnpackToStack(RnArrayNative& stack) const;

    RnObject* At(RnIntNative index) override {
        return _data->At(index);
    }

    [[nodiscard]] RnIntNative GetDataItemCount() const override {
        return _data->GetDataItemCount();
    }

    void SetData(RnObject* data) override {
        _data = data;
    }

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_OBJECT_PACK;
    }
};
