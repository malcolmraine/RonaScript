/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
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
******************************************************************************/

#pragma once

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
