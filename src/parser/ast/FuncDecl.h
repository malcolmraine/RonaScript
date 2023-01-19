/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_FUNCDECL_H
#define RONASCRIPT_FUNCDECL_H

#include <vector>
#include <string>
#include "AstNode.h"

class ScopeNode;

class ArgDecl;

class Token;

class FuncDecl : public AstNode
{
 public:
	FuncDecl();
	~FuncDecl();
	std::string ToString(bool nl) override;
	std::string id;
	std::vector<ArgDecl*> args;
	std::string type;
	std::vector<Token*> qualifiers;
	std::shared_ptr<ScopeNode> scope = nullptr;
};

#endif //RONASCRIPT_FUNCDECL_H
