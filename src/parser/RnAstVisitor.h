

#pragma once

class StringLiteral;
class ScopeNode;
class ImportStmt;
class ArgDecl;
class AstNode;
class Loop;
class AliasDecl;
class Expr;
class VarDecl;
class FloatLiteral;
class UnaryExpr;
class ReturnStmt;
class FuncCall;
class IndexedExpr;
class ClassDecl;
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
class Module;

template <typename T>
class RnAstVisitor {
    virtual T GeneralVisit(AstNode* node) = 0;
    virtual T GeneralVisit(const std::shared_ptr<AstNode>& node) = 0;
    virtual T Visit(StringLiteral* node) = 0;
    virtual T Visit(FloatLiteral* node) = 0;
    virtual T Visit(IntLiteral* node) = 0;
    virtual T Visit(ArrayLiteral* node) = 0;
    virtual T Visit(ScopeNode* node) = 0;
    virtual T Visit(Loop* node) = 0;
    virtual T Visit(ImportStmt* node) = 0;
    virtual T Visit(Module* node) = 0;
    virtual T Visit(FuncDecl* node) = 0;
    virtual T Visit(FuncCall* node) = 0;
    virtual T Visit(VarDecl* node) = 0;
    virtual T Visit(Name* node) = 0;
    virtual T Visit(ClassDecl* node) = 0;
    virtual T Visit(ExitStmt* node) = 0;
    virtual T Visit(ReturnStmt* node) = 0;
    virtual T Visit(AttributeAccess* node) = 0;
    virtual T Visit(TryBlock* node) = 0;
    virtual T Visit(CatchBlock* node) = 0;
    virtual T Visit(ConditionalStmt* node) = 0;
    virtual T Visit(DeleteStmt* node) = 0;
    virtual T Visit(BoolLiteral* node) = 0;
    virtual T Visit(UnaryExpr* node) = 0;
    virtual T Visit(Expr* node) = 0;
    virtual T Visit(AliasDecl* node) = 0;
    virtual T Visit(ArgDecl* node) = 0;
    virtual T Visit(AssignmentStmt* node) = 0;
    virtual T Visit(BinaryExpr* node) = 0;
    virtual T Visit(ContinueStmt* node) = 0;
    virtual T Visit(IndexedExpr* node) = 0;
    virtual T Visit(BreakStmt* node) = 0;
};
