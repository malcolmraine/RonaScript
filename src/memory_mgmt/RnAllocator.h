/*****************************************************************************
* File: RnAllocator.h
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

#pragma once

#include <cstdlib>
#include "../common/RnBuildInfo.h"

enum HeapState : uint8_t { EMPTY, PARTIAL, FULL };

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
    static bool IsAddressWithinHeap(void* addr, MemoryHeap* heap);
    bool IsAddressWithinAllocator(void* addr) const;
    MemoryHeap* GetHeapForAddress(void* addr) const;
    [[nodiscard]] size_t GetTotalMemorySize() const;
    virtual void SetMaxSize(size_t n) = 0;
    [[nodiscard]] virtual size_t GetMaxSize() const = 0;
};
