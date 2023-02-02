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

#include "../parser/../parser/ast/Ast.h"
#include "RnInstruction.h"

#include <vector>

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

class RnCodeGenVisitor {
public:
    RnCodeGenVisitor() = default;
    ~RnCodeGenVisitor() = default;

    InstructionBlock GeneralVisit(AstNode* node);
    InstructionBlock GeneralVisit(const std::shared_ptr<AstNode>& node);
    std::vector<RnInstruction*> Visit(StringLiteral* node);
    std::vector<RnInstruction*> Visit(FloatLiteral* node);
    std::vector<RnInstruction*> Visit(IntLiteral* node);
    std::vector<RnInstruction*> Visit(ArrayLiteral* node);
    std::vector<RnInstruction*> Visit(ScopeNode* node);
    std::vector<RnInstruction*> Visit(ForLoop* node);
    std::vector<RnInstruction*> Visit(WhileLoop* node);
    std::vector<RnInstruction*> Visit(ImportStmt* node);
    std::vector<RnInstruction*> Visit(Module* node);
    std::vector<RnInstruction*> Visit(FuncDecl* node);
    std::vector<RnInstruction*> Visit(FuncCall* node);
    std::vector<RnInstruction*> Visit(VarDecl* node);
    std::vector<RnInstruction*> Visit(Name* node);
    std::vector<RnInstruction*> Visit(ClassDecl* node);
    std::vector<RnInstruction*> Visit(ExitStmt* node);
    std::vector<RnInstruction*> Visit(ReturnStmt* node);
    std::vector<RnInstruction*> Visit(AttributeAccess* node);
    std::vector<RnInstruction*> Visit(TryBlock* node);
    std::vector<RnInstruction*> Visit(CatchBlock* node);
    std::vector<RnInstruction*> Visit(IfStmt* node);
    std::vector<RnInstruction*> Visit(ElifStmt* node);
    std::vector<RnInstruction*> Visit(ElseStmt* node);
    std::vector<RnInstruction*> Visit(DeleteStmt* node);
    std::vector<RnInstruction*> Visit(BoolLiteral* node);
    std::vector<RnInstruction*> Visit(UnaryExpr* node);
    std::vector<RnInstruction*> Visit(Expr* node);
    std::vector<RnInstruction*> Visit(AliasDecl* node);
    std::vector<RnInstruction*> Visit(ArgDecl* node);
    std::vector<RnInstruction*> Visit(AssignmentStmt* node);
    std::vector<RnInstruction*> Visit(BinaryExpr* node);
    std::vector<RnInstruction*> Visit(ContinueStmt* node);
    std::vector<RnInstruction*> Visit(IndexedExpr* node);
    std::vector<RnInstruction*> Visit(BreakStmt* node);

    InstructionBlock GetInstructions() { return _instructions; }

private:
    void WrapContext(InstructionBlock& block);

private:
    std::vector<RnInstruction*> _instructions;
    std::vector<int> _scope_start_indices;
};

#endif  //RONASCRIPT_RNCODEGENVISITOR_H