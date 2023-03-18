/*****************************************************************************
* File: RnCodeGenVisitor.h
* Description:
* Author: Malcolm Hall
* Date: 6/26/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNCODEGENVISITOR_H
#define RONASCRIPT_RNCODEGENVISITOR_H

#include <vector>
#include "../parser/RnAstVisitor.h"
#include "../parser/ast/Ast.h"
#include "RnInstruction.h"

class StringLiteral;
class ScopeNode;
class ImportStmt;
class ArgDecl;
class AstNode;
class ForLoop;
class AliasDecl;
class Expr;
class VarDecl;
class FloatLiteral;
class UnaryExpr;
class ReturnStmt;
class FuncCall;
class IndexedExpr;
class ClassDecl;
class WhileLoop;
class BreakStmt;
class IfStmt;
class FuncDecl;
class BinaryExpr;
class AssignmentStmt;
class ArrayLiteral;
class ElifStmt;
class IntLiteral;
class ContinueStmt;
class ElseStmt;
class Name;
class TryBlock;
class CatchBlock;
class BoolLiteral;
class DeleteStmt;
class ExitStmt;
class AttributeAccess;

typedef std::vector<RnInstruction*> InstructionBlock;

class RnCodeGenVisitor : public RnAstVisitor<InstructionBlock> {
public:
    RnCodeGenVisitor() = default;
    ~RnCodeGenVisitor() = default;

    InstructionBlock GeneralVisit(AstNode* node) override;
    InstructionBlock GeneralVisit(const std::shared_ptr<AstNode>& node) override;
    InstructionBlock Visit(StringLiteral* node) override;
    InstructionBlock Visit(FloatLiteral* node) override;
    InstructionBlock Visit(IntLiteral* node) override;
    InstructionBlock Visit(ArrayLiteral* node) override;
    InstructionBlock Visit(ScopeNode* node) override;
    InstructionBlock Visit(ForLoop* node) override;
    InstructionBlock Visit(WhileLoop* node) override;
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
    InstructionBlock Visit(IfStmt* node) override;
    InstructionBlock Visit(ElifStmt* node) override;
    InstructionBlock Visit(ElseStmt* node) override;
    InstructionBlock Visit(DeleteStmt* node) override;
    InstructionBlock Visit(BoolLiteral* node) override;
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
};

#endif  //RONASCRIPT_RNCODEGENVISITOR_H