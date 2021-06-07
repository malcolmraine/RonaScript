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
#include "../parser/ast/BoolLiteral.h"

/******************************************************************************
 * @brief Constructor for the CodeGenerator class.
 */
CodeGenerator::CodeGenerator() = default;

/******************************************************************************
 * @brief Destructor for the CodeGenerator class.
 */
CodeGenerator::~CodeGenerator() {
    for (auto &instruction : this->instructions) {
        delete instruction;
    }

    for (auto &[key, module] : this->modules) {
        module->instructions.clear();
        delete module;
    }

    this->instructions.clear();
}

/******************************************************************************
 * @brief Generate the instructions for a BinaryExpr AST node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_binary_expr(BinaryExpr *node) {
    InstructionVec output;
    InstructionVec left = generate_node(node->left);
    InstructionVec right = generate_node(node->right);
    output.insert(output.end(), left.begin(), left.end());
    output.insert(output.end(), right.begin(), right.end());

    if (node->op == "+") {
        output.emplace_back(new Instruction(OP_BINARY_ADD));
    } else if (node->op == "-") {
        output.emplace_back(new Instruction(OP_BINARY_SUB));
    } else if (node->op == "/") {
        output.emplace_back(new Instruction(OP_BINARY_DIV));
    } else if (node->op == "*") {
        output.emplace_back(new Instruction(OP_BINARY_MUL));
    } else if (node->op == "%") {
        output.emplace_back(new Instruction(OP_BINARY_MOD));
    } else if (node->op == "^") {
        output.emplace_back(new Instruction(OP_BINARY_XOR));
    } else if (node->op == "<") {
        output.emplace_back(new Instruction(OP_BINARY_LT));
    } else if (node->op == ">") {
        output.emplace_back(new Instruction(OP_BINARY_GT));
    } else if (node->op == "<=") {
        output.emplace_back(new Instruction(OP_BINARY_LTE));
    } else if (node->op == ">=") {
        output.emplace_back(new Instruction(OP_BINARY_GTE));
    } else if (node->op == "==") {
        output.emplace_back(new Instruction(OP_BINARY_EQ));
    } else if (node->op == "|") {
        output.emplace_back(new Instruction(OP_BINARY_OR));
    } else if (node->op == "||") {
        output.emplace_back(new Instruction(OP_LOGICAL_OR));
    } else if (node->op == "&") {
        output.emplace_back(new Instruction(OP_BINARY_AND));
    } else if (node->op == "&&") {
        output.emplace_back(new Instruction(OP_LOGICAL_AND));
    }

    return output;
}

/******************************************************************************
 * @brief Generate the instructions for a UnaryExpr Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_unary_expr(UnaryExpr *node) {
    std::string id = dynamic_cast<Name *>(node->expr)->value;

    if (node->op == "++") {
        return {
                new Instruction(OP_LOAD_LITERAL, new RonaObject(1L)),
                new Instruction(OP_LOAD_VAR, new RonaObject(id)),
                new Instruction(OP_BINARY_ADD),
                new Instruction(OP_STORE, new RonaObject(id)),
        };
    } else if (node->op == "--") {
        return {
                new Instruction(OP_LOAD_LITERAL, new RonaObject(1L)),
                new Instruction(OP_LOAD_VAR, new RonaObject(id)),
                new Instruction(OP_BINARY_SUB),
                new Instruction(OP_STORE, new RonaObject(id)),
        };

    } else if (node->op == "**") {
        return {
                new Instruction(OP_LOAD_VAR, new RonaObject(id)),
                new Instruction(OP_LOAD_VAR, new RonaObject(id)),
                new Instruction(OP_BINARY_MUL),
                new Instruction(OP_STORE, new RonaObject(id)),
        };

    } else if (node->op == "!") {
        return {
                new Instruction(OP_LOAD_VAR, new RonaObject(id)),
                new Instruction(OP_UNARY_NOT),
                new Instruction(OP_STORE, new RonaObject(id)),
        };

    } else if (node->op == "~") {
        return {
                new Instruction(OP_LOAD_VAR, new RonaObject(id)),
                new Instruction(OP_UNARY_INVERT),
                new Instruction(OP_STORE, new RonaObject(id)),
        };

    } else if (node->op == "-") {
        return {
                new Instruction(OP_LOAD_LITERAL, new RonaObject(0L)),
                new Instruction(OP_LOAD_VAR, new RonaObject(id)),
                new Instruction(OP_BINARY_SUB),
                new Instruction(OP_STORE, new RonaObject(node->expr->to_string())),
        };

    } else if (node->op == "+") {
        return {
                new Instruction(OP_LOAD_LITERAL, new RonaObject(0L)),
                new Instruction(OP_LOAD_VAR, new RonaObject(id)),
                new Instruction(OP_BINARY_ADD),
                new Instruction(OP_STORE, new RonaObject(node->expr->to_string())),
        };
    } else {
        return InstructionVec();
    }
}

/******************************************************************************
 * @brief Generate the instructions for a Expr Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_expr(Expr *node) {
    if (node != nullptr && node->expr != nullptr) {
        return generate_node(node->expr);
    } else {
        return InstructionVec();
    }
}

/******************************************************************************
 * @brief Generate the instructions for a IndexedExpr Ast node.
 * @return
 */
InstructionVec CodeGenerator::generate_indexed_expr(IndexedExpr *node) {
    InstructionVec out;
    InstructionVec idx = generate_node(node->idx);
    out.insert(out.end(), idx.begin(), idx.end());

    if (node->is_access) {
        InstructionVec expr = generate_node(node->expr);
        out.insert(out.end(), expr.begin(), expr.end());
        out.emplace_back(new Instruction(OP_ARRAY_ACCESS));
    } else {
        out.emplace_back(new Instruction(OP_ARRAY_STORE, new RonaObject(dynamic_cast<Name *>(node->expr)->value)));
    }

    return out;
}

/******************************************************************************
 * @brief Generate the instructions for a ListLiteral Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_list_literal(ListLiteral *node) {
    InstructionVec out;

    for (auto &item : node->items) {
        InstructionVec item_instructions = generate_node(item);
        out.insert(out.begin(), item_instructions.begin(), item_instructions.end());
    }
    out.emplace_back(new Instruction(OP_BUILD_ARRAY, new RonaObject(static_cast<long>(node->items.size()))));

    return out;
}

/******************************************************************************
 * @brief Generate the instructions for a IfStmt Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_if_stmt(IfStmt *node) {
    InstructionVec out = generate_expr(dynamic_cast<Expr *>(node->test));
    InstructionVec body = generate_scope(dynamic_cast<ScopeNode *>(node->consequent));
    out.push_back(new Instruction(OP_COMPARE, new RonaObject(static_cast<long>(body.size() + 1))));
    out.insert(out.end(), body.begin(), body.end());

    InstructionVec alternative;

    if (node->alternative->node_type == ELIF_STMT) {
        alternative = generate_elif_stmt(dynamic_cast<ElifStmt *>(node->alternative));
    } else if (node->alternative->node_type == ELSE_STMT) {
        alternative = generate_else_stmt(dynamic_cast<ElseStmt *>(node->alternative));
    } else {
        alternative = {};
    }

    if (!alternative.empty()) {
        out.push_back(new Instruction(OP_JUMP, new RonaObject(static_cast<long>(alternative.size()))));
        out.insert(out.end(), alternative.begin(), alternative.end());
    }

    return out;
}

/******************************************************************************
 * @brief Generate the instructions for a ElifStmt Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_elif_stmt(ElifStmt *node) {
    InstructionVec out = generate_expr(dynamic_cast<Expr *>(node->test));
    InstructionVec body = generate_scope(dynamic_cast<ScopeNode *>(node->consequent));
    out.push_back(new Instruction(OP_COMPARE, new RonaObject(static_cast<long>(body.size() + 1))));
    out.insert(out.end(), body.begin(), body.end());

    InstructionVec alternative;

    if (node->alternative->node_type == ELIF_STMT) {
        alternative = generate_elif_stmt(dynamic_cast<ElifStmt *>(node->alternative));
    } else if (node->alternative->node_type == ELSE_STMT) {
        alternative = generate_else_stmt(dynamic_cast<ElseStmt *>(node->alternative));
    } else {
        alternative = {};
    }

    if (!alternative.empty()) {
        out.push_back(new Instruction(OP_JUMP, new RonaObject(static_cast<long>(alternative.size()))));
        out.insert(out.end(), alternative.begin(), alternative.end());
    }

    return out;
}

/******************************************************************************
 * @brief Generate the instructions for a ElseStmt Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_else_stmt(ElseStmt *node) {
    return generate_scope(node->consequent);
}

/******************************************************************************
 * @brief Generate the instructions for a ForLoop Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_for_loop(ForLoop *node) {
    InstructionVec out;
    InstructionVec init = generate_node(node->init);
    InstructionVec test = generate_node(node->test);
    InstructionVec body = generate_scope(node->scope);
    InstructionVec update = generate_node(node->update);
    long cmp_jump_length = body.size() + update.size() + 4; // NOLINT(cppcoreguidelines-narrowing-conversions)
    long reset_jump_length = -(body.size() + test.size() + update.size() +
                               5); // NOLINT(cppcoreguidelines-narrowing-conversions)

    out.insert(out.end(), init.begin(), init.end());
    out.insert(out.end(), test.begin(), test.end());
    out.emplace_back(new Instruction(OP_COMPARE, new RonaObject(static_cast<long>(cmp_jump_length))));
    out.emplace_back(new Instruction(OP_CREATE_CONTEXT));
    out.insert(out.end(), body.begin(), body.end());
    out.emplace_back(new Instruction(OP_DESTROY_CONTEXT));
    out.insert(out.end(), update.begin(), update.end());
    out.emplace_back(new Instruction(OP_JUMP, new RonaObject(static_cast<long>(reset_jump_length))));
    out.insert(out.begin(), new Instruction(OP_CREATE_CONTEXT));
    out.emplace_back(new Instruction(OP_DESTROY_CONTEXT));

    // Go back through the instructions and fill in the break and continue statement index operands
    long idx = 0;

    for (auto &instruction : out) {
        if (instruction->opcode == OP_BREAK) {
            instruction->operands.emplace_back(new RonaObject(static_cast<long>(out.size() - idx - 1)));
        } else if (instruction->opcode == OP_CONTINUE) {
            instruction->operands.emplace_back(new RonaObject(static_cast<long>(test.size() - idx + 1)));
        }
        idx++;
    }

    return out;
}

/******************************************************************************
 * @brief Generate the instructions for a WhileLoop Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_while_loop(WhileLoop *node) {
    InstructionVec out;
    InstructionVec test = generate_node(node->test);
    InstructionVec body = generate_scope(node->scope);
    long cmp_jump_length = body.size() + 3;
    long reset_jump_length = -(body.size() + test.size() + 4);

    out.insert(out.end(), test.begin(), test.end());
    out.emplace_back(new Instruction(OP_COMPARE, new RonaObject(static_cast<long>(cmp_jump_length))));
    out.emplace_back(new Instruction(
            OP_CREATE_CONTEXT,
            new RonaObject(static_cast<long>(body.size() + 1))
    ));

    out.insert(out.end(), body.begin(), body.end());
    out.emplace_back(new Instruction(OP_DESTROY_CONTEXT));
    out.emplace_back(new Instruction(OP_JUMP, new RonaObject(static_cast<long>(reset_jump_length))));

    // Go back through the instructions and fill in the break and continue statement index operands
    long idx = 0;
    for (auto &instruction : out) {
        if (instruction->opcode == OP_BREAK) {
            instruction->operands.emplace_back(new RonaObject(static_cast<long>(out.size() - idx - 2)));
        } else if (instruction->opcode == OP_CONTINUE) {
            instruction->operands.emplace_back(new RonaObject(static_cast<long>(test.size() - idx - 3)));
        }
        idx++;
    }

    return out;
}

/******************************************************************************
 * @brief Generate the instructions for a BreakStmt Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_break_stmt(BreakStmt *node) {
    return {new Instruction(OP_BREAK)};
}

/******************************************************************************
 * @brief Generate the instructions for a ContinueStmt Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_continue_stmt(ContinueStmt *node) {
    return {new Instruction(OP_CONTINUE)};
}

/******************************************************************************
 * @brief Generate the instructions for a ReturnStmt Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_return_stmt(ReturnStmt *node) {
    InstructionVec out;

    if (node->expr == nullptr) {
        out.emplace_back(new Instruction(OP_LOAD_NULL));
    } else {
        InstructionVec expr = generate_node(node->expr);
        out.insert(out.begin(), expr.begin(), expr.end());
    }
    out.emplace_back(new Instruction(OP_RETURN));

    return out;
}

/******************************************************************************
 * @brief Generate the instructions for a ScopeNode Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_scope(ScopeNode *node) {
    InstructionVec out;

    for (auto child : node->class_decls) {
        std::vector<Instruction *> child_instructions = generate_class_decl(child);
        out.insert(out.end(), child_instructions.begin(), child_instructions.end());
    }

    for (auto child : node->var_decls) {
        std::vector<Instruction *> child_instructions = generate_var_decl(child);
        out.insert(out.end(), child_instructions.begin(), child_instructions.end());
    }

    for (auto child : node->func_decls) {
        std::vector<Instruction *> child_instructions = generate_func_decl(child);
        out.insert(out.end(), child_instructions.begin(), child_instructions.end());
    }

    for (auto child : node->children) {
        std::vector<Instruction *> child_instructions = generate_node(child);
        out.insert(out.end(), child_instructions.begin(), child_instructions.end());
    }

    return out;
}

/******************************************************************************
 * @brief Generate the instructions for a FuncDecl Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_func_decl(FuncDecl *node) {
    InstructionVec out;
    InstructionVec scope = generate_scope(node->scope);
    auto *instruction = new Instruction(OP_MAKE_FUNC);

    for (auto &arg : node->args) {
        instruction->operands.emplace_back(new RonaObject(static_cast<std::string>(arg->id->value)));
        instruction->operands.emplace_back(new RonaObject(static_cast<std::string>(arg->type)));
    }

    instruction->operands.emplace_back(new RonaObject(node->id));
    instruction->operands.emplace_back(new RonaObject(get_type(node->type)));
    instruction->operands.emplace_back(new RonaObject(static_cast<long>(scope.size())));
    instruction->operands.emplace_back(new RonaObject(static_cast<long>(node->args.size())));

    out.emplace_back(instruction);

//    for (auto &arg : node->args) {
//        out.emplace_back(new Instruction(
//                OP_MAKE_ARG,
//                new RonaObject(arg->id->value),
//                new RonaObject(arg->type))
//        );
//    }
    out.insert(out.end(), scope.begin(), scope.end());

    return out;
}

/******************************************************************************
 * @brief Generate the instructions for a FuncCall Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_func_call(FuncCall *node) {
    InstructionVec out;

    for (auto &arg : node->args) {
        InstructionVec arg_instructions = generate_node(arg);
        out.insert(out.end(), arg_instructions.begin(), arg_instructions.end());
    }

    out.emplace_back(new Instruction(
            OP_CALL_FUNC,
            new RonaObject(node->id->value),
            new RonaObject(static_cast<long>(node->args.size()))
    ));

    return out;
}

/******************************************************************************
 * @brief Generate the instructions for a VarDecl Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_var_decl(VarDecl *node) {
    InstructionVec out;

    if (node->is_const) {
        out.emplace_back(new Instruction(
                OP_MAKE_CONST,
                new RonaObject(node->type),
                new RonaObject(node->id)
        ));

        if (node->init_value != nullptr) {
            InstructionVec init = generate_node(node->init_value);

            if (init[0]->opcode == OP_LOAD_LITERAL) {
                out.back()->operands.emplace_back(init[0]->operands[0]);
                // TODO: delete generated init instructions
            } else {
                // TODO: Throw exception
            }
        }
    } else {
        out.emplace_back(new Instruction(
                OP_MAKE_VAR,
                new RonaObject(node->type),
                new RonaObject(node->id)
        ));

        if (node->init_value != nullptr) {
            InstructionVec init = generate_node(node->init_value);
            out.insert(out.end(), init.begin(), init.end());
            out.emplace_back(new Instruction(OP_STORE, new RonaObject(node->id)));
        }
    }

    return out;
}

/******************************************************************************
 * @brief Generate the instructions for a AssignmentStmt Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_assignment_stmt(AssignmentStmt *node) {
    InstructionVec out;

    if (node->idx == nullptr) {
        InstructionVec expr = generate_node(node->expr);
        out.insert(out.end(), expr.begin(), expr.end());
        out.emplace_back(new Instruction(OP_STORE, new RonaObject(node->id->value)));
    } else {
        InstructionVec expr = generate_node(node->expr);
        InstructionVec idx = generate_node(node->idx);
        out.insert(out.end(), expr.begin(), expr.end());
        out.insert(out.end(), idx.begin(), idx.end());
        out.emplace_back(new Instruction(OP_ARRAY_STORE, new RonaObject(node->id->value)));
    }

    return out;
}

/******************************************************************************
 * @brief Generate the instructions for a Require Ast node.
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_require(RequireStmt *node) {
    auto instruction = new Instruction(OP_MAKE_MODULE, new RonaObject(node->module->name));
    this->modules[node->module->name]->make_instruction = instruction;

    return {instruction};
}

/******************************************************************************
 * @brief Generate the instructions for a ClassDecl Ast node.
 * @param node
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_class_decl(ClassDecl *node) {
    InstructionVec output;
    InstructionVec scope = generate_scope(node->scope);
    output.emplace_back(new Instruction(
            OP_MAKE_CLASS,
            new RonaObject(node->id),
            new RonaObject(static_cast<long>(scope.size()))
    ));
    output.insert(output.end(), scope.begin(), scope.end());

    return output;
}

/******************************************************************************
 * @brief Generate the instructions for a generic Ast node.
 * @param node
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_node(AstNode *node) {
    switch (node->node_type) {
        case ASSIGNMENT_STMT:
            return generate_assignment_stmt(dynamic_cast<AssignmentStmt *>(node));
        case BINARY_EXPR:
            return generate_binary_expr(dynamic_cast<BinaryExpr *>(node));
        case INDEXED_EXPR:
            return generate_indexed_expr(dynamic_cast<IndexedExpr *>(node));;
        case WHILE_LOOP:
            return generate_while_loop(dynamic_cast<WhileLoop *>(node));;
        case FOR_LOOP:
            return generate_for_loop(dynamic_cast<ForLoop *>(node));;
        case CLASS_DECL:
            return generate_class_decl(dynamic_cast<ClassDecl *>(node));;
        case EXPR:
            return generate_expr(dynamic_cast<Expr *>(node));
        case CONTINUE_STMT:
            return generate_continue_stmt(dynamic_cast<ContinueStmt *>(node));
        case FUNC_CALL:
            return generate_func_call(dynamic_cast<FuncCall *>(node));
        case FUNC_DECL:
            return generate_func_decl(dynamic_cast<FuncDecl *>(node));
        case LIST_LITERAL:
            return generate_list_literal(dynamic_cast<ListLiteral *>(node));
        case STRING_LITERAL:
            return {new Instruction(OP_LOAD_LITERAL, new RonaObject(dynamic_cast<StringLiteral *>(node)->data))};
        case BOOL_LITERAL:
            return {new Instruction(OP_LOAD_LITERAL, new RonaObject(dynamic_cast<BoolLiteral *>(node)->data))};
        case FLOAT_LITERAL:
            return {new Instruction(OP_LOAD_LITERAL, new RonaObject(dynamic_cast<FloatLiteral *>(node)->data))};
        case INT_LITERAL:
            return {new Instruction(OP_LOAD_LITERAL, new RonaObject(dynamic_cast<IntLiteral *>(node)->data))};
        case REQUIRE:
            return generate_require(dynamic_cast<RequireStmt *>(node));
        case IF_STMT:
            return generate_if_stmt(dynamic_cast<IfStmt *>(node));
        case ELIF_STMT:
            return generate_elif_stmt(dynamic_cast<ElifStmt *>(node));
        case ELSE_STMT:
            return generate_else_stmt(dynamic_cast<ElseStmt *>(node));
        case RETURN_STMT:
            return generate_return_stmt(dynamic_cast<ReturnStmt *>(node));
        case UNARY_EXPR:
            return generate_unary_expr(dynamic_cast<UnaryExpr *>(node));
        case VAR_DECL:
            return generate_var_decl(dynamic_cast<VarDecl *>(node));
        case SCOPE:
            return generate_scope(dynamic_cast<ScopeNode *>(node));
        case BREAK_STMT:
            return generate_break_stmt(dynamic_cast<BreakStmt *>(node));
        case NAME:
            return {new Instruction(OP_LOAD_VAR, new RonaObject(dynamic_cast<Name *>(node)->value))};
        case ALIAS_DECL:
            return generate_alias_decl(dynamic_cast<AliasDecl *>(node));
        default:
            return InstructionVec();
    }
}

/******************************************************************************
 * @brief Generate the instructions for a TryBlock Ast node.
 * @param node
 * @return InstructionVec
 */
InstructionVec CodeGenerator::generate_try_block(TryBlock *node) {
//    InstructionVec out;
//    InstructionVec try_scope = generate_scope(node->scope);
//    InstructionVec catch_scope = generate_scope(node->catch_block->scope);
//    out.emplace_back(new Instruction(
//            OP_TRY_CONTEXT,
//            new RonaObject(CONTEXT_CREATE_FLAG),
//            new RonaObject(static_cast<long>(try_scope.size()))
//            ));
    return InstructionVec();
}

/******************************************************************************
 * @brief Generate the instructions for each of the modules.
 */
void CodeGenerator::generate_modules(Ast *ast) {
    for (auto &[key, module] : ast->modules) {
        auto code_block = new ModuleCodeBlock;
        code_block->instructions = generate_scope(module->scope);
        code_block->i_cnt = code_block->instructions.size();
        this->modules[key] = code_block;
    }
}

/******************************************************************************
 * @brief
 * @param node
 * @return
 */
InstructionVec CodeGenerator::generate_alias_decl(AliasDecl *node) {
    return {new Instruction(
            OP_MAKE_ALIAS,
            new RonaObject(node->alias_name->value),
            new RonaObject(node->base_name->value)
    )};
}

/******************************************************************************
 * @brief Generate the instructions for an Ast instance.
 * @param ast
 * @return InstructionVec
 */
std::vector<Instruction *> CodeGenerator::generate(Ast *ast) {
    generate_modules(ast);
    this->instructions = generate_scope(ast->root);

    for (auto &[key, code_block] : this->modules) {
        code_block->i_start = this->instructions.size();
        this->instructions.insert(
                this->instructions.begin(),
                code_block->instructions.begin(),
                code_block->instructions.end()
        );
        code_block->make_instruction->operands.push_back(new RonaObject(code_block->i_start));
        code_block->make_instruction->operands.push_back(new RonaObject(code_block->i_cnt));
    }

    return this->instructions;
}

/******************************************************************************
 * @brief Dump the instruction vector to a string format.
 * @return InstructionVec
 */
std::string CodeGenerator::dumps() {
    std::string output;

    for (auto &instruction : this->instructions) {
        output += instruction->to_string() + "\n";
    }

    return output;
}



