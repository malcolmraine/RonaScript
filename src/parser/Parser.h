/*****************************************************************************
* File: Parser.h
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
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

#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../common/RnCompilerPhase.h"
#include "../common/RnType.h"
#include "../lexer/Token.h"
#include "../util/RnSequencer.h"
#include "ast/Ast.h"

class Token;
class ScopeNode;
class ImportStmt;
class ArgDecl;
class AstNode;
class AliasDecl;
class Expr;
class VarDecl;
class UnaryExpr;
class ReturnStmt;
class FuncCall;
class IndexedExpr;
class ClassDecl;
class Loop;
class FlowControl;
class ConditionalStmt;
class FuncDecl;
class BinaryExpr;
class AssignmentStmt;
class ArrayLiteral;
class Name;
class TryBlock;
class CatchBlock;
class DeleteStmt;
class ExitStmt;
class AttributeAccess;
class RnTypeComposite;

enum Associativity {
    LEFT,
    RIGHT,
    NO_ASSOCIATIVITY,
};

enum ParserState {
    GENERAL_CONTEXT,
    CLASS_DECL_CONTEXT,
    FUNC_DECL_CONTEXT,
};

class Parser : public RnCompilerPhase<Token*, std::shared_ptr<Ast>>,
               public RnSequencer<Token*, TokenType> {
public:
    Parser();
    ~Parser() override;

    void ConditionalBufAdvance(TokenType t);
    std::shared_ptr<ImportStmt> ParseImportStmt();
    std::shared_ptr<VarDecl> ParseVarDecl(std::vector<Token*> qualifiers = {});
    std::shared_ptr<FuncDecl> ParseFuncDecl(std::vector<Token*> qualifiers = {});
    std::shared_ptr<ClassDecl> ParseClassDecl();
    std::shared_ptr<AstNode> GetExprComponent();
    std::shared_ptr<AstNode> ParseExpr(TokenType stop_token = TokenType::SEMICOLON);
    std::shared_ptr<UnaryExpr> ParseUnaryExpr(
        const std::shared_ptr<AstNode>& expr = nullptr);
    std::shared_ptr<FlowControl> ParseFlowControlStmt();
    std::shared_ptr<ReturnStmt> ParseReturnStmt();
    std::shared_ptr<DeleteStmt> ParseDeleteStmt();
    std::shared_ptr<ExitStmt> ParseExitStmt();
    std::shared_ptr<AstNode> ParseAssignmentStatement(
        const std::shared_ptr<AstNode>& rexpr = nullptr);
    std::shared_ptr<ConditionalStmt> ParseConditionalStmt();
    std::shared_ptr<ScopeNode> ParseScope();
    std::shared_ptr<FuncCall> ParseFuncCall(
        const std::shared_ptr<AstNode>& expr = nullptr);
    std::shared_ptr<ArrayLiteral> ParseArrayLiteral();
    std::shared_ptr<Loop> ParseWhileLoop();
    std::shared_ptr<Loop> ParseForLoop();
    std::shared_ptr<AliasDecl> ParseAliasDecl();
    std::shared_ptr<AstNode> ParseIndexedExpr(
        const std::shared_ptr<AstNode>& expr = nullptr);
    std::shared_ptr<Name> ParseName();
    std::shared_ptr<TryBlock> ParseTryBlock();
    std::shared_ptr<CatchBlock> ParseCatchBlock();
    std::shared_ptr<Module> ParseModule();
    void RevertScope();
    void ConvertScope(const std::shared_ptr<ScopeNode>& scope);
    [[nodiscard]] std::string DumpsAst() const;
    void Parse();
    std::shared_ptr<AstNode> TransformBinaryExpr(
        std::shared_ptr<BinaryExpr> binary_expr);

    TokenType GetCurrentAsExpectedType() override;
    size_t GetTokenCount();
    std::string ItemToString(Token* token) override;
    void HandleUnexpectedItem() override;
    [[noreturn]] void ThrowError(const std::string& message);
    std::shared_ptr<RnTypeComposite> ParseType();
    void Reset() override;
    void Run() override;
    void SetInput(Token* const* input, size_t size) {
        SetFromPtr(input, size);
    }

public:
    std::string working_dir = ".";
    std::filesystem::path file;
    std::shared_ptr<Ast> ast;
    static std::vector<std::string> parsed_files;

private:
    std::shared_ptr<AstNode> AddCurrentFileInfo(std::shared_ptr<AstNode> node);

private:
    size_t _scope_count = 0;  // Simple way to check if we are missing scope reversions
    std::shared_ptr<ScopeNode> _current_scope = nullptr;
    std::unordered_set<TokenType> unary_lookback_set = {
        TokenType::EQUAL, TokenType::R_PARAN, TokenType::COMMA};
    static std::unordered_map<TokenType, int> _prec_tbl;
    static std::unordered_map<TokenType, Associativity> _associativity;
    std::unordered_map<std::string, std::string> _pragma_table;
    ParserState _previous_state = GENERAL_CONTEXT;
    ParserState _current_state = GENERAL_CONTEXT;
    std::unordered_map<std::string, std::shared_ptr<RnTypeComposite>>
        _user_defined_type_map;
    size_t _intern_count =
        0;  // Used to track how much space we should reserve in the const internment later
};
