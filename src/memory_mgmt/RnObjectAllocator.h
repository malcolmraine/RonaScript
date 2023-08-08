/*****************************************************************************
* File: RnObjectAllocator.h
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

#include <memory>
#include <string>
#include "RnSlabAllocator.h"

template <class T>
class RnObjectAllocator {
public:
    /*************************************************************************/
    RnObjectAllocator(size_t heap_size, size_t max_size)
        : _allocator(sizeof(T), heap_size, max_size) {}

    /*************************************************************************/
    ~RnObjectAllocator() = default;

    /*************************************************************************/
    template <typename... Args>
    T* CreateObject(Args... args) {
        auto addr = _allocator.Malloc(1);
        if (!addr) {
            throw std::runtime_error("Failed to allocate " + std::to_string(sizeof(T)) +
                                     " bytes for object");
        }
        return std::construct_at<T>(reinterpret_cast<T*>(addr),
                                    std::forward<Args>(args)...);
    }

    /*************************************************************************/
    void FreeObject(T* object) {
        object->~T();
        //    std::destroy_at<T>(object);
        _allocator.Free(object);
    }

    /*************************************************************************/
    template <typename FUNC>
    void FreeIf(FUNC fn) {
        auto current_heap = _allocator.FirstHeap();
        while (current_heap) {
            auto current_block = reinterpret_cast<MemoryBlock*>(current_heap->memory);
            while (current_block && !current_block->available) {
                auto obj = reinterpret_cast<T*>(BLOCK_MEMORY_ADDR(current_block));
                if (fn(obj)) {
                    FreeObject(obj);
                }
                current_block = NEXT_BLOCK(current_block);
            }
            current_heap = current_heap->next;
        }
    }

private:
    RnSlabAllocator _allocator;
};
