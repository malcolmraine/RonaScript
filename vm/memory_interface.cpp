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
#include "memory_interface.h"
#include "../exceptions/MemoryError.h"

/******************************************************************************
 * @brief
 */
MemoryInterface::MemoryInterface() {
    this->memory = Memory::get_instance();
}

/******************************************************************************
 * @brief
 */
MemoryInterface::~MemoryInterface() {

}

/******************************************************************************
 * @brief
 * @param id
 * @param value
 */
void MemoryInterface::set(RonaObject *id, RonaObject *value) {
    if (exists(id) && !this->sym_tbl[id->to_string()]->is_const) {
        this->sym_tbl[id->to_string()]->data = value->data;
    } else {
        throw MemoryError("Assignment to const \'" + id->to_string() + "\'");
    }
}

/******************************************************************************
 * @brief
 * @param id
 * @return
 */
RonaObject *MemoryInterface::get(RonaObject *id) {
    if (exists(id)) {
        return this->sym_tbl[id->to_string()];
    } else {
        return nullptr;
        // TODO: Throw exception
    }
}

/******************************************************************************
 * @brief
 * @param id
 */
void MemoryInterface::remove(RonaObject *id) {
    if (exists(id)) {
        this->sym_tbl.erase(id->to_string());
    } else {
        // TODO: Throw exception
    }
}

/******************************************************************************
 * @brief
 * @param id
 * @param data
 * @return
 */
RonaObject *MemoryInterface::make_obj(RonaObject *id, long data) {
    auto obj = new RonaObject(RN_TYPE_INT);
    obj->set(data);

    if (!exists(id)) {
        this->sym_tbl[id->to_string()] = obj;
    } else {
        // TODO: Throw exception
    }

    return obj;
}

/******************************************************************************
 * @brief
 * @param id
 * @param data
 * @return
 */
RonaObject *MemoryInterface::make_obj(RonaObject *id, double data) {
    auto obj = new RonaObject(RN_TYPE_FLOAT);
    obj->set(data);

    if (!exists(id)) {
        this->sym_tbl[id->to_string()] = obj;
    } else {
        // TODO: Throw exception
    }

    return obj;
}

/******************************************************************************
 * @brief
 * @param id
 * @param data
 * @return
 */
RonaObject *MemoryInterface::make_obj(RonaObject *id, std::string data) {
    auto obj = new RonaObject(RN_TYPE_STRING);
    obj->set(data);

    if (!exists(id)) {
        this->sym_tbl[id->to_string()] = obj;
    } else {
        // TODO: Throw exception
    }

    return obj;
}

/******************************************************************************
 * @brief
 * @param id
 * @param type
 * @return
 */
RonaObject *MemoryInterface::make_obj(RonaObject *id, RonaType_t type) {
    auto obj = this->memory->obj_malloc();
    obj->set_type(type);

    if (!exists(id)) {
        this->sym_tbl[id->to_string()] = obj;
    } else {
        // TODO: Throw exception
    }

    return obj;
}

/******************************************************************************
 * @brief
 * @param id
 * @return
 */
bool MemoryInterface::exists(RonaObject *id) {
    return this->sym_tbl.find(id->to_string()) != this->sym_tbl.end();
}