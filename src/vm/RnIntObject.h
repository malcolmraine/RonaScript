/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNINTOBJECT_H
#define RONASCRIPT_RNINTOBJECT_H

#include "RnNumericObject.h"

class RnIntObject : public RnNumericObject {
public:
    explicit RnIntObject(RnIntNative data = 0);
    ~RnIntObject() override;
    RnObject* operator*(RnObject* obj) override;
    RnObject* operator^(RnObject* obj) override;
    RnObject* operator%(RnObject* obj) override;
    RnObject* operator+(RnObject* obj) override;
    RnObject* operator-(RnObject* obj) override;
    RnObject* operator==(RnObject* obj) override;
    RnObject* operator!=(RnObject* obj) override;
    RnObject* operator/(RnObject* obj) override;

    RnObject* operator|(RnObject* obj) override {
        return RnObject::Create(static_cast<bool>(_data.i_data | obj->ToInt()));
    }

    RnObject* operator||(RnObject* obj) override {
        return RnObject::Create(static_cast<bool>(ToBool() || obj->ToBool()));
    }

    RnObject* operator&&(RnObject* obj) override {
        return RnObject::Create(static_cast<bool>(ToBool() && obj->ToBool()));
    }

    RnObject* operator&(RnObject* obj) override {
        return RnObject::Create(static_cast<bool>(_data.i_data & obj->ToInt()));
    }

    RnObject* operator>(RnObject* obj) override {
        return RnObject::Create(_data.i_data > obj->ToInt());
    }

    RnObject* operator<(RnObject* obj) override {
        return RnObject::Create(_data.i_data < obj->ToInt());
    }

    RnObject* operator>=(RnObject* obj) override {
        return RnObject::Create(_data.i_data >= obj->ToInt());
    }

    RnObject* operator<=(RnObject* obj) override {
        return RnObject::Create(_data.i_data <= obj->ToInt());
    }

    RnObject* operator>>(RnObject* obj) override {
        return RnObject::Create(_data.i_data >> obj->ToInt());
    }

    RnObject* operator<<(RnObject* obj) override {
        return RnObject::Create(_data.i_data << obj->ToInt());
    }

    [[nodiscard]] RnFloatNative ToFloat() const override {
        return static_cast<RnFloatNative>(_data.i_data);
    }

    [[nodiscard]] RnStringNative ToString() const override {
        return std::to_string(_data.i_data);
    }

    [[nodiscard]] RnBoolNative ToBool() const override {
        return static_cast<bool>(_data.i_data);
    }

    void SetData(RnFloatNative data) override {
        ConstInitCheck();
        _data.i_data = static_cast<RnIntNative>(data);
    }

    void SetData(RnObject* data) override {
        ConstInitCheck();
        _data.i_data = data->ToInt();
    }

    [[nodiscard]] RnType::Type GetType() const override {
        return RnType::RN_INT;
    }
};

#endif  //RONASCRIPT_RNINTOBJECT_H