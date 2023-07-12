/*****************************************************************************
* File: RnObjectAllocator.h
* Description:
* Author: Malcolm Hall
* Date: 6/17/23
* Version: 1
*
*****************************************************************************/

#pragma once

#include <memory>
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
            auto current_block = RnSlabAllocator::GetBlockForAddr(current_heap->memory);
            while (current_block) {
                auto block_memory_addr =
                    reinterpret_cast<T*>(BLOCK_MEMORY_ADDR(current_block));
                if (fn(block_memory_addr)) {
                    FreeObject(block_memory_addr);
                }
                current_block = NEXT_BLOCK(current_block);
            }
            current_heap = current_heap->next;
        }
    }

private:
    RnSlabAllocator _allocator;
};
