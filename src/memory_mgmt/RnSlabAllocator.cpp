/*****************************************************************************
* File: RnSlabAllocator.cpp
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

#include "RnSlabAllocator.h"

/*****************************************************************************/
RnSlabAllocator::RnSlabAllocator(size_t object_size, size_t heap_size, size_t max_size)
    : RnLinearAllocator(heap_size, max_size), _object_size(object_size) {}

/*****************************************************************************/
RnSlabAllocator::~RnSlabAllocator() = default;

/*****************************************************************************/
void* RnSlabAllocator::Malloc(size_t n) {
    return RnLinearAllocator::Malloc(n * _object_size);
}

/*****************************************************************************/
void* RnSlabAllocator::Calloc(size_t n, int c) {
    return RnLinearAllocator::Calloc(n * _object_size, c);
}

/*****************************************************************************/
void* RnSlabAllocator::Realloc(void* data, size_t n) {
    return RnLinearAllocator::Realloc(data, n * _object_size);
}

/*****************************************************************************/
void RnSlabAllocator::Free(void* addr) {
    RnLinearAllocator::Free(addr);
}
