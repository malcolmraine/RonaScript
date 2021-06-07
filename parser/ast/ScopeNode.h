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

#ifndef RONASCRIPT_SCOPE_NODE_H
#define RONASCRIPT_SCOPE_NODE_H

#include <vector>
#include "NodeType.h"
#include "AstNode.h"
#include "ClassDecl.h"
#include "VarDecl.h"
#include "FuncDecl.h"

class ClassDecl;

class FuncDecl;

class ScopeNode : public AstNode {
public:
    ScopeNode();
    ~ScopeNode();
    std::string to_string(bool nl = true) override;
    void add_subtree(AstNode *subtree, bool hoist = false);
    void add_class_decl(ClassDecl *class_decl);
    void add_var_decl(VarDecl *var_decl);
    void add_func_decl(FuncDecl *func_decl);

    std::vector<AstNode *> children;        // All other scope children
    std::vector<VarDecl *> var_decls;       // Variable declarations
    std::vector<ClassDecl *> class_decls;   // Class declarations
    std::vector<FuncDecl *> func_decls;     // Function declarations
    ScopeNode *parent = nullptr;
};


#endif //RONASCRIPT_SCOPE_NODE_H
