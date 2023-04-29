/*****************************************************************************
* File: RnCodeGenVisitor.h
* Description:
* Author: Malcolm Hall
* Date: 6/26/22
* Version: 1
*
******************************************************************************/

#pragma once

#include <vector>
#include "../common/RnType.h"
#include "RnAstVisitor.h"

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
class Module;
class ExitStmt;
class AttributeAccess;

class RnAstValidator : public RnAstVisitor<bool> {
public:
    enum ASSIGNMENT_TYPE { RETURN_VALUE, ASSIGNMENT_VALUE };
    RnAstValidator() = default;
    ~RnAstValidator() = default;

    bool GeneralVisit(AstNode* node) override;
    bool GeneralVisit(const std::shared_ptr<AstNode>& node) override;
    bool Visit(StringLiteral* node) override;
    bool Visit(FloatLiteral* node) override;
    bool Visit(IntLiteral* node) override;
    bool Visit(ArrayLiteral* node) override;
    bool Visit(ScopeNode* node) override;
    bool Visit(Loop* node) override;
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
    bool Visit(BoolLiteral* node) override;
    bool Visit(UnaryExpr* node) override;
    bool Visit(Expr* node) override;
    bool Visit(AliasDecl* node) override;
    bool Visit(ArgDecl* node) override;
    bool Visit(AssignmentStmt* node) override;
    bool Visit(BinaryExpr* node) override;
    bool Visit(ContinueStmt* node) override;
    bool Visit(IndexedExpr* node) override;
    bool Visit(BreakStmt* node) override;

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
