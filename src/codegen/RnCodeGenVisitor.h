/*****************************************************************************
* File: RnCodeGenVisitor.cpp
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

#include <utility>
#include <vector>
#include "../parser/RnAstVisitor.h"
#include "../parser/ast/Ast.h"
#include "RnInstruction.h"

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

class RnCodeGenVisitor : public RnAstVisitor<InstructionBlock> {
public:
    RnCodeGenVisitor() = default;
    ~RnCodeGenVisitor() = default;

    InstructionBlock GeneralVisit(AstNode* node) override;
    InstructionBlock GeneralVisit(const std::shared_ptr<AstNode>& node) override;
    InstructionBlock Visit(ArrayLiteral* node) override;
    InstructionBlock Visit(LiteralValue* node) override;
    InstructionBlock Visit(ScopeNode* node) override;
    InstructionBlock Visit(Loop* node) override;
    InstructionBlock Visit(ImportStmt* node) override;
    InstructionBlock Visit(Module* node) override;
    InstructionBlock Visit(FuncDecl* node) override;
    InstructionBlock Visit(FuncCall* node) override;
    InstructionBlock Visit(VarDecl* node) override;
    InstructionBlock Visit(Name* node) override;
    InstructionBlock Visit(ClassDecl* node) override;
    InstructionBlock Visit(ExitStmt* node) override;
    InstructionBlock Visit(ReturnStmt* node) override;
    InstructionBlock Visit(AttributeAccess* node) override;
    InstructionBlock Visit(TryBlock* node) override;
    InstructionBlock Visit(CatchBlock* node) override;
    InstructionBlock Visit(ConditionalStmt* node) override;
    InstructionBlock Visit(DeleteStmt* node) override;
    InstructionBlock Visit(UnaryExpr* node) override;
    InstructionBlock Visit(Expr* node) override;
    InstructionBlock Visit(AliasDecl* node) override;
    InstructionBlock Visit(ArgDecl* node) override;
    InstructionBlock Visit(AssignmentStmt* node) override;
    InstructionBlock Visit(BinaryExpr* node) override;
    InstructionBlock Visit(IndexedExpr* node) override;
    InstructionBlock Visit(FlowControl* node) override;

private:
    static void WrapContext(InstructionBlock& block);

private:
    InstructionBlock _instructions;
    std::vector<int> _scope_start_indices;
    std::vector<std::vector<RnInstruction*>> _break_instructions;
    std::vector<std::vector<RnInstruction*>> _continue_instructions;
};
