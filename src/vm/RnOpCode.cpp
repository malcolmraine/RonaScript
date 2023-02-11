/******************************************************************************
* File: RnOpCode.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/19/22
* Version: 1
*
******************************************************************************/

#include "RnOpCode.h"

/*****************************************************************************/
std::string GetOpCodeAsString(RnOpCode opcode) {
    switch (opcode) {
        case OP_ATTR_ACCESS:
            return "OP_ATTR_ACCESS";
        case OP_RESOLVE_NAMESPACE:
            return "OP_RESOLVE_NAMESPACE";
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
        case OP_BREAK:
            return "OP_BREAK";
        case OP_CONTINUE:
            return "OP_CONTINUE";
        case OP_TRY_CONTEXT:
            return "OP_TRY_CONTEXT";
        case OP_UNARY_INVERT:
            return "OP_UNARY_INVERT";
        case OP_UNARY_DECREMENT:
            return "OP_UNARY_DECREMENT";
        case OP_UNARY_INCREMENT:
            return "OP_UNARY_INCREMENT";
        case OP_UNARY_NOT:
            return "OP_UNARY_NOT";
        case OP_UNARY_NEGATION:
            return "OP_UNARY_NEGATION";
        case OP_RETURN:
            return "OP_RETURN";
        case OP_LOAD_INT:
            return "OP_LOAD_INT";
        case OP_LOAD_FLOAT:
            return "OP_LOAD_FLOAT";
        case OP_LOAD_STRING:
            return "OP_LOAD_STRING";
        case OP_LOAD_BOOL:
            return "OP_LOAD_BOOL";
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
        case OP_MAKE_CLASS:
            return "OP_MAKE_CLASS";
        case OP_MAKE_FUNC:
            return "OP_MAKE_FUNC";
        case OP_MAKE_MODULE:
            return "OP_MAKE_MODULE";
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
        default:
            return "UNKNOWN";
    }
}

/*****************************************************************************/
RnOpCode GetOpCodeFromOperator(const std::string& op) {
    if (op == "+") {
        return OP_BINARY_ADD;
    } else if (op == "-") {
        return OP_BINARY_SUB;
    } else if (op == "/") {
        return OP_BINARY_DIV;
    } else if (op == "*") {
        return OP_BINARY_MUL;
    } else if (op == "^") {
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
    } else if (op == "::") {
        return OP_RESOLVE_NAMESPACE;
    } else if (op == "->") {
        return OP_ATTR_ACCESS;
    } else if (op == "%") {
        return OP_BINARY_MOD;
    } else {
        return OP_NOP;
    }
}