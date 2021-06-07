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

#include "ScopeNode.h"

/******************************************************************************
 * @brief
 */
ScopeNode::ScopeNode() {
    this->node_type = SCOPE;
}

/******************************************************************************
 * @brief
 */
ScopeNode::~ScopeNode() {
    for (auto &node : this->var_decls) {
        delete node;
    }
    for (auto &node : this->class_decls) {
        delete node;
    }
    for (auto &node : this->func_decls) {
        delete node;
    }

    for (auto &node : this->children) {
        delete node;
    }
}

/******************************************************************************
 * @brief
 * @return
 */
std::string ScopeNode::to_string(bool nl) {
    std::string output = make_tab_str() + "Scope( )\n";

    for (auto &child : this->class_decls) {
        child->nest_lvl = this->nest_lvl + 1;
        output += child->to_string();
    }

    for (auto &child : this->var_decls) {
        child->nest_lvl = this->nest_lvl + 1;
        output += child->to_string();
    }

    for (auto &child : this->func_decls) {
        child->nest_lvl = this->nest_lvl + 1;
        output += child->to_string();
    }

    for (auto &child : this->children) {
        child->nest_lvl = this->nest_lvl + 1;
        output += child->to_string();
    }

    return output;
}

/******************************************************************************
 * @brief
 * @param subtree
 * @param hoist
 */
void ScopeNode::add_subtree(AstNode *subtree, bool hoist) {
    subtree->nest_lvl = this->nest_lvl + 1;

    if (hoist) {
        this->children.insert(this->children.begin(), subtree);
    } else {
        this->children.emplace_back(subtree);
    }
}

/******************************************************************************
 * @brief
 * @param class_decl
 */
void ScopeNode::add_class_decl(ClassDecl *class_decl) {
    class_decl->nest_lvl = this->nest_lvl + 1;
    this->class_decls.emplace_back(class_decl);
}

/******************************************************************************
 * @brief
 * @param var_decl
 */
void ScopeNode::add_var_decl(VarDecl *var_decl) {
    var_decl->nest_lvl = this->nest_lvl + 1;
    this->var_decls.emplace_back(var_decl);

}

/******************************************************************************
 * @brief
 * @param func_decl
 */
void ScopeNode::add_func_decl(FuncDecl *func_decl) {
    func_decl->nest_lvl = this->nest_lvl + 1;
    this->func_decls.emplace_back(func_decl);
}
