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

#ifndef RONASCRIPT_VIRTUAL_MACHINE_H
#define RONASCRIPT_VIRTUAL_MACHINE_H

#include "scope.h"
#include "instruction.h"
#include "rona_function.h"
#include "rona_type.h"
#include "builtins.h"
#include "rona_module.h"
#include "memory.h"

class Scope;

class VirtualMachine {
public:
    VirtualMachine();
    ~VirtualMachine();
    void run();
    void execute(Instruction *instruction);

    // All operation method work on the current scope
    void op_binary_add();
    void op_binary_sub();
    void op_binary_mul();
    void op_binary_div();
    void op_binary_mod();
    void op_binary_gte();
    void op_binary_lte();
    void op_binary_gt();
    void op_binary_lt();
    void op_binary_eq();
    void op_binary_rsh();
    void op_binary_lsh();
    void op_binary_or();
    void op_binary_xor();
    void op_binary_and();
    void op_binary_power();
    void op_logical_or();
    void op_logical_and();
    void op_unary_decrement();
    void op_unary_increment();
    void op_store();
    void op_load_literal();
    void op_load_null();
    void op_load_var();
    RonaObject* call_func_by_name(RonaObject *id, int arg_cnt);
    void op_call_func();
    void run_in_scope(Scope* scope, long i_start, long i_end);
    void op_make_var();
    void op_make_class();
    void op_make_func();
    void op_create_context();
    void op_destroy_context();
    void op_delete();
    void op_pop();
    void op_make_alias();
    void op_unary_not();
    void op_break();
    void op_continue();
    void unary_invert();
    void op_nop();
    void op_jump();
    void op_compare();
    void op_make_module();
    void op_exit();
    void op_make_const();
    void op_array_store();
    void op_array_access();
    void op_make_arg();
    void op_build_array();
    void gc_mark();
    Scope *make_scope();
    void delete_scope(Scope *scope);

    Memory* memory = nullptr;
    BuiltinMethodManager builtinMethodManager;
    bool exit_flag = false;
    int exit_code = 0;
    Scope *global_scope = nullptr;  // Topmost parent scope
    std::vector<Scope*> child_scopes;
    Scope *curr_scope = nullptr; // Current working scope
    std::map<std::string, RonaModule *> modules;
    std::vector<Instruction *> instructions;
    std::map<std::string, std::string> alias_tbl;
    long i_idx = 0;  // Current instruction index
    long i_cnt = 0;  // Number of instructions
    RonaObject *cls_construct_method_name = nullptr;
};


#endif //RONASCRIPT_VIRTUAL_MACHINE_H
