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

#include "code_generator.h"

/******************************************************************************
 * @brief
 */
CodeGenerator::CodeGenerator() {

}

/******************************************************************************
 * @brief
 */
CodeGenerator::~CodeGenerator() {

}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_binary_expr(BinaryExpr *node) {
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_unary_expr(UnaryExpr *node) {
    if (node->op == "++") {
        std::vector<Instruction *> _instructions{
                Instruction::make_ldl(static_cast<long>(1)),
                Instruction::make_ldv(node->id->to_string(), static_cast<long>(0)),
                Instruction::make_add(),
                Instruction::make_mov(node->id->to_string(), static_cast<long>(0)),
        };
    } else if (node->op == "--") {
        std::vector<Instruction *> _instructions{
                Instruction::make_ldl(static_cast<long>(1)),
                Instruction::make_ldv(node->id->to_string(), static_cast<long>(0)),
                Instruction::make_sub(),
                Instruction::make_mov(node->id->to_string(), static_cast<long>(0)),
        };

    } else if (node->op == "**") {
        std::vector<Instruction *> _instructions{
                Instruction::make_ldv(node->id->to_string(), static_cast<long>(0)),
                Instruction::make_ldv(node->id->to_string(), static_cast<long>(0)),
                Instruction::make_mul(),
                Instruction::make_mov(node->id->to_string(), static_cast<long>(0)),
        };

    } else if (node->op == "!") {
        std::vector<Instruction *> _instructions{
                Instruction::make_ldv(node->id->to_string(), static_cast<long>(0)),
                Instruction::make_not(),
                Instruction::make_mov(node->id->to_string(), static_cast<long>(0)),
        };

    } else if (node->op == "~") {
        std::vector<Instruction *> _instructions{
                Instruction::make_ldv(node->id->to_string(), static_cast<long>(0)),
                Instruction::make_inv(),
                Instruction::make_mov(node->id->to_string(), static_cast<long>(0)),
        };

    } else if (node->op == "-") {
        std::vector<Instruction *> _instructions{
                Instruction::make_ldl(static_cast<long>(0)),
                Instruction::make_ldv(node->id->to_string(), static_cast<long>(0)),
                Instruction::make_sub(),
                Instruction::make_mov(node->id->to_string(), static_cast<long>(0)),
        };

    } else if (node->op == "+") {
        std::vector<Instruction *> _instructions{
                Instruction::make_ldl(static_cast<long>(0)),
                Instruction::make_ldv(node->id->to_string(), static_cast<long>(0)),
                Instruction::make_add(),
                Instruction::make_mov(node->id->to_string(), static_cast<long>(0)),
        };
    }
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_expr(Expr *node) {
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_indexed_expr(IndexedExpr *node) {
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_list_literal(ListLiteral *node) {
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_if_stmt(IfStmt *node) {
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_elif_stmt(ElifStmt *node) {
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_else_stmt(ElseStmt *node) {
    return generate_scope(node->consequent);
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_for_loop(ForLoop *node) {
    std::vector<Instruction *> out;
    std::vector<Instruction *> init = generate_var_decl(node->init);
    std::vector<Instruction *> test = generate_expr(reinterpret_cast<Expr *>(node->test));
    std::vector<Instruction *> body = generate_scope(node->scope);
    std::vector<Instruction *> update = generate_expr(reinterpret_cast<Expr *>(node->update));

    out.insert(out.end(), init.begin(), init.end());
    out.insert(out.end(), test.begin(), test.end());
    out.emplace_back(Instruction::make_cmp(body.size() + update.size() + 2));

    out.insert(out.end(), body.begin(), body.end());
    out.insert(out.end(), update.begin(), update.end());
    out.emplace_back(Instruction::make_jmp(-(body.size() + test.size() + update.size() + 2)));

    return out;
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_while_loop(WhileLoop *node) {
    std::vector<Instruction *> output;

    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_alias_decl(AliasDecl *node) {
    return {
            Instruction::make_als(node->alias_name->to_string(), node->base_name->to_string())
    };

}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_break_stmt(BreakStmt *node) {
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_flow_stmt(FlowStmt *node) {
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_return_stmt(ReturnStmt *node) {
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_scope(ScopeNode *node) {
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_func_decl(FuncDecl *node) {
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_func_call(FuncCall *node) {
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_var_decl(VarDecl *node) {
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_assignment_stmt(AssignmentStmt *node) {
    return std::vector<Instruction *>();
}

/******************************************************************************
 * @brief
 * @return
 */
std::vector<Instruction *> CodeGenerator::generate_require(RequireStmt *node) {
    return std::vector<Instruction *>();
}
