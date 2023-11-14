/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
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

#include "Parser.h"
#include <memory>
#include <utility>
#include "../common/RnConfig.h"
#include "../common/RnInternment.h"
#include "../lexer/Lexer.h"
#include "../util/LoopCounter.h"
#include "../util/RnStack.h"
#include "../util/String.h"
#include "../util/log.h"
#include "ast/AliasDecl.h"
#include "ast/ArgDecl.h"
#include "ast/ArrayLiteral.h"
#include "ast/AssignmentStmt.h"
#include "ast/Ast.h"
#include "ast/AstNode.h"
#include "ast/BinaryExpr.h"
#include "ast/CatchBlock.h"
#include "ast/ClassDecl.h"
#include "ast/ConditionalStmt.h"
#include "ast/DeleteStmt.h"
#include "ast/ExitStmt.h"
#include "ast/Expr.h"
#include "ast/FlowControl.h"
#include "ast/FuncCall.h"
#include "ast/FuncDecl.h"
#include "ast/ImportStmt.h"
#include "ast/IndexedExpr.h"
#include "ast/LiteralValue.h"
#include "ast/Loop.h"
#include "ast/Module.h"
#include "ast/Name.h"
#include "ast/NodeType.h"
#include "ast/NullLiteral.h"
#include "ast/ReturnStmt.h"
#include "ast/ScopeNode.h"
#include "ast/TryBlock.h"
#include "ast/UnaryExpr.h"
#include "ast/VarDecl.h"

std::vector<std::string> Parser::parsed_files;

std::unordered_map<TokenType, int> Parser::_prec_tbl = {
    {TokenType::DBL_COLON, 200},  {TokenType::R_ARROW, 200},
    {TokenType::R_PARAN, 100},    {TokenType::STAR, 90},
    {TokenType::SLASH, 90},       {TokenType::PERCENT, 90},
    {TokenType::PLUS, 80},        {TokenType::MINUS, 80},
    {TokenType::DBL_R_CARAT, 70}, {TokenType::DBL_L_CARAT, 70},
    {TokenType::L_CARAT, 60},     {TokenType::R_CARAT, 60},
    {TokenType::LEQ, 60},         {TokenType::GEQ, 60},
    {TokenType::DBL_EQUAL, 50},   {TokenType::NOT_EQUAL, 50},
    {TokenType::AMPER, 40},       {TokenType::DBL_AMPER, 40},
    {TokenType::UP_ARROW, 30},    {TokenType::BAR, 20},
    {TokenType::DBL_BAR, 20},     {TokenType::L_PARAN, 0},
};

std::unordered_map<TokenType, Associativity> Parser::_associativity = {
    {TokenType::R_PARAN, NO_ASSOCIATIVITY},
    {TokenType::R_ARROW, LEFT},
    {TokenType::DOUBLE_COLON, LEFT},
    {TokenType::STAR, LEFT},
    {TokenType::SLASH, LEFT},
    {TokenType::PERCENT, LEFT},
    {TokenType::PLUS, LEFT},
    {TokenType::MINUS, LEFT},
    {TokenType::DBL_R_CARAT, LEFT},
    {TokenType::DBL_L_CARAT, LEFT},
    {TokenType::L_CARAT, LEFT},
    {TokenType::R_CARAT, LEFT},
    {TokenType::GEQ, LEFT},
    {TokenType::LEQ, LEFT},
    {TokenType::DBL_EQUAL, LEFT},
    {TokenType::NOT_EQUAL, LEFT},
    {TokenType::AMPER, LEFT},
    {TokenType::DBL_AMPER, LEFT},
    {TokenType::UP_ARROW, LEFT},
    {TokenType::BAR, LEFT},
    {TokenType::DBL_BAR, LEFT},
    {TokenType::L_PARAN, NO_ASSOCIATIVITY},
};

/*****************************************************************************/
Parser::Parser() {
    FillBuffer(nullptr);
    ast = std::make_shared<Ast>();
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
    _current_scope->symbol_table->AddSymbol(
        "lload", std::make_shared<RnTypeComposite>(RnType::RN_OBJECT));

    _current_scope->pragma_table["bounds"] = "not-enforced";
    _current_scope->pragma_table["typing"] = "not-enforced";
}

/*****************************************************************************/
Parser::~Parser() = default;

/*****************************************************************************/
void Parser::ConditionalBufAdvance(TokenType t) {
    if (Current()->token_type == t) {
        AdvanceBuffer(1);
    }
}

/*****************************************************************************/
AstNodePtr<ImportStmt> Parser::ParseImportStmt() {
    auto node = AstNode::CreateNode<ImportStmt>();
    AddCurrentFileInfo(node);
    Expect({TokenType::NAME, TokenType::STRING_LITERAL});
    AdvanceBuffer(1);
    node->source_file = Current()->lexeme;
    Expect(TokenType::SEMICOLON);
    AdvanceBuffer(1);

    // Parse the module and create a new subtree from it
    std::filesystem::path module_path;
    auto source_file = String::Replace(node->source_file, ".", "/") + ".rn";

    module_path = std::filesystem::path(node->file_info.GetFilePath()).parent_path() / source_file;
    if (!std::filesystem::exists(module_path)) {
        module_path = std::filesystem::path(RnConfig::GetLibraryPath()) / source_file;
        if (!std::filesystem::exists(module_path)) {
            ThrowError("Could not open file " + module_path.string());
        }
    }

    // Check if the file has already been parsed
    if (std::find(parsed_files.begin(), parsed_files.end(), module_path) !=
        parsed_files.end()) {
        return node;
    } else {
        parsed_files.push_back(module_path);
    }

    if (std::filesystem::absolute(module_path) == std::filesystem::absolute(file)) {
        ThrowError("Circular dependency error: " + module_path.string());
    }

    Lexer lexer;
    lexer.LoadFile(module_path);
    lexer.ProcessTokens();
    Parser parser;
    parser.parsed_files = parsed_files;
    parser.working_dir = module_path.parent_path();
    parser.SetFromPtr(lexer.tokens.data(), lexer.tokens.size());
    parser.AdvanceBuffer(2);
    parser.Parse();
    node->ast = parser.ast;
    AdvanceBuffer(1);

    return node;
}

/*****************************************************************************/
AstNodePtr<VarDecl> Parser::ParseVarDecl(std::vector<Token*> qualifiers) {
    auto node = AstNode::CreateNode<VarDecl>();
    AddCurrentFileInfo(node);
    Expect(TokenType::NAME);

    switch (Current()->token_type) {
        case TokenType::CONST:
            node->is_const = true;
            break;
        case TokenType::GLOBAL:
            node->is_global = true;
            break;
        case TokenType::LOCAL:
            node->is_local = true;
            break;
        case TokenType::VAR:
            break;
        case TokenType::LITERAL:
            node->is_literal = true;
            break;
        default:
            assert(false);
    }

    if (!qualifiers.empty()) {
        node->qualifiers = std::move(qualifiers);
    }

    AdvanceBuffer(1);
    AddCurrentFileInfo(node);
    node->id = Current()->lexeme;
    Expect(TokenType::COLON);
    AdvanceBuffer(1);
    AdvanceBuffer(1);
    node->type = ParseType();

    if (Current()->token_type == TokenType::EQUAL) {
        AdvanceBuffer(1);
        node->init_value = ParseExpr();
    } else {
        ConditionalBufAdvance(TokenType::SEMICOLON);
    }

    if (node->is_literal) {
        if (!node->init_value || !node->init_value->IsLiteral())
            ThrowError(
                "A single, literal value must be assigned to a literal declaration.");
        else
            _current_scope->AddLiteral(node->id, node->init_value);
    }

    return node;
}

/*****************************************************************************/
AstNodePtr<FuncDecl> Parser::ParseFuncDecl(std::vector<Token*> qualifiers) {
    auto node = AstNode::CreateNode<FuncDecl>();
    AddCurrentFileInfo(node);

    if (!qualifiers.empty()) {
        node->qualifiers = std::move(qualifiers);
    }

    ConditionalBufAdvance(TokenType::ROUTINE);

    if (Current()->token_type == TokenType::R_PARAN) {
        node->is_closure = true;
    } else {
        node->id = Current()->lexeme;
        AdvanceBuffer(1);
    }

    AdvanceBuffer(1);

    // Get the function arguments
    std::map<std::string, std::shared_ptr<RnTypeComposite>> arg_symbols;
    MAKE_LOOP_COUNTER(1000)
    while (Current()->token_type != TokenType::L_PARAN) {
        INCR_LOOP_COUNTER
        auto arg = new ArgDecl();

        if (!Current()->IsType()) {
            Expect(TokenType::NAME);
            AdvanceBuffer(1);
        }

        if (Current()->token_type == TokenType::NAME) {
            arg->AddChild(ParseName());

            if (arg_symbols.find(arg->GetChild<Name>(0)->value) != arg_symbols.end()) {
                ThrowError("Redeclaration of argument '" +
                                         arg->GetChild<Name>(0)->value + "' in routine '" +
                                         node->id + "'");
            }
            AdvanceBuffer(1);  // Advance past the ':' separating the name from the type

            if (Current()->IsType()) {
                arg->SetType(ParseType());
                AdvanceBuffer(1);
            } else {
                ThrowError("Invalid type '" + Current()->lexeme +
                           "' for parameter '" + arg->GetChild<Name>(0)->value +
                           "' while declaring routine '" + node->id + "'");
            }

        } else if (Current()->IsType()) {
            auto arg_name = AstNode::CreateNode<Name>();
            arg_name->value = "$" + std::to_string(arg_symbols.size() + 1);
            arg->AddChild(arg_name);
            arg->SetType(ParseType());
        }

        ConditionalBufAdvance(TokenType::COMMA);
        node->args.emplace_back(arg);
        arg_symbols[arg->GetChild<Name>(0)->value] = arg->GetType();

        if (Current()->token_type != TokenType::VAR && !Current()->IsType()) {
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
            ThrowError("Class constructor must return object type");
        }

        AdvanceBuffer(1);
    } else {
        node->type = std::make_shared<RnTypeComposite>(RnType::RN_VOID);
    }

    // Get the function's scope
    auto previous_scope_count = _scope_count;
    node->scope = ParseScope();
    assert(_scope_count == previous_scope_count);
    assert(node->scope);

    for (const auto& symbol : arg_symbols) {
        node->scope->symbol_table->AddSymbol(symbol.first, symbol.second, node);
    }

    if (_current_state == CLASS_DECL_CONTEXT) {
        node->scope->symbol_table->AddSymbol(
            "this", std::make_shared<RnTypeComposite>(RnType::RN_OBJECT), node);
    }
    return node;
}

/*****************************************************************************/
AstNodePtr<ClassDecl> Parser::ParseClassDecl() {
    auto node = AstNode::CreateNode<ClassDecl>();
    AddCurrentFileInfo(node);
    Expect(TokenType::NAME);
    AdvanceBuffer(1);

    node->id = Current()->lexeme;
    _user_defined_type_map[node->id] =
        std::make_shared<RnTypeComposite>(RnType::RN_OBJECT);
    Expect({TokenType::EXTENDS, TokenType::BEGIN, TokenType::R_BRACE, TokenType::IS});
    AdvanceBuffer(1);

    // Check for inherited classes and parse if necessary
    if (Current()->token_type == TokenType::EXTENDS) {
        Expect(TokenType::NAME);
        AdvanceBuffer(1);

        MAKE_LOOP_COUNTER(DEFAULT_ITERATION_MAX)
        while (Current()->token_type == TokenType::NAME) {
            INCR_LOOP_COUNTER
            node->parent_classes.emplace_back(ParseName());
            ConditionalBufAdvance(TokenType::COMMA);
        }
    }
    AdvanceBuffer(1);

    _previous_state = _current_state;
    _current_state = ParserState::CLASS_DECL_CONTEXT;
    auto previous_scope_count = _scope_count;
    node->scope = ParseScope();
    assert(_scope_count == previous_scope_count);
    _current_state = _previous_state;

    return node;
}

/*****************************************************************************/
AstNodePtr<AstNode> Parser::GetExprComponent() {
    AstNodePtr<AstNode> node = nullptr;

    if ((Lookback() && Lookback()->IsOperator()) && Current()->IsUnaryOp()) {
        return ParseUnaryExpr();
    } else if (Current()->token_type == TokenType::ROUTINE) {
        node = ParseFuncDecl();
    } else if (Current()->token_type == TokenType::R_BRACK) {
        node = ParseArrayLiteral();
    } else if (Current()->token_type == TokenType::NAME) {
        if (_current_scope->GetLiteral(Current()->lexeme)) {
            node = _current_scope->GetLiteral(Current()->lexeme);
            AdvanceBuffer(1);
        } else {
            node = ParseName();
            if (Current()->IsOneOf({TokenType::DBL_MINUS, TokenType::DBL_PLUS})) {
                node = ParseUnaryExpr(node);
            }
        }
    } else if (Current()->IsLiteral()) {
        AdvanceBuffer(1);
        switch (Lookback()->token_type) {
            case TokenType::INT_LITERAL: {
                node = AstNode::CreateNode<LiteralValue>();
                node->node_type = AST_INT_LITERAL;
                AstNode::CastNode<LiteralValue>(node)->data =
                    static_cast<RnIntNative>(std::stol(Lookback()->lexeme));
                _intern_count++;
                break;
            }
            case TokenType::FLOAT_LITERAL: {
                node = AstNode::CreateNode<LiteralValue>();
                node->node_type = AST_FLOAT_LITERAL;
                AstNode::CastNode<LiteralValue>(node)->data =
                    static_cast<RnFloatNative>(std::stod(Lookback()->lexeme));
                _intern_count++;
                break;
            }
            case TokenType::STRING_LITERAL: {
                node = AstNode::CreateNode<LiteralValue>();
                node->node_type = AST_STRING_LITERAL;
                AstNode::CastNode<LiteralValue>(node)->data =
                    Lookback()->lexeme;
                _intern_count++;
                break;
            }
            case TokenType::BOOL_LITERAL: {
                node = AstNode::CreateNode<LiteralValue>();
                node->node_type = AST_BOOL_LITERAL;
                AstNode::CastNode<LiteralValue>(node)->data =
                    Lookback()->lexeme == "true";
                _intern_count++;
                break;
            }
            case TokenType::NULL_LITERAL: {
                node = AstNode::CreateNode<NullLiteral>();
                break;
            }
            default:
                break;
        }
    }

    if (!node) {
        ThrowError("Failed to parse expression component");
    }
    return AddCurrentFileInfo(node);
}

/*****************************************************************************/
AstNodePtr<AstNode> Parser::ParseExpr(TokenType stop_token) {
    RnStack<Token*> op_stack;
    RnStack<AstNodePtr<AstNode>> result_stack;
    result_stack.Push(nullptr);

    auto make_binary_expr = [this, &result_stack, &op_stack]() mutable {
        auto node = AstNode::CreateNode<BinaryExpr>();
        node->_right = result_stack.Pop();
        node->_left = result_stack.Pop();
        node->_op = op_stack.Pop()->lexeme;
        return std::static_pointer_cast<BinaryExpr>(TransformBinaryExpr(node));
    };

    auto parse_bracketed_node = [this, &result_stack]() mutable {
        if (Lookback()->token_type == TokenType::NAME ||
            Current()->token_type == TokenType::L_PARAN) {
            result_stack.push_back(ParseIndexedExpr(result_stack.Pop()));
        } else {
            result_stack.Push(ParseArrayLiteral());
        }
    };

    // Grab any immediate unary operators and apply them to the closest
    // expression. This may need some fine-tuning.
    if (Current()->IsUnaryOp()) {
        auto node = AstNode::CreateNode<UnaryExpr>();
        node->op = Current()->lexeme;
        AdvanceBuffer(1);
        if (Current()->token_type == TokenType::R_PARAN) {
            node->expr = ParseExpr(TokenType::L_PARAN);
        } else {
            node->expr = GetExprComponent();
        }
        result_stack.push_back(node);
    }

    bool break_next_iteration = false;
    MAKE_LOOP_COUNTER(DEFAULT_ITERATION_MAX)
    while (!result_stack.IsEmpty()) {
        INCR_LOOP_COUNTER
        if (Lookback() &&
            (Lookback()->IsOperator() ||
             unary_lookback_set.contains(Lookback()->token_type)) &&
            Current()->IsUnaryOp()) {
            if (Peek()->IsOneOf({TokenType::INT_LITERAL, TokenType::FLOAT_LITERAL})) {
                result_stack.push_back(GetExprComponent());
            } else {
                result_stack.Push(ParseUnaryExpr());
            }
        }

        if (!Current()) {
            ThrowError("Failed to parse expression");
        }

        if (Current()->token_type == TokenType::R_BRACK) {
            parse_bracketed_node();
        }

        if (Current()->IsOneOf(
                {TokenType::BEGIN, TokenType::R_BRACE, TokenType::SEMICOLON, TokenType::L_BRACK}) ||
            (Current()->token_type == TokenType::L_PARAN && op_stack.IsEmpty()) ||
            Current()->token_type == stop_token || Current()->IsCompoundOp()) {
            // We should only get here at the end of an expression and at
            // the end of an expression we should always be here.
            ConditionalBufAdvance(stop_token);

            // Return the subtree
            if (result_stack.Size() == 1 ||
                (result_stack.Size() == 2 && op_stack.IsEmpty())) {
                return AddCurrentFileInfo(result_stack.Pop());
            } else {
                if (!op_stack.IsEmpty() &&
                    op_stack.back()->IsOneOf(
                        {TokenType::L_PARAN, TokenType::R_PARAN})) {
                    op_stack.Pop();
                }

                while (result_stack.Size() > 2 && !op_stack.IsEmpty()) {
                    if (op_stack.back()->token_type == TokenType::R_PARAN) {
                        op_stack.Pop();
                    }

                    result_stack.Push(make_binary_expr());
                    if (Current()->token_type == TokenType::R_BRACK) {
                        parse_bracketed_node();
                    } else if (Current()->token_type == TokenType::R_PARAN) {
                        result_stack.Push(ParseFuncCall(result_stack.Pop()));
                    }
                }
                return AddCurrentFileInfo(result_stack.Pop());
            }
        } else {
            // Shunting-yard _prec_tbl parsing
            if (Current()->IsOperator() ||
                Current()->IsOneOf({TokenType::L_PARAN, TokenType::R_PARAN})) {
                if (Current()->token_type == TokenType::R_PARAN) {
                    if (!Lookback()->IsOperator() && result_stack.Top() &&
                        Lookback()->token_type != TokenType::R_PARAN) {
                        result_stack.Push(ParseFuncCall(result_stack.Pop()));
                    } else {
                        op_stack.push_back(Current());
                        AdvanceBuffer(1);
                    }
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
                        result_stack.Push(TransformBinaryExpr(make_binary_expr()));

                        // Handle _left associativity
                        if (!op_stack.IsEmpty() and
                            _associativity[op_stack.back()->token_type] == LEFT) {
                            result_stack.Push(make_binary_expr());
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
        if (break_next_iteration || Lookback() == Current()) {
            break;
        } else {
            break_next_iteration = EndOfSequence();
        }
    }

    if (result_stack.Size() == 3) {
        return AddCurrentFileInfo(make_binary_expr());
    } else {
        if (!result_stack.IsEmpty()) {
            return AddCurrentFileInfo(result_stack.back());
        }
    }
    return AstNode::CreateNode<Expr>();
}

/*****************************************************************************/
AstNodePtr<UnaryExpr> Parser::ParseUnaryExpr(
    const AstNodePtr<AstNode>& expr) {
    auto node = AstNode::CreateNode<UnaryExpr>();
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
AstNodePtr<FlowControl> Parser::ParseFlowControlStmt() {
    auto node = AstNode::CreateNode<FlowControl>();
    AddCurrentFileInfo(node);
    Expect(TokenType::SEMICOLON);
    if (Current()->token_type == TokenType::CONTINUE) {
        node->node_type = AST_CONTINUE_STMT;
    } else {
        node->node_type = AST_BREAK_STMT;
    }
    AdvanceBuffer(2);

    return node;
}

/*****************************************************************************/
AstNodePtr<ReturnStmt> Parser::ParseReturnStmt() {
    auto node = AstNode::CreateNode<ReturnStmt>();
    AddCurrentFileInfo(node);
    AdvanceBuffer(1);

    if (Current()->token_type == TokenType::SEMICOLON) {
        AdvanceBuffer(1);
    } else {
        node->expr = ParseExpr();
    }

    return node;
}

/*****************************************************************************/
AstNodePtr<DeleteStmt> Parser::ParseDeleteStmt() {
    auto node = AstNode::CreateNode<DeleteStmt>();
    AddCurrentFileInfo(node);
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
AstNodePtr<ExitStmt> Parser::ParseExitStmt() {
    auto node = AstNode::CreateNode<ExitStmt>();
    AddCurrentFileInfo(node);
    Expect({TokenType::SEMICOLON, TokenType::INT_LITERAL});
    AdvanceBuffer(1);

    if (Current()->token_type == TokenType::SEMICOLON) {
        node->exit_code = AstNode::CreateNode<LiteralValue>();
        node->exit_code->node_type = AST_INT_LITERAL;
        node->exit_code->data = 0L;
        _intern_count++;
        AdvanceBuffer(1);
    } else {
        node->exit_code = AstNode::CreateNode<LiteralValue>();
        node->exit_code->node_type = AST_INT_LITERAL;
        node->exit_code->data = static_cast<RnIntNative>(std::stoi(Current()->lexeme));
        _intern_count++;
        AdvanceBuffer(2);
    }

    return node;
}

/*****************************************************************************/
AstNodePtr<AstNode> Parser::ParseAssignmentStatement(
    const AstNodePtr<AstNode>& rexpr) {
    auto node = AstNode::CreateNode<AssignmentStmt>();
    AddCurrentFileInfo(node);
    node->SetLexpr(rexpr ? rexpr : ParseExpr());

    std::string op;
    if (Current()->IsCompoundOp()) {
        op = Current()->lexeme[0];
        AdvanceBuffer(1);
    } else if (Lookback()->IsCompoundOp()) {
        op = Lookback()->lexeme[0];
    }

    if (!op.empty()) {
        auto bin_expr = AstNode::CreateNode<BinaryExpr>();
        bin_expr->_left = node->GetLexpr();
        bin_expr->_op = op;
        bin_expr->_right = ParseExpr();
        auto final_rexpr = TransformBinaryExpr(bin_expr);
        if (bin_expr != final_rexpr && final_rexpr->node_type == AST_UNARY_EXPR) {
            return final_rexpr;
        } else {
            node->SetRexpr(final_rexpr);
        }
    } else {
        ConditionalBufAdvance(TokenType::EQUAL);
        node->SetRexpr(ParseExpr());
    }

    return node;
}

/*****************************************************************************/
AstNodePtr<ConditionalStmt> Parser::ParseConditionalStmt() {
    auto node = AstNode::CreateNode<ConditionalStmt>();
    AddCurrentFileInfo(node);

    if (Current()->IsOneOf({TokenType::IF, TokenType::ELIF})) {
        if (Current()->token_type == TokenType::IF) {
            node->node_type = AST_IF_STMT;
        } else {
            node->node_type = AST_ELIF_STMT;
        }
        AdvanceBuffer(1);
        node->test = ParseExpr(TokenType::COLON);
        node->consequent = ParseScope();

        // Keep parsing conditionals until no more are found
        AstNodePtr<ConditionalStmt> parent_node = node;
        while (Current() && Current()->IsOneOf({TokenType::ELIF, TokenType::ELSE})) {
            parent_node->alternative = AstNode::CreateNode<ConditionalStmt>();
            AddCurrentFileInfo(parent_node->alternative);

            if (Current()->token_type == TokenType::ELIF) {
                parent_node->alternative->node_type = AST_ELIF_STMT;
                AdvanceBuffer(1);
                parent_node->alternative->test = ParseExpr(TokenType::COLON);
                parent_node->alternative->consequent = ParseScope();
            } else {
                parent_node->alternative->node_type = AST_ELSE_STMT;
                Expect(TokenType::COLON);
                AdvanceBuffer(2);
                parent_node->alternative->consequent = ParseScope();
            }

            parent_node = parent_node->alternative;
        }
    } else {
        ThrowError("Invalid token '" + Current()->lexeme +
                                 "' for conditional statement.");
    }

    return node;
}

/*****************************************************************************/
AstNodePtr<ScopeNode> Parser::ParseScope() {
    auto node = AstNode::CreateNode<ScopeNode>();
    AddCurrentFileInfo(node);
    node->parent = _current_scope;
    node->symbol_table->SetParent(_current_scope->symbol_table);

    // Pragmas are carried downward until they are unset or out of scope
    for (const auto& entry : _current_scope->pragma_table) {
        node->pragma_table[entry.first] = entry.second;
    }

    ConditionalBufAdvance(TokenType::BEGIN);
    ConditionalBufAdvance(TokenType::R_BRACE);
    ConditionalBufAdvance(TokenType::COLON);

    auto previous_scope_count = _scope_count;
    ConvertScope(node);
    Parse();
    RevertScope();
    assert(_scope_count == previous_scope_count);

    if (!Current() && _data_idx > _data_size) {
        Log::WARN("Out of bounds parser index");
        return node;
    }

    if (Current()->IsOneOf({TokenType::END, TokenType::L_BRACE})) {
        AdvanceBuffer(1);
    }

    return node;
}

/*****************************************************************************/
AstNodePtr<FuncCall> Parser::ParseFuncCall(const AstNodePtr<AstNode>& expr) {
    auto node = AstNode::CreateNode<FuncCall>();
    AddCurrentFileInfo(node);

    if (expr == nullptr) {
        node->expr = ParseExpr();
    } else {
        node->expr = expr;
    }
    ConditionalBufAdvance(TokenType::R_PARAN);

    MAKE_LOOP_COUNTER(DEFAULT_ITERATION_MAX)
    while (!Current()->IsOneOf({TokenType::L_PARAN, TokenType::SEMICOLON})) {
        INCR_LOOP_COUNTER
        node->args.emplace_back(ParseExpr(TokenType::COMMA));
    }
    AdvanceBuffer(1);

    return node;
}

/*****************************************************************************/
AstNodePtr<ArrayLiteral> Parser::ParseArrayLiteral() {
    auto node = AstNode::CreateNode<ArrayLiteral>();
    AddCurrentFileInfo(node);
    ConditionalBufAdvance(TokenType::R_BRACK);

    while (!Current()->IsOneOf({TokenType::L_BRACK, TokenType::SEMICOLON})) {
        node->AddChild(ParseExpr(TokenType::COMMA));
    }
    AdvanceBuffer(1);

    return node;
}

/*****************************************************************************/
AstNodePtr<Loop> Parser::ParseWhileLoop() {
    auto node = AstNode::CreateNode<Loop>();
    node->node_type = AST_WHILE_LOOP;
    AddCurrentFileInfo(node);
    ConditionalBufAdvance(TokenType::WHILE);
    node->test = ParseExpr(TokenType::COLON);
    auto previous_scope_count = _scope_count;
    node->scope = ParseScope();
    assert(_scope_count == previous_scope_count);

    return node;
}

/*****************************************************************************/
AstNodePtr<Loop> Parser::ParseForLoop() {
    auto node = AstNode::CreateNode<Loop>();
    node->node_type = AST_FOR_LOOP;
    AddCurrentFileInfo(node);
    Expect(TokenType::R_PARAN);
    AdvanceBuffer(1);

    if (Peek()->token_type == TokenType::VAR ||
        Peek()->token_type == TokenType::LOCAL) {
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
    ConditionalBufAdvance(TokenType::BEGIN);
    ConditionalBufAdvance(TokenType::R_BRACE);
    auto previous_scope_count = _scope_count;
    node->scope = ParseScope();
    assert(_scope_count == previous_scope_count);

    if (node->init->node_type == AST_VAR_DECL) {
        auto var_decl = AstNode::CastNode<VarDecl>(node->init);
        node->scope->symbol_table->AddSymbol(var_decl->id, var_decl->type, var_decl);
    }

    return node;
}

/*****************************************************************************/
AstNodePtr<AliasDecl> Parser::ParseAliasDecl() {
    auto node = AstNode::CreateNode<AliasDecl>();
    AddCurrentFileInfo(node);

    Expect(TokenType::NAME);
    AdvanceBuffer(1);
    node->AddChild(ParseName());

    node->alias_type = Peek()->token_type == TokenType::TYPE ? TYPE_ALIAS : NAME_ALIAS;

    if (node->alias_type == NAME_ALIAS) {
        Expect(TokenType::NAME);
        AdvanceBuffer(1);
        node->AddChild(ParseName());
        AdvanceBuffer(1);
    } else {
        Expect({TokenType::OBJECT, TokenType::STRING, TokenType::INT, TokenType::FLOAT,
                TokenType::ARRAY, TokenType::CALLABLE});
        AdvanceBuffer(1);
        node->base_type = ParseType();
        AdvanceBuffer(1);
        _user_defined_type_map[node->GetChild<Name>(1)->value] = node->base_type;
    }
    return node;
}

/*****************************************************************************/
AstNodePtr<AstNode> Parser::ParseIndexedExpr(
    const AstNodePtr<AstNode>& expr) {
    auto node = AstNode::CreateNode<IndexedExpr>();
    AddCurrentFileInfo(node);
    ConditionalBufAdvance(TokenType::R_BRACK);
    node->expr = expr ? expr : ParseExpr();
    node->idx = ParseExpr(TokenType::L_BRACK);

    // Since the first parent expression is passed, we only loop for
    // subsequent index accesses.
    if (Current()->token_type == TokenType::R_BRACK) {
        AstNodePtr<AstNode> multiDimNode = ParseIndexedExpr(node);
        while (Current()->token_type == TokenType::R_BRACK) {
            multiDimNode = ParseIndexedExpr(node);
        }
        return multiDimNode;
    }

    return node;
}

/*****************************************************************************/
AstNodePtr<Name> Parser::ParseName() {
    auto node = AstNode::CreateNode<Name>();
    AddCurrentFileInfo(node);
    node->value = Current()->lexeme;
    AdvanceBuffer(1);
    _intern_count++;

    return node;
}

/*****************************************************************************/
AstNodePtr<TryBlock> Parser::ParseTryBlock() {
    auto node = AstNode::CreateNode<TryBlock>();
    AddCurrentFileInfo(node);
    AdvanceBuffer(1);
    auto previous_scope_count = _scope_count;
    node->scope = ParseScope();
    assert(_scope_count == previous_scope_count);
    node->catch_block = ParseCatchBlock();
    node->exception_ids = node->catch_block->GetExceptionIds();

    return node;
}

/*****************************************************************************/
AstNodePtr<CatchBlock> Parser::ParseCatchBlock() {
    auto node = AstNode::CreateNode<CatchBlock>();
    AddCurrentFileInfo(node);

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

    auto previous_scope_count = _scope_count;
    node->SetScope(ParseScope());
    assert(_scope_count == previous_scope_count);

    return node;
}

/*****************************************************************************/
void Parser::RevertScope() {
    assert(_current_scope);
    if (_current_scope->parent) {
        _current_scope = _current_scope->parent;
        _scope_count--;
    }
}

/*****************************************************************************/
void Parser::ConvertScope(const AstNodePtr<ScopeNode>& scope) {
    assert(scope);

    scope->parent = _current_scope;
    _current_scope = scope;
    _scope_count++;
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
        MAKE_LOOP_COUNTER(DEFAULT_ITERATION_MAX)
        while (true) {
            if (EndOfSequence()) {
                return;
            }
            INCR_LOOP_COUNTER
            switch (Current()->token_type) {
                case TokenType::BLOCK_COMMENT:
                case TokenType::INLINE_COMMENT:
                    AdvanceBuffer(1);
                    break;
                case TokenType::IMPORT: {
                    auto node = ParseImportStmt();
                    if (node)
                        // Any import parsing should have thrown so a simple existence
                        // check is fine here
                        _current_scope->AddSubTree(node, true);
                    break;
                }
                case TokenType::MODULE: {
                    auto node = ParseModule();
                    ast->modules[node->name->value] = node;
                    break;
                }
                case TokenType::R_BRACE:
                case TokenType::BEGIN: {
                    auto previous_scope_count = _scope_count;
                    _current_scope->AddSubTree(ParseScope());
                    assert(_scope_count == previous_scope_count);
                    break;
                }
                case TokenType::END:
                case TokenType::L_BRACE:
                case TokenType::ELIF:
                case TokenType::ELSE:
                    return;
                case TokenType::CONST:
                case TokenType::LITERAL:
                case TokenType::GLOBAL:
                case TokenType::LOCAL:
                case TokenType::VAR:
                    _current_scope->AddVarDecl(ParseVarDecl(qualifiers));
                    if (Current()) {
                        ConditionalBufAdvance(TokenType::SEMICOLON);
                    }
                    qualifiers.clear();
                    break;
                case TokenType::TYPE:
                case TokenType::ALIAS:
                    _current_scope->AddSubTree(ParseAliasDecl());
                    break;
                case TokenType::ROUTINE:
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
                case TokenType::CONTINUE:
                    _current_scope->AddSubTree(ParseFlowControlStmt());
                    break;
                case TokenType::CLASS: {
                    _previous_state = _current_state;
                    _current_state = CLASS_DECL_CONTEXT;
                    _current_scope->AddClassDecl(ParseClassDecl());
                    _current_state = _previous_state;
                    _previous_state = GENERAL_CONTEXT;
                    break;
                }
                case TokenType::R_PARAN:
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
                        if (_current_scope->GetLiteral(Current()->lexeme)) {
                            ThrowError("Misuse of literal replacement '" +
                                                     Current()->lexeme + "'");
                        }
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
                            if (expr->node_type == AST_FUNC_CALL)
                                AstNode::CastNode<FuncCall>(expr)
                                    ->SetDiscardReturnValue(true);
                            _current_scope->AddSubTree(expr);
                        }
                    }
                    break;
                }
                case TokenType::IF:
                    _current_scope->AddSubTree(ParseConditionalStmt());
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
                    HandleUnexpectedItem();
                    break;
            }
            if (EndOfSequence()) {
                break;
            }
        }
    }
    RnConstStore::Init(_intern_count);
}

/*****************************************************************************/
AstNodePtr<AstNode> Parser::TransformBinaryExpr(
    AstNodePtr<BinaryExpr> binary_expr) {
    assert(binary_expr);

    if (binary_expr->_op == "->" || binary_expr->_op == "::") {
        if (binary_expr->_right->node_type == AST_INDEXED_EXPR) {
            auto right_tmp =
                AstNode::CastNode<IndexedExpr>(binary_expr->_right);
            right_tmp->file_info = binary_expr->file_info;
            binary_expr->_right = right_tmp->expr;
            right_tmp->expr = binary_expr;

            return right_tmp;
        } else if (binary_expr->_right->node_type == AST_FUNC_CALL) {
            auto right_tmp = AstNode::CastNode<FuncCall>(binary_expr->_right);
            right_tmp->file_info = binary_expr->file_info;
            binary_expr->_right = right_tmp->expr;
            right_tmp->expr = TransformBinaryExpr(binary_expr);

            return right_tmp;
        }
    }

    return binary_expr;
}

/*****************************************************************************/
AstNodePtr<Module> Parser::ParseModule() {
    Expect(TokenType::NAME);
    AdvanceBuffer(1);
    auto node = AstNode::CreateNode<Module>();
    AddCurrentFileInfo(node);
    node->name = ParseName();
    Expect(TokenType::IS);
    CheckExpected();
    AdvanceBuffer(1);
    auto previous_scope_count = _scope_count;
    node->scope = ParseScope();
    assert(_scope_count == previous_scope_count);

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
    ThrowError("Unexpected token '" + Current()->lexeme + "'");
}

/*****************************************************************************/
void Parser::ThrowError(const std::string& message) {
    auto full_msg = message + " in file " + Current()->file_info.ToString();
    full_msg += "\n\n" + Current()->file_info.GetContextualBlock();
    throw std::runtime_error(full_msg);
}

/*****************************************************************************/
std::shared_ptr<RnTypeComposite> Parser::ParseType() {
    auto basic_type = RnType::StringToType(Current()->lexeme);
    if (basic_type == RnType::RN_UNKNOWN) {
        if (_user_defined_type_map.find(Current()->lexeme) !=
            _user_defined_type_map.end()) {
            auto type_composite = _user_defined_type_map[Current()->lexeme];
            AdvanceBuffer(1);
            return type_composite;
        }
    }

    auto type = std::make_shared<RnTypeComposite>(basic_type);
    if (_current_scope->pragma_table["require"] == "bounds") {
        if (Peek()->token_type != TokenType::R_CARAT) {
            ThrowError("Missing bounds on type " + Current()->lexeme);
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

/*****************************************************************************/
void Parser::Reset() {
    FillBuffer(nullptr);
    ast = std::make_shared<Ast>();
    _current_scope = ast->root;
}

/*****************************************************************************/
void Parser::Run() {
    AdvanceBuffer(2);
    Parse();
}

/*****************************************************************************/
AstNodePtr<AstNode> Parser::AddCurrentFileInfo(AstNodePtr<AstNode> node) {
    if (Current()) {
        node->file_info = Current()->file_info;
    }
    return node;
}
