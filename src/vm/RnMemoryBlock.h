/*****************************************************************************
* File: RnMemoryBlock.h
* Description:
* Author: Malcolm Hall
* Date: 5/20/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNMEMORYBLOCK_H
#define RONASCRIPT_RNMEMORYBLOCK_H

#include "RnObject.h"

class RnMemoryBlock
{
 public:
	explicit RnMemoryBlock() = default;
	~RnMemoryBlock() = default;

	[[nodiscard]] bool IsMarked() const
	{
		return _is_marked;
	}

	void SetMarkedFlag()
	{
		_is_marked = true;
	}

	void UnsetMarkedFlag()
	{
		_is_marked = false;
	}

	void Reset()
	{
		delete _obj;
		_obj = nullptr;
		UnsetMarkedFlag();
	}

	RnObject* GetStoredObject()
	{
		return _obj;
	}

	void SetStoredObject(RnObject* obj)
	{
		_obj = obj;
	}

 protected:
//	RnMemoryBlock* _previous = nullptr;
//	RnMemoryBlock* _next = nullptr;
	RnObject* _obj = nullptr;
	bool _is_marked = false;
};

#endif //RONASCRIPT_RNMEMORYBLOCK_H