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
    OP_BINARY_ADD,      // op1:          op2:                    
    OP_BINARY_SUB,      // op1:          op2:                    
    OP_BINARY_MUL,      // op1:          op2:                    
    OP_BINARY_DIV,      // op1:          op2:                    
    OP_BINARY_MOD,      // op1:          op2:                    
    OP_BINARY_GTE,      // op1:          op2:                    
    OP_BINARY_LTE,      // op1:          op2:                    
    OP_BINARY_GT,       // op1:          op2:                    
    OP_BINARY_LT,       // op1:          op2:                    
    OP_BINARY_EQ,       // op1:          op2:                    
    OP_BINARY_POWER,    // op1:          op2:                    
    OP_BINARY_RSH,      // op1:          op2:                    
    OP_BINARY_LSH,      // op1:          op2:                    
    OP_BINARY_OR,       // op1:          op2:                    
    OP_BINARY_XOR,      // op1:          op2:                    
    OP_BINARY_AND,      // op1:          op2:
    OP_LOGICAL_OR,
    OP_LOGICAL_AND,
    OP_STORE,           // op1: name     op2:                    
    OP_POP,             // op1:          op2:
    OP_UNARY_NOT,       // op1: name     op2:                    
    OP_BREAK,           // op1: rel_idx  op2:                    
    OP_CONTINUE,        // op1: rel_idx  op2:                    
    OP_TRY_CONTEXT,
    OP_UNARY_INVERT,    // op1: name     op2:                    
    OP_UNARY_DECREMENT, // op1: name     op2:                    
    OP_UNARY_INCREMENT, // op1: name     op2:                    
    OP_RETURN,          // op1: rel_idx  op2:                   
    OP_LOAD_LITERAL,    // op1: value    op2:
    OP_LOAD_NULL,
    OP_LOAD_VAR,        // op1: name     op2:                    
    OP_CALL_FUNC,       // op1: name     op2: arg count
    OP_MAKE_CONST,      // op1: type     op2: visibility         
    OP_MAKE_VAR,        // op1: type     op2: visibility         
    OP_MAKE_CLASS,      // op1: name     op2: instr. count       
    OP_MAKE_FUNC,       // op1: name     op2: instr. count       
    OP_MAKE_MODULE,     // op1: name     op2: instr. count       
    OP_CREATE_CONTEXT,    // op1: flag     op2:
    OP_DESTROY_CONTEXT,
    OP_DELETE,          // op1: name     op2:                    
    OP_NOP,             // op1:          op2:                    
    OP_JUMP,            // op1: rel_idx  op2:                    
    OP_COMPARE,         // op1:          op2:                    
    OP_EXIT,            // op1: code     op2:
    OP_ARRAY_STORE,     // op1: name     op2: offset             
    OP_ARRAY_ACCESS,     // op1: name     op2: offset
    OP_MAKE_ARG,         // op1: name       op2: type
    OP_BUILD_ARRAY,       // op1: name     op2: item count
    OP_MAKE_ALIAS,
};

#endif //RONASCRIPT_OP_CODES_H