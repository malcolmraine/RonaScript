/*****************************************************************************
* File: RnAllocator.h
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
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

#include "RnAllocator.h"
#include <cassert>
#include <stdexcept>

/*****************************************************************************/
void RnAllocator::FreeAllHeaps() const {
    auto current = FirstHeap();
    MemoryHeap* prev_heap = nullptr;
    while (current) {
        std::free(current->memory);
        prev_heap = current->prev;
        current = current->next;
    }
    while (prev_heap) {
        std::free(prev_heap->next);
        prev_heap = prev_heap->prev;
    }
    std::free(prev_heap);
}

/*****************************************************************************/
MemoryHeap* RnAllocator::AddNewHeap(size_t n) const {
    auto* heap = new MemoryHeap;
    heap->next = nullptr;
    heap->prev = nullptr;
    auto last_heap = LastHeap();

    if (last_heap) {
        assert(!last_heap->next);
        last_heap->next = heap;
        heap->prev = last_heap;
    } else {
        assert(!FirstHeap() && !CurrentHeap());
    }
    heap->state = HeapState::EMPTY;
    heap->memory = new char[n];

    if (!heap->memory) {
        throw std::runtime_error("Bad malloc during heap creation");
    }
    heap->size = n;

    return heap;
}

/*****************************************************************************/
bool RnAllocator::IsAddressWithinHeap(void* addr, MemoryHeap* heap) {
    return addr >= heap->memory && addr <= (heap->memory + heap->size);
}

/*****************************************************************************/
bool RnAllocator::IsAddressWithinAllocator(void* addr) const {
    auto heap = GetHeapForAddress(addr);
    if (heap) {
        return true;
    }
    return false;
}

/*****************************************************************************/
MemoryHeap* RnAllocator::GetHeapForAddress(void* addr) const {
    auto current = FirstHeap();
    while (current) {
        if (IsAddressWithinHeap(addr, current)) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

/*****************************************************************************/
size_t RnAllocator::GetTotalMemorySize() const {
    size_t memory_sum = 0;
    auto current = FirstHeap();
    while (current) {
        memory_sum += current->size;
        current = current->next;
    }
    return memory_sum;
}