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

#ifndef RONASCRIPT_SCOPE_H
#define RONASCRIPT_SCOPE_H

#include <string>
#include <vector>
#include <memory>
#include "memory.h"
#include "rona_object.h"
#include "rona_function.h"
#include "rona_class.h"

class RonaObject;

class Memory;

class RonaFunction;


class Scope {
public:
    Scope();
    ~Scope();
    RonaObject *get_obj(RonaObject *id);
    bool set(RonaObject *id, RonaObject *value, bool by_reference = false);
    RonaObject *make_var(RonaObject *id, RonaType_t type);
    void set_parent_scope(Scope *scope);
    RonaObject *pop_from_stack();
    void cleanup();
    void delete_rona_object(RonaObject *obj);
    void reset();

    Memory *memory = nullptr;
    std::vector<RonaObject *> global_mem_idx; // Stores pointers to all active variables in this or lower level scopes
    Scope *parent = nullptr;
    std::vector<RonaObject *> stack;
};


#endif //RONASCRIPT_SCOPE_H
