/******************************************************************************
* File: parser.h
* Description: Definition the Parser class.
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

#ifndef RONASCRIPT_PARSER_H
#define RONASCRIPT_PARSER_H

#include <vector>
#include <string>
#include <map>
#include "../lexer/token.h"
#include "../lexer/token_type.h"
#include "ast/Ast.h"
#include "ast/StringLiteral.h"
#include "ast/ScopeNode.h"
#include "ast/RequireStmt.h"
#include "ast/ArgDecl.h"
#include "ast/AstNode.h"
#include "ast/ForLoop.h"
#include "ast/AliasDecl.h"
#include "ast/Expr.h"
#include "ast/VarDecl.h"
#include "ast/FloatLiteral.h"
#include "ast/UnaryExpr.h"
#include "ast/ReturnStmt.h"
#include "ast/FuncCall.h"
#include "ast/IndexedExpr.h"
#include "ast/Ast.h"
#include "ast/ClassDecl.h"
#include "ast/NodeType.h"
#include "ast/WhileLoop.h"
#include "ast/BreakStmt.h"
#include "ast/IfStmt.h"
#include "ast/FuncDecl.h"
#include "ast/BinaryExpr.h"
#include "ast/AssignmentStmt.h"
#include "ast/ListLiteral.h"
#include "ast/ElifStmt.h"
#include "ast/IntLiteral.h"
#include "ast/ContinueStmt.h"
#include "ast/ElseStmt.h"
#include "ast/Name.h"
#include "ast/try_block.h"
#include "ast/catch_block.h"
#include "ast/BoolLiteral.h"

enum Associativity_t {
    LEFT,
    RIGHT,
    NONE,
};

class Parser {
public:
    Parser();
    ~Parser();
    void adv_buf(int n);
    void conditional_buf_adv(TokenType::TokenType_t t);
    RequireStmt *parse_require_stmt();
    VarDecl *parse_var_decl(std::vector<Token *> qualifiers = {});
    FuncDecl *parse_func_decl(std::vector<Token *> qualifiers = {});
    ArgDecl *parse_arg_decl();
    ClassDecl *parse_class_decl();
    AstNode *get_expr_cmpnt();
    AstNode *parse_expr(TokenType::TokenType_t stop_token = TokenType::SEMICOLON);
    UnaryExpr *parse_unary_expr(Name *name = nullptr);
    BreakStmt *parse_break_stmt();
    ContinueStmt *parse_continue_stmt();
    ReturnStmt *parse_return_stmt();
    AssignmentStmt *parse_assignment_stmt(Name *name = nullptr);
    IfStmt *parse_if_stmt();
    ElifStmt *parse_elif_stmt();
    ElseStmt *parse_else_stmt();
    ScopeNode *parse_scope();
    FuncCall *parse_func_call(Name *name = nullptr);
    ListLiteral *parse_list_literal();
    WhileLoop *parse_while_loop();
    ForLoop *parse_for_loop();
    AliasDecl *parse_alias_decl();
    AstNode *parse_indexed_expr(AstNode *expr = nullptr);
    Name *parse_name();
    TryBlock *parse_try_block();
    CatchBlock *parse_catch_block();
    void revert_scope();
    void convert_scope(ScopeNode *scope);
    std::string dumps_ast();
    void parse();
    std::vector<Token *> get_qualifiers();
    void load_tokens(std::vector<Token *> t);

    Ast *ast;
    ScopeNode *current_scope = nullptr;
    std::vector<Token *> tokens;
    std::string working_dir;
    Token *current = nullptr;
    Token *peek = nullptr;
    Token *lookback = nullptr;
    std::map<std::string, std::string> type_alias_map;
    std::map<std::string, std::string> name_alias_map;

protected:
    void expect(TokenType::TokenType_t token_type);
    void check_expected_token();
    bool end_of_buffer_reached = false;
    std::map<TokenType::TokenType_t, std::string> char_map;
    TokenType::TokenType_t expected_token = TokenType::UNDEFINED;
    std::map<std::string, int> prec_tbl;
    std::map<std::string, Associativity_t> associativity;
    unsigned long idx = 0;
};

#endif //RONASCRIPT_PARSER_H