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
#include <utility>
#include "../support/log.h"
#include "../exceptions/UnexpectedSymbolError.h"
#include "../lexer/lexer.h"
#include "ast/module.h"

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

    // Setup the char map.rn
    this->char_map[TokenType::R_BRACE] = "{";
    this->char_map[TokenType::L_BRACE] = "}";
    this->char_map[TokenType::R_PARAN] = "(";
    this->char_map[TokenType::L_PARAN] = ")";
    this->char_map[TokenType::R_BRACK] = "[";
    this->char_map[TokenType::L_BRACK] = "]";
    this->char_map[TokenType::R_CARAT] = "<";
    this->char_map[TokenType::L_CARAT] = ">";
    this->char_map[TokenType::SLASH] = "/";
    this->char_map[TokenType::PLUS] = "+";
    this->char_map[TokenType::STAR] = "*";
    this->char_map[TokenType::MINUS] = "-";
    this->char_map[TokenType::PERCENT] = "%";
    this->char_map[TokenType::AMPER] = "&";
    this->char_map[TokenType::BAR] = "|";
    this->char_map[TokenType::UP_ARROW] = "^";
    this->char_map[TokenType::TILDE] = "~";
    this->char_map[TokenType::BLOCK_COMMENT] = "/*";
    this->char_map[TokenType::INLINE_COMMENT] = "//";
    this->char_map[TokenType::DOLLAR] = "$";
    this->char_map[TokenType::NOT] = "!";
    this->char_map[TokenType::EQUAL] = "=";
    this->char_map[TokenType::COMMA] = ",";
    this->char_map[TokenType::DOT] = ".";
    this->char_map[TokenType::COLON] = ":";
    this->char_map[TokenType::SEMICOLON] = ":";
    this->char_map[TokenType::DBL_QUOTE] = "\"";
    this->char_map[TokenType::DBL_PLUS] = "++";
    this->char_map[TokenType::DBL_MINUS] = "--";
    this->char_map[TokenType::DBL_EQUAL] = "==";
    this->char_map[TokenType::PLUS_EQUAL] = "+=";
    this->char_map[TokenType::MINUS_EQUAL] = "-=";
    this->char_map[TokenType::PERCENT_EQUAL] = "%=";
    this->char_map[TokenType::SLASH_EQUAL] = "/=";
    this->char_map[TokenType::NOT_EQUAL] = "!=";
    this->char_map[TokenType::AMPER_EQUAL] = "&=";
    this->char_map[TokenType::BAR_EQUAL] = "|=";
    this->char_map[TokenType::STAR_EQUAL] = "*=";
    this->char_map[TokenType::DBL_STAR] = "**";
    this->char_map[TokenType::TILDE_EQUAL] = "~=";
    this->char_map[TokenType::DBL_AMPER] = "&&";
    this->char_map[TokenType::DBL_BAR] = "||";
    this->char_map[TokenType::EMPTY_LIST] = "{[]";
    this->char_map[TokenType::LEQ] = "<=";
    this->char_map[TokenType::GEQ] = ">=";
    this->char_map[TokenType::XOREQ] = "^=";
    this->char_map[TokenType::R_ARROW] = "->";
    this->char_map[TokenType::FLOAT] = "float";
    this->char_map[TokenType::BOOL] = "bool";
    this->char_map[TokenType::INT] = "int";
    this->char_map[TokenType::STRING] = "string";
    this->char_map[TokenType::OBJECT] = "object";
    this->char_map[TokenType::ARRAY] = "array";
    this->char_map[TokenType::CLASS] = "class";
    this->char_map[TokenType::VAR] = "var";
    this->char_map[TokenType::FUNC] = "func";
    this->char_map[TokenType::REQUIRE] = "require";
    this->char_map[TokenType::RETURN] = "return";
    this->char_map[TokenType::BREAK] = "break";
    this->char_map[TokenType::VOID] = "void";
    this->char_map[TokenType::NONE] = "null";
    this->char_map[TokenType::CONSTRUCT] = "construct";
    this->char_map[TokenType::DESTRUCT] = "destruct";
    this->char_map[TokenType::IF] = "if";
    this->char_map[TokenType::ELIF] = "elif";
    this->char_map[TokenType::ELSE] = "else";
    this->char_map[TokenType::IS] = "is";
    this->char_map[TokenType::ALIAS] = "alias";
    this->char_map[TokenType::WHILE] = "while";
    this->char_map[TokenType::FOR] = "for";
}

/******************************************************************************
 * @brief
 */
Parser::~Parser() {
    delete ast;
}

/******************************************************************************
 * @brief
 * @param n
 */
void Parser::adv_buf(int n) {
    for (int i = 0; i < n; i++) {
        if (this->idx < this->tokens.size() - 1) {
            this->idx++;
            this->current = this->tokens[this->idx];

            if (idx < this->tokens.size() - 1) {
                this->peek = this->tokens[this->idx + 1];
            } else {
                this->peek = this->tokens[this->tokens.size() - 1];
            }

            if (idx >= 1) {
                this->lookback = this->tokens[this->idx - 1];
            }
        } else {
            this->end_of_buffer_reached = true;
        }
    }
}

/******************************************************************************
 * @brief
 * @param t
 */
void Parser::conditional_buf_adv(TokenType::TokenType_t t) {
    if (this->current->tok_type == t) {
        adv_buf(1);
    }
}

/******************************************************************************
 * @brief
 */

RequireStmt *Parser::parse_require_stmt() {
    auto node = new RequireStmt();
    node->file_pos.char_num = this->current->file_pos.char_num;
    node->file_pos.line_num = this->current->file_pos.line_num;

    expect(TokenType::NAME);
    adv_buf(1);
    node->source_file = this->current->lexeme;

    expect(TokenType::SEMICOLON);
    adv_buf(1);

    // Check if the module has already been parsed
    if (this->ast->modules.find(node->source_file) != this->ast->modules.end()) {
        node->module = this->ast->modules[node->source_file];
    } else {
        // Parse the module and create a new subtree from it
        Lexer lexer;
        lexer.load_file(this->working_dir + "/" + node->get_full_src_file());
        lexer.get_tokens();
        Parser parser;
        parser.tokens = lexer.tokens;
        parser.parse();
        node->module = new Module();
        node->module->name = node->source_file;
        node->module->scope = parser.ast->root;
        this->ast->modules[node->source_file] = node->module;
    }
    adv_buf(1);

    return node;
}

/******************************************************************************
 * @brief
 */
VarDecl *Parser::parse_var_decl(std::vector<Token *> qualifiers) {
    auto node = new VarDecl();
    if (this->current->tok_type == TokenType::CONST) {
        node->is_const = true;
    }

    if (!qualifiers.empty()) {
        node->qualifiers = std::move(qualifiers);
    }
    node->file_pos.line_num = this->current->file_pos.line_num;
    node->file_pos.char_num = this->current->file_pos.char_num;
    conditional_buf_adv(TokenType::VAR);
    conditional_buf_adv(TokenType::CONST);
    node->id = this->current->lexeme;
    expect(TokenType::COLON);
    adv_buf(2);
    node->type = this->current->lexeme;
    adv_buf(1);

    if (this->current->tok_type == TokenType::EQUAL) {
        adv_buf(1);
        node->init_value = parse_expr();
    } else {
        conditional_buf_adv(TokenType::SEMICOLON);
    };

    return node;
}

/******************************************************************************
 * @brief
 */
FuncDecl *Parser::parse_func_decl(std::vector<Token *> qualifiers) {
    auto node = new FuncDecl();

    if (!qualifiers.empty()) {
        node->qualifiers = std::move(qualifiers);
    }
    node->file_pos.line_num = this->current->file_pos.line_num;
    node->file_pos.char_num = this->current->file_pos.char_num;
    conditional_buf_adv(TokenType::FUNC);

    node->id = this->current->lexeme;
    adv_buf(2);

    // Get the function arguments
    while (this->current->tok_type != TokenType::L_PARAN) {
        auto arg = new ArgDecl();
        conditional_buf_adv(TokenType::VAR);

        if (this->current->tok_type == TokenType::NAME) {
            arg->id = parse_name();
        }
        conditional_buf_adv(TokenType::COLON);

        if (this->current->is_type()) {
            arg->type = this->current->lexeme;
            adv_buf(1);
        }
        conditional_buf_adv(TokenType::COMMA);
        node->args.emplace_back(arg);
    }

    conditional_buf_adv(TokenType::L_PARAN);

    // Get the function's return type
    if (this->current->tok_type == TokenType::COLON) {
        adv_buf(1);
        node->type = this->current->lexeme;
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

    if (this->current->tok_type == TokenType::NAME) {
        node->id = parse_name();
    }

    expect(TokenType::COLON);
    conditional_buf_adv(TokenType::COLON);

    if (this->current->is_type()) {
        node->type = this->current->lexeme;
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

    expect(TokenType::NAME);
    adv_buf(1);
    node->id = this->current->lexeme;
    adv_buf(1);

    // Check for inherited classes and parse if necessary
    if (this->current->tok_type == TokenType::R_PARAN) {
        adv_buf(1);

        while (this->current->tok_type != TokenType::L_PARAN) {
            node->parent_classes.emplace_back(parse_name());
            conditional_buf_adv(TokenType::COMMA);
        }
        adv_buf(1);
        conditional_buf_adv(TokenType::EQUAL);
    }

    node->scope = parse_scope();

    return node;
}

/******************************************************************************
 * @brief
 */
AstNode *Parser::get_expr_cmpnt() {
    // FuncCall, IndexedExpr, Expr,
    AstNode *node = nullptr;
    if (this->current->tok_type == TokenType::NAME) {
        if (this->peek->tok_type == TokenType::R_PARAN) {
            node = parse_func_call();
        } else if (this->peek->tok_type == TokenType::R_BRACK) {
            node = parse_indexed_expr();
        } else {
            node = parse_name();

            if (this->current->tok_type == TokenType::R_BRACK) {
                node = parse_indexed_expr(node);
            }
        }
    } else if (this->current->is_literal()) {
        if (this->current->tok_type == TokenType::INT_LITERAL) {
            node = new IntLiteral(static_cast<long>(std::stoi(this->current->lexeme)));
        } else if (this->current->tok_type == TokenType::FLOAT_LITERAL) {
            node = new FloatLiteral(std::stof(this->current->lexeme));
        } else if (this->current->tok_type == TokenType::STRING_LITERAL) {
            node = new StringLiteral(static_cast<std::string>(this->current->lexeme));
        } else if (this->current->tok_type == TokenType::BOOL_LITERAL) {
            node = new BoolLiteral(this->current->lexeme == "true" ? true : false);
        }
        adv_buf(1);
    } else if (this->current->tok_type == TokenType::EMPTY_LIST) {
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

    if (this->current->tok_type == TokenType::R_BRACK) {

        auto node = parse_list_literal();
        conditional_buf_adv(TokenType::L_BRACK);
        conditional_buf_adv(TokenType::SEMICOLON);
        conditional_buf_adv(stop_token);

        return node;
    } else if (lookback->is_operator() && current->is_unary_op() && peek->tok_type == TokenType::NAME) {
        return parse_unary_expr();
    } else {
        while (!result_stack.empty()) {

            // Handle string concatenation and single string assignment
            if (this->current->tok_type == TokenType::STRING_LITERAL) {
                if (this->lookback->tok_type == TokenType::EQUAL && this->peek->tok_type == TokenType::SEMICOLON) {
                    auto node = new StringLiteral(this->current->lexeme);
                    adv_buf(1);
                    conditional_buf_adv(TokenType::SEMICOLON);
                    conditional_buf_adv(stop_token);

                    return node;
                } else {
                    auto node = new StringLiteral(this->current->lexeme);
                    adv_buf(1);
                    conditional_buf_adv(TokenType::SEMICOLON);
                    conditional_buf_adv(stop_token);

                    result_stack.emplace_back(node);
                }
            }

            // End of expression
            if (this->current->tok_type == TokenType::R_BRACE
                || this->current->tok_type == TokenType::SEMICOLON
                || this->current->tok_type == TokenType::L_BRACK
                || (this->current->tok_type == TokenType::L_PARAN && op_stack.empty())
                || this->current->tok_type == stop_token) {
                // We should only get here at the end of an expression and at
                // the end of an expression we should always be here.

                conditional_buf_adv(TokenType::SEMICOLON);
                conditional_buf_adv(stop_token);

                // Return the subtree
                if (result_stack.size() == 1 || (result_stack.size() == 2 and op_stack.empty())) {
                    auto node = result_stack.back();
                    result_stack.pop_back();
                    conditional_buf_adv(TokenType::SEMICOLON);
                    conditional_buf_adv(stop_token);

                    return node;

                } else {
                    if (!op_stack.empty()
                        && (op_stack.back()->tok_type == TokenType::L_PARAN
                            || op_stack.back()->tok_type == TokenType::R_PARAN)
                            ) {
                        op_stack.pop_back();
                    }

                    while (result_stack.size() > 2 && !op_stack.empty()) {
                        if (op_stack.back()->tok_type == TokenType::R_PARAN) {
                            op_stack.pop_back();
                        }

                        if (!op_stack.empty()) {
                            if (op_stack.back()->is_unary_op() && this->lookback->is_operator() &&
                                result_stack.back()->node_type == NAME) {
                                auto node = new UnaryExpr();
                                node->expr = dynamic_cast<Name *>(result_stack.back());
                                node->op = op_stack.back()->lexeme;
                                op_stack.pop_back();
                                result_stack.push_back(node);

                            } else {
                                auto node = new BinaryExpr();
                                node->right = result_stack.back();
                                result_stack.pop_back();
                                node->left = result_stack.back();
                                result_stack.pop_back();
                                node->op = op_stack.back()->lexeme;
                                op_stack.pop_back();
                                result_stack.push_back(node);
                            }
                        }

                    }

                    auto expr = result_stack.back();
                    result_stack.pop_back();
                    conditional_buf_adv(TokenType::SEMICOLON);
                    conditional_buf_adv(stop_token);

                    return expr;
                }
            } else {
                // Shunting-yard prec_tbl parsing
                // https://en.wikipedia.org/wiki/Shunting-yard_algorithm
                if (this->current->is_operator()
                    || this->current->tok_type == TokenType::L_PARAN
                    || this->current->tok_type == TokenType::R_PARAN) {
                    if (op_stack.empty() || this->prec_tbl[this->current->lexeme] > prec_tbl[op_stack.back()->lexeme]) {
                        // Push operator onto operator stack if it is higher prec_tbl than top of stack
                        op_stack.push_back(this->current);
                        adv_buf(1);
                    } else if (this->prec_tbl[this->current->lexeme] <= this->prec_tbl[op_stack.back()->lexeme]) {
                        // Create subtree from result stack if it is lower prec_tbl than top of operator stack
                        if (this->lookback->is_operator() && this->peek->tok_type == TokenType::NAME) {
                            result_stack.push_back(parse_unary_expr());
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

                        if (this->current->is_operator()) {
                            op_stack.push_back(this->current);
                            adv_buf(1);
                        }
                    }

                    // If matching parantheses are found, pop the operator stack and advance the buffer
                    if (!op_stack.empty() && op_stack.back()->tok_type == TokenType::R_PARAN &&
                        this->current->tok_type == TokenType::L_PARAN) {
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
UnaryExpr *Parser::parse_unary_expr(Name *name) {
    auto node = new UnaryExpr();

    if (this->current->tok_type == TokenType::NAME) {
        node->expr = parse_name();
        node->op = this->current->lexeme;
        adv_buf(1);
    } else if (name != nullptr) {
        node->expr = name;
        node->op = this->current->lexeme;
        adv_buf(1);
    } else {
        node->op = this->current->lexeme;
        adv_buf(1);
        node->expr = parse_name();
    }

    return node;
}

/******************************************************************************
 * @brief
 */
BreakStmt *Parser::parse_break_stmt() {
    auto node = new BreakStmt();
    adv_buf(1);
    expect(TokenType::SEMICOLON);
    adv_buf(1);

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
ContinueStmt *Parser::parse_continue_stmt() {
    auto node = new ContinueStmt();
    adv_buf(1);
    expect(TokenType::SEMICOLON);
    adv_buf(1);

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
ReturnStmt *Parser::parse_return_stmt() {
    auto node = new ReturnStmt();
    adv_buf(1);

    if (this->current->tok_type == TokenType::SEMICOLON) {
        adv_buf(1);
    } else {
        node->expr = parse_expr();
    }

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
AssignmentStmt *Parser::parse_assignment_stmt(Name *name) {
    auto node = new AssignmentStmt();

    if (name == nullptr) {
        node->id = parse_name();
    } else {
        node->id = name;
    }

    if (this->current->is_cmpnd_op()) {
        node->expr = new BinaryExpr();
        dynamic_cast<BinaryExpr *>(node->expr)->left = name; // Add Name Class
        dynamic_cast<BinaryExpr *>(node->expr)->op = this->current->lexeme[0];
        adv_buf(1);
        dynamic_cast<BinaryExpr *>(node->expr)->right = parse_expr();
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

    if (this->current->tok_type == TokenType::R_PARAN) {
        adv_buf(1);
        node->test = parse_expr();
        //conditional_buf_adv(TokenType::R_BRACE);
        node->consequent = parse_scope();
        conditional_buf_adv(TokenType::L_BRACE);

        if (this->current->tok_type == TokenType::ELIF) {
            node->alternative = parse_elif_stmt();
        } else if (this->current->tok_type == TokenType::ELSE) {
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

    if (this->current->tok_type == TokenType::R_PARAN) {
        adv_buf(1);
        node->test = parse_expr();
        conditional_buf_adv(TokenType::R_BRACE);
        node->consequent = parse_scope();
        conditional_buf_adv(TokenType::L_BRACE);

        if (this->current->tok_type == TokenType::ELIF) {
            node->alternative = parse_elif_stmt();
        } else if (this->current->tok_type == TokenType::ELSE) {
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
    convert_scope(node);
    conditional_buf_adv(TokenType::R_BRACE);
    parse();

    if (this->current->tok_type == TokenType::L_BRACE) {
        revert_scope();
        adv_buf(1);
    }

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
FuncCall *Parser::parse_func_call(Name *name) {
    auto node = new FuncCall();

    if (name == nullptr) {
        node->id = parse_name();
    } else {
        node->id = name;
    }
    conditional_buf_adv(TokenType::R_PARAN);

    while (this->current->tok_type != TokenType::L_PARAN && this->current->tok_type != TokenType::SEMICOLON) {
        node->args.emplace_back(parse_expr(TokenType::COMMA));
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
    conditional_buf_adv(TokenType::R_BRACK);

    while (this->current->tok_type != TokenType::L_BRACK && this->current->tok_type != TokenType::SEMICOLON) {
        node->items.emplace_back(parse_expr(TokenType::COMMA));
    }
    conditional_buf_adv(TokenType::L_BRACK);
    conditional_buf_adv(TokenType::SEMICOLON);

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
WhileLoop *Parser::parse_while_loop() {
    auto node = new WhileLoop();
    conditional_buf_adv(TokenType::WHILE);

    if (this->current->tok_type == TokenType::R_PARAN) { ;
        node->test = parse_expr(TokenType::COMMA);
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

    if (this->peek->tok_type == TokenType::EQUAL || this->peek->is_cmpnd_op()) {
        auto name = parse_name();
        node->update = parse_assignment_stmt(name);
    } else {
        node->update = parse_expr();
    }
    conditional_buf_adv(TokenType::L_PARAN);
    conditional_buf_adv(TokenType::R_BRACE);
    node->scope = parse_scope();

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
AliasDecl *Parser::parse_alias_decl() {
    auto node = new AliasDecl();
    node->alias_type = this->current->tok_type == TokenType::TYPE ? TYPE_ALIAS : NAME_ALIAS;
    expect(TokenType::NAME);

    adv_buf(1);
    node->alias_name = parse_name();
    expect(TokenType::NAME);
    adv_buf(1);

    node->base_name = parse_name();
    adv_buf(1);

    expect(TokenType::SEMICOLON);

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
AstNode *Parser::parse_indexed_expr(AstNode *expr) {
    auto node = new IndexedExpr();

    if (expr != nullptr) {
        node->expr = expr;
    } else {
        node->expr = parse_name();
    }

    adv_buf(1);
    node->idx = parse_expr(TokenType::L_BRACK);

    // Handle array indexed assignment
    if (this->current->tok_type == TokenType::EQUAL) {
        adv_buf(1);
        auto output = new AssignmentStmt();
        output->id = dynamic_cast<Name *>(node->expr);
        output->idx = node->idx;
        output->expr = parse_expr();

        return output;
    } else {
        node->is_access = true;

        return node;
    }
}

/******************************************************************************
 * @brief
 * @return
 */
Name *Parser::parse_name() {
    std::string delimiter = "->";
    auto name = new Name();
    name->value = this->current->lexeme;
    adv_buf(1);

    while (this->current->tok_type == TokenType::R_ARROW) {
        adv_buf(1);
        name->value += delimiter + this->current->lexeme;
        adv_buf(1);
    }

    expect(TokenType::NAME);

    if (this->current->tok_type == TokenType::NAME) {
        name->value += this->current->lexeme;
        adv_buf(1);
    }

    return name;
}

/******************************************************************************
 * @brief
 * @return
 */
TryBlock *Parser::parse_try_block() {
    auto node = new TryBlock();
    adv_buf(1);
    node->scope = parse_scope();
    node->catch_block = this->parse_catch_block();
    node->exception_ids = node->catch_block->exception_ids;

    return node;
}

/******************************************************************************
 * @brief
 * @return
 */
CatchBlock *Parser::parse_catch_block() {
    auto node = new CatchBlock();

    // current token should now be CATCH: catch (Exception1, Exception2, ...) {}
    adv_buf(1);

    // Check if any specific exceptions are caught, otherwise all exceptions are caught
    if (this->current->tok_type == TokenType::R_PARAN) {
        adv_buf(1);
        while (this->current->tok_type == TokenType::NAME) {
            node->exception_ids.emplace_back(parse_name());
            conditional_buf_adv(TokenType::COMMA);
        }
        adv_buf(1);
    }

    node->scope = parse_scope();

    return node;
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
    std::string output = this->ast->root->to_string();

    for (auto &[key, module] : this->ast->modules) {
        output += module->to_string();
    }

    return output;
}

/******************************************************************************
 * @brief
 */
void Parser::parse() {
    std::vector<Token *> qualifiers;

    if (!this->tokens.empty()) {
        while (!this->end_of_buffer_reached) {
            //Log::DEBUG(this->current->to_string());
            Name *name;

            switch (this->current->tok_type) {
                case TokenType::BLOCK_COMMENT:
                case TokenType::INLINE_COMMENT:
                    adv_buf(1);
                    break;
                case TokenType::REQUIRE:
                    this->current_scope->add_subtree(parse_require_stmt(), true);
                    break;
                case TokenType::R_BRACE:
                    this->current_scope->add_subtree(parse_scope());
                    break;
                case TokenType::L_BRACE:
                    return;
                case TokenType::PUBLIC:
                case TokenType::PROTECTED:
                case TokenType::PRIVATE:
                case TokenType::STATIC:
                case TokenType::LITERAL:
                case TokenType::REFERENCE:
                    qualifiers = get_qualifiers();
                    break;
                case TokenType::CONST:
                case TokenType::VAR:
                    this->current_scope->add_var_decl(parse_var_decl(qualifiers));
                    conditional_buf_adv(TokenType::SEMICOLON);
                    qualifiers.clear();
                    break;
                case TokenType::TYPE:
                case TokenType::ALIAS:
                    this->current_scope->add_subtree(parse_alias_decl());
                    break;
                case TokenType::FUNC:
                    this->current_scope->add_func_decl(parse_func_decl(qualifiers));
                    qualifiers.clear();
                    break;
                case TokenType::RETURN:
                    this->current_scope->add_subtree(parse_return_stmt());
                    break;
                case TokenType::BREAK:
                    this->current_scope->add_subtree(parse_break_stmt());
                    break;
                case TokenType::CONTINUE:
                    this->current_scope->add_subtree(parse_continue_stmt());
                    break;
                case TokenType::CLASS:
                    this->current_scope->add_class_decl(parse_class_decl());
                    break;
                case TokenType::NAME:
                    name = parse_name();

                    if (this->current->tok_type == TokenType::EQUAL || this->current->is_cmpnd_op()) {
                        this->current_scope->add_subtree(parse_assignment_stmt(name));
                    } else if (this->current->is_unary_op()) {
                        this->current_scope->add_subtree(parse_unary_expr(name));
                    } else if (this->current->tok_type == TokenType::R_PARAN) {
                        this->current_scope->add_subtree(parse_func_call(name));
                    } else if (this->current->tok_type == TokenType::R_BRACK) {
                        this->current_scope->add_subtree(parse_indexed_expr(name));
                    }
                    break;
                case TokenType::IF:
                    this->current_scope->add_subtree(parse_if_stmt());
                    break;
                case TokenType::ELIF:
                case TokenType::ELSE:
                    throw std::exception();
                case TokenType::WHILE:
                    this->current_scope->add_subtree(parse_while_loop());
                    break;
                case TokenType::FOR:
                    this->current_scope->add_subtree(parse_for_loop());
                    break;
                case TokenType::TRY:
                    this->current_scope->add_subtree(parse_try_block());
                    break;
                case TokenType::DBL_MINUS:
                case TokenType::DBL_PLUS:
                case TokenType::DBL_STAR:
                    this->current_scope->add_subtree(parse_unary_expr());
                    break;
                case TokenType::SEMICOLON:
                    adv_buf(1);
                    break;
                default:
                    throw UnexpectedSymbolError(this->current->lexeme, "", this->current->file_pos.line_num,
                                                this->current->file_pos.char_num);
            }
        }
    }
}

/******************************************************************************
 * @brief
 * @param token_type
 */
void Parser::expect(TokenType::TokenType_t token_type) {
    expected_token = token_type;
}

/******************************************************************************
 * @brief
 */
void Parser::check_expected_token() {
    if (this->expected_token != TokenType::UNDEFINED && this->current->tok_type != this->expected_token) {
        //throw UnexpectedSymbolError(this->current->lexeme, this->char_map[this->expected_token], this->current->file_pos.line_num, this->current->file_pos.char_num);
    }

    this->expected_token = TokenType::UNDEFINED;
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Token *> Parser::get_qualifiers() {
    std::vector<Token *> qualifiers;

    while (this->current->is_qualifier()) {
        qualifiers.push_back(this->current);
        adv_buf(1);
    }

    return qualifiers;
}

/******************************************************************************
 * @brief
 * @param t
 */
void Parser::load_tokens(std::vector<Token *> t) {
    this->tokens = std::move(t);

    if (!tokens.empty()) {
        this->current = tokens[0];
    }

    if (tokens.size() > 1) {
        this->peek = tokens[1];
    }
}
