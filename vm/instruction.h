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

#ifndef RONASCRIPT_INSTRUCTION_H
#define RONASCRIPT_INSTRUCTION_H

#include <variant>
#include <vector>
#include <string>
#include "op_codes.h"
#include "rona_object.h"

class RonaObject;


class Instruction {
public:
    Instruction();
    Instruction(OpCode_t op);
    Instruction(OpCode_t op, RonaObject *operand1);
    Instruction(OpCode_t op, RonaObject *operand1, RonaObject *operand2);
    Instruction(OpCode_t op, RonaObject *operand1, RonaObject *operand2, RonaObject *operand3);
    ~Instruction();
    static Instruction *make_jmp(long rel_idx);
    static Instruction *make_ctx(bool begin);
    static Instruction *make_cmp(long rel_idx);
    static Instruction *make_mov(std::string id, long offset);
    static Instruction *make_ldv(std::string id, long offset);
    static Instruction *make_ldl(std::string value);
    static Instruction *make_ldl(long value);
    static Instruction *make_ldl(double value);
    static Instruction *make_ldl(bool value);
    static Instruction *make_add();
    static Instruction *make_sub();
    static Instruction *make_mul();
    static Instruction *make_div();
    static Instruction *make_mod();
    static Instruction *make_gte();
    static Instruction *make_lte();
    static Instruction *make_gt();
    static Instruction *make_lt();
    static Instruction *make_eq();
    static Instruction *make_als(std::string base, std::string alias);
    static Instruction *make_ldf(const std::string &id, long arg_cnt);
    static Instruction *make_and();
    static Instruction *make_or();
    static Instruction *make_xor();
    static Instruction *make_inv();
    static Instruction *make_not();
    static Instruction *make_lsh();
    static Instruction *make_rsh();
    static Instruction *make_pop();
    static Instruction *make_nop();
    static Instruction *make_del(std::string id);

    OpCode_t opcode;
    std::vector<RonaObject *> operands;
};


#endif //RONASCRIPT_INSTRUCTION_H
