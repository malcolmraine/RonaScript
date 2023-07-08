/*****************************************************************************
* File: SlabAllocator.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/16/23
* Version: 1
*
*****************************************************************************/

#include "RnSlabAllocator.h"

/*****************************************************************************/
RnSlabAllocator::RnSlabAllocator(size_t object_size, size_t heap_size, size_t max_size)
	: RnLinearAllocator(heap_size, max_size), _object_size(object_size)
{
}

/*****************************************************************************/
RnSlabAllocator::~RnSlabAllocator() = default;

/*****************************************************************************/
void* RnSlabAllocator::Malloc(size_t n)
{
	return RnLinearAllocator::Malloc(n * _object_size);
}

/*****************************************************************************/
void* RnSlabAllocator::Calloc(size_t n, int c)
{
	return RnLinearAllocator::Calloc(n * _object_size, c);
}

/*****************************************************************************/
void* RnSlabAllocator::Realloc(void* data, size_t n)
{
	return RnLinearAllocator::Realloc(data, n * _object_size);
}

/*****************************************************************************/
void RnSlabAllocator::Free(void* addr)
{
	RnLinearAllocator::Free(addr);
}
