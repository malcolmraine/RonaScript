/******************************************************************************
* File: RnOpCode.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/19/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
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
******************************************************************************/

#include "RnOpCode.h"

/*****************************************************************************/
RnStringNative GetOpCodeAsString(RnOpCode opcode) {
    switch (opcode) {
        case OP_LOAD_ATTR:
            return "OP_LOAD_ATTR";
        case OP_BINARY_ADD:
            return "OP_BINARY_ADD";
        case OP_BINARY_SUB:
            return "OP_BINARY_SUB";
        case OP_BINARY_MUL:
            return "OP_BINARY_MUL";
        case OP_BINARY_DIV:
            return "OP_BINARY_DIV";
        case OP_BINARY_MOD:
            return "OP_BINARY_MOD";
        case OP_BINARY_GTE:
            return "OP_BINARY_GTE";
        case OP_BINARY_LTE:
            return "OP_BINARY_LTE";
        case OP_BINARY_GT:
            return "OP_BINARY_GT";
        case OP_BINARY_LT:
            return "OP_BINARY_LT";
        case OP_BINARY_EQ:
            return "OP_BINARY_EQ";
        case OP_BINARY_NEQ:
            return "OP_BINARY_NEQ";
        case OP_BINARY_POWER:
            return "OP_BINARY_POWER";
        case OP_BINARY_RSH:
            return "OP_BINARY_RSH";
        case OP_BINARY_LSH:
            return "OP_BINARY_LSH";
        case OP_BINARY_OR:
            return "OP_BINARY_OR";
        case OP_BINARY_XOR:
            return "OP_BINARY_XOR";
        case OP_BINARY_AND:
            return "OP_BINARY_AND";
        case OP_LOGICAL_OR:
            return "OP_LOGICAL_OR";
        case OP_LOGICAL_AND:
            return "OP_LOGICAL_AND";
        case OP_STORE:
            return "OP_STORE";
        case OP_POP:
            return "OP_POP";
        case OP_TRY_CONTEXT:
            return "OP_TRY_CONTEXT";
        case OP_UNARY_INVERT:
            return "OP_UNARY_INVERT";
        case OP_FAST_SUB:
            return "OP_FAST_SUB";
        case OP_FAST_ADD:
            return "OP_FAST_ADD";
        case OP_UNARY_NOT:
            return "OP_UNARY_NOT";
        case OP_UNARY_NEGATION:
            return "OP_UNARY_NEGATION";
        case OP_RETURN:
            return "OP_RETURN";
        case OP_LOAD_LITERAL:
            return "OP_LOAD_LITERAL";
        case OP_LOAD_VALUE:
            return "OP_LOAD_VALUE";
        case OP_LOAD_NULL:
            return "OP_LOAD_NULL";
        case OP_CALL:
            return "OP_CALL";
        case OP_MAKE_CONST:
            return "OP_MAKE_CONST";
        case OP_MAKE_LOCAL:
            return "OP_MAKE_LOCAL";
        case OP_MAKE_VAR:
            return "OP_MAKE_VAR";
        case OP_MAKE_GLOBAL:
            return "OP_MAKE_GLOBAL";
        case OP_MAKE_CLASS:
            return "OP_MAKE_CLASS";
        case OP_MAKE_FUNC:
            return "OP_MAKE_FUNC";
        case OP_CREATE_CONTEXT:
            return "OP_CREATE_CONTEXT";
        case OP_DESTROY_CONTEXT:
            return "OP_DESTROY_CONTEXT";
        case OP_RESET_CONTEXT:
            return "OP_RESET_CONTEXT";
        case OP_DELETE:
            return "OP_DELETE";
        case OP_NOP:
            return "OP_NOP";
        case OP_JUMPF:
            return "OP_JUMPF";
        case OP_JUMPB:
            return "OP_JUMPB";
        case OP_JUMPF_IF:
            return "OP_JUMPF_IF";
        case OP_JUMPB_IF:
            return "OP_JUMPB_IF";
        case OP_EXIT:
            return "OP_EXIT";
        case OP_INDEX_ACCESS:
            return "OP_INDEX_ACCESS";
        case OP_MAKE_ARG:
            return "OP_MAKE_ARG";
        case OP_MAKE_ALIAS:
            return "OP_MAKE_ALIAS";
        case OP_MAKE_ARRAY:
            return "OP_MAKE_ARRAY";
        case OP_CHECK_MEMBERSHIP:
            return "OP_CHECK_MEMBERSHIP";
        case OP_MAKE_CLOSURE:
            return "OP_MAKE_CLOSURE";
        case OP_IMPORT:
            return "OP_IMPORT";
        default:
            return "UNKNOWN";
    }
}

/*****************************************************************************/
RnOpCode GetOpCodeFromOperator(const RnStringNative& op) {
    if (op == "+") {
        return OP_BINARY_ADD;
    } else if (op == "-") {
        return OP_BINARY_SUB;
    } else if (op == "/") {
        return OP_BINARY_DIV;
    } else if (op == "*") {
        return OP_BINARY_MUL;
    } else if (op == "^" || op == "xor") {
        return OP_BINARY_XOR;
    } else if (op == "~") {
        return OP_UNARY_INVERT;
    } else if (op == "!") {
        return OP_UNARY_NEGATION;
    } else if (op == "==") {
        return OP_BINARY_EQ;
    } else if (op == "**") {
        return OP_BINARY_POWER;
    } else if (op == "&") {
        return OP_BINARY_AND;
    } else if (op == "&&" || op == "and") {
        return OP_LOGICAL_AND;
    } else if (op == "|") {
        return OP_BINARY_OR;
    } else if (op == "||" || op == "or") {
        return OP_LOGICAL_OR;
    } else if (op == "<") {
        return OP_BINARY_LT;
    } else if (op == ">") {
        return OP_BINARY_GT;
    } else if (op == "<=") {
        return OP_BINARY_LTE;
    } else if (op == ">=") {
        return OP_BINARY_GTE;
    } else if (op == "!=") {
        return OP_BINARY_NEQ;
    } else if (op == "?") {
        return OP_BINARY_ADD;
    } else if (op == "->") {
        return OP_LOAD_ATTR;
    } else if (op == "%") {
        return OP_BINARY_MOD;
    } else if (op == "in") {
        return OP_CHECK_MEMBERSHIP;
    } else if (op == "<<") {
        return OP_BINARY_LSH;
    } else if (op == ">>") {
        return OP_BINARY_RSH;
    } else {
        return OP_NOP;
    }
}