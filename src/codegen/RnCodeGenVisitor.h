/*****************************************************************************
* File: RnCodeGenVisitor.h
* Description:
* Author: Malcolm Hall
* Date: 6/26/22
* Version: 1
*
******************************************************************************/

#pragma once

#include <utility>
#include <vector>
#include "../parser/RnAstVisitor.h"
#include "../parser/ast/Ast.h"
#include "RnInstruction.h"

class StringLiteral;
class ScopeNode;
class ImportStmt;
class ArgDecl;
class AstNode;
class AliasDecl;
class Expr;
class VarDecl;
class FloatLiteral;
class UnaryExpr;
class ReturnStmt;
class FuncCall;
class IndexedExpr;
class ClassDecl;
class Loop;
class BreakStmt;
class ConditionalStmt;
class FuncDecl;
class BinaryExpr;
class AssignmentStmt;
class ArrayLiteral;
class IntLiteral;
class ContinueStmt;
class Name;
class TryBlock;
class CatchBlock;
class BoolLiteral;
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
    InstructionBlock Visit(ContinueStmt* node) override;
    InstructionBlock Visit(IndexedExpr* node) override;
    InstructionBlock Visit(BreakStmt* node) override;

    InstructionBlock GetInstructions() {
        return _instructions;
    }

private:
    static void WrapContext(InstructionBlock& block);

private:
    InstructionBlock _instructions;
    std::vector<int> _scope_start_indices;
    std::vector<std::vector<RnInstruction*>> _break_instructions;
    std::vector<std::vector<RnInstruction*>> _continue_instructions;
};
