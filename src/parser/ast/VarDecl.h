/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_VARDECL_H
#define RONASCRIPT_VARDECL_H

#include <string>
#include <vector>
#include "AstNode.h"
#include "../RnTypeComposite.h"

class RnCodeGenVisitor;

class RnInstruction;

class Token;

class RnInstruction;

class VarDecl : public AstNode
{
 public:
	VarDecl();
	~VarDecl();
	std::string ToString(bool nl) override;
	std::string id;
	RnTypeComposite type;
	std::vector<Token*> qualifiers;
	std::shared_ptr<AstNode> init_value = nullptr;
	bool is_const = false;
	bool is_local = false;
	bool is_global = false;
};

#endif //RONASCRIPT_VARDECL_H
