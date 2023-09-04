/*****************************************************************************
* File: RnAstValidator.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
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

#include <vector>
#include "../common/RnCompilerPhase.h"
#include "../common/RnType.h"
#include "RnAstVisitor.h"

class LiteralValue;
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
class Module;
class ExitStmt;
class AttributeAccess;
class Ast;

class RnAstValidator : public RnAstVisitor<bool>, RnCompilerPhase<Ast*, bool> {
public:
    enum ASSIGNMENT_TYPE { RETURN_VALUE, ASSIGNMENT_VALUE };
    RnAstValidator() = default;
    ~RnAstValidator() = default;

    bool GeneralVisit(AstNode* node) override;
    bool GeneralVisit(const std::shared_ptr<AstNode>& node) override;
    bool Visit(ArrayLiteral* node) override;
    bool Visit(ScopeNode* node) override;
    bool Visit(Loop* node) override;
    bool Visit(LiteralValue* node) override;
    bool Visit(ImportStmt* node) override;
    bool Visit(Module* node) override;
    bool Visit(FuncDecl* node) override;
    bool Visit(FuncCall* node) override;
    bool Visit(VarDecl* node) override;
    bool Visit(Name* node) override;
    bool Visit(ClassDecl* node) override;
    bool Visit(ExitStmt* node) override;
    bool Visit(ReturnStmt* node) override;
    bool Visit(AttributeAccess* node) override;
    bool Visit(TryBlock* node) override;
    bool Visit(CatchBlock* node) override;
    bool Visit(ConditionalStmt* node) override;
    bool Visit(DeleteStmt* node) override;
    bool Visit(UnaryExpr* node) override;
    bool Visit(Expr* node) override;
    bool Visit(AliasDecl* node) override;
    bool Visit(ArgDecl* node) override;
    bool Visit(AssignmentStmt* node) override;
    bool Visit(BinaryExpr* node) override;
    bool Visit(IndexedExpr* node) override;
    bool Visit(FlowControl* node) override;
    void Run() override;

private:
    void SymbolRedeclarationCheck(const std::string& symbol);
    void SymbolExistsCheck(const std::string& symbol);
    std::shared_ptr<RnTypeComposite> EvaluateSubtreeType(
        const std::shared_ptr<AstNode>& subtree);
    static std::shared_ptr<RnTypeComposite> ResolveTypes(
        const std::shared_ptr<RnTypeComposite>& type1,
        const std::shared_ptr<RnTypeComposite>& type2);  // TODO: Unit test
    bool CanAssignTypeTo(const std::shared_ptr<RnTypeComposite>& destination,
                         const std::shared_ptr<RnTypeComposite>& source,
                         ASSIGNMENT_TYPE assignment_type);  // TODO: Unit test

private:
    ScopeNode* _current_scope = nullptr;
    std::shared_ptr<RnTypeComposite> _current_type_reference;
};
