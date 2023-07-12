/*****************************************************************************
* File: SlabAllocator.h
* Description:
* Author: Malcolm Hall
* Date: 6/16/23
* Version: 1
*
*****************************************************************************/

#pragma once

#include "RnLinearAllocator.h"

class RnSlabAllocator : public RnLinearAllocator {
 public:
	explicit RnSlabAllocator(size_t object_size, size_t heap_size, size_t max_size);
	~RnSlabAllocator();
	[[nodiscard]] void* Malloc(size_t n) override;
	[[nodiscard]] void* Calloc(size_t n, int c) override;
	[[nodiscard]] void* Realloc(void* data, size_t n) override;
	void Free(void* addr) override;

protected:
	size_t _object_size = 0;
};
