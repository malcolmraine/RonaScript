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

#include "memory.h"
#include <iostream>
#include <utility>


/******************************************************************************
 * @brief
 */
Memory::Memory() {
    this->heap = new MemoryBlock;
    this->head = this->heap;
    this->tail = new MemoryBlock;
    this->head->next = this->tail;
    this->tail->previous = this->head;
}

/******************************************************************************
 * @brief
 */
Memory::~Memory() {
    MemoryBlock *current = this->head;

    while (current != nullptr) {
        delete current->obj;
        current->obj = nullptr;
        current = current->next;
        delete current->previous;
        current->previous = nullptr;
    }
}

/******************************************************************************
 * @brief
 * @return
 */
Memory *Memory::get_instance() {
    if (!instance) {
        instance = new Memory;
    }

    return instance;
}

/******************************************************************************
 * @brief
 * @return
 */
RonaObject *Memory::obj_malloc() {
    auto obj = new RonaObject(RN_TYPE_NULL);
    MemoryBlock *block = find_first_free();

    if (block == nullptr) {
        resize(1);
        this->tail->previous->obj = obj;
    } else {
        block->obj = obj;
    }

    return obj;
}

/******************************************************************************
 * @brief
 * @param obj
 */
void Memory::add_to_heap(RonaObject *obj) {
    MemoryBlock *block = find_first_free();

    if (block == nullptr) {
        resize(1);
        this->tail->previous->obj = obj;
    } else {
        block->obj = obj;
    }
}

/******************************************************************************
 * @brief
 * @return
 */
long Memory::heap_size_bytes() {
    return this->_heap_size * (sizeof(MemoryBlock) + (sizeof(RonaObject) - 2));
}

/******************************************************************************
 * @brief
 */
void Memory::sweep() {
    MemoryBlock *current = this->head->next;

    while (current != nullptr && current != this->head && current != this->tail) {
        if (current->obj != nullptr) {
            if (!current->obj->gc_flag) {
                delete current->obj;
                current->obj = nullptr;
            } else {
                current->obj->gc_flag = false;
            }
        }
        current = current->next;
    }

    contract_heap();
}

/******************************************************************************
 * @brief Increase the size of the heap by n blocks
 * @param n
 * @return
 */
bool Memory::resize(int n) {
    for (int i = 0; i < n; i++) {
        auto block = new MemoryBlock;
        this->tail->previous->next = block;
        block->previous = this->tail->previous;
        this->tail->previous = block;
        block->next = this->tail;
        this->_heap_size++;
    }

    return true;
}

/******************************************************************************
 * @brief
 * @return
 */
MemoryBlock *Memory::find_first_free() {
    MemoryBlock *current = this->head;

    while (current != nullptr) {
        if (current->obj == nullptr && current != this->head && current != this->tail) {
            return current;
        } else {
            current = current->next;
        }
    }

    return nullptr;
}

/******************************************************************************
 * @brief
 */
void Memory::contract_heap() {
    MemoryBlock *current = this->head;
    MemoryBlock *obj_to_delete = nullptr;

    while (current != this->tail) {
        current = current->next;
        obj_to_delete = current;

        if (obj_to_delete->obj == nullptr && obj_to_delete != this->tail) {
            obj_to_delete->previous->next = obj_to_delete->next;
            obj_to_delete->next->previous = obj_to_delete->previous;
            delete obj_to_delete;
            this->_heap_size--;
        }
    }
}


