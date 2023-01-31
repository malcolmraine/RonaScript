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

#include <vector>
#include "NodeType.h"
#include "AstNode.h"
#include "../RnAstSymbolTable.h"

class ClassDecl;
class FuncDecl;
class VarDecl;

class ScopeNode : public AstNode
{
 public:
	ScopeNode();
	~ScopeNode();
	std::string ToString(bool nl) override;
	void add_subtree(const std::shared_ptr<AstNode>& subtree, bool hoist = false);
	void add_class_decl(const std::shared_ptr<ClassDecl>& class_decl);
	void add_var_decl(const std::shared_ptr<VarDecl>& var_decl);
	void add_func_decl(const std::shared_ptr<FuncDecl>& func_decl);
	std::vector<std::shared_ptr<AstNode>> children;        // All other scope children
//	std::vector<std::shared_ptr<VarDecl>> var_decls;       // Variable declarations
//	std::vector<std::shared_ptr<ClassDecl>> class_decls;   // Class declarations
//	std::vector<std::shared_ptr<FuncDecl>> func_decls;     // Function declarations
	std::shared_ptr<ScopeNode> parent = nullptr;
	std::shared_ptr<RnAstSymbolTable> symbol_table = nullptr;
};

#endif //RONASCRIPT_SCOPE_NODE_H
