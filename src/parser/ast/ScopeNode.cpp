/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "ScopeNode.h"
#include "../../codegen/RnCodeGenVisitor.h"
#include "ClassDecl.h"
#include "FuncDecl.h"
#include "VarDecl.h"

/*****************************************************************************/
ScopeNode::ScopeNode() {
    node_type = AST_SCOPE;
    symbol_table = std::make_shared<RnAstSymbolTable>();
}

/*****************************************************************************/
ScopeNode::~ScopeNode() {
    //	var_decls.clear();
    //	class_decls.clear();
    //	func_decls.clear();
    children.clear();
}

/*****************************************************************************/
std::string ScopeNode::ToString(bool nl) {
    std::string s = MakeTabStr() + "Scope( )";
    if (nl) {
        s += "\n";
    }

    for (auto& child : children) {
        child->nest_lvl = nest_lvl + 1;
        s += child->ToString(nl);
    }

    return s;
}

/*****************************************************************************/
void ScopeNode::AddSubTree(const std::shared_ptr<AstNode>& subtree, bool hoist) {
    subtree->nest_lvl = nest_lvl + 1;

    if (hoist) {
        children.insert(children.begin(), subtree);
    } else {
        children.emplace_back(subtree);
    }
}

/*****************************************************************************/
void ScopeNode::AddClassDecl(const std::shared_ptr<ClassDecl>& class_decl) {
    class_decl->nest_lvl = nest_lvl + 1;
    children.emplace_back(class_decl);
}

/*****************************************************************************/
void ScopeNode::AddVarDecl(const std::shared_ptr<VarDecl>& var_decl) {
    var_decl->nest_lvl = nest_lvl + 1;
    children.emplace_back(var_decl);
}

/*****************************************************************************/
void ScopeNode::AddFuncDecl(const std::shared_ptr<FuncDecl>& func_decl) {
    func_decl->nest_lvl = nest_lvl + 1;
    children.emplace_back(func_decl);
}

/*****************************************************************************/
void ScopeNode::AddLiteral(const std::string& name,
                           const std::shared_ptr<AstNode>& node) {
    _literal_map[name] = node;
}

/*****************************************************************************/
std::shared_ptr<AstNode> ScopeNode::GetLiteral(const std::string& name) {
    auto iter = _literal_map.find(name);
    if (iter != _literal_map.end())
        return iter->second;
    return nullptr;
}
