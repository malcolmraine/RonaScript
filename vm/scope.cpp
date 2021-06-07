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
#include "../exceptions/NameNotFoundError.h"
#include <iostream>

/******************************************************************************
 * @brief
 * @return
 */
RonaObject *Scope::pop_from_stack() {
    RonaObject *obj = this->stack.back();
    this->stack.pop_back();

    return obj;
}

/******************************************************************************
 * @brief
 */
Scope::Scope() {
    this->mem_if = new MemoryInterface();
}

/******************************************************************************
 * @brief
 */
Scope::~Scope() {
    this->parent = nullptr;
    delete this->mem_if;
}

/******************************************************************************
 * @brief
 * @param id
 * @return
 */
RonaObject *Scope::get(RonaObject *id) {
    std::string delimiter = "->";

    if (id->to_string().find(delimiter) == std::string::npos) {
        if (this->mem_if->exists(id)) {
            return this->mem_if->get(id);
        } else if (this->parent != nullptr) {
            return this->parent->get(id);
        } else {
            //return nullptr;
            throw NameNotFoundError(id->to_string());
        }
    } else {
        std::string s = id->to_string();
        size_t pos = 0;
        std::vector<std::string> tokens;
        while ((pos = s.find(delimiter)) != std::string::npos) {
            std::string token = s.substr(0, pos);
            tokens.emplace_back(token);
            s.erase(0, pos + delimiter.length());
        }
        tokens.emplace_back(s);

        MemoryInterface *memoryInterface = this->mem_if;
        for (int i = 0; i < tokens.size() - 1; i++) {
            memoryInterface = std::get<RonaClass*>(memoryInterface->sym_tbl[tokens[i]]->data[0])->scope->mem_if;
        }

        RonaObject *final_id = this->mem_if->memory->obj_malloc();
        final_id->data.emplace_back(tokens[tokens.size() - 1]);

        return memoryInterface->sym_tbl[tokens[tokens.size() - 1]];
    }
}

/******************************************************************************
 * @brief
 * @param id
 * @param value
 * @return
 */
bool Scope::set(RonaObject *id, RonaObject *value) {
    std::string delimiter = "->";

    if (id->to_string().find(delimiter) == std::string::npos) {
        if (this->mem_if->exists(id)) {
            this->mem_if->set(id, value);
            return true;
        } else if (this->parent != nullptr) {
            this->parent->set(id, value);
            return true;
        } else {
            //return false;
            throw NameNotFoundError(id->to_string());
        }
    } else {
        std::string s = id->to_string();
        size_t pos = 0;
        std::vector<std::string> tokens;
        while ((pos = s.find(delimiter)) != std::string::npos) {
            std::string token = s.substr(0, pos);
            std::cout << token << std::endl;
            tokens.emplace_back(token);
            s.erase(0, pos + delimiter.length());
        }
        tokens.emplace_back(s);

        MemoryInterface *memoryInterface = this->mem_if;
        for (int i = 0; i < tokens.size() - 1; i++) {
            memoryInterface = std::get<RonaClass*>(memoryInterface->sym_tbl[tokens[i]]->data[0])->scope->mem_if;
        }

        RonaObject *final_id = this->mem_if->memory->obj_malloc();
        final_id->data.emplace_back(tokens[tokens.size() - 1]);

        RonaObject *obj = memoryInterface->sym_tbl[tokens[tokens.size() - 1]];

        if (obj->data.empty()) {
            obj->data.emplace_back(value->data[0]);
        } else {
            obj->data[0] = value->data[0];
        }
        return true;
    }
}

/******************************************************************************
 * @brief
 * @param id
 * @param type
 * @return
 */
RonaObject *Scope::make_var(RonaObject *id, RonaType_t type) {
    return nullptr;
}

/******************************************************************************
 * @brief
 * @param scope
 */
void Scope::set_parent_scope(Scope *scope) {

}

/******************************************************************************
 * @brief
 */
void Scope::cleanup() {

}

/******************************************************************************
 * @brief
 * @param obj
 */
void Scope::remove(RonaObject *obj) {

}
