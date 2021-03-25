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

class Scope;

class VirtualMachine {
public:
    VirtualMachine();

    ~VirtualMachine();

    void run();

    void execute(Instruction *instruction);

    // All operation method work on the current scope
    void op_add();

    void op_sub();

    void op_mul();

    void op_div();

    void op_mod();

    void op_gte();

    void op_lte();

    void op_gt();

    void op_lt();

    void op_eq();

    void op_mov();

    void op_ldl();

    void op_ldv();

    void op_ldf();

    void op_mkv();

    void op_mkc();

    void op_mkf();

    void op_ctx();

    void op_rsh();

    void op_lsh();

    void op_or();

    void op_xor();

    void op_and();

    void op_del();

    static void delete_operands(RonaObject *a);

    static void delete_operands(RonaObject *a, RonaObject *b);

    static void delete_operands(RonaObject *a, RonaObject *b, RonaObject *c);

    static void delete_operands(RonaObject *a, RonaObject *b, RonaObject *c, RonaObject *d);

    static void delete_operands(RonaObject *a, RonaObject *b, RonaObject *c, RonaObject *d, RonaObject *e);

    bool is_builtin_func(RonaObject *id);

    void run_in_scope(Scope *scope, long i_start, long i_end);

    void cleanup() const;

    void load_instruction(Instruction *instruction);

    Scope *global_scope = nullptr;  // Topmost parent scope
//    std::vector<Scope*> child_scopes;
    Scope *curr_scope = nullptr; // Current working scope
    std::vector<Instruction *> instructions;
    long i_idx = 0;  // Current instruction index
    long i_cnt = 0;  // Number of instructions
    long last_cleanup_idx = 0; // Last instruction index that cleanup was run
    long cleanup_intvl = 100; // Cleanup interval (number of instructions between cleanup cycles)

};


#endif //RONASCRIPT_VIRTUAL_MACHINE_H
