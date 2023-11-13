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

#include "FuncDecl.h"
#include "../../codegen/RnCodeGenVisitor.h"
#include "ArgDecl.h"
#include "ScopeNode.h"

/*****************************************************************************/
FuncDecl::FuncDecl() {
    node_type = AST_FUNC_DECL;
}

/*****************************************************************************/
FuncDecl::~FuncDecl() {
    scope.reset();
}

/*****************************************************************************/
std::string FuncDecl::ToString(bool nl) {
    std::string s = MakeTabStr() + "FuncDecl( " + id + ", " + type->ToString() + " )";
    if (nl) {
        s += "\n";
    }

    for (auto& arg : args) {
        arg->nest_lvl = nest_lvl + 1;
        s += arg->ToString(true);
    }

    if (scope != nullptr) {
        scope->nest_lvl = nest_lvl + 1;
        s += scope->ToString(true);
    }

    return s;
}
