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

#include "parser.h"
#include <iostream>


/******************************************************************************
 * @brief
 */
Parser::Parser() {
    ast = new Ast();
    current_scope = ast->root;

    // Setup the associativity table
    associativity["("] = NONE;
    associativity["*"] = LEFT;
    associativity["/"] = LEFT;
    associativity["%"] = LEFT;
    associativity["+"] = LEFT;
    associativity["-"] = LEFT;
    associativity[">>"] = LEFT;
    associativity["<<"] = LEFT;
    associativity[">"] = LEFT;
    associativity["<"] = LEFT;
    associativity[">="] = LEFT;
    associativity["<="] = LEFT;
    associativity["=="] = LEFT;
    associativity["!="] = LEFT;
    associativity["&"] = LEFT;
    associativity["&&"] = LEFT;
    associativity["^"] = LEFT;
    associativity["|"] = LEFT;
    associativity["||"] = LEFT;
    associativity[")"] = NONE;

    // Setup the prec_tbl table
    prec_tbl["("] = 100;
    prec_tbl["*"] = 90;
    prec_tbl["/"] = 90;
    prec_tbl["%"] = 90;
    prec_tbl["+"] = 80;
    prec_tbl["-"] = 80;
    prec_tbl[">>"] = 70;
    prec_tbl["<<"] = 70;
    prec_tbl[">"] = 60;
    prec_tbl["<"] = 60;
    prec_tbl[">="] = 60;
    prec_tbl["<="] = 60;
    prec_tbl["=="] = 50;
    prec_tbl["!="] = 50;
    prec_tbl["&"] = 40;
    prec_tbl["&&"] = 40;
    prec_tbl["^"] = 30;
    prec_tbl["|"] = 20;
    prec_tbl["||"] = 20;
    prec_tbl[")"] = NONE;
}

/******************************************************************************
 * @brief
 */
Parser::~Parser() {
    for (auto &token : tokens) {
        delete token;
    }
}

/******************************************************************************
 * @brief
 * @return
 */
Token *Parser::peek() {
    if (idx < tokens.size() - 1) {
        return tokens[idx + 1];
    } else {
        return tokens[tokens.size() - 1];
    }
}

/******************************************************************************
 * @brief
 * @return
 */
Token *Parser::current() {
    return tokens[idx];
}

/******************************************************************************
 * @brief
 * @return
 */
Token *Parser::lookback() {
    if (idx > 0) {
        return tokens[idx - 1];
    } else {
        return tokens[0];
    }
}

/******************************************************************************
 * @brief
 * @param n
 */
void Parser::adv_buf(int n) {
    for (int i = 0; i < n; i++) {
        if (idx < tokens.size() - 1) {
            idx++;
        }
    }
}

/******************************************************************************
 * @brief
 * @param t
 */
void Parser::conditional_buf_adv(TokenType::TokenType_t t) {
    if (current()->tok_type == t) {
        adv_buf(1);
    }
}

/******************************************************************************
 * @brief
 */
RequireStmt *Parser::parse_require_stmt() {
    auto node = new RequireStmt();

    // Expect STRING_LITERAL
    adv_buf(1);
    node->source_file = peek()->lexeme;

    // Expect SEMICOLON
    adv_buf(1);

    return node;
}

/******************************************************************************
 * @brief
 */
VarDecl *Parser::parse_var_decl() {
    auto node = new VarDecl();
    node->file_pos.line_num = current()->file_pos.line_num;
    node->file_pos.char_num = current()->file_pos.char_num;

    // Expect NAME
    adv_buf(1);
    node->id = current()->lexeme;

    // Expect ':'
    adv_buf(2);
    node->type = current()->lexeme;
    adv_buf(1);

    if (current()->tok_type == TokenType::EQUAL) {
        adv_buf(1);
        node->init_value = parse_expr();
    }

    return node;
}

/******************************************************************************
 * @brief
 */
FuncDecl *Parser::parse_func_decl() {
    auto node = new FuncDecl();

    // Expect NAME
    adv_buf(1);
    node->id = current()->lexeme;
    adv_buf(1);
    conditional_buf_adv(TokenType::R_PARAN);

    // Get the function arguments
    while (current()->tok_type != TokenType::L_PARAN) {
        auto arg = new ArgDecl();
        conditional_buf_adv(TokenType::VAR);

        if (current()->tok_type == TokenType::NAME) {
            arg->id = parse_name();
        }

        conditional_buf_adv(TokenType::COLON);

        if (current()->is_type()) {
            arg->type = current()->lexeme;
            adv_buf(1);
        }

        conditional_buf_adv(TokenType::COMMA);
        node->args.emplace_back(arg);
    }

    conditional_buf_adv(TokenType::L_PARAN);

    // Get the function's return type
    if (current()->tok_type == TokenType::COLON) {
        adv_buf(1);
        node->type = current()->lexeme;
        adv_buf(1);
    } else {
        node->type = "void";
    }

    // Get the function's scope
    node->scope = parse_scope();

    return node;
}

/******************************************************************************
 * @brief
 */
ArgDecl *Parser::parse_arg_decl() {
    auto node = new ArgDecl();
    conditional_buf_adv(TokenType::VAR);

    if (current()->tok_type == TokenType::NAME) {
        node->id = parse_name();
    }

    // Expect COLON
    conditional_buf_adv(TokenType::COLON);

    if (current()->is_type()) {
        node->type = current()->lexeme;
        adv_buf(1);
    }
    conditional_buf_adv(TokenType::COMMA);

    return node;
}

/******************************************************************************
 * @brief
 */
ClassDecl *Parser::parse_class_decl() {
    auto node = new ClassDecl();

    // Expect NAME
    adv_buf(1);
    node->id = current()->lexeme;
    adv_buf(1);
    node->scope = parse_scope();

    return node;
}

/******************************************************************************
 * @brief
 */
AstNode *Parser::get_expr_cmpnt() {
    // FuncCall, IndexedExpr, Expr,
    AstNode *node = nullptr;
    if (current()->tok_type == TokenType::NAME) {
        if (peek()->tok_type == TokenType::R_PARAN) {
            node = parse_func_decl();
        } else if (peek()->tok_type == TokenType::R_BRACK) {
            node = parse_indexed_expr();
        } else {
            node = parse_name();
        }
    } else if (current()->is_literal()) {
        if (current()->tok_type == TokenType::INT_LITERAL) {
            node = new IntLiteral(std::stoi(current()->lexeme));
        } else if (current()->tok_type == TokenType::FLOAT_LITERAL) {
            node = new FloatLiteral(std::stof(current()->lexeme));
        } else if (current()->tok_type == TokenType::STRING_LITERAL) {
            node = new StringLiteral(current()->lexeme);
        }
        adv_buf(1);
    } else if (current()->tok_type == TokenType::EMPTY_LIST) {
        node = new ListLiteral();
        adv_buf(1);
    }

    return node;
}

/******************************************************************************
 * @brief
 */
AstNode *Parser::parse_expr(TokenType::TokenType_t stop_token) {
    std::vector<Token *> op_stack;
    std::vector<AstNode *> result_stack;
    result_stack.emplace_back(new Expr());

    if (current()->tok_type == TokenType::STRING_LITERAL) {
        auto node = new Expr();
        node->expr = new StringLiteral(current()->lexeme);
        adv_buf(1);
        conditional_buf_adv(TokenType::SEMICOLON);

        return node;
    } else if (current()->tok_type == TokenType::R_BRACK) {
        auto node = new Expr();
        node->expr = parse_list_literal();
        conditional_buf_adv(TokenType::L_BRACK);
        conditional_buf_adv(TokenType::SEMICOLON);

        return node;
    } else {
        while (!result_stack.empty()) {
            // End of expression
            if (current()->tok_type == TokenType::R_BRACE
                || current()->tok_type == TokenType::SEMICOLON
                || current()->tok_type == stop_token) {
                // We should only get here at the end of an expression and at
                // the end of an expression we should always be here.

                conditional_buf_adv(TokenType::SEMICOLON);
                conditional_buf_adv(stop_token);

                // Return the subtree
                if (result_stack.size() == 1 || (result_stack.size() == 2 and op_stack.empty())) {
                    auto node = result_stack.back();
                    result_stack.pop_back();

                    conditional_buf_adv(TokenType::SEMICOLON);

                    // TODO: Dont use dynamic_cast
                    return reinterpret_cast<Expr *>(node);
                } else {
                    if (op_stack.back()->tok_type == TokenType::L_PARAN
                        || op_stack.back()->tok_type == TokenType::R_PARAN
                            ) {
                        op_stack.pop_back();
                    }

                    while (result_stack.size() > 2) {
                        if (op_stack.back()->tok_type == TokenType::R_PARAN) {
                            op_stack.pop_back();
                        }

                        if (op_stack.back()->is_unary_op()) {
                            auto node = new UnaryExpr();
                            node->id = dynamic_cast<Name *>(result_stack.back());
                            node->op = op_stack.back()->lexeme;
                            result_stack.push_back(node);
                        } else {
                            auto node = new BinaryExpr();
                            node->right = result_stack.back();
                            result_stack.pop_back();
                            node->left = result_stack.back();
                            result_stack.pop_back();
                            node->op = op_stack.back()->lexeme;
                            result_stack.push_back(node);
                        }
                    }
                    auto expr = result_stack.back();
                    result_stack.pop_back();
                    conditional_buf_adv(TokenType::SEMICOLON);

                    // TODO: Dont use dynamic_cast
                    return expr;
                }
            } else {
                // Shunting-yard prec_tbl parsing
                // https://en.wikipedia.org/wiki/Shunting-yard_algorithm
                if (current()->is_operator()
                    || current()->tok_type == TokenType::L_PARAN
                    || current()->tok_type == TokenType::R_PARAN) {
                    if (op_stack.empty() || this->prec_tbl[current()->lexeme] > prec_tbl[op_stack.back()->lexeme]) {
                        // Push operator onto operator stack if it is higher prec_tbl than top of stack
                        op_stack.push_back(current());
                        adv_buf(1);
                    } else if (this->prec_tbl[current()->lexeme] <= this->prec_tbl[op_stack.back()->lexeme]) {
                        // Create subtree from result stack if it is lower prec_tbl than top of operator stack
                        if (op_stack.back()->is_unary_op()) {
                            auto node = new UnaryExpr();

                            // TODO: Dont use dynamic_cast
                            node->id = dynamic_cast<Name *>(result_stack.back());
                            result_stack.pop_back();
                            node->op = op_stack.back()->lexeme;
                            op_stack.pop_back();
                            result_stack.push_back(node);
                        } else if (op_stack.back()->is_binary_op()) {
                            auto node = new BinaryExpr();
                            node->right = result_stack.back();
                            result_stack.pop_back();
                            node->left = result_stack.back();
                            result_stack.pop_back();
                            node->op = op_stack.back()->lexeme;
                            op_stack.pop_back();

                            // Handle left associativity
                            if (!op_stack.empty() and associativity[op_stack.back()->lexeme] == LEFT) {
                                auto expr2 = new BinaryExpr();
                                expr2->left = result_stack.back();
                                result_stack.pop_back();
                                expr2->right = node;
                                expr2->op = op_stack.back()->lexeme;
                                op_stack.pop_back();

                                result_stack.push_back(expr2);
                            } else {
                                result_stack.push_back(node);
                            }
                        }

                        op_stack.push_back(current());
                        adv_buf(1);
                    }

                    // If matching parantheses are found, pop the operator stack and advance the buffer
                    if (op_stack.back()->tok_type == TokenType::R_PARAN and current()->tok_type == TokenType::L_PARAN) {
                        op_stack.pop_back();
                        adv_buf(1);
                    }
                } else {
                    result_stack.push_back(get_expr_cmpnt());
                }
            }
        }
    }
    return new Expr();
}

/******************************************************************************
 * @brief
 */
UnaryExpr *Parser::parse_unary_expr() {
    auto node = new UnaryExpr();
    node->op = current()->lexeme;

    return node;
}

///******************************************************************************
// * @brief
// */
//BinaryExpr *Parser::parse_binary_expr() {
//    auto node = new BinaryExpr();
//    node->right = new Expr();
//}

/******************************************************************************
 * @brief
 */
BreakStmt *Parser::parse_break_stmt() {
    auto node = new BreakStmt();
    adv_buf(1);
    // Expect ';'
    adv_buf(1);

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
FlowStmt *Parser::parse_flow_stmt() {
    auto node = new FlowStmt();

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
ReturnStmt *Parser::parse_return_stmt() {
    auto node = new ReturnStmt();
    adv_buf(1);
    node->expr = parse_expr();

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
AssignmentStmt *Parser::parse_assignment_stmt() {
    auto node = new AssignmentStmt();
    conditional_buf_adv(TokenType::EQUAL);
    node->id = new Name(current()->lexeme); //parse_name();
    adv_buf(1);

    if (current()->is_cmpnd_op()) {
//        node->expr = new BinaryExpr();
//        node->expr->left = nullptr; // Add Name Class
//        node->expr->op = current()->lexeme[0];
//        node->expr->right = parse_expr();
    } else {
        adv_buf(1);
        node->expr = parse_expr();
        conditional_buf_adv(TokenType::SEMICOLON);
    }

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
IfStmt *Parser::parse_if_stmt() {
    auto node = new IfStmt();
    adv_buf(1);

    if (current()->tok_type == TokenType::R_PARAN) {
        adv_buf(1);
        node->test = parse_expr();
        conditional_buf_adv(TokenType::R_BRACE);
        node->consequent = parse_scope();
        conditional_buf_adv(TokenType::L_BRACE);

        if (current()->tok_type == TokenType::ELIF) {
            node->alternative = parse_elif_stmt();
        } else if (current()->tok_type == TokenType::ELSE) {
            node->alternative = parse_else_stmt();
        } else {
            node->alternative = nullptr;
        }
    }

    conditional_buf_adv(TokenType::SEMICOLON);

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
ElifStmt *Parser::parse_elif_stmt() {
    auto node = new ElifStmt();
    adv_buf(1);

    if (current()->tok_type == TokenType::R_PARAN) {
        adv_buf(1);
        node->test = parse_expr();
        conditional_buf_adv(TokenType::R_BRACE);
        node->consequent = parse_scope();
        conditional_buf_adv(TokenType::L_BRACE);

        if (current()->tok_type == TokenType::ELIF) {
            node->alternative = parse_elif_stmt();
        } else if (current()->tok_type == TokenType::ELSE) {
            node->alternative = parse_else_stmt();
        } else {
            node->alternative = nullptr;
        }
    }

    conditional_buf_adv(TokenType::SEMICOLON);

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
ElseStmt *Parser::parse_else_stmt() {
    auto node = new ElseStmt();
    adv_buf(1);
    node->consequent = parse_scope();

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
ScopeNode *Parser::parse_scope() {
    auto node = new ScopeNode();
    node->parent = current_scope;
    convert_scope(current_scope);
    adv_buf(1);
    conditional_buf_adv(TokenType::R_BRACE);
    parse();

    if (current()->tok_type == TokenType::L_BRACE) {
        revert_scope();
        adv_buf(1);
    }

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
FuncCall *Parser::parse_func_call() {
    auto node = new FuncCall();
    node->id = current()->lexeme;
    adv_buf(1);
    conditional_buf_adv(TokenType::R_PARAN);

    while (current()->tok_type != TokenType::L_PARAN && current()->tok_type != TokenType::SEMICOLON) {
        node->args.emplace_back(parse_expr());
    }
    adv_buf(1);

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
ListLiteral *Parser::parse_list_literal() {
    auto node = new ListLiteral();
    adv_buf(1);
    conditional_buf_adv(TokenType::R_BRACK);

    while (current()->tok_type != TokenType::L_BRACK && current()->tok_type != TokenType::SEMICOLON) {
        node->items.emplace_back(parse_expr(TokenType::SEMICOLON));
    }
    adv_buf(1);

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
WhileLoop *Parser::parse_while_loop() {
    auto node = new WhileLoop();
    conditional_buf_adv(TokenType::WHILE);

    if (current()->tok_type == TokenType::R_PARAN) {
        node->test = parse_expr();
    }
    node->scope = parse_scope();

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
ForLoop *Parser::parse_for_loop() {
    auto node = new ForLoop();
    adv_buf(2);
    node->init = parse_var_decl();
    conditional_buf_adv(TokenType::SEMICOLON);
    node->test = parse_expr();
    conditional_buf_adv(TokenType::SEMICOLON);
    node->update = parse_expr();
    conditional_buf_adv(TokenType::L_PARAN);
    node->scope = parse_scope();

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
AliasDecl *Parser::parse_alias_decl() {
    auto node = new AliasDecl();

    // Expect NAME token

    adv_buf(1);
    node->alias_name = new Name(current()->lexeme);
    adv_buf(1);

    // Expect 'is'

    adv_buf(1);
    node->base_name = new Name(current()->lexeme);
    adv_buf(1);

    // Expect ';'

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
IndexedExpr *Parser::parse_indexed_expr() {
    return nullptr;
}

/******************************************************************************
 * @brief
 * @return
 */
Name *Parser::parse_name() {
    std::string delimiter = "->";
    auto name = new Name();
    name->value = current()->lexeme;
    adv_buf(1);

    while (current()->tok_type == TokenType::R_ARROW) {
        adv_buf(1);
        name->value += current()->lexeme;
        adv_buf(1);
    }

    // Expect NAME

    name->value += current()->lexeme;
    adv_buf(1);

    return name;
}

/******************************************************************************
 * @brief
 */
void Parser::revert_scope() {
    if (current_scope->parent != nullptr) {
        current_scope = current_scope->parent;
    }
}

/******************************************************************************
 * @brief
 */
void Parser::convert_scope(ScopeNode *scope) {
    scope->parent = current_scope;
    current_scope = scope;
}

/******************************************************************************
 * @brief
 * @return
 */
std::string Parser::dumps_ast() {
    return " ";
}

/******************************************************************************
 * @brief
 */
void Parser::parse() {
    std::cout << "HERE";
    while (idx < tokens.size()) {
        switch (current()->tok_type) {
            case TokenType::BLOCK_COMMENT:
            case TokenType::INLINE_COMMENT:
                adv_buf(1);
                break;
            case TokenType::REQUIRE:
                current_scope->add_subtree(parse_require_stmt());
                break;
            case TokenType::R_BRACE:
                current_scope->add_subtree(parse_scope());
                break;
            case TokenType::L_BRACE:
                revert_scope();
                adv_buf(1);
                return;
            case TokenType::VAR:
                current_scope->add_subtree(parse_var_decl());
                conditional_buf_adv(TokenType::SEMICOLON);
                break;
            case TokenType::ALIAS:
                current_scope->add_subtree(parse_alias_decl());
                break;
            case TokenType::FUNCTION:
                current_scope->add_subtree(parse_func_decl(), true);
                break;
            case TokenType::RETURN:
                current_scope->add_subtree(parse_return_stmt());
                break;
            case TokenType::BREAK:
                current_scope->add_subtree(parse_break_stmt());
                break;
            case TokenType::CLASS:
                current_scope->add_subtree(parse_class_decl(), true);
                break;
            case TokenType::NAME:
                if (peek()->tok_type == TokenType::EQUAL || peek()->is_cmpnd_op()) {
                    current_scope->add_subtree(parse_assignment_stmt());
                } else if (peek()->is_unary_op()) {
                    current_scope->add_subtree(parse_unary_expr());
                }
                break;
            case TokenType::R_PARAN:
                current_scope->add_subtree(parse_func_call());
                break;
            case TokenType::IF:
                current_scope->add_subtree(parse_if_stmt());
                break;
            case TokenType::ELIF:
            case TokenType::ELSE:
                throw std::exception();
            case TokenType::WHILE:
                current_scope->add_subtree(parse_while_loop());
                break;
            case TokenType::FOR:
                current_scope->add_subtree(parse_for_loop());
                break;
            default:
                adv_buf(1);

                if (idx == tokens.size() - 1) {
                    return;
                }
                break;

        }
    }
}
