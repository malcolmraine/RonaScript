/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_NODETYPE_H
#define RONASCRIPT_NODETYPE_H

enum NodeType_t {
    AST_ASSIGNMENT_STMT,
    AST_BINARY_EXPR,
    AST_INDEXED_EXPR,
    AST_WHILE_LOOP,
    AST_FOR_LOOP,
    AST_CLASS_DECL,
    AST_EXPR,
    AST_CONTINUE_STMT,
    AST_FUNC_CALL,
    AST_FUNC_DECL,
    AST_LIST_LITERAL,
    AST_STRING_LITERAL,
    AST_BOOL_LITERAL,
    AST_FLOAT_LITERAL,
    AST_INT_LITERAL,
    AST_IMPORT,
    AST_IF_STMT,
    AST_ELIF_STMT,
    AST_ELSE_STMT,
    AST_ARG_DECL,
    AST_ALIAS_DECL,
    AST_RETURN_STMT,
    AST_UNARY_EXPR,
    AST_VAR_DECL,
    AST_SCOPE,
    AST_BREAK_STMT,
    AST_NAME,
    AST_MODULE,
    AST_DEFAULT,
    AST_EXIT_STMT,
    AST_DELETE_STMT,
    AST_TRY_BLOCK,
    AST_CATCH_BLOCK,

};

#endif  //RONASCRIPT_NODETYPE_H
