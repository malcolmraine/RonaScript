/******************************************************************************
* File:
* Description:
* Author: Malcolm Hall
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
*******************************************************************************/

#ifndef RONASCRIPT_MEMORY_H
#define RONASCRIPT_MEMORY_H

#include <map>
#include <string>
#include <memory>
#include "rona_object.h"
#include "rona_class.h"
#include "rona_function.h"
#include "rona_type.h"
#include "rona_class_decl.h"

#define MEMORY_SIZE_LIMIT 1000000000
#define HEAP_CONTRACTION_THRSH 1000000


class RonaFunction;
class RonaObject;
class RonaClass;

struct MemoryBlock {
    RonaObject *obj = nullptr;
    MemoryBlock *next = nullptr;
    MemoryBlock *previous = nullptr;
};


class Memory {
public:
    static Memory *instance;

    static Memory *get_instance();
    RonaObject *obj_malloc();
    void add_to_heap(RonaObject *obj);
    long heap_size_bytes();
    void sweep();
    bool resize(int n);
    MemoryBlock *find_first_free();
    void contract_heap();


protected:
    MemoryBlock *heap = nullptr;
    MemoryBlock *head = nullptr;
    MemoryBlock *tail = nullptr;
    long _heap_size = 0;

private:
    Memory();
    ~Memory();
};


#endif //RONASCRIPT_MEMORY_H
