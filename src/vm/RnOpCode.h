/*****************************************************************************
* File: RnOpCode.h
* Description:
* Author: Malcolm Hall
* Date: 3/22/23
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
*****************************************************************************/

#pragma once

#include "../common/RnType.h"

typedef enum : uint8_t {
    OP_BINARY_ADD,
    OP_BINARY_SUB,
    OP_BINARY_MUL,
    OP_BINARY_DIV,
    OP_BINARY_MOD,
    OP_BINARY_GTE,
    OP_BINARY_LTE,
    OP_BINARY_GT,
    OP_BINARY_LT,
    OP_BINARY_EQ,
    OP_BINARY_NEQ,
    OP_BINARY_POWER,
    OP_BINARY_RSH,
    OP_BINARY_LSH,
    OP_BINARY_OR,
    OP_BINARY_XOR,
    OP_BINARY_AND,
    OP_LOGICAL_OR,
    OP_LOGICAL_AND,
    OP_UNARY_NOT,
    OP_TRY_CONTEXT,
    OP_UNARY_INVERT,
    OP_UNARY_DECREMENT,
    OP_UNARY_INCREMENT,
    OP_UNARY_NEGATION,
    OP_STORE,
    OP_POP,
    OP_CALL,
    OP_RETURN,
    OP_LOAD_NULL,
    OP_LOAD_LITERAL,
    OP_LOAD_VALUE,
    OP_LOAD_ATTR,
    OP_MAKE_CONST,
    OP_MAKE_VAR,
    OP_MAKE_GLOBAL,
    OP_MAKE_LOCAL,
    OP_MAKE_CLASS,
    OP_MAKE_FUNC,
    OP_MAKE_ARG,
    OP_MAKE_ALIAS,
    OP_MAKE_ARRAY,
    OP_MAKE_CLOSURE,
    OP_CREATE_CONTEXT,
    OP_DESTROY_CONTEXT,
    OP_RESET_CONTEXT,
    OP_DELETE,
    OP_NOP,
    OP_JUMPF,
    OP_JUMPB,
    OP_JUMPF_IF,
    OP_JUMPB_IF,
    OP_EXIT,
    OP_INDEX_ACCESS,
    OP_CHECK_MEMBERSHIP,
    OP_IMPORT,
} RnOpCode;

RnStringNative GetOpCodeAsString(RnOpCode opcode);
RnOpCode GetOpCodeFromOperator(const RnStringNative& op);
