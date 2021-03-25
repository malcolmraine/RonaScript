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

#include "scope.h"


/******************************************************************************
 * @brief
 */
Scope::Scope() {
    this->memory = new Memory();

}

/******************************************************************************
 * @brief
 */
Scope::~Scope() {
    cleanup();
}

/******************************************************************************
 * @brief
 * @param id
 * @return
 */
RonaObject *Scope::get_obj(RonaObject *id) {
    if (this->memory->exists(id)) {
        return this->memory->get(id);
    } else {
        if (this->parent == nullptr) {
            // Raise exception
            throw std::exception();
        } else {
            return this->parent->get_obj(id);
        }
    }
}

/******************************************************************************
 * @brief
 * @param id
 * @param value
 * @param by_reference
 * @return
 */
bool Scope::set(RonaObject *id, RonaObject *value, bool by_reference) {
    if (this->memory->exists(id)) {
        this->memory->set_var(id, value);
    } else {
        if (this->parent == nullptr) {
            // Raise exception
            throw std::exception();
        } else {
            this->parent->set(id, value, value->type() == RONA_ARRAY || value->type() == RONA_CLASS);;
        }
    }

    return true;
}

/******************************************************************************
 * @brief
 * @param id
 * @param type
 * @return
 */
RonaObject *Scope::make_var(RonaObject *id, RonaType_t type) {
    auto *var = new RonaObject(type);
    this->memory->set_var(id, var);
    this->global_mem_idx.emplace_back(var);

//    if (parent != nullptr) {
//        parent->global_mem_idx.emplace_back(var);
//    }

    return var;
}

/******************************************************************************
 * @brief
 * @param scope
 */
void Scope::set_parent_scope(Scope *scope) {
    this->parent = scope;
}

/******************************************************************************
 * @brief Garbage collection
 */
void Scope::cleanup() {
    // Cleanup the memory index
    for (auto &i : global_mem_idx) {
        Memory::collect(i);

        if (i->get_reference_cnt() == 0) {
            //Memory::collect(i);
        } else {
            // Move the reference to the parent index to be dealt with later
            if (std::find(this->global_mem_idx.begin(), this->global_mem_idx.end(), i) != this->global_mem_idx.end()) {
                this->parent->global_mem_idx.emplace_back(i);
            }
        }
    }

    this->global_mem_idx.clear();

    // Cleanup the stack
//    for (auto & i : stack) {
//        Memory::collect(i);
//
//        if (i->get_reference_cnt() == 0) {
//            //Memory::collect(i);
//        } else {
//            // Move the reference to the parent index to be dealt with later
//            if (std::find(global_mem_idx.begin(), global_mem_idx.end(), i) != global_mem_idx.end()) {
//                parent->global_mem_idx.emplace_back(i);
//            }
//        }
//    }
//
//    stack.clear();
//    memory->cleanup();

    //delete memory;
}

/******************************************************************************
 * @brief
 * @param obj
 */
void Scope::delete_rona_object(RonaObject *obj) {
    if (obj->type() == RONA_ARRAY) {
        for (auto &i : obj->data) {
            delete_rona_object(std::get<RonaObject *>(i));
        }
    } else {
        Memory::collect(obj);
    }
}

/******************************************************************************
 * @brief
 * @return
 */
RonaObject *Scope::pop_from_stack() {
    RonaObject *a = this->stack.back();
    this->stack.pop_back();

    return a;
}

/******************************************************************************
 * @brief
 */
void Scope::reset() {

}










