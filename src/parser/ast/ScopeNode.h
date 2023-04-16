/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_SCOPE_NODE_H
#define RONASCRIPT_SCOPE_NODE_H

#include <unordered_set>
#include <vector>
#include "../RnAstSymbolTable.h"
#include "AstNode.h"
#include "NodeType.h"

class ClassDecl;
class FuncDecl;
class VarDecl;

class ScopeNode : public AstNode {
public:
    ScopeNode();
    ~ScopeNode();
    std::string ToString(bool nl) override;
    void AddSubTree(const std::shared_ptr<AstNode>& subtree, bool hoist = false);
    void AddClassDecl(const std::shared_ptr<ClassDecl>& class_decl);
    void AddVarDecl(const std::shared_ptr<VarDecl>& var_decl);
    void AddFuncDecl(const std::shared_ptr<FuncDecl>& func_decl);
    void AddLiteral(const std::string& name, const std::shared_ptr<AstNode>& node);
    std::shared_ptr<AstNode> GetLiteral(const std::string& name);

    std::vector<std::shared_ptr<AstNode>> children;  // All other scope children
    std::shared_ptr<ScopeNode> parent = nullptr;
    std::shared_ptr<RnAstSymbolTable> symbol_table = nullptr;
    std::unordered_map<std::string, std::string> pragma_table;
    std::map<std::string, std::shared_ptr<AstNode>> _literal_map;
};

#endif  //RONASCRIPT_SCOPE_NODE_H
