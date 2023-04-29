/*****************************************************************************
* File: RnCodeGenVisitor.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/26/22
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

#include "RnCodeGenVisitor.h"
#include "../parser/ast/AliasDecl.h"
#include "../parser/ast/ArgDecl.h"
#include "../parser/ast/ArrayLiteral.h"
#include "../parser/ast/AssignmentStmt.h"
#include "../parser/ast/AttributeAccess.h"
#include "../parser/ast/BinaryExpr.h"
#include "../parser/ast/LiteralValue.h"
#include "../parser/ast/FlowControl.h"
#include "../parser/ast/CatchBlock.h"
#include "../parser/ast/ClassDecl.h"
#include "../parser/ast/DeleteStmt.h"
#include "../parser/ast/ExitStmt.h"
#include "../parser/ast/Expr.h"
#include "../parser/ast/FuncCall.h"
#include "../parser/ast/FuncDecl.h"
#include "../parser/ast/ImportStmt.h"
#include "../parser/ast/IndexedExpr.h"
#include "../parser/ast/ReturnStmt.h"
#include "../parser/ast/TryBlock.h"
#include "../parser/ast/UnaryExpr.h"
#include "../parser/ast/VarDecl.h"
#include "../parser/ast/Loop.h"
#include "../parser/ast/ConditionalStmt.h"
#include "../vm/RnObject.h"

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::GeneralVisit(AstNode* node) {
    if (!node) {
        return {};
    }

    switch (node->node_type) {
        case AST_ASSIGNMENT_STMT:
            return Visit(dynamic_cast<AssignmentStmt*>(node));
        case AST_BINARY_EXPR:
            return Visit(dynamic_cast<BinaryExpr*>(node));
        case AST_INDEXED_EXPR:
            return Visit(dynamic_cast<IndexedExpr*>(node));
        case AST_WHILE_LOOP:
        case AST_FOR_LOOP:
            return Visit(dynamic_cast<Loop*>(node));
        case AST_CLASS_DECL:
            return Visit(dynamic_cast<ClassDecl*>(node));
        case AST_EXPR:
            return Visit(dynamic_cast<Expr*>(node));
        case AST_CONTINUE_STMT:
            return Visit(dynamic_cast<FlowControl*>(node));
        case AST_FUNC_CALL:
            return Visit(dynamic_cast<FuncCall*>(node));
        case AST_FUNC_DECL:
            return Visit(dynamic_cast<FuncDecl*>(node));
        case AST_LIST_LITERAL:
            return Visit(dynamic_cast<ArrayLiteral*>(node));
        case AST_INT_LITERAL:
        case AST_STRING_LITERAL:
        case AST_BOOL_LITERAL:
        case AST_FLOAT_LITERAL:
            return Visit(dynamic_cast<LiteralValue*>(node));
        case AST_IMPORT:
            return Visit(dynamic_cast<ImportStmt*>(node));
        case AST_IF_STMT:
        case AST_ELIF_STMT:
        case AST_ELSE_STMT:
            return Visit(dynamic_cast<ConditionalStmt*>(node));
        case AST_ARG_DECL:
            return Visit(dynamic_cast<ArgDecl*>(node));
        case AST_ALIAS_DECL:
            return Visit(dynamic_cast<AliasDecl*>(node));
        case AST_RETURN_STMT:
            return Visit(dynamic_cast<ReturnStmt*>(node));
        case AST_UNARY_EXPR:
            return Visit(dynamic_cast<UnaryExpr*>(node));
        case AST_VAR_DECL:
            return Visit(dynamic_cast<VarDecl*>(node));
        case AST_SCOPE:
            return Visit(dynamic_cast<ScopeNode*>(node));
        case AST_BREAK_STMT:
            return Visit(dynamic_cast<FlowControl*>(node));
        case AST_MODULE:
            return Visit(dynamic_cast<Module*>(node));
        case AST_EXIT_STMT:
            return Visit(dynamic_cast<ExitStmt*>(node));
        case AST_DELETE_STMT:
            return Visit(dynamic_cast<DeleteStmt*>(node));
        case AST_TRY_BLOCK:
            return Visit(dynamic_cast<TryBlock*>(node));
        case AST_CATCH_BLOCK:
            return Visit(dynamic_cast<CatchBlock*>(node));
        case AST_NAME:
            return Visit(dynamic_cast<Name*>(node));
        default:
            return {};
    }
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::GeneralVisit(const std::shared_ptr<AstNode>& node) {
    if (node) {
        return GeneralVisit(node.get());
    } else {
        return {};
    }
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(LiteralValue* node) {
    switch (node->node_type) {
        case AST_STRING_LITERAL:
            return {new RnInstruction(
                OP_LOAD_LITERAL,
                RnConstStore::InternValue(std::get<RnStringNative>(node->data)))};
        case AST_BOOL_LITERAL:
            return {new RnInstruction(
                OP_LOAD_LITERAL,
                RnConstStore::InternValue(std::get<RnBoolNative>(node->data)))};
        case AST_INT_LITERAL:
            return {new RnInstruction(
                OP_LOAD_LITERAL,
                RnConstStore::InternValue(std::get<RnIntNative>(node->data)))};
        case AST_FLOAT_LITERAL:
            return {new RnInstruction(
                OP_LOAD_LITERAL,
                RnConstStore::InternValue(std::get<RnFloatNative>(node->data)))};
        default:
            throw std::runtime_error("Invalid literal type");
    }
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(ArrayLiteral* node) {
    InstructionBlock instructions;

    for (auto& item : node->items) {
        auto instruction_block = GeneralVisit(item);
        instructions.insert(instructions.begin(), instruction_block.begin(),
                            instruction_block.end());
    }
    instructions.push_back(new RnInstruction(OP_MAKE_ARRAY, node->items.size()));
    return instructions;
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(ScopeNode* node) {
    InstructionBlock instructions;
    for (auto& child : node->children) {
        InstructionBlock instruction_block = GeneralVisit(child);
        instructions.insert(instructions.end(), instruction_block.begin(),
                            instruction_block.end());
    }
    return instructions;
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(Loop* node) {
    _break_instructions.emplace_back();
    _continue_instructions.emplace_back();
    InstructionBlock instructions;
    InstructionBlock init = GeneralVisit(node->init);
    InstructionBlock test = GeneralVisit(node->test);
    InstructionBlock update = GeneralVisit(node->update);
    InstructionBlock scope = GeneralVisit(node->scope);
    test.push_back(new RnInstruction(OP_JUMPF_IF, scope.size() + update.size() + 2));
    scope.insert(scope.begin(), test.begin(), test.end());
    scope.insert(scope.begin(), new RnInstruction(OP_CREATE_CONTEXT));
    scope.push_back(new RnInstruction(OP_RESET_CONTEXT));
    instructions.reserve(scope.size() + test.size() + init.size() + update.size());

    if (!init.empty()) {
        instructions.insert(instructions.end(), init.begin(), init.end());
    }
    instructions.insert(instructions.end(), scope.begin(), scope.end());

    if (!update.empty()) {
        instructions.insert(instructions.end(), update.begin(), update.end());
    }
    instructions.emplace_back(
        new RnInstruction(OP_JUMPB, (scope.size() + update.size()) - 1));
    instructions.push_back(new RnInstruction(OP_DESTROY_CONTEXT));
    WrapContext(instructions);

    // Fill in jump offsets for continue and break statements
    auto break_instructions = _break_instructions.back();
    if (!break_instructions.empty()) {
        for (size_t i = 0; i < instructions.size(); i++) {
            auto it = std::find(break_instructions.begin(), break_instructions.end(),
                                instructions[i]);
            if (it != break_instructions.end()) {
                (*it)->SetArg1((instructions.size() - 2) - i);
            }
        }
    }
    _break_instructions.pop_back();

    auto continue_instructions = _continue_instructions.back();
    if (!continue_instructions.empty()) {
        for (size_t i = 0; i < instructions.size(); i++) {
            auto it = std::find(continue_instructions.begin(),
                                continue_instructions.end(), instructions[i]);
            if (it != continue_instructions.end()) {
                (*it)->SetArg1((instructions.size() - update.size() - 4) - i);
            }
        }
    }
    _continue_instructions.pop_back();
    return instructions;
}
//
///*****************************************************************************/
//InstructionBlock RnCodeGenVisitor::Visit(WhileLoop* node) {
//    InstructionBlock instructions;
//    InstructionBlock scope = GeneralVisit(node->scope);
//    scope.insert(scope.begin(), new RnInstruction(OP_CREATE_CONTEXT));
//    scope.push_back(new RnInstruction(OP_RESET_CONTEXT));
//    InstructionBlock test = GeneralVisit(node->test);
//    instructions.reserve(scope.size() + test.size());
//    instructions.insert(instructions.end(), test.begin(), test.end());
//    instructions.emplace_back(new RnInstruction(OP_JUMPF_IF, scope.size() + 1));
//    instructions.insert(instructions.end(), scope.begin(), scope.end());
//    instructions.emplace_back(
//        new RnInstruction(OP_JUMPB, scope.size() + test.size() + 1));
//
//    return instructions;
//}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(ImportStmt* node) {
    return {};
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(Module* node) {
    InstructionBlock instructions = GeneralVisit(node->scope);
    instructions.insert(
        instructions.begin(),
        new RnInstruction(OP_MAKE_MODULE, RnConstStore::InternValue(node->name->value),
                          instructions.size()));
    return instructions;
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(FuncDecl* node) {
    InstructionBlock instructions;
    InstructionBlock scope = GeneralVisit(node->scope);
    instructions.reserve(scope.size());
    auto make_instruction = new RnInstruction(
        OP_MAKE_FUNC, RnConstStore::InternValue(node->id),
        static_cast<long>(node->type->GetType()), static_cast<long>(scope.size()));
    instructions.emplace_back(make_instruction);

    for (auto& arg : node->args) {
        InstructionBlock arg_decl = GeneralVisit(arg);
        instructions.insert(instructions.end(), arg_decl.begin(), arg_decl.end());
    }

    instructions.insert(instructions.end(), scope.begin(), scope.end());
    return instructions;
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(FuncCall* node) {
    InstructionBlock instructions;
    InstructionBlock expr = GeneralVisit(node->expr);
    for (auto& arg : node->args) {
        InstructionBlock arg_instructions = GeneralVisit(arg);
        instructions.insert(instructions.end(), arg_instructions.begin(),
                            arg_instructions.end());
    }
    instructions.insert(instructions.end(), expr.begin(), expr.end());
    instructions.emplace_back(
        new RnInstruction(OP_CALL, static_cast<long>(node->args.size())));

    if (node->GetDiscardReturnValue()) {
        instructions.push_back(new RnInstruction(OP_POP));
    }

    return instructions;
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(VarDecl* node) {
    InstructionBlock instructions;
    auto interned_id = RnConstStore::InternValue(node->id);
    RnOpCode opcode = OP_MAKE_VAR;
    if (node->is_const) {
        opcode = OP_MAKE_CONST;
    } else if (node->is_local) {
        opcode = OP_MAKE_LOCAL;
    } else if (node->is_global) {
        opcode = OP_MAKE_GLOBAL;
    } else if (node->is_literal) {
        switch (node->init_value->node_type) {
            case AST_STRING_LITERAL:
                RnConstStore::InternValue(
                    std::get<RnStringNative >(std::dynamic_pointer_cast<LiteralValue>(node->init_value)->data));
                return {};
            case AST_BOOL_LITERAL:
                RnConstStore::InternValue(
                    std::get<RnBoolNative >(std::dynamic_pointer_cast<LiteralValue>(node->init_value)
                        ->data));
                return {};
            case AST_FLOAT_LITERAL:
                RnConstStore::InternValue(
                    std::get<RnFloatNative >(std::dynamic_pointer_cast<LiteralValue>(node->init_value)->data));
                return {};
            case AST_INT_LITERAL:
                RnConstStore::InternValue(
                    std::get<RnIntNative >(std::dynamic_pointer_cast<LiteralValue>(node->init_value)->data));
                return {};
            default:
                assert(false);
                return {};
        }
    }

    instructions.emplace_back(
        new RnInstruction(opcode, node->type->GetType(), interned_id));

    if (node->init_value) {
        InstructionBlock expr = GeneralVisit(node->init_value);
        instructions.insert(instructions.end(), expr.begin(), expr.end());
        instructions.push_back(new RnInstruction(OP_LOAD_VALUE, interned_id));
        instructions.push_back(new RnInstruction(OP_STORE));
    }
    return instructions;
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(Name* node) {
    return {new RnInstruction(OP_LOAD_VALUE, RnConstStore::InternValue(node->value))};
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(ClassDecl* node) {
    InstructionBlock instructions;
    InstructionBlock scope = GeneralVisit(node->scope);
    instructions.reserve(scope.size());
    instructions.emplace_back(new RnInstruction(
        OP_MAKE_CLASS, RnConstStore::InternValue(node->id), scope.size()));
    instructions.insert(instructions.end(), scope.begin(), scope.end());
    return instructions;
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(ExitStmt* node) {
    return {new RnInstruction(OP_EXIT, std::get<RnIntNative >(node->exit_code->data))};
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(ReturnStmt* node) {
    InstructionBlock instructions = GeneralVisit(node->expr);
    instructions.emplace_back(new RnInstruction(OP_RETURN));

    return instructions;
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(AttributeAccess* node) {
    return {};
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(TryBlock* node) {
    // TODO: Generate code for TryBlock
    return {};
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(CatchBlock* node) {
    // TODO: Generate code for CatchBlock
    return {};
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(ConditionalStmt* node) {
    InstructionBlock instructions;
    InstructionBlock test = GeneralVisit(node->test);
    InstructionBlock consequent = GeneralVisit(node->consequent);
    InstructionBlock alternative = GeneralVisit(node->alternative);

    WrapContext(consequent);
    if (node->node_type == AST_ELSE_STMT) {
        instructions.insert(instructions.end(), consequent.begin(), consequent.end());
        return instructions;
    }

    instructions.insert(instructions.end(), test.begin(), test.end());
    auto jumpf = new RnInstruction(OP_JUMPF_IF, consequent.size());
    instructions.push_back(jumpf);
    instructions.insert(instructions.end(), consequent.begin(), consequent.end());


    if (!alternative.empty()) {
        jumpf->SetArg1(jumpf->GetArg1() + 1);
        WrapContext(alternative);
        instructions.emplace_back(new RnInstruction(OP_JUMPF, alternative.size()));
        instructions.insert(instructions.end(), alternative.begin(), alternative.end());
    }
    instructions.push_back(new RnInstruction(OP_DESTROY_CONTEXT));

    return instructions;
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(DeleteStmt* node) {
    size_t internvalue =
        RnConstStore::InternValue(dynamic_pointer_cast<Name>(node->GetName())->value);

    return {new RnInstruction(OP_DELETE, internvalue)};
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(UnaryExpr* node) {

    if (node->op == "++") {
        return {new RnInstruction(
            OP_UNARY_INCREMENT,
            RnConstStore::InternValue(dynamic_pointer_cast<Name>(node->expr)->value))};
    } else if (node->op == "--") {
        return {new RnInstruction(
            OP_UNARY_DECREMENT,
            RnConstStore::InternValue(dynamic_pointer_cast<Name>(node->expr)->value))};
    } else {
        InstructionBlock instructions = GeneralVisit(node->expr);
        if (node->op == "-") {
            instructions.push_back(new RnInstruction(OP_UNARY_NEGATION));
        } else if (node->op == "~") {
            instructions.push_back(new RnInstruction(OP_UNARY_INVERT));
        } else if (node->op == "!") {
            instructions.push_back(new RnInstruction(OP_UNARY_NOT));
        }
        return instructions;
    }
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(Expr* node) {
    return GeneralVisit(node->expr);
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(AliasDecl* node) {
    return {new RnInstruction(OP_MAKE_ALIAS,
                              RnConstStore::InternValue(node->base_name->value),
                              RnConstStore::InternValue(node->alias_name->value))};
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(ArgDecl* node) {
    return {new RnInstruction(OP_MAKE_ARG, node->GetType()->GetType(),
                              RnConstStore::InternValue(node->GetId()->value))};
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(AssignmentStmt* node) {
    InstructionBlock instructions;
    InstructionBlock lvalue = GeneralVisit(node->GetLexpr());
    InstructionBlock rvalue = GeneralVisit(node->GetRexpr());
    instructions.insert(instructions.end(), rvalue.begin(), rvalue.end());
    instructions.insert(instructions.end(), lvalue.begin(), lvalue.end());
    instructions.push_back(new RnInstruction(OP_STORE));
    return instructions;
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(BinaryExpr* node) {
    InstructionBlock instructions;
    RnOpCode opcode = GetOpCodeFromOperator(node->_op);

    if (opcode == OP_RESOLVE_NAMESPACE) {
        instructions = GeneralVisit(node->_left);
        instructions.emplace_back(new RnInstruction(
            opcode, RnConstStore::InternValue(
                        std::static_pointer_cast<Name>(node->_right)->value)));
    } else if (opcode == OP_ATTR_ACCESS) {
        instructions = GeneralVisit(node->_left);
        instructions.push_back(new RnInstruction(
            opcode, RnConstStore::InternValue(
                        std::static_pointer_cast<Name>(node->_right)->value)));
    } else {
        InstructionBlock left = GeneralVisit(node->_left);
        InstructionBlock right = GeneralVisit(node->_right);
        instructions.insert(instructions.end(), left.begin(), left.end());
        instructions.insert(instructions.end(), right.begin(), right.end());
        instructions.emplace_back(new RnInstruction(opcode));
    }

    return instructions;
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(IndexedExpr* node) {
    InstructionBlock instructions;
    InstructionBlock expr = GeneralVisit(node->expr);
    InstructionBlock index = GeneralVisit(node->idx);
    instructions.insert(instructions.end(), expr.begin(), expr.end());
    instructions.insert(instructions.end(), index.begin(), index.end());
    instructions.emplace_back(new RnInstruction(OP_INDEX_ACCESS));
    return instructions;
}

/*****************************************************************************/
InstructionBlock RnCodeGenVisitor::Visit(FlowControl* node) {
    auto instruction = new RnInstruction(OP_JUMPF);

    if (node->node_type == AST_CONTINUE_STMT) {
        _continue_instructions.back().push_back(instruction);
    } else {
        _break_instructions.back().push_back(instruction);
    }
    return {instruction};
}

/*****************************************************************************/
void RnCodeGenVisitor::WrapContext(InstructionBlock& block) {
    block.insert(block.begin(), new RnInstruction(OP_CREATE_CONTEXT));
    block.push_back(new RnInstruction(OP_DESTROY_CONTEXT));
}
