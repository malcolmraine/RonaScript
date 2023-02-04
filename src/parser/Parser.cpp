/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "Parser.h"
#include <iostream>
#include <memory>
#include <utility>
#include "../exceptions/UnexpectedSymbolError.h"
#include "../lexer/Lexer.h"
#include "../util/RnStack.h"
#include "../util/log.h"
#include "ast/AliasDecl.h"
#include "ast/ArgDecl.h"
#include "ast/ArrayLiteral.h"
#include "ast/AssignmentStmt.h"
#include "ast/Ast.h"
#include "ast/AstNode.h"
#include "ast/BinaryExpr.h"
#include "ast/BoolLiteral.h"
#include "ast/BreakStmt.h"
#include "ast/CatchBlock.h"
#include "ast/ClassDecl.h"
#include "ast/ContinueStmt.h"
#include "ast/DeleteStmt.h"
#include "ast/ElifStmt.h"
#include "ast/ElseStmt.h"
#include "ast/ExitStmt.h"
#include "ast/Expr.h"
#include "ast/FloatLiteral.h"
#include "ast/ForLoop.h"
#include "ast/FuncCall.h"
#include "ast/FuncDecl.h"
#include "ast/IfStmt.h"
#include "ast/ImportStmt.h"
#include "ast/IndexedExpr.h"
#include "ast/IntLiteral.h"
#include "ast/Module.h"
#include "ast/Name.h"
#include "ast/NodeType.h"
#include "ast/NullLiteral.h"
#include "ast/ReturnStmt.h"
#include "ast/ScopeNode.h"
#include "ast/StringLiteral.h"
#include "ast/TryBlock.h"
#include "ast/UnaryExpr.h"
#include "ast/VarDecl.h"
#include "ast/WhileLoop.h"

/*****************************************************************************/
Parser::Parser() {
    FillBuffer(nullptr);
    ast = new Ast();
    _current_scope = ast->root;

    // TODO: Add rest of builtin functions
    _current_scope->symbol_table->AddSymbol(
        "print", std::make_shared<RnTypeComposite>(RnType::RN_NULL));
    _current_scope->symbol_table->AddSymbol(
        "prompt", std::make_shared<RnTypeComposite>(RnType::RN_NULL));
    _current_scope->symbol_table->AddSymbol(
        "read", std::make_shared<RnTypeComposite>(RnType::RN_NULL));
    _current_scope->symbol_table->AddSymbol(
        "write", std::make_shared<RnTypeComposite>(RnType::RN_NULL));
    _current_scope->symbol_table->AddSymbol(
        "array_merge", std::make_shared<RnTypeComposite>(RnType::RN_NULL));
    _current_scope->symbol_table->AddSymbol(
        "array_push", std::make_shared<RnTypeComposite>(RnType::RN_NULL));
    _current_scope->symbol_table->AddSymbol(
        "array_pop", std::make_shared<RnTypeComposite>(RnType::RN_NULL));
    _current_scope->symbol_table->AddSymbol(
        "count", std::make_shared<RnTypeComposite>(RnType::RN_INT));

    _current_scope->pragma_table["bounds"] = "not-enforced";
    _current_scope->pragma_table["typing"] = "not-enforced";

    // Set up the associativity table
    associativity[TokenType::R_PARAN] = NO_ASSOCIATIVITY;
    associativity[TokenType::R_ARROW] = LEFT;
    associativity[TokenType::DOUBLE_COLON] = LEFT;
    associativity[TokenType::STAR] = LEFT;
    associativity[TokenType::SLASH] = LEFT;
    associativity[TokenType::PERCENT] = LEFT;
    associativity[TokenType::PLUS] = LEFT;
    associativity[TokenType::MINUS] = LEFT;
    associativity[TokenType::DBL_R_CARAT] = LEFT;
    associativity[TokenType::DBL_L_CARAT] = LEFT;
    associativity[TokenType::L_CARAT] = LEFT;
    associativity[TokenType::R_CARAT] = LEFT;
    associativity[TokenType::GEQ] = LEFT;
    associativity[TokenType::LEQ] = LEFT;
    associativity[TokenType::DBL_EQUAL] = LEFT;
    associativity[TokenType::NOT_EQUAL] = LEFT;
    associativity[TokenType::AMPER] = LEFT;
    associativity[TokenType::DBL_AMPER] = LEFT;
    associativity[TokenType::UP_ARROW] = LEFT;
    associativity[TokenType::BAR] = LEFT;
    associativity[TokenType::DBL_BAR] = LEFT;
    associativity[TokenType::L_PARAN] = NO_ASSOCIATIVITY;

    // Set up the _prec_tbl table
    _prec_tbl[TokenType::DBL_COLON] = 200;
    _prec_tbl[TokenType::R_ARROW] = 200;
    //_prec_tbl["["] = 150;
    _prec_tbl[TokenType::R_PARAN] = 100;
    _prec_tbl[TokenType::STAR] = 90;
    _prec_tbl[TokenType::SLASH] = 90;
    _prec_tbl[TokenType::PERCENT] = 90;
    _prec_tbl[TokenType::PLUS] = 80;
    _prec_tbl[TokenType::MINUS] = 80;
    _prec_tbl[TokenType::DBL_R_CARAT] = 70;
    _prec_tbl[TokenType::DBL_L_CARAT] = 70;
    _prec_tbl[TokenType::L_CARAT] = 60;
    _prec_tbl[TokenType::R_CARAT] = 60;
    _prec_tbl[TokenType::LEQ] = 60;
    _prec_tbl[TokenType::GEQ] = 60;
    _prec_tbl[TokenType::DBL_EQUAL] = 50;
    _prec_tbl[TokenType::NOT_EQUAL] = 50;
    _prec_tbl[TokenType::AMPER] = 40;
    _prec_tbl[TokenType::DBL_AMPER] = 40;
    _prec_tbl[TokenType::UP_ARROW] = 30;
    _prec_tbl[TokenType::BAR] = 20;
    _prec_tbl[TokenType::DBL_BAR] = 20;
    _prec_tbl[TokenType::L_PARAN] = 0;

    // Set up the char map.rn
    _char_map[TokenType::R_BRACE] = "{";
    _char_map[TokenType::L_BRACE] = "}";
    _char_map[TokenType::R_PARAN] = "(";
    _char_map[TokenType::L_PARAN] = ")";
    _char_map[TokenType::R_BRACK] = "[";
    _char_map[TokenType::L_BRACK] = "]";
    _char_map[TokenType::R_CARAT] = "<";
    _char_map[TokenType::L_CARAT] = ">";
    _char_map[TokenType::SLASH] = "/";
    _char_map[TokenType::PLUS] = "+";
    _char_map[TokenType::STAR] = "*";
    _char_map[TokenType::MINUS] = "-";
    _char_map[TokenType::PERCENT] = "%";
    _char_map[TokenType::AMPER] = "&";
    _char_map[TokenType::BAR] = "|";
    _char_map[TokenType::UP_ARROW] = "^";
    _char_map[TokenType::TILDE] = "~";
    _char_map[TokenType::BLOCK_COMMENT] = "/*";
    _char_map[TokenType::INLINE_COMMENT] = "//";
    _char_map[TokenType::DOLLAR] = "$";
    _char_map[TokenType::NOT] = "!";
    _char_map[TokenType::EQUAL] = "=";
    _char_map[TokenType::COMMA] = ",";
    _char_map[TokenType::DOT] = ".";
    _char_map[TokenType::COLON] = ":";
    _char_map[TokenType::SEMICOLON] = ":";
    _char_map[TokenType::DBL_QUOTE] = "\"";
    _char_map[TokenType::DBL_PLUS] = "++";
    _char_map[TokenType::DBL_MINUS] = "--";
    _char_map[TokenType::DBL_EQUAL] = "==";
    _char_map[TokenType::PLUS_EQUAL] = "+=";
    _char_map[TokenType::MINUS_EQUAL] = "-=";
    _char_map[TokenType::PERCENT_EQUAL] = "%=";
    _char_map[TokenType::SLASH_EQUAL] = "/=";
    _char_map[TokenType::NOT_EQUAL] = "!=";
    _char_map[TokenType::AMPER_EQUAL] = "&=";
    _char_map[TokenType::BAR_EQUAL] = "|=";
    _char_map[TokenType::STAR_EQUAL] = "*=";
    _char_map[TokenType::DBL_STAR] = "**";
    _char_map[TokenType::TILDE_EQUAL] = "~=";
    _char_map[TokenType::DBL_AMPER] = "&&";
    _char_map[TokenType::DBL_BAR] = "||";
    _char_map[TokenType::EMPTY_LIST] = "{[]";
    _char_map[TokenType::LEQ] = "<=";
    _char_map[TokenType::GEQ] = ">=";
    _char_map[TokenType::XOREQ] = "^=";
    _char_map[TokenType::R_ARROW] = "->";
    _char_map[TokenType::DOUBLE_COLON] = "::";
    _char_map[TokenType::FLOAT] = "float";
    _char_map[TokenType::BOOL] = "bool";
    _char_map[TokenType::INT] = "int";
    _char_map[TokenType::STRING] = "string";
    _char_map[TokenType::OBJECT] = "object";
    _char_map[TokenType::ARRAY] = "array";
    _char_map[TokenType::CLASS] = "class";
    _char_map[TokenType::VAR] = "var";
    _char_map[TokenType::FUNC] = "func";
    _char_map[TokenType::R_BRACE] = "begin";
    _char_map[TokenType::L_BRACE] = "end";
    _char_map[TokenType::IMPORT] = "IMPORT";
    _char_map[TokenType::RETURN] = "return";
    _char_map[TokenType::BREAK] = "break";
    _char_map[TokenType::VOID] = "void";
    _char_map[TokenType::NULL_LITERAL] = "null";
    _char_map[TokenType::CONSTRUCT] = "construct";
    _char_map[TokenType::DESTRUCT] = "destruct";
    _char_map[TokenType::IF] = "if";
    _char_map[TokenType::ELIF] = "elif";
    _char_map[TokenType::ELSE] = "else";
    _char_map[TokenType::IS] = "is";
    _char_map[TokenType::ALIAS] = "alias";
    _char_map[TokenType::WHILE] = "while";
    _char_map[TokenType::FOR] = "for";
    _char_map[TokenType::LOCAL] = "local";
    _char_map[TokenType::GLOBAL] = "global";
}

/*****************************************************************************/
Parser::~Parser() {
    delete ast;
}

/*****************************************************************************/
void Parser::ConditionalBufAdvance(TokenType t) {
    if (Current()->token_type == t) {
        AdvanceBuffer(1);
    }
}

/*****************************************************************************/
std::shared_ptr<ImportStmt> Parser::ParseImportStmt() {
    auto node = std::make_shared<ImportStmt>();
    node->file_info = new FileInfo(Current()->file_info);
    Expect(TokenType::NAME);
    AdvanceBuffer(1);
    node->source_file = Current()->lexeme;
    Expect(TokenType::SEMICOLON);
    AdvanceBuffer(1);

    // Check if the module has already been parsed
    if (ast->modules.find(node->source_file) != ast->modules.end()) {
        node->module = ast->modules[node->source_file];
    } else {
        // Parse the module and create a new subtree from it
        Lexer lexer;
        std::filesystem::path module_file =
            working_dir + "/" + node->GetFullSourceFile();

        if (std::filesystem::absolute(module_file) == std::filesystem::absolute(file)) {
            throw std::runtime_error("Circular dependency error: " +
                                     module_file.string());
        }

        lexer.LoadFile(module_file);
        lexer.ProcessTokens();
        Parser parser;
        parser.tokens = lexer.tokens;
        parser.working_dir = module_file.parent_path();
        parser.LoadTokens(lexer.tokens);
        parser.Parse();
        node->module = parser.ast->modules[node->source_file];
        parser.ast->modules[node->source_file].reset();
        ast->modules[node->source_file] = node->module;
    }
    AdvanceBuffer(1);

    return node;
}

/*****************************************************************************/
std::shared_ptr<VarDecl> Parser::ParseVarDecl(std::vector<Token*> qualifiers) {
    auto node = std::make_shared<VarDecl>();
    node->file_info = new FileInfo(Current()->file_info);
    Expect(TokenType::NAME);

    if (Current()->token_type == TokenType::CONST) {
        node->is_const = true;
    } else if (Current()->token_type == TokenType::GLOBAL) {
        node->is_global = true;
    } else if (Current()->token_type == TokenType::LOCAL) {
        node->is_local = true;
    }

    if (!qualifiers.empty()) {
        node->qualifiers = std::move(qualifiers);
    }

    AdvanceBuffer(1);
    node->file_pos.line_num = Current()->file_pos.line_num;
    node->file_pos.char_num = Current()->file_pos.char_num;
    node->id = Current()->lexeme;
    Expect(TokenType::COLON);
    AdvanceBuffer(1);
    Expect({
        TokenType::STRING,
        TokenType::INT,
        TokenType::FLOAT,
        TokenType::BOOL,
        TokenType::ARRAY,
        TokenType::CALLABLE,
        TokenType::OBJECT,
    });
    AdvanceBuffer(1);
    node->type = ParseType();

    if (Current()->token_type == TokenType::EQUAL) {
        AdvanceBuffer(1);
        node->init_value = ParseExpr();
    } else {
        ConditionalBufAdvance(TokenType::SEMICOLON);
    }

    return node;
}

/*****************************************************************************/
std::shared_ptr<FuncDecl> Parser::ParseFuncDecl(std::vector<Token*> qualifiers) {
    auto node = std::make_shared<FuncDecl>();
    node->file_info = new FileInfo(Current()->file_info);

    if (!qualifiers.empty()) {
        node->qualifiers = std::move(qualifiers);
    }

    ConditionalBufAdvance(TokenType::FUNC);

    node->id = Current()->lexeme;
    AdvanceBuffer(2);

    // Get the function arguments
    std::map<std::string, std::shared_ptr<RnTypeComposite>> arg_symbols;
    while (Current()->token_type != TokenType::L_PARAN) {
        auto arg = new ArgDecl();
        Expect(TokenType::NAME);
        AdvanceBuffer(1);

        if (Current()->token_type == TokenType::NAME) {
            arg->SetId(ParseName());

            if (arg_symbols.find(arg->GetId()->value) != arg_symbols.end()) {
                throw std::runtime_error("Redeclaration of argument '" +
                                         arg->GetId()->value + "' in routine '" +
                                         node->id + "'");
            }
        }
        AdvanceBuffer(1);  // Advance past the ':' separating the name from the type

        if (Current()->IsType()) {
            arg->SetType(ParseType());
            AdvanceBuffer(1);
        } else {
            throw std::runtime_error("Invalid type '" + Current()->lexeme +
                                     "' for parameter '" + arg->GetId()->value +
                                     "' while declaring routine '" + node->id + "'");
        }

        ConditionalBufAdvance(TokenType::COMMA);
        node->args.emplace_back(arg);
        arg_symbols[arg->GetId()->value] = arg->GetType();

        if (Current()->token_type != TokenType::VAR) {
            ConditionalBufAdvance(TokenType::COMMA);
            break;
        }
    }

    ConditionalBufAdvance(TokenType::L_PARAN);

    // Get the function's return type
    if (Current()->token_type == TokenType::COLON) {
        AdvanceBuffer(1);
        node->type = ParseType();

        if (node->type->GetType() != RnType::RN_OBJECT &&
            _current_state == CLASS_DECL_CONTEXT && node->id == "construct") {
            throw std::runtime_error("Class constructor must return object type");
        }

        AdvanceBuffer(1);
    } else {
        node->type = std::make_shared<RnTypeComposite>(RnType::RN_VOID);
    }

    // Get the function's scope
    node->scope = ParseScope();
    for (const auto& symbol : arg_symbols) {
        node->scope->symbol_table->AddSymbol(symbol.first, symbol.second);
    }

    if (_current_state == CLASS_DECL_CONTEXT) {
        node->scope->symbol_table->AddSymbol(
            "this", std::make_shared<RnTypeComposite>(RnType::RN_OBJECT));
    }
    return node;
}

/*****************************************************************************/
std::shared_ptr<ClassDecl> Parser::ParseClassDecl() {
    auto node = std::make_shared<ClassDecl>();
    node->file_info = new FileInfo(Current()->file_info);
    Expect(TokenType::NAME);
    AdvanceBuffer(1);

    node->id = Current()->lexeme;
    Expect({TokenType::EXTENDS, TokenType::R_BRACE, TokenType::IS});
    AdvanceBuffer(1);

    // Check for inherited classes and parse if necessary
    if (Current()->token_type == TokenType::EXTENDS) {
        Expect(TokenType::NAME);
        AdvanceBuffer(1);

        while (Current()->token_type == TokenType::NAME) {
            node->parent_classes.emplace_back(ParseName());
            ConditionalBufAdvance(TokenType::COMMA);
        }
    }
    AdvanceBuffer(1);

    _previous_state = _current_state;
    _current_state = ParserState::CLASS_DECL_CONTEXT;
    node->scope = ParseScope();
    _current_state = _previous_state;

    return node;
}

/*****************************************************************************/
std::shared_ptr<AstNode> Parser::GetExprComponent() {
    std::shared_ptr<AstNode> node = nullptr;

    if ((Lookback() && Lookback()->IsOperator()) && Current()->IsUnaryOp() &&
        Peek()->token_type == TokenType::NAME) {
        return ParseUnaryExpr();
    } else if (Current()->token_type == TokenType::R_BRACK) {
        node = ParseArrayLiteral();
    } else if (Current()->token_type == TokenType::NAME) {
        node = ParseName();
        if (Current()->IsUnaryOp()) {
            node = ParseUnaryExpr(node);
        }
    } else if (Current()->IsLiteral()) {
        AdvanceBuffer(1);
        switch (Lookback()->token_type) {
            case TokenType::INT_LITERAL: {
                node = std::make_shared<IntLiteral>(
                    static_cast<long>(std::stoi(Lookback()->lexeme)));
                break;
            }
            case TokenType::FLOAT_LITERAL: {
                node = std::make_shared<FloatLiteral>(std::stof(Lookback()->lexeme));
                break;
            }
            case TokenType::STRING_LITERAL: {
                node = std::make_shared<StringLiteral>(static_cast<std::string>(
                    Lookback()->lexeme.substr(1, Lookback()->lexeme.length() - 2)));
                break;
            }
            case TokenType::BOOL_LITERAL: {
                node = std::make_shared<BoolLiteral>(Lookback()->lexeme == "true");
                break;
            }
            case TokenType::NAME: {
                // TODO: Remove, probably deadcode
                node = ParseName();
                break;
            }
            case TokenType::NULL_LITERAL: {
                node = std::make_shared<NullLiteral>();
                break;
            }
            default:
                break;
        }
    }

    if (node) {
        node->file_info = new FileInfo(Current()->file_info);
    }
    return node;
}

/*****************************************************************************/
std::shared_ptr<AstNode> Parser::ParseExpr(TokenType stop_token) {
    RnStack<Token*> op_stack;
    RnStack<std::shared_ptr<AstNode>> result_stack;
    result_stack.Push(std::shared_ptr<AstNode>());

    auto make_binary_expr = [this, &result_stack, &op_stack]() mutable {
        auto node = std::make_shared<BinaryExpr>();
        node->_right = result_stack.Pop();
        node->_left = result_stack.Pop();
        node->_op = op_stack.Pop()->lexeme;
        return std::static_pointer_cast<BinaryExpr>(TransformBinaryExpr(node));
    };

    while (!result_stack.IsEmpty()) {
        if (Lookback() &&
            (Lookback()->IsOperator() ||
             unary_lookback_set.contains(Lookback()->token_type)) &&
            Current()->IsUnaryOp()) {
            if (Peek()->token_type == TokenType::INT_LITERAL ||
                Peek()->token_type == TokenType::FLOAT_LITERAL) {
                result_stack.push_back(GetExprComponent());
            } else {
                result_stack.Push(ParseUnaryExpr());
            }
        }

        if (Current()->token_type == TokenType::R_BRACK) {
            if (Lookback()->token_type == TokenType::NAME ||
                Current()->token_type == TokenType::L_PARAN) {
                result_stack.push_back(ParseIndexedExpr(result_stack.Pop()));
            } else {
                result_stack.Push(ParseArrayLiteral());
            }
        }

        if (Current()->token_type == TokenType::R_BRACE ||
            Current()->token_type == TokenType::SEMICOLON ||
            Current()->token_type == TokenType::L_BRACK ||
            (Current()->token_type == TokenType::L_PARAN && op_stack.IsEmpty()) ||
            Current()->token_type == stop_token || Current()->IsCompoundOp()) {
            // We should only get here at the end of an expression and at
            // the end of an expression we should always be here.
            ConditionalBufAdvance(stop_token);

            // Return the subtree
            if (result_stack.Size() == 1 ||
                (result_stack.Size() == 2 && op_stack.IsEmpty())) {
                return result_stack.Pop();
            } else {
                if (!op_stack.IsEmpty() &&
                    (op_stack.back()->token_type == TokenType::L_PARAN ||
                     op_stack.back()->token_type == TokenType::R_PARAN)) {
                    op_stack.Pop();
                }

                while (result_stack.Size() > 2 && !op_stack.IsEmpty()) {
                    if (op_stack.back()->token_type == TokenType::R_PARAN) {
                        op_stack.Pop();
                    }
                    auto node = make_binary_expr();
                    if (Current()->token_type == TokenType::R_BRACK) {
                        result_stack.Push(ParseIndexedExpr(node));
                    } else if (Current()->token_type == TokenType::R_PARAN) {
                        result_stack.Push(ParseFuncCall(node));
                    } else {
                        result_stack.Push(node);
                    }
                }
                return result_stack.Pop();
            }
        } else {
            // Shunting-yard _prec_tbl parsing
            if (Current()->IsOperator() ||
                Current()->token_type == TokenType::L_PARAN ||
                Current()->token_type == TokenType::R_PARAN) {

                if (Current()->token_type == TokenType::R_PARAN &&
                    !Lookback()->IsOperator() && result_stack.Top()) {
                    result_stack.Push(ParseFuncCall(result_stack.Pop()));
                } else if (op_stack.IsEmpty() ||
                           _prec_tbl[Current()->token_type] >
                               _prec_tbl[op_stack.back()->token_type]) {
                    // Push operator onto operator stack if it is higher _prec_tbl than Top of stack
                    op_stack.Push(Current());
                    AdvanceBuffer(1);
                } else if (_prec_tbl[Current()->token_type] <=
                           _prec_tbl[op_stack.back()->token_type]) {
                    // Create subtree from result stack if it is lower _prec_tbl than Top of operator stack
                    if (op_stack.back()->IsBinaryOp()) {
                        auto transformed_node = TransformBinaryExpr(make_binary_expr());

                        // Handle _left associativity
                        if (!op_stack.IsEmpty() and
                            associativity[op_stack.back()->token_type] == LEFT) {
                            auto expr2 = std::make_shared<BinaryExpr>();
                            expr2->_left = result_stack.Pop();
                            expr2->_right = transformed_node;
                            expr2->_op = op_stack.Pop()->lexeme;
                            auto transformed_expr = TransformBinaryExpr(expr2);
                            result_stack.Push(transformed_expr);
                        } else {
                            result_stack.Push(transformed_node);
                        }
                    }

                    if (Current()->IsOperator()) {
                        op_stack.Push(Current());
                        AdvanceBuffer(1);
                    }
                }

                // If matching parantheses are found, Pop the operator stack and advance the buffer
                if (!op_stack.IsEmpty() &&
                    op_stack.back()->token_type == TokenType::R_PARAN &&
                    Current()->token_type == TokenType::L_PARAN) {
                    op_stack.Pop();
                    AdvanceBuffer(1);
                }
            } else if (Current()->token_type == TokenType::R_BRACK) {
                result_stack.Push(ParseIndexedExpr(result_stack.Pop()));
            } else {
                result_stack.Push(GetExprComponent());

                if (Current()->token_type == TokenType::R_BRACK) {
                    result_stack.Push(ParseIndexedExpr(result_stack.Pop()));
                }
            }
        }
    }

    return std::make_shared<Expr>();
}

/*****************************************************************************/
std::shared_ptr<UnaryExpr> Parser::ParseUnaryExpr(
    const std::shared_ptr<AstNode>& expr) {
    auto node = std::make_shared<UnaryExpr>();
    node->op = Current()->lexeme;
    AdvanceBuffer(1);

    if (expr != nullptr) {
        node->expr = expr;
    } else {
        node->expr = ParseExpr();
    }

    return node;
}

/*****************************************************************************/
std::shared_ptr<BreakStmt> Parser::ParseBreakStmt() {
    auto node = std::make_shared<BreakStmt>();
    AdvanceBuffer(1);
    Expect(TokenType::SEMICOLON);
    AdvanceBuffer(1);

    return node;
}

/*****************************************************************************/
std::shared_ptr<ContinueStmt> Parser::ParseContinueStmt() {
    auto node = std::make_shared<ContinueStmt>();
    AdvanceBuffer(1);
    Expect(TokenType::SEMICOLON);
    AdvanceBuffer(1);

    return node;
}

/*****************************************************************************/
std::shared_ptr<ReturnStmt> Parser::ParseReturnStmt() {
    auto node = std::make_shared<ReturnStmt>();
    AdvanceBuffer(1);

    if (Current()->token_type == TokenType::SEMICOLON) {
        AdvanceBuffer(1);
    } else {
        node->expr = ParseExpr();
    }

    return node;
}

/*****************************************************************************/
std::shared_ptr<DeleteStmt> Parser::ParseDeleteStmt() {
    auto node = std::make_shared<DeleteStmt>();
    Expect(TokenType::NAME);
    AdvanceBuffer(1);

    if (Current()->token_type == TokenType::SEMICOLON) {
        AdvanceBuffer(1);
    } else {
        node->SetName(ParseExpr());
    }

    return node;
}

/*****************************************************************************/
std::shared_ptr<ExitStmt> Parser::ParseExitStmt() {
    auto node = std::make_shared<ExitStmt>();
    Expect({TokenType::SEMICOLON, TokenType::INT_LITERAL});
    AdvanceBuffer(1);

    if (Current()->token_type == TokenType::SEMICOLON) {
        node->exit_code = std::make_shared<IntLiteral>(0L);
        AdvanceBuffer(1);
    } else {
        node->exit_code = std::make_shared<IntLiteral>(
            static_cast<long>(std::stoi(Current()->lexeme)));
        AdvanceBuffer(2);
    }

    return node;
}

/*****************************************************************************/
std::shared_ptr<AssignmentStmt> Parser::ParseAssignmentStatement(
    const std::shared_ptr<AstNode>& rexpr) {
    auto node = std::make_shared<AssignmentStmt>();
    node->SetLexpr(rexpr ? rexpr : ParseExpr());

    std::string op;
    if (Current()->IsCompoundOp()) {
        op = Current()->lexeme[0];
        AdvanceBuffer(1);
    } else if (Lookback()->IsCompoundOp()) {
        op = Lookback()->lexeme[0];
    }

    if (!op.empty()) {
        node->SetRexpr(std::make_shared<BinaryExpr>());
        std::dynamic_pointer_cast<BinaryExpr>(node->GetRexpr())->_left = rexpr;
        std::dynamic_pointer_cast<BinaryExpr>(node->GetRexpr())->_op = op;
        std::dynamic_pointer_cast<BinaryExpr>(node->GetRexpr())->_right = ParseExpr();
    } else {
        ConditionalBufAdvance(TokenType::EQUAL);
        node->SetRexpr(ParseExpr());
    }

    return node;
}

/*****************************************************************************/
std::shared_ptr<IfStmt> Parser::ParseIfStmt() {
    auto node = std::make_shared<IfStmt>();
    AdvanceBuffer(1);
    node->test = ParseExpr(TokenType::COLON);
    node->consequent = ParseScope();
    ConditionalBufAdvance(TokenType::L_BRACE);

    if (Current()->token_type == TokenType::ELIF) {
        node->alternative = ParseElifStmt();
    } else if (Current()->token_type == TokenType::ELSE) {
        node->alternative = ParseElseStmt();
    } else {
        node->alternative = nullptr;
    }
    ConditionalBufAdvance(TokenType::SEMICOLON);

    return node;
}

/*****************************************************************************/
std::shared_ptr<ElifStmt> Parser::ParseElifStmt() {
    auto node = std::make_shared<ElifStmt>();
    AdvanceBuffer(1);
    node->test = ParseExpr(TokenType::COLON);
    node->consequent = ParseScope();
    ConditionalBufAdvance(TokenType::L_BRACE);

    if (Current()->token_type == TokenType::ELIF) {
        node->alternative = ParseElifStmt();
    } else if (Current()->token_type == TokenType::ELSE) {
        node->alternative = ParseElseStmt();
    } else {
        node->alternative = nullptr;
    }

    ConditionalBufAdvance(TokenType::SEMICOLON);

    return node;
}

/*****************************************************************************/
std::shared_ptr<ElseStmt> Parser::ParseElseStmt() {
    auto node = std::make_shared<ElseStmt>();
    AdvanceBuffer(1);
    node->consequent = ParseScope();

    return node;
}

/*****************************************************************************/
std::shared_ptr<ScopeNode> Parser::ParseScope() {
    auto node = std::make_shared<ScopeNode>();
    node->parent = _current_scope;
    node->symbol_table->SetParent(_current_scope->symbol_table);

    // Pragmas are carried downward until they are unset or out of scope
    for (const auto& entry : _current_scope->pragma_table) {
        node->pragma_table[entry.first] = entry.second;
    }
    ConvertScope(node);
    ConditionalBufAdvance(TokenType::R_BRACE);
    ConditionalBufAdvance(TokenType::COLON);
    Parse();

    if (Current()->token_type == TokenType::L_BRACE ||
        Current()->token_type == TokenType::ELSE ||
        Current()->token_type == TokenType::ELIF) {
        RevertScope();

        ConditionalBufAdvance(TokenType::L_BRACE);
    }

    return node;
}

/*****************************************************************************/
std::shared_ptr<FuncCall> Parser::ParseFuncCall(const std::shared_ptr<AstNode>& expr) {
    auto node = std::make_shared<FuncCall>();

    if (expr == nullptr) {
        node->expr = ParseExpr();
    } else {
        node->expr = expr;
    }
    ConditionalBufAdvance(TokenType::R_PARAN);

    while (!Current()->IsOneOf({TokenType::L_PARAN, TokenType::SEMICOLON})) {
        node->args.emplace_back(ParseExpr(TokenType::COMMA));
    }
    AdvanceBuffer(1);

    return node;
}

/*****************************************************************************/
std::shared_ptr<ArrayLiteral> Parser::ParseArrayLiteral() {
    auto node = std::make_shared<ArrayLiteral>();
    ConditionalBufAdvance(TokenType::R_BRACK);

    while (!Current()->IsOneOf({TokenType::L_BRACK, TokenType::SEMICOLON})) {
        node->items.emplace_back(ParseExpr(TokenType::COMMA));
    }
    AdvanceBuffer(1);

    return node;
}

/*****************************************************************************/
std::shared_ptr<WhileLoop> Parser::ParseWhileLoop() {
    auto node = std::make_shared<WhileLoop>();
    ConditionalBufAdvance(TokenType::WHILE);
    node->test = ParseExpr(TokenType::COLON);
    node->scope = ParseScope();

    return node;
}

/*****************************************************************************/
std::shared_ptr<ForLoop> Parser::ParseForLoop() {
    auto node = std::make_shared<ForLoop>();
    Expect(TokenType::R_PARAN);
    AdvanceBuffer(1);

    if (Peek()->token_type == TokenType::VAR) {
        AdvanceBuffer(1);
        node->init = ParseVarDecl();

    } else if (Peek()->token_type == TokenType::NAME) {
        AdvanceBuffer(1);

        if (Peek()->token_type == TokenType::EQUAL) {
            auto lexpr = ParseExpr(TokenType::EQUAL);
            node->init = ParseAssignmentStatement(lexpr);
        } else {
            node->init = ParseExpr();
        }
    } else {
        Expect(TokenType::SEMICOLON);
        AdvanceBuffer(1);
    }

    ConditionalBufAdvance(TokenType::SEMICOLON);
    node->test = ParseExpr();
    ConditionalBufAdvance(TokenType::SEMICOLON);

    if (Peek()->token_type == TokenType::EQUAL || Peek()->IsCompoundOp()) {
        auto name = ParseName();
        node->update = ParseAssignmentStatement(name);
    } else {
        node->update = ParseExpr();
    }
    ConditionalBufAdvance(TokenType::L_PARAN);
    ConditionalBufAdvance(TokenType::R_BRACE);
    node->scope = ParseScope();

    if (node->init->node_type == AST_VAR_DECL) {
        auto var_decl = std::dynamic_pointer_cast<VarDecl>(node->init);
        node->scope->symbol_table->AddSymbol(var_decl->id, var_decl->type);
    }

    return node;
}

/*****************************************************************************/
std::shared_ptr<AliasDecl> Parser::ParseAliasDecl() {
    auto node = std::make_shared<AliasDecl>();
    node->alias_type =
        Current()->token_type == TokenType::TYPE ? TYPE_ALIAS : NAME_ALIAS;
    Expect(TokenType::NAME);

    AdvanceBuffer(1);
    node->alias_name = ParseName();
    Expect(TokenType::NAME);
    AdvanceBuffer(1);

    node->base_name = ParseName();
    AdvanceBuffer(1);
    return node;
}

/*****************************************************************************/
std::shared_ptr<AstNode> Parser::ParseIndexedExpr(
    const std::shared_ptr<AstNode>& expr) {
    auto node = std::make_shared<IndexedExpr>();
    ConditionalBufAdvance(TokenType::R_BRACK);
    node->expr = expr ? expr : ParseExpr();
    node->idx = ParseExpr(TokenType::L_BRACK);

    // Since the first parent expression is passed, we only loop for
    // subsequent index accesses.
    if (Current()->token_type == TokenType::R_BRACK) {
        std::shared_ptr<AstNode> multiDimNode = ParseIndexedExpr(node);
        while (Current()->token_type == TokenType::R_BRACK) {
            multiDimNode = ParseIndexedExpr(node);
        }
        return multiDimNode;
    }

    return node;
}

/*****************************************************************************/
std::shared_ptr<Name> Parser::ParseName() {
    auto name = std::make_shared<Name>();
    name->value = Current()->lexeme;
    AdvanceBuffer(1);

    return name;
}

/*****************************************************************************/
std::shared_ptr<TryBlock> Parser::ParseTryBlock() {
    auto node = std::make_shared<TryBlock>();
    AdvanceBuffer(1);
    node->scope = ParseScope();
    node->catch_block = ParseCatchBlock();
    node->exception_ids = node->catch_block->GetExceptionIds();

    return node;
}

/*****************************************************************************/
std::shared_ptr<CatchBlock> Parser::ParseCatchBlock() {
    auto node = std::make_shared<CatchBlock>();

    // current token should now be CATCH: catch (Exception1, Exception2, ...) {}
    AdvanceBuffer(1);

    // Check if any specific exceptions are caught, otherwise all exceptions are caught
    if (Current()->token_type == TokenType::R_PARAN) {
        AdvanceBuffer(1);
        while (Current()->token_type == TokenType::NAME) {
            node->AddExceptionId(ParseName());
            ConditionalBufAdvance(TokenType::COMMA);
        }
        AdvanceBuffer(1);
    }

    node->SetScope(ParseScope());

    return node;
}

/*****************************************************************************/
void Parser::RevertScope() {
    if (_current_scope->parent) {
        _current_scope = _current_scope->parent;
    }
}

/*****************************************************************************/
void Parser::ConvertScope(const std::shared_ptr<ScopeNode>& scope) {
    scope->parent = _current_scope;
    _current_scope = scope;
}

/*****************************************************************************/
std::string Parser::DumpsAst() const {
    std::string output;

    for (auto& [key, module] : ast->modules) {
        output += module->ToString(true);
    }
    output += ast->root->ToString(true);

    return output;
}

/*****************************************************************************/
void Parser::Parse() {
    std::vector<Token*> qualifiers;

    if (GetTokenCount()) {
        while (!EndOfSequence()) {
            switch (Current()->token_type) {
                case TokenType::BLOCK_COMMENT:
                case TokenType::INLINE_COMMENT:
                    AdvanceBuffer(1);
                    break;
                case TokenType::IMPORT:
                    _current_scope->AddSubTree(ParseImportStmt(), true);
                    break;
                case TokenType::MODULE: {
                    auto node = ParseModule();
                    ast->modules[node->name->value] = node;
                    break;
                }
                case TokenType::R_BRACE:
                    _current_scope->AddSubTree(ParseScope());
                    break;
                case TokenType::L_BRACE:
                case TokenType::ELIF:
                case TokenType::ELSE:
                    return;
                case TokenType::PUBLIC:
                case TokenType::PROTECTED:
                case TokenType::PRIVATE:
                case TokenType::STATIC:
                case TokenType::LITERAL:
                case TokenType::REFERENCE:
                    qualifiers = GetQualifiers();
                    break;
                case TokenType::CONST:
                case TokenType::GLOBAL:
                case TokenType::LOCAL:
                case TokenType::VAR:
                    _current_scope->AddVarDecl(ParseVarDecl(qualifiers));
                    ConditionalBufAdvance(TokenType::SEMICOLON);
                    qualifiers.clear();
                    break;
                case TokenType::TYPE:
                case TokenType::ALIAS:
                    _current_scope->AddSubTree(ParseAliasDecl());
                    break;
                case TokenType::FUNC:
                    _current_scope->AddFuncDecl(ParseFuncDecl(qualifiers));
                    qualifiers.clear();
                    break;
                case TokenType::RETURN:
                    _current_scope->AddSubTree(ParseReturnStmt());
                    break;
                case TokenType::DELETE:
                    _current_scope->AddSubTree(ParseDeleteStmt());
                    break;
                case TokenType::EXIT:
                    _current_scope->AddSubTree(ParseExitStmt());
                    break;
                case TokenType::BREAK:
                    _current_scope->AddSubTree(ParseBreakStmt());
                    break;
                case TokenType::CONTINUE:
                    _current_scope->AddSubTree(ParseContinueStmt());
                    break;
                case TokenType::CLASS: {
                    _previous_state = _current_state;
                    _current_state = CLASS_DECL_CONTEXT;
                    _current_scope->AddClassDecl(ParseClassDecl());
                    _current_state = _previous_state;
                    _previous_state = GENERAL_CONTEXT;
                    break;
                }
                case TokenType::NAME: {
                    if (Current()->lexeme == "__pragma") {
                        AdvanceBuffer(2);
                        auto key = Current()->lexeme;
                        Expect(TokenType::COMMA);
                        AdvanceBuffer(2);
                        auto value = Current()->lexeme;
                        Expect(TokenType::L_PARAN);
                        AdvanceBuffer(2);
                        _current_scope->pragma_table[key] = value;
                        //                        _pragma_table[key] = value;
                        ConditionalBufAdvance(TokenType::SEMICOLON);
                        Log::DEBUG("Pragma: " + key + ", " + value);
                    } else {
                        auto expr = ParseExpr(TokenType::EQUAL);

                        if (Current()->token_type == TokenType::EQUAL ||
                            Lookback()->token_type == TokenType::EQUAL ||
                            Current()->IsCompoundOp()) {
                            _current_scope->AddSubTree(ParseAssignmentStatement(expr));
                        } else if (Current()->IsUnaryOp()) {
                            _current_scope->AddSubTree(ParseUnaryExpr(expr));
                        } else if (Current()->token_type == TokenType::R_PARAN) {
                            _current_scope->AddSubTree(ParseFuncCall(expr));
                        } else if (Current()->token_type == TokenType::R_BRACK) {
                            _current_scope->AddSubTree(ParseIndexedExpr(expr));
                        } else if (Current()->IsUnaryOp()) {
                            if (Current()->IsOneOf(
                                    {TokenType::DBL_MINUS, TokenType::DBL_PLUS})) {
                                _current_scope->AddSubTree(ParseUnaryExpr(expr));
                            }
                        } else {
                            _current_scope->AddSubTree(expr);
                        }
                    }
                    break;
                }
                case TokenType::IF:
                    _current_scope->AddSubTree(ParseIfStmt());
                    break;
                case TokenType::WHILE:
                    _current_scope->AddSubTree(ParseWhileLoop());
                    break;
                case TokenType::FOR:
                    _current_scope->AddSubTree(ParseForLoop());
                    break;
                case TokenType::TRY:
                    _current_scope->AddSubTree(ParseTryBlock());
                    break;
                case TokenType::DBL_MINUS:
                case TokenType::DBL_PLUS:
                    _current_scope->AddSubTree(ParseUnaryExpr());
                    break;
                case TokenType::SEMICOLON:
                    AdvanceBuffer(1);
                    break;
                default:
                    break;
            }
        }
    }
}

/*****************************************************************************/
std::vector<Token*> Parser::GetQualifiers() {
    std::vector<Token*> qualifiers;

    while (Current()->IsQualifier()) {
        qualifiers.push_back(Current());
        AdvanceBuffer(1);
    }

    return qualifiers;
}

/*****************************************************************************/
void Parser::LoadTokens(std::vector<Token*> t) {
    LoadData(std::move(t));
    AdvanceBuffer(2);
}

/*****************************************************************************/
std::shared_ptr<AstNode> Parser::TransformBinaryExpr(
    std::shared_ptr<BinaryExpr> binary_expr) {
    if (binary_expr->_op == "->" || binary_expr->_op == "::") {
        if (binary_expr->_right->node_type == AST_INDEXED_EXPR) {
            auto right_tmp =
                std::dynamic_pointer_cast<IndexedExpr>(binary_expr->_right);
            binary_expr->_right = right_tmp->expr;
            right_tmp->expr = binary_expr;

            return right_tmp;
        } else if (binary_expr->_right->node_type == AST_FUNC_CALL) {
            auto right_tmp = std::dynamic_pointer_cast<FuncCall>(binary_expr->_right);
            binary_expr->_right = right_tmp->expr;
            right_tmp->expr = TransformBinaryExpr(binary_expr);

            return right_tmp;
        }
    }

    return binary_expr;
}

/*****************************************************************************/
std::shared_ptr<Module> Parser::ParseModule() {
    Expect(TokenType::NAME);
    AdvanceBuffer(1);
    auto node = std::make_shared<Module>();
    node->name = ParseName();
    Expect(TokenType::IS);
    CheckExpected();
    AdvanceBuffer(1);
    node->scope = ParseScope();

    return node;
}

/*****************************************************************************/
TokenType Parser::GetCurrentAsExpectedType() {
    return Current()->token_type;
}

/*****************************************************************************/
size_t Parser::GetTokenCount() {
    return GetDataSize();
}

/*****************************************************************************/
std::string Parser::ItemToString(Token* token) {
    return token->ToString();
}

/*****************************************************************************/
void Parser::HandleUnexpectedItem() {
    auto msg = "Unexpected token '" + Current()->lexeme + "'";
    msg += +" in file " + Current()->file_info.ToString();
    msg += "\n\n" + Current()->file_info.GetContextualBlock();
    throw std::runtime_error(msg);
}

/*****************************************************************************/
std::shared_ptr<RnTypeComposite> Parser::ParseType() {
    auto basic_type = RnType::StringToType(Current()->lexeme);
    auto type = std::make_shared<RnTypeComposite>(basic_type);

    if (_current_scope->pragma_table["require"] == "bounds") {
        if (Peek()->token_type != TokenType::R_CARAT) {
            throw std::runtime_error("Missing bounds on type " + Current()->lexeme);
        }
    }
    AdvanceBuffer(1);

    if (Current()->token_type == TokenType::R_CARAT) {
        AdvanceBuffer(1);
        char* p;
        size_t lower_bnd_idx = Current()->lexeme.find("..");
        size_t upper_bnd_idx = Current()->lexeme.find("..") + 2;
        auto lower =
            std::strtof(Current()->lexeme.substr(0, lower_bnd_idx).c_str(), &p);
        auto upper = std::strtof(
            Current()->lexeme.substr(lower_bnd_idx + 2, upper_bnd_idx).c_str(), &p);
        type->SetBounds(lower, upper);
        AdvanceBuffer(2);
    }
    return type;
}
