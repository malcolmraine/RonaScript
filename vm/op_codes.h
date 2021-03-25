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

#ifndef RONASCRIPT_OP_CODES_H
#define RONASCRIPT_OP_CODES_H

enum OpCode_t {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_GTE,
    OP_LTE,
    OP_GT,
    OP_LT,
    OP_EQ,
    OP_MOV,
    OP_POP,
    OP_ALS,
    OP_NOT,
    OP_BRK,
    OP_INV,
    OP_LDL,
    OP_LDV,
    OP_LDF,
    OP_MKV,
    OP_MKC,
    OP_MKF,
    OP_CTX,
    OP_RSH,
    OP_LSH,
    OP_OR,
    OP_XOR,
    OP_AND,
    OP_DEL,
    OP_NOP,
    OP_JMP,
    OP_CMP,
};

#endif //RONASCRIPT_OP_CODES_H


