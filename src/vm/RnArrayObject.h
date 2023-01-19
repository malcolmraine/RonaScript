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

#include "RnObjectBase.h"
#include <vector>

class RnArrayObject : public RnObjectBase<std::vector<RnObject*>>
{
 public:
	RnArrayObject();
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
	void SetData(std::vector<RnObject*> data) override;
	void SetData(RnObject* data) override;
	[[nodiscard]] RnStringNative ToString() const override;
	[[nodiscard]] std::vector<RnObject*> ToList() const override;
	[[nodiscard]]  RnBoolNative ToBool() const override;

	[[nodiscard]] RnType::Type GetType() const override
	{
		return RnType::RN_ARRAY;
	}
};

#endif //RONASCRIPT_RNLISTOBJECT_H