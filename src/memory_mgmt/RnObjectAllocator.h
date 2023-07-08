/*****************************************************************************
* File: RnObjectAllocator.h
* Description:
* Author: Malcolm Hall
* Date: 6/17/23
* Version: 1
*
*****************************************************************************/

#pragma once

#include "RnSlabAllocator.h"

class RnObjectAllocator
{
 public:
	RnObjectAllocator();
	~RnObjectAllocator();

	template<class T>
	T CreateObject() const;



 // Need to register sub-allocators for each registered object type
};
