/*****************************************************************************
* File: RnClass.h
* Description:
* Author: Malcolm Hall
* Date: 6/29/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNCLASS_H
#define RONASCRIPT_RNCLASS_H

#include "RnScope.h"

// This is a class declaration
class RnClass : public RnScope
{
 public:
	RnClass(RnScope* parent);
	~RnClass();
	void CopySymbols(RnScope* target);

	[[nodiscard]] std::string GetName() const
	{
		return _name;
	}

	void SetName(const std::string& name)
	{
		_name = name;
	}

 private:
	std::string _name;
};

#endif //RONASCRIPT_RNCLASS_H