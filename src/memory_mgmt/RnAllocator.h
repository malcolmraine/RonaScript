/*****************************************************************************
* File: RnAllocator.h
* Description:
* Author: Malcolm Hall
* Date: 6/17/23
* Version: 1
*
*****************************************************************************/

#pragma once

#include <cstdlib>

// Switch to builtin malloc/calloc/realloc
#define USE_STD_MALLOC 0

enum HeapState : uint8_t {
	EMPTY,
	PARTIAL,
	FULL
};

struct MemoryHeap {
	~MemoryHeap() = default;
	size_t size = 0;
	HeapState state = HeapState::EMPTY;
	char* memory = nullptr;
	MemoryHeap* next = nullptr;
	MemoryHeap* prev = nullptr;
};

class RnAllocator {
 public:
	[[nodiscard]] virtual void* Malloc(size_t n) = 0;
	[[nodiscard]] virtual void* Calloc(size_t n, int c) = 0;
	[[nodiscard]] virtual void* Calloc(size_t n) {
		return Calloc(n, 0);
	}
	[[nodiscard]] virtual void* Realloc(void* data, size_t n) = 0;
	virtual void Free(void* addr) = 0;
	[[nodiscard]] virtual size_t GetBytesInUse() const = 0;
	[[nodiscard]] virtual size_t GetBytesFree() const = 0;
	[[nodiscard]] virtual MemoryHeap* CurrentHeap() const = 0;
	[[nodiscard]] virtual MemoryHeap* LastHeap() const = 0;
	[[nodiscard]] virtual MemoryHeap* FirstHeap() const = 0;
	void FreeAllHeaps() const;
	[[nodiscard]] MemoryHeap* AddNewHeap(size_t n) const;
	static bool IsAddressWithinHeap(void* addr, MemoryHeap* heap) ;
	bool IsAddressWithinAllocator(void* addr) const;
	MemoryHeap* GetHeapForAddress(void* addr) const;
	[[nodiscard]] size_t GetTotalMemorySize() const;
	virtual void SetMaxSize(size_t n) = 0;
	virtual size_t GetMaxSize() const = 0;
};
