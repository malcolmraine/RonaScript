/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
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

#include "VarDecl.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
VarDecl::VarDecl() : type(std::make_shared<RnTypeComposite>(RnType::RN_NULL)) {
    node_type = AST_VAR_DECL;
}

/*****************************************************************************/
VarDecl::~VarDecl() = default;

/*****************************************************************************/
std::string VarDecl::ToString(bool nl) {
    std::string s = MakeTabStr();

    if (is_const) {
        s.append("ConstDecl");
    } else if (is_local) {
        s.append("LocalDecl");
    } else if (is_global) {
        s.append("GlobalDecl");
    } else if (is_literal) {
        s.append("LiteralDecl");
    } else {
        s.append("VarDecl");
    }

    s.append("( ").append(id).append(", ").append(type->ToString());

    for (auto& qualifier : qualifiers) {
        s.append(", ").append(qualifier->lexeme);
    }

    s.append(" )");
    if (nl) {
        s.append("\n");
    }

    if (init_value != nullptr) {
        init_value->nest_lvl = nest_lvl + 1;
        s.append(MakeTabStr()).append("AssignmentStmt( ").append(id).append(" )");
        if (nl) {
            s.append("\n");
        }
        s.append(init_value->ToString(nl));
    }

    return s;
}
