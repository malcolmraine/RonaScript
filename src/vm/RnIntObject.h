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

#include "RnObjectBase.h"

class RnIntObject : public RnObjectBase<RnIntNative>
{
 public:
	explicit RnIntObject(RnIntNative data = 0);
	~RnIntObject() override;
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

	inline RnObject* operator<(RnObject* obj) override
	{
		return RnObject::Create(GetData() < obj->ToInt());
	}
	RnObject* operator>=(RnObject* obj) override;
	RnObject* operator<=(RnObject* obj) override;
	RnObject* operator%(RnObject* obj) override;
	RnObject* operator>>(RnObject* obj) override;
	RnObject* operator<<(RnObject* obj) override;
	RnObject* operator*(RnObject* obj) override;
	[[nodiscard]] RnIntNative ToInt() const override;
	[[nodiscard]] RnFloatNative ToFloat() const override;
	[[nodiscard]] RnStringNative ToString() const override;
	[[nodiscard]] std::vector<RnObject*> ToList() const override;
	[[nodiscard]]  RnBoolNative ToBool() const override;
	void SetData(RnIntNative data) override;
	void SetData(RnFloatNative data) override;

	void SetData(RnObject* data) override
	{
		SetData(data->ToInt());
	}

	[[nodiscard]] RnType::Type GetType() const override
	{
		return RnType::RN_INT;
	}

};

#endif //RONASCRIPT_RNINTOBJECT_H