/*****************************************************************************
* File: RnCodeGenVisitor.h
* Description:
* Author: Malcolm Hall
* Date: 6/26/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNASTVALIDATOR_H
#define RONASCRIPT_RNASTVALIDATOR_H

#include "RnTypeComposite.h"

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

class Module;

class ExitStmt;

class AttributeAccess;

class RnAstValidator {
public:
    RnAstValidator() = default;
    ~RnAstValidator() = default;

    void SymbolRedeclarationCheck(const std::string& symbol);
    void SymbolExistsCheck(const std::string& symbol);
    std::shared_ptr<RnTypeComposite> EvaluateSubtreeType(
        const std::shared_ptr<AstNode>& subtree);
    static std::shared_ptr<RnTypeComposite> ResolveTypes(
        const std::shared_ptr<RnTypeComposite>& type1,
        const std::shared_ptr<RnTypeComposite>& type2);
    bool CanAssignTypeTo(const std::shared_ptr<RnTypeComposite>& destination,
                         const std::shared_ptr<RnTypeComposite>& source);
    bool GeneralVisit(AstNode* node);
    bool GeneralVisit(const std::shared_ptr<AstNode>& node);
    bool Visit(StringLiteral* node);
    bool Visit(FloatLiteral* node);
    bool Visit(IntLiteral* node);
    bool Visit(ArrayLiteral* node);
    bool Visit(ScopeNode* node);
    bool Visit(ForLoop* node);
    bool Visit(WhileLoop* node);
    bool Visit(ImportStmt* node);
    bool Visit(Module* node);
    bool Visit(FuncDecl* node);
    bool Visit(FuncCall* node);
    bool Visit(VarDecl* node);
    bool Visit(Name* node);
    bool Visit(ClassDecl* node);
    bool Visit(ExitStmt* node);
    bool Visit(ReturnStmt* node);
    bool Visit(AttributeAccess* node);
    bool Visit(TryBlock* node);
    bool Visit(CatchBlock* node);
    bool Visit(IfStmt* node);
    bool Visit(ElifStmt* node);
    bool Visit(ElseStmt* node);
    bool Visit(DeleteStmt* node);
    bool Visit(BoolLiteral* node);
    bool Visit(UnaryExpr* node);
    bool Visit(Expr* node);
    bool Visit(AliasDecl* node);
    bool Visit(ArgDecl* node);
    bool Visit(AssignmentStmt* node);
    bool Visit(BinaryExpr* node);
    bool Visit(ContinueStmt* node);
    bool Visit(IndexedExpr* node);
    bool Visit(BreakStmt* node);

    ScopeNode* _current_scope = nullptr;
};

#endif  //RONASCRIPT_RNASTVALIDATOR_H