/*****************************************************************************
* File: RnAllocator.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/17/23
* Version: 1
*
*****************************************************************************/

#include "RnAllocator.h"
#include <cassert>
#include <stdexcept>

/*****************************************************************************/
void RnAllocator::FreeAllHeaps() const
{
	auto current = FirstHeap();
	MemoryHeap* prev_heap = nullptr;
	while (current)
	{
		std::free(current->memory);
		prev_heap = current->prev;
		current = current->next;
	}
	while (prev_heap)
	{
		std::free(prev_heap->next);
		prev_heap = prev_heap->prev;
	}
	std::free(prev_heap);
}

/*****************************************************************************/
MemoryHeap* RnAllocator::AddNewHeap(size_t n) const
{
	auto* heap = new MemoryHeap;
	auto last_heap = LastHeap();

	if (last_heap)
	{
		assert(!last_heap->next);
		last_heap->next = heap;
		heap = last_heap->next;
	}
	else
	{
		assert(!FirstHeap() && !CurrentHeap());
	}
	heap->next = nullptr;
	heap->prev = nullptr;
	heap->state = HeapState::EMPTY;
	heap->memory = new char[n];

	if (!heap->memory)
	{
		throw std::runtime_error("Bad malloc during heap creation");
	}
	heap->size = n;

	return heap;
}

/*****************************************************************************/
bool RnAllocator::IsAddressWithinHeap(void* addr, MemoryHeap* heap)
{
	return addr >= heap->memory && addr <= (heap->memory + heap->size);
}

/*****************************************************************************/
bool RnAllocator::IsAddressWithinAllocator(void* addr) const
{
	auto heap = GetHeapForAddress(addr);
	if (heap)
	{
		return true;
	}
	return false;
}

/*****************************************************************************/
MemoryHeap* RnAllocator::GetHeapForAddress(void* addr) const
{
	auto current = FirstHeap();
	while (current)
	{
		if (IsAddressWithinHeap(addr, current))
		{
			return current;
		}
		current = current->next;
	}
	return nullptr;
}

/*****************************************************************************/
size_t RnAllocator::GetTotalMemorySize() const
{
	size_t memory_sum = 0;
	auto current = FirstHeap();
	while (current)
	{
		memory_sum += current->size;
		current = current->next;
	}
	return memory_sum;
}