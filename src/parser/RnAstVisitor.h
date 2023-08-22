/*****************************************************************************
* File: RnAstVisitor.cpp
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

class LiteralValue;
class ScopeNode;
class ImportStmt;
class ArgDecl;
class AstNode;
class Loop;
class AliasDecl;
class Expr;
class VarDecl;
class UnaryExpr;
class ReturnStmt;
class FuncCall;
class IndexedExpr;
class ClassDecl;
class FlowControl;
class ConditionalStmt;
class FuncDecl;
class BinaryExpr;
class AssignmentStmt;
class ArrayLiteral;
class LiteralValue;
class Name;
class TryBlock;
class CatchBlock;
class DeleteStmt;
class ExitStmt;
class AttributeAccess;
class Module;

template <typename T>
class RnAstVisitor {
    virtual T GeneralVisit(AstNode* node) = 0;
    virtual T GeneralVisit(const std::shared_ptr<AstNode>& node) = 0;
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
    virtual T Visit(LiteralValue* node) = 0;
    virtual T Visit(AttributeAccess* node) = 0;
    virtual T Visit(TryBlock* node) = 0;
    virtual T Visit(CatchBlock* node) = 0;
    virtual T Visit(ConditionalStmt* node) = 0;
    virtual T Visit(DeleteStmt* node) = 0;
    virtual T Visit(UnaryExpr* node) = 0;
    virtual T Visit(Expr* node) = 0;
    virtual T Visit(AliasDecl* node) = 0;
    virtual T Visit(ArgDecl* node) = 0;
    virtual T Visit(AssignmentStmt* node) = 0;
    virtual T Visit(BinaryExpr* node) = 0;
    virtual T Visit(FlowControl* node) = 0;
    virtual T Visit(IndexedExpr* node) = 0;
};
