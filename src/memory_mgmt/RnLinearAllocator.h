/*****************************************************************************
* File: RnLinearAllocator.h
* Description:
* Author: Malcolm Hall
* Date: 6/16/23
* Version: 1
*
*****************************************************************************/

#pragma once

#include "RnAllocator.h"
#include <cstdlib>

struct MemoryBlock {
	uint32_t size = 0;
	uint8_t available = 1;
};

#define MIN_BLOCK_SIZE 8
#define NEXT_BLOCK(x) (reinterpret_cast<MemoryBlock*>((reinterpret_cast<char*>(x) + sizeof(MemoryBlock) + x->size)))
#define TOTAL_BLOCK_SIZE(block) (block->size + sizeof(MemoryBlock))
#define BLOCK_MEMORY_ADDR(block) (reinterpret_cast<void*>(reinterpret_cast<char*>(block) + sizeof(block)))

class RnLinearAllocator : public RnAllocator
{
 public:
	explicit RnLinearAllocator(size_t heap_size, size_t max_size);
	~RnLinearAllocator();
	[[nodiscard]] void* Malloc(size_t n) override;
	[[nodiscard]] void* Calloc(size_t n, int c) override;
	[[nodiscard]] void* Realloc(void* data, size_t n) override;
	void Free(void* addr) override;
	static void SetInstance(RnLinearAllocator* instance);
	static RnLinearAllocator* Instance();

	/*************************************************************************/
	[[nodiscard]] inline size_t GetBytesInUse() const override
	{
		return _bytes_in_use;
	}

	/*************************************************************************/
	[[nodiscard]] inline size_t GetBytesFree() const override
	{
		return GetTotalMemorySize() - _bytes_in_use;
	}

	/*************************************************************************/
	[[nodiscard]] MemoryHeap* CurrentHeap() const override
	{
		return _current_heap;
	}

	/*************************************************************************/
	[[nodiscard]] MemoryHeap* LastHeap() const override
	{
		return _last_heap;
	}

	/*************************************************************************/
	[[nodiscard]] MemoryHeap* FirstHeap() const override
	{
		return _first_heap;
	}

	/*************************************************************************/
	void SetMaxSize(size_t n) override
	{
		_max_size = n;
	}

	/*************************************************************************/
	[[nodiscard]] size_t GetMaxSize() const override
	{
		return _max_size;
	}

 private:
	[[nodiscard]] void* MallocInternal(size_t n, MemoryBlock* start);
	static bool SplitBlock(MemoryBlock* block, size_t n);
	MemoryBlock* GetMergedBlockUntil(MemoryBlock* block, size_t n);

	/*************************************************************************/
	[[nodiscard]] static inline MemoryBlock* GetBlockForAddr(void* addr)
	{
		return (MemoryBlock*)((char*)addr - sizeof(MemoryBlock));
	}

 private:
	MemoryBlock* _first_block = nullptr; 	// First block of the current heap
	MemoryBlock* _current_block = nullptr; 	// Current block in the current heap
	MemoryHeap* _first_heap = nullptr; 		// First heap in the list
	MemoryHeap* _current_heap = nullptr; 	// Current working heap
	MemoryHeap* _last_heap = nullptr; 		// Last heap in the list
	size_t _bytes_in_use = 0;				// Total bytes in use
	size_t _blocks_in_use = 0;				// Total blocks in use
	size_t _max_size = 0;					// Max size in bytes
	static RnLinearAllocator* _instance;
};
