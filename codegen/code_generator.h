/******************************************************************************
* File:
* Description:
* Author: Malcolm Hall
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
*******************************************************************************/
#ifndef RONASCRIPT_CODEGEN_H
#define RONASCRIPT_CODEGEN_H

#include <vector>
#include <string>
#include <map>
#include "../lexer/token.h"
#include "../lexer/token_type.h"
#include "../parser/ast/Ast.h"
#include "../parser/ast/StringLiteral.h"
#include "../parser/ast/ScopeNode.h"
#include "../parser/ast/RequireStmt.h"
#include "../parser/ast/ArgDecl.h"
#include "../parser/ast/AstNode.h"
#include "../parser/ast/ForLoop.h"
#include "../parser/ast/AliasDecl.h"
#include "../parser/ast/Expr.h"
#include "../parser/ast/VarDecl.h"
#include "../parser/ast/FloatLiteral.h"
#include "../parser/ast/UnaryExpr.h"
#include "../parser/ast/ReturnStmt.h"
#include "../parser/ast/FuncCall.h"
#include "../parser/ast/IndexedExpr.h"
#include "../parser/ast/Ast.h"
#include "../parser/ast/ClassDecl.h"
#include "../parser/ast/NodeType.h"
#include "../parser/ast/WhileLoop.h"
#include "../parser/ast/BreakStmt.h"
#include "../parser/ast/IfStmt.h"
#include "../parser/ast/FuncDecl.h"
#include "../parser/ast/BinaryExpr.h"
#include "../parser/ast/AssignmentStmt.h"
#include "../parser/ast/ListLiteral.h"
#include "../parser/ast/ElifStmt.h"
#include "../parser/ast/IntLiteral.h"
#include "../parser/ast/ContinueStmt.h"
#include "../parser/ast/ElseStmt.h"
#include "../parser/ast/Name.h"
#include "../parser/ast/try_block.h"
#include "../parser/ast/catch_block.h"
#include "../parser/ast/Ast.h"
#include "../vm/instruction.h"

typedef std::vector<Instruction *> InstructionVec;

struct ModuleCodeBlock {
    InstructionVec instructions;
    Instruction *make_instruction;   // Will need to reference later to set the start index and instruction count operands
    long i_start = 0;   // Instruction start index
    long i_cnt = 0; // Instruction count
};

class CodeGenerator {
public:
    CodeGenerator();
    ~CodeGenerator();
    InstructionVec generate_binary_expr(BinaryExpr *node);
    InstructionVec generate_unary_expr(UnaryExpr *node);
    InstructionVec generate_expr(Expr *node);
    InstructionVec generate_indexed_expr(IndexedExpr *node);
    InstructionVec generate_list_literal(ListLiteral *node);
    InstructionVec generate_if_stmt(IfStmt *node);
    InstructionVec generate_elif_stmt(ElifStmt *node);
    InstructionVec generate_else_stmt(ElseStmt *node);
    InstructionVec generate_for_loop(ForLoop *node);
    InstructionVec generate_while_loop(WhileLoop *node);
    InstructionVec generate_break_stmt(BreakStmt *node);
    InstructionVec generate_continue_stmt(ContinueStmt *node);
    InstructionVec generate_return_stmt(ReturnStmt *node);
    InstructionVec generate_scope(ScopeNode *node);
    InstructionVec generate_func_decl(FuncDecl *node);
    InstructionVec generate_func_call(FuncCall *node);
    InstructionVec generate_var_decl(VarDecl *node);
    InstructionVec generate_assignment_stmt(AssignmentStmt *node);
    InstructionVec generate_require(RequireStmt *node);
    InstructionVec generate_class_decl(ClassDecl *node);
    InstructionVec generate_try_block(TryBlock *node);
    InstructionVec generate_alias_decl(AliasDecl *node);
    InstructionVec generate_node(AstNode *node);
    void generate_modules(Ast *ast);
    InstructionVec generate(Ast *ast);
    std::string dumps();

    InstructionVec instructions;

private:
    std::map<std::string, ModuleCodeBlock *> modules;
};


#endif //RONASCRIPT_CODEGEN_H
