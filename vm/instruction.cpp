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

#include <utility>
#include "instruction.h"

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
 * @param opcode
 * @param operand1
 * @param operand2
 * @param operand3
 */
Instruction::Instruction(OpCode_t op, RonaObject *operand1, RonaObject *operand2, RonaObject *operand3, RonaObject *operand4) {
    this->opcode = op;
    this->operands.emplace_back(operand1);
    this->operands.emplace_back(operand2);
    this->operands.emplace_back(operand3);
    this->operands.emplace_back(operand4);
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
 * @return
 */
std::string Instruction::to_string() {
    std::string output;

    switch (this->opcode) {
        case OP_BINARY_ADD:
            output += "OP_BINARY_ADD";
            break;
        case OP_BINARY_SUB:
            output += "OP_BINARY_SUB";
            break;
        case OP_BINARY_MUL:
            output += "OP_BINARY_MUL";
            break;
        case OP_BINARY_DIV:
            output += "OP_BINARY_DIV";
            break;
        case OP_BINARY_MOD:
            output += "OP_BINARY_MOD";
            break;
        case OP_BINARY_GTE:
            output += "OP_BINARY_GTE";
            break;
        case OP_BINARY_LTE:
            output += "OP_BINARY_LTE";
            break;
        case OP_BINARY_GT:
            output += "OP_BINARY_GT";
            break;
        case OP_BINARY_LT:
            output += "OP_BINARY_LT";
            break;
        case OP_BINARY_EQ:
            output += "OP_BINARY_EQ";
            break;
        case OP_BINARY_POWER:
            output += "OP_BINARY_POWER";
            break;
        case OP_BINARY_RSH:
            output += "OP_BINARY_RSH";
            break;
        case OP_BINARY_LSH:
            output += "OP_BINARY_LSH";
            break;
        case OP_BINARY_OR:
            output += "OP_BINARY_OR";
            break;
        case OP_BINARY_XOR:
            output += "OP_BINARY_XOR";
            break;
        case OP_BINARY_AND:
            output += "OP_BINARY_AND";
            break;
        case OP_STORE:
            output += "OP_STORE";
            break;
        case OP_POP:
            output += "OP_POP";
            break;
        case OP_UNARY_NOT:
            output += "OP_UNARY_NOT";
            break;
        case OP_BREAK:
            output += "OP_BREAK";
            break;
        case OP_CONTINUE:
            output += "OP_CONTINUE";
            break;
        case OP_TRY_CONTEXT:
            output += "OP_TRY_CONTEXT";
            break;
        case OP_UNARY_INVERT:
            output += "OP_UNARY_INVERT";
            break;
        case OP_UNARY_DECREMENT:
            output += "OP_UNARY_DECREMENT";
            break;
        case OP_UNARY_INCREMENT:
            output += "OP_UNARY_INCREMENT";
            break;
        case OP_RETURN:
            output += "OP_RETURN";
            break;
        case OP_LOAD_LITERAL:
            output += "OP_LOAD_LITERAL";
            break;
        case OP_LOAD_VAR:
            output += "OP_LOAD_VAR";
            break;
        case OP_CALL_FUNC:
            output += "OP_CALL_FUNC";
            break;
        case OP_MAKE_CONST:
            output += "OP_MAKE_CONST";
            break;
        case OP_MAKE_VAR:
            output += "OP_MAKE_VAR";
            break;
        case OP_MAKE_CLASS:
            output += "OP_MAKE_CLASS";
            break;
        case OP_MAKE_FUNC:
            output += "OP_MAKE_FUNC";
            break;
        case OP_MAKE_MODULE:
            output += "OP_MAKE_MODULE";
            break;
        case OP_CREATE_CONTEXT:
            output += "OP_CREATE_CONTEXT";
            break;
        case OP_DESTROY_CONTEXT:
            output += "OP_DESTROY_CONTEXT";
            break;
        case OP_DELETE:
            output += "OP_DELETE";
            break;
        case OP_NOP:
            output += "OP_NOP";
            break;
        case OP_JUMP:
            output += "OP_JUMP";
            break;
        case OP_COMPARE:
            output += "OP_COMPARE";
            break;
        case OP_EXIT:
            output += "OP_EXIT";
            break;
        case OP_ARRAY_STORE:
            output += "OP_ARRAY_STORE";
            break;
        case OP_ARRAY_ACCESS:
            output += "OP_ARRAY_ACCESS";
            break;
        case OP_MAKE_ARG:
            output += "OP_MAKE_ARG";
            break;
        case OP_BUILD_ARRAY:
            output += "OP_BUILD_ARRAY";
            break;
        case OP_LOGICAL_OR:
            output += "OP_LOGICAL_OR";
            break;
        case OP_LOGICAL_AND:
            output += "OP_LOGICAL_AND";
            break;
        case OP_LOAD_NULL:
            output += "OP_LOAD_NULL";
            break;
        case OP_MAKE_ALIAS:
            output += "OP_MAKE_ALIAS";
            break;
    }

    output = "\033[33m" + output + "\033[0m";

    for (auto &operand : this->operands) {
        if (operand->data.empty()) {
            output += "  " + get_type_as_string(operand->type());
        } else {
            output += "  " + operand->to_string();
        }
    }

    return output;
}
