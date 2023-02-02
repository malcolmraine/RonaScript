/*****************************************************************************
* File: RnOpCode.h
* Description:
* Author: Malcolm Hall
* Date: 5/19/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNOPCODE_H
#define RONASCRIPT_RNOPCODE_H

#include <string>

typedef enum {
    OP_ATTR_ACCESS,
    OP_RESOLVE_NAMESPACE,
    OP_BINARY_ADD,    // op1:          op2:
    OP_BINARY_SUB,    // op1:          op2:
    OP_BINARY_MUL,    // op1:          op2:
    OP_BINARY_DIV,    // op1:          op2:
    OP_BINARY_MOD,    // op1:          op2:
    OP_BINARY_GTE,    // op1:          op2:
    OP_BINARY_LTE,    // op1:          op2:
    OP_BINARY_GT,     // op1:          op2:
    OP_BINARY_LT,     // op1:          op2:
    OP_BINARY_EQ,     // op1:          op2:
    OP_BINARY_NEQ,    // op1:          op2:
    OP_BINARY_POWER,  // op1:          op2:
    OP_BINARY_RSH,    // op1:          op2:
    OP_BINARY_LSH,    // op1:          op2:
    OP_BINARY_OR,     // op1:          op2:
    OP_BINARY_XOR,    // op1:          op2:
    OP_BINARY_AND,    // op1:          op2:
    OP_LOGICAL_OR,
    OP_LOGICAL_AND,
    OP_STORE,      // op1: name     op2:
    OP_POP,        // op1:          op2:
    OP_UNARY_NOT,  // op1: name     op2:
    OP_BREAK,      // op1: rel_idx  op2:
    OP_CONTINUE,   // op1: rel_idx  op2:
    OP_TRY_CONTEXT,
    OP_UNARY_INVERT,     // op1: name     op2:
    OP_UNARY_DECREMENT,  // op1: name     op2:
    OP_UNARY_INCREMENT,  // op1: name     op2:
    OP_UNARY_NEGATION,
    OP_RETURN,    // op1: rel_idx  op2:
    OP_LOAD_INT,  // op1: value    op2:
    OP_LOAD_FLOAT,
    OP_LOAD_STRING,
    OP_LOAD_NULL,
    OP_LOAD_BOOL,
    OP_CALL,  // op1: name     op2: _arg1 count
    OP_MAKE_CONST,
    OP_MAKE_VAR,
    OP_MAKE_CLASS,
    OP_MAKE_FUNC,       // op1: name     op2: instr. count
    OP_MAKE_MODULE,     // op1: name     op2: instr. count
    OP_CREATE_CONTEXT,  // op1: flag     op2:
    OP_DESTROY_CONTEXT,
    OP_DELETE,  // op1: name     op2:
    OP_NOP,     // op1:          op2:
    OP_JUMPF,   // op1: rel_idx  op2:
    OP_JUMPB,
    OP_JUMPF_IF,
    OP_JUMPB_IF,
    OP_COMPARE,       // op1:          op2:
    OP_EXIT,          // op1:      op2:
    OP_ARRAY_STORE,   // op1: name     op2: offset
    OP_INDEX_ACCESS,  // op1: name     op2: offset
    OP_MAKE_ARG,      // op1: name       op2: type
    OP_MAKE_ALIAS,
    OP_MAKE_ARRAY,
    OP_LOAD_VALUE
} RnOpCode;

std::string GetOpCodeAsString(RnOpCode opcode);
RnOpCode GetOpCodeFromOperator(const std::string& op);

#endif  //RONASCRIPT_RNOPCODE_H
