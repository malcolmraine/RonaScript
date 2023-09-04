/*****************************************************************************
* File: RnLinearAllocator.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2021 Malcolm Hall
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************/

#include "RnLinearAllocator.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

/*****************************************************************************/
RnLinearAllocator* RnLinearAllocator::_instance = nullptr;

/*****************************************************************************/
void RnLinearAllocator::SetInstance(RnLinearAllocator* instance) {
    _instance = instance;
}

/*****************************************************************************/
RnLinearAllocator* RnLinearAllocator::Instance() {
    return _instance;
}

/*****************************************************************************/
RnLinearAllocator::RnLinearAllocator(size_t heap_size, size_t max_size)
    : _max_size(max_size) {
    _max_size = max_size;
    _current_heap = AddNewHeap(heap_size);
    _first_heap = _current_heap;
    _last_heap = _current_heap;
    _first_block = (MemoryBlock*)_current_heap->memory;
    _first_block->size = _current_heap->size - sizeof(MemoryBlock);
    _first_block->available = 1;
    _current_block = _first_block;
}

/*****************************************************************************/
RnLinearAllocator::~RnLinearAllocator() {
    FreeAllHeaps();
}

/*****************************************************************************/
void* RnLinearAllocator::Malloc(size_t n) {
#if USE_STD_MALLOC == 1
    return std::malloc(n);
#else
    if (n > _current_heap->size) {
        throw std::runtime_error("Memory allocation request (" + std::to_string(n) +
                                 " bytes) is larger than the allowed heap size.");
    }
    if (n == 0) {
        return nullptr;
    }

    // First attempt to allocate starting from the current block, then from the
    // first block
    auto result = MallocInternal(n, _current_block);
    if (!result) {
        result = MallocInternal(n, _first_block);
    }

    // If the current heap isn't going to work, maybe we can get another heap
    if (!result && (GetTotalMemorySize() < _max_size)) {
        //		std::cout << "Adding a new heap...\n";
        _current_heap = AddNewHeap(_current_heap->size);
        _last_heap = _current_heap;
        _first_block = (MemoryBlock*)_current_heap->memory;
        _first_block->size = _current_heap->size - sizeof(MemoryBlock);
        _first_block->available = 1;
        _current_block = _first_block;
        result = RnLinearAllocator::Malloc(n);  // Only attempt to malloc once here
    }
    return result;
#endif
}

/*****************************************************************************/
void* RnLinearAllocator::Calloc(size_t n, int c) {
#if USE_STD_MALLOC == 1
    return std::calloc(n, c);
#else
    // Allocate memory of size n and initialize to 0
    auto result = Malloc(n);
    if (result) {
        std::memset(result, c, n);
    }
    return result;
#endif
}

/*************************************************************************/
void* RnLinearAllocator::Realloc(void* data, size_t n) {
#if USE_STD_MALLOC == 1
    return std::realloc(data, n);
#else
    if (!data) {
        return Malloc(n);
    }

    auto old_block = GetBlockForAddr(data);
    // First try to expand or contract the current block
    if (old_block->size == n) {
        return BLOCK_MEMORY_ADDR(old_block);
    } else if (old_block->size > n) {
        // If the old block is bigger than the requested size, attempt
        // to split it
        SplitBlock(old_block, n);
        return BLOCK_MEMORY_ADDR(old_block);
    } else if (old_block->size < n && NEXT_BLOCK(old_block)->available) {
        // Attempt to merge adjacent free blocks until we have one that is
        // big enough
        size_t old_block_size = TOTAL_BLOCK_SIZE(old_block);
        auto candidate_block = GetMergedBlockUntil(NEXT_BLOCK(old_block), n);
        if (candidate_block->size >= n) {
            _bytes_in_use -= old_block_size;
            _bytes_in_use += TOTAL_BLOCK_SIZE(candidate_block);
            return BLOCK_MEMORY_ADDR(candidate_block);
        }
    }

    // If using the current block doesn't work, move the contents to a new block
    auto new_block = Malloc(old_block->size);
    std::memcpy(new_block, data, old_block->size);
    Free(data);
    return BLOCK_MEMORY_ADDR(new_block);
#endif
}

/*****************************************************************************/
void RnLinearAllocator::Free(void* addr) {
#if USE_STD_MALLOC == 1
    return std::free(addr);
#else
    if (!IsAddressWithinAllocator(addr)) {
        throw std::runtime_error("Out of bounds memory deallocation");
    }

    auto* block = GetBlockForAddr(addr);
    block->available = 1;
    _blocks_in_use--;
    _bytes_in_use -= TOTAL_BLOCK_SIZE(block);

    // If we can, we should try to merge the next block if it's free to help
    // reduce fragmentation
    auto merged_block = GetMergedBlockUntil(block, 0);
    if (GetHeapForAddress(addr) == _current_heap) {
        _current_block = merged_block;
    }
#endif
}

/*****************************************************************************/
[[nodiscard]] void* RnLinearAllocator::MallocInternal(size_t n, MemoryBlock* start) {
    auto current = start;
    while (current) {
        if (!current->available || current->size < n) {
            if (IsAddressWithinHeap(NEXT_BLOCK(current), _current_heap)) {
                current = NEXT_BLOCK(current);
            } else {
                return nullptr;
            }
        }
        if (!current) {
            return nullptr;
        }

        if (current->size >= n && current->available) {
            // Separate into two blocks
            SplitBlock(current, n);
            current->available = 0;
            _current_block = NEXT_BLOCK(current);
            _blocks_in_use++;
            _bytes_in_use += TOTAL_BLOCK_SIZE(current);
            return BLOCK_MEMORY_ADDR(current);
        }
    }
    return nullptr;
}

/*****************************************************************************/
bool RnLinearAllocator::SplitBlock(MemoryBlock* block, size_t n) {
    // Split a memory block of size M bytes into two separate blocks of size n and
    // M - n. No split occurs if the second block size would be less than the
    // minimum required size
    if ((block->size - n) > MIN_BLOCK_SIZE) {
        auto size_diff = block->size - n - sizeof(MemoryBlock);
        block->size = n;
        auto next = NEXT_BLOCK(block);
        next->available = 1;
        next->size = size_diff;
        return true;
    }
    return false;
}

/*****************************************************************************/
MemoryBlock* RnLinearAllocator::GetMergedBlockUntil(MemoryBlock* block, size_t n) {
    // Merge the block with the next blocks if available until a memory block of
    // up to n bytes is created. If n == 0, keep merging until next unavailable
    // block
    auto current = NEXT_BLOCK(block);
    while (current && IsAddressWithinHeap(current, _current_heap)) {
        if (!current->available) {
            break;
        }

        block->size += TOTAL_BLOCK_SIZE(current);
        if (n > 0 && current->size >= n) {
            break;
        }
        current = NEXT_BLOCK(current);
    }
    return block;
}