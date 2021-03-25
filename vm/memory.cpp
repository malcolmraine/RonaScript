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

}

/******************************************************************************
 * @brief
 */
Memory::~Memory() {
    cleanup();
}

/******************************************************************************
 * @brief
 * @param obj
 */
void Memory::collect(RonaObject *obj) {
    if (obj->get_reference_cnt() == 0) {
        if (obj->type() == RONA_ARRAY) {
            for (auto &element : obj->data) {
                Memory::collect(std::get<RonaObject *>(element));
            }
        } else {
            delete obj;
        }
    } else {
        obj->free_reference_cnt();
    }
}

/******************************************************************************
 * @brief
 * @param id
 * @param value
 */
void Memory::set_var(RonaObject *id, RonaObject *value, bool by_reference) {
    if (exists(id)) {
        remove(id);
    }

    if (by_reference) {
        value->add_reference_cnt();
        this->_obj_store[id->to_string()] = value;
    } else {
        *this->_obj_store[id->to_string()] = *value;
    }
}

/******************************************************************************
 * @brief
 * @param id
 * @return
 */
RonaObject *Memory::get(RonaObject *id) {
    if (exists(id)) {
        RonaObject *var = this->_obj_store[id->to_string()];
        var->add_reference_cnt();

        return var;
    } else {
        return nullptr;
    }
}

/******************************************************************************
 * @brief
 * @param id
 */
void Memory::remove(RonaObject *id) {
    this->_obj_store.erase(id->to_string());
}

/******************************************************************************
 * @brief
 * @param id
 * @param data
 * @return
 */
RonaObject *Memory::make_var(RonaObject *id, long data) {
    auto *x = new RonaObject(data);
    this->_obj_store[id->to_string()] = x;

    return x;
}

/******************************************************************************
 * @brief
 * @param id
 * @param data
 * @return
 */
RonaObject *Memory::make_var(RonaObject *id, double data) {
    auto *x = new RonaObject(data);
    this->_obj_store[id->to_string()] = x;

    return x;
}

/******************************************************************************
 * @brief
 * @param id
 * @param data
 * @return
 */
RonaObject *Memory::make_var(RonaObject *id, std::string data) {
    auto *x = new RonaObject(std::move(data));
    this->_obj_store[id->to_string()] = x;

    return x;
}

/******************************************************************************
 * @brief
 * @param id
 * @return
 */
RonaObject *Memory::make_var(RonaObject *id) {
    auto *x = new RonaObject();
    this->_obj_store[id->to_string()] = x;

    return x;
}

/******************************************************************************
 * @brief
 * @param id
 * @return
 */
bool Memory::exists(RonaObject *id) {
    return this->_obj_store.count(id->to_string()) > 0;
}

/******************************************************************************
 * @brief
 * @param id
 * @return
 */
bool Memory::is_var(RonaObject *id) {
    if (exists(id)) {
        return this->_obj_store[id->to_string()]->type() != RONA_FUNCTION;
    } else {
        return false;
    }
}

/******************************************************************************
 * @brief
 * @param id
 * @return
 */
bool Memory::is_func(RonaObject *id) {
    if (exists(id)) {
        return this->_obj_store[id->to_string()]->type() == RONA_FUNCTION;
    } else {
        return false;
    }
}

/******************************************************************************
 * @brief
 * @param id
 * @return
 */
RonaFunction *Memory::make_func(RonaObject *id) {
    auto *func = new RonaFunction();
    this->_obj_store[id->to_string()] = new RonaObject(func);

    return func;
}

/******************************************************************************
 * @brief
 */
void Memory::cleanup() {
    for (auto const &x : this->_obj_store) {
        Memory::collect(x.second);
    }
    this->_obj_store.clear();
}
