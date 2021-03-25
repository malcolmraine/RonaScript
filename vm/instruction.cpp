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

#include "instruction.h"

#include <utility>

/******************************************************************************
 * @brief
 */
Instruction::Instruction() {
    this->opcode = OP_NOP;
}

/******************************************************************************
 * @brief
 * @param opcode
 */
Instruction::Instruction(OpCode_t op) {
    this->opcode = op;
}

/******************************************************************************
 * @brief
 * @param opcode
 * @param operand1
 */
Instruction::Instruction(OpCode_t op, RonaObject *operand1) {
    this->opcode = op;
    this->operands.emplace_back(operand1);
}

/******************************************************************************
 * @brief
 * @param opcode
 * @param operand1
 * @param operand2
 */
Instruction::Instruction(OpCode_t op, RonaObject *operand1, RonaObject *operand2) {
    this->opcode = op;
    this->operands.emplace_back(operand1);
    this->operands.emplace_back(operand2);
}

/******************************************************************************
 * @brief
 * @param opcode
 * @param operand1
 * @param operand2
 * @param operand3
 */
Instruction::Instruction(OpCode_t op, RonaObject *operand1, RonaObject *operand2, RonaObject *operand3) {
    this->opcode = op;
    this->operands.emplace_back(operand1);
    this->operands.emplace_back(operand2);
    this->operands.emplace_back(operand3);
}

/******************************************************************************
 * @brief
 */
Instruction::~Instruction() {
    for (auto &operand : this->operands) {
        delete operand;
    }
}

/******************************************************************************
 * @brief
 * @param rel_idx
 * @return
 */
Instruction *Instruction::make_jmp(long rel_idx) {
    return new Instruction(OP_JMP, new RonaObject(rel_idx));
}

/******************************************************************************
 * @brief
 * @param begin
 * @return
 */
Instruction *Instruction::make_ctx(bool begin) {
    return new Instruction(
            OP_CTX,
            new RonaObject(static_cast<long>(begin ? 1 : 0))
    );
}

/******************************************************************************
 * @brief
 * @param rel_idx
 * @return
 */
Instruction *Instruction::make_cmp(long rel_idx) {
    return new Instruction(OP_CMP, new RonaObject(rel_idx));
}

/******************************************************************************
 * @brief
 * @param id
 * @param offset
 * @return
 */
Instruction *Instruction::make_mov(std::string id, long offset) {
    return new Instruction(
            OP_MOV,
            new RonaObject(std::move(id)),
            new RonaObject(offset)
    );
}

/******************************************************************************
 * @brief
 * @param id
 * @param offset
 * @return
 */
Instruction *Instruction::make_ldv(std::string id, long offset) {
    return new Instruction(
            OP_LDV,
            new RonaObject(std::move(id)),
            new RonaObject(offset)
    );
}

/******************************************************************************
 * @brief
 * @param value
 * @return
 */
Instruction *Instruction::make_ldl(std::string value) {
    return new Instruction(OP_LDL, new RonaObject(std::move(value)));
}

/******************************************************************************
 * @brief
 * @param value
 * @return
 */
Instruction *Instruction::make_ldl(long value) {
    return new Instruction(OP_LDL, new RonaObject(value));
}

/******************************************************************************
 * @brief
 * @param value
 * @return
 */
Instruction *Instruction::make_ldl(double value) {
    return new Instruction(OP_LDL, new RonaObject(value));
}

/******************************************************************************
 * @brief
 * @param value
 * @return
 */
Instruction *Instruction::make_ldl(bool value) {
    return new Instruction(OP_LDL, new RonaObject(value));
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_add() {
    return new Instruction(OP_ADD);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_sub() {
    return new Instruction(OP_SUB);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_mul() {
    return new Instruction(OP_MUL);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_div() {
    return new Instruction(OP_DIV);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_mod() {
    return new Instruction(OP_MOD);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_gte() {
    return new Instruction(OP_GTE);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_lte() {
    return new Instruction(OP_LTE);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_gt() {
    return new Instruction(OP_GT);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_lt() {
    return new Instruction(OP_LT);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_eq() {
    return new Instruction(OP_EQ);
}

/******************************************************************************
 * @brief
 * @param base
 * @param alias
 * @return
 */
Instruction *Instruction::make_als(std::string base, std::string alias) {
    return new Instruction(
            OP_ALS,
            new RonaObject(std::move(base)),
            new RonaObject(std::move(alias))
    );
}

/******************************************************************************
 * @brief
 * @param id
 * @param arg_cnt
 * @return
 */
Instruction *Instruction::make_ldf(const std::string &id, long arg_cnt) {
    return new Instruction(OP_LDF, new RonaObject(arg_cnt));
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_and() {
    return new Instruction(OP_AND);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_or() {
    return new Instruction(OP_OR);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_xor() {
    return new Instruction(OP_XOR);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_inv() {
    return new Instruction(OP_INV);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_not() {
    return new Instruction(OP_NOT);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_lsh() {
    return new Instruction(OP_LSH);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_rsh() {
    return new Instruction(OP_RSH);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_pop() {
    return new Instruction(OP_NOP);
}

/******************************************************************************
 * @brief
 * @return
 */
Instruction *Instruction::make_nop() {
    return new Instruction(OP_NOP);
}

/******************************************************************************
 * @brief
 * @param id
 * @return
 */
Instruction *Instruction::make_del(std::string id) {
    return new Instruction(OP_DEL, new RonaObject(std::move(id)));
}
