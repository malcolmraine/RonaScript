/*****************************************************************************
* File: RnAstValidator.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/26/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
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

#include "RnAstValidator.h"
#include "../parser/ast/AliasDecl.h"
#include "../parser/ast/ArgDecl.h"
#include "../parser/ast/ArrayLiteral.h"
#include "../parser/ast/AssignmentStmt.h"
#include "../parser/ast/AttributeAccess.h"
#include "../parser/ast/BinaryExpr.h"
#include "../parser/ast/CatchBlock.h"
#include "../parser/ast/ClassDecl.h"
#include "../parser/ast/ConditionalStmt.h"
#include "../parser/ast/DeleteStmt.h"
#include "../parser/ast/ExitStmt.h"
#include "../parser/ast/Expr.h"
#include "../parser/ast/FlowControl.h"
#include "../parser/ast/FuncCall.h"
#include "../parser/ast/FuncDecl.h"
#include "../parser/ast/ImportStmt.h"
#include "../parser/ast/IndexedExpr.h"
#include "../parser/ast/LiteralValue.h"
#include "../parser/ast/Loop.h"
#include "../parser/ast/ReturnStmt.h"
#include "../parser/ast/ScopeNode.h"
#include "../parser/ast/TryBlock.h"
#include "../parser/ast/UnaryExpr.h"
#include "../parser/ast/VarDecl.h"
#include "../util/log.h"

/*****************************************************************************/
void RnAstValidator::SymbolRedeclarationCheck(const std::string& symbol) {
    if (_current_scope->symbol_table->SymbolExists(symbol)) {
        throw std::runtime_error("Redeclaration of symbol '" + symbol + "'");
    }
}

/*****************************************************************************/
void RnAstValidator::SymbolExistsCheck(const std::string& symbol) {
    if (!_current_scope->symbol_table->SymbolExists(symbol)) {
        throw std::runtime_error("Unknown symbol '" + symbol + "'");
    }
}

/*****************************************************************************/
std::shared_ptr<RnTypeComposite> RnAstValidator::ResolveTypes(
    const std::shared_ptr<RnTypeComposite>& type1,
    const std::shared_ptr<RnTypeComposite>& type2) {
    auto type1_bounds = type1->GetFloatBounds();
    auto type2_bounds = type2->GetFloatBounds();
    auto lower_bound = std::min(type1_bounds.lower, type2_bounds.lower);
    auto upper_bound = std::max(type1_bounds.upper, type2_bounds.upper);
    auto resolved_type = std::make_shared<RnTypeComposite>(type1->GetType());
    resolved_type->SetBounds(lower_bound, upper_bound);
    return resolved_type;
}

/*****************************************************************************/
bool RnAstValidator::CanAssignTypeTo(
    const std::shared_ptr<RnTypeComposite>& destination,
    const std::shared_ptr<RnTypeComposite>& source, ASSIGNMENT_TYPE assignment_type) {

    std::map<RnType::Type, std::unordered_set<RnType::Type>> compatibility_groups = {
        {RnType::RN_INT, {RnType::RN_INT, RnType::RN_BOOLEAN, RnType::RN_FLOAT}},
        {RnType::RN_FLOAT, {RnType::RN_FLOAT, RnType::RN_BOOLEAN, RnType::RN_INT}},
        {RnType::RN_STRING, {RnType::RN_STRING}},
        {RnType::RN_BOOLEAN,
         {RnType::RN_FLOAT, RnType::RN_BOOLEAN, RnType::RN_INT, RnType::RN_STRING}},
        {RnType::RN_OBJECT, {RnType::RN_OBJECT, RnType::RN_CLASS_INSTANCE}},
        {RnType::RN_FUNCTION, {RnType::RN_FUNCTION}},
        {RnType::RN_CLASS_INSTANCE, {RnType::RN_OBJECT, RnType::RN_CLASS_INSTANCE}},
        {RnType::RN_ARRAY, {RnType::RN_ARRAY}}};

    bool bounds_ok = true;
    bool types_ok = true;

    std::string action_msg;
    if (assignment_type == ASSIGNMENT_TYPE::ASSIGNMENT_VALUE) {
        action_msg = "Trying to assign ";
    } else {
        action_msg = "Trying to return ";
    }

    if (!source->IsWithinRange(*destination)) {
        if (_current_scope->pragma_table["bounds"] == "strict") {
            bounds_ok = false;
            throw std::runtime_error(action_msg + "out of bounds value");
        } else if (_current_scope->pragma_table["bounds"] == "warn") {
            Log::WARN("Warning: " + action_msg + "out of bounds value");
            bounds_ok = true;
        } else {
            bounds_ok = true;
        }
    }

    types_ok = destination->GetType() == source->GetType();

    if (_current_scope->pragma_table["typing"] == "strict") {
        if (!types_ok) {
            throw std::runtime_error(action_msg + "incompatible type");
        }
    } else {
        if (compatibility_groups[destination->GetType()].contains(source->GetType())) {
            if (!types_ok && _current_scope->pragma_table["typing"] == "warn") {
                Log::WARN("Warning: " + action_msg + "incompatible type");
            }
            types_ok = true;
        } else {
            if (!types_ok) {
                throw std::runtime_error(action_msg + "incompatible type " +
                                         source->ToString() + " to " +
                                         destination->ToString());
            }
        }
    }

    return types_ok && bounds_ok;
}

/*****************************************************************************/
std::shared_ptr<RnTypeComposite> RnAstValidator::EvaluateSubtreeType(
    const std::shared_ptr<AstNode>& subtree) {
    switch (subtree->node_type) {
        case AST_BINARY_EXPR: {
            auto node = std::dynamic_pointer_cast<BinaryExpr>(subtree);
            if (node->_op == "->") {
                //                if (node->_left->node_type == AST_NAME) {
                //                    auto previous_scope = _current_scope;
                //                    auto name_node = std::dynamic_pointer_cast<Name>(node->_left);
                //                    _current_scope = std::dynamic_pointer_cast<ClassDecl>(
                //                                         _current_scope->symbol_table
                //                                             ->GetSymbolEntry(name_node->value)
                //                                             ->GetTypeDeclNode())
                //                                         ->scope.get();
                //                    auto subtree_type = EvaluateSubtreeType(name_node);
                //                    _current_scope = previous_scope;
                //                    return subtree_type;
                //                }
                return EvaluateSubtreeType(node->_right);
            }
            return ResolveTypes(EvaluateSubtreeType(node->_left),
                                EvaluateSubtreeType(node->_right));
        }
        case AST_INDEXED_EXPR:
            // TODO: Evaluate type information for indexed expressions
            return std::make_shared<RnTypeComposite>(RnType::RN_NULL);
        case AST_FUNC_CALL: {
            auto node = std::dynamic_pointer_cast<FuncCall>(subtree);
            return EvaluateSubtreeType(node->expr);
        }
        case AST_LIST_LITERAL: {
            auto node = std::dynamic_pointer_cast<ArrayLiteral>(subtree);
            auto type = std::make_shared<RnTypeComposite>(RnType::RN_ARRAY);
            auto value = static_cast<RnIntNative>(node->GetChildren().size());
            type->SetBounds(value, value);
            return type;
        }
        case AST_STRING_LITERAL: {
            auto node = std::dynamic_pointer_cast<LiteralValue>(subtree);
            auto type = std::make_shared<RnTypeComposite>(RnType::RN_STRING);
            auto value =
                static_cast<RnIntNative>(std::get<RnStringNative>(node->data).length());
            type->SetBounds(value, value);
            return type;
        }
        case AST_BOOL_LITERAL: {
            auto node = std::dynamic_pointer_cast<LiteralValue>(subtree);
            auto type = std::make_shared<RnTypeComposite>(RnType::RN_BOOLEAN);
            auto value =
                static_cast<RnIntNative>(std::get<RnBoolNative>(node->data) ? 1 : 0);
            type->SetBounds(value, value);
            return type;
        }
        case AST_FLOAT_LITERAL: {
            auto node = std::dynamic_pointer_cast<LiteralValue>(subtree);
            auto type = std::make_shared<RnTypeComposite>(RnType::RN_FLOAT);
            type->SetBounds(std::get<RnFloatNative>(node->data),
                            std::get<RnFloatNative>(node->data));
            return type;
        }
        case AST_INT_LITERAL: {
            auto node = std::dynamic_pointer_cast<LiteralValue>(subtree);
            auto type = std::make_shared<RnTypeComposite>(RnType::RN_INT);
            type->SetBounds(std::get<RnIntNative>(node->data),
                            std::get<RnIntNative>(node->data));
            return type;
        }
        case AST_RETURN_STMT: {
            auto node = std::dynamic_pointer_cast<ReturnStmt>(subtree);
            return EvaluateSubtreeType(node->expr);
        }
        case AST_UNARY_EXPR: {
            auto node = std::dynamic_pointer_cast<UnaryExpr>(subtree);
            return EvaluateSubtreeType(node->expr);
        }
        case AST_NAME: {
            auto node = std::dynamic_pointer_cast<Name>(subtree);
            return _current_scope->symbol_table->GetSymbolEntry(node->value)->GetType();
        }
        default:
            throw std::runtime_error("Could not evaluate subtree");
    }
}

/*****************************************************************************/
bool RnAstValidator::GeneralVisit(AstNode* node) {
    if (!node) {
        return false;
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
        case AST_STRING_LITERAL:
        case AST_BOOL_LITERAL:
        case AST_FLOAT_LITERAL:
        case AST_INT_LITERAL:
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
bool RnAstValidator::GeneralVisit(const std::shared_ptr<AstNode>& node) {
    if (node) {
        return GeneralVisit(node.get());
    } else {
        return {};
    }
}

/*****************************************************************************/
bool RnAstValidator::Visit(LiteralValue* node) {
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(ArrayLiteral* node) {
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(ScopeNode* node) {
    _current_scope = node;

    for (const auto& child : node->children) {
        GeneralVisit(child);
    }
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(Loop* node) {
    GeneralVisit(node->scope);
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(ImportStmt* node) {
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(Module* node) {
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(FuncDecl* node) {
    SymbolRedeclarationCheck(node->id);
    _current_scope->symbol_table->AddSymbol(node->id, node->type);

    for (auto arg : node->args) {
        node->scope->symbol_table->AddSymbol(arg->GetChild<Name>(0)->value, arg->GetType());
    }
    _current_type_reference = node->type;
    GeneralVisit(node->scope);
    _current_type_reference = nullptr;
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(FuncCall* node) {
    if (node->expr->node_type == AST_NAME) {
        SymbolExistsCheck(std::dynamic_pointer_cast<Name>(node->expr)->value);
    }
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(VarDecl* node) {
    SymbolRedeclarationCheck(node->id);
    _current_scope->symbol_table->AddSymbol(node->id, node->type);

    if (node->init_value) {
        if (!CanAssignTypeTo(node->type, EvaluateSubtreeType(node->init_value),
                             ASSIGNMENT_VALUE)) {
            throw std::runtime_error("Type error.");
        }
    }
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(Name* node) {
    SymbolRedeclarationCheck(node->value);
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(ClassDecl* node) {
    SymbolRedeclarationCheck(node->id);
    _current_scope->symbol_table->AddSymbol(
        node->id, std::make_shared<RnTypeComposite>(RnType::RN_OBJECT));
    GeneralVisit(node->scope);
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(ExitStmt* node) {
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(ReturnStmt* node) {
    CanAssignTypeTo(_current_type_reference, EvaluateSubtreeType(node->expr),
                    RETURN_VALUE);
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(AttributeAccess* node) {
    SymbolExistsCheck(std::dynamic_pointer_cast<Name>(node->GetName())->value);
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(TryBlock* node) {
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(CatchBlock* node) {
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(ConditionalStmt* node) {
    if (node->consequent)
        GeneralVisit(node->consequent);
    if (node->alternative)
        GeneralVisit(node->alternative);
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(DeleteStmt* node) {
    if (node->GetName()->node_type == AST_NAME) {
        SymbolExistsCheck(std::dynamic_pointer_cast<Name>(node->GetName())->value);
    }
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(UnaryExpr* node) {
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(Expr* node) {
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(AliasDecl* node) {
    SymbolRedeclarationCheck(node->GetChild<Name>(1)->value);
    _current_scope->symbol_table->AddSymbol(
        node->GetChild<Name>(1)->value,
        _current_scope->symbol_table->GetSymbolEntry(node->GetChild<Name>(0)->value)
            ->GetType());
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(ArgDecl* node) {
    // Enforce bounds requirements here
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(AssignmentStmt* node) {
    if (!CanAssignTypeTo(EvaluateSubtreeType(node->GetLexpr()),
                         EvaluateSubtreeType(node->GetRexpr()), ASSIGNMENT_VALUE)) {
        throw std::runtime_error("Type error.");
    }
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(BinaryExpr* node) {
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(IndexedExpr* node) {
    return true;
}

/*****************************************************************************/
bool RnAstValidator::Visit(FlowControl* node) {
    return true;
}

/*****************************************************************************/
void RnAstValidator::Run() {
    GeneralVisit(_input->root);
}