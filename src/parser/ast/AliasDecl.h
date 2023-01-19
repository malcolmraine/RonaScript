/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_ALIASDECL_H
#define RONASCRIPT_ALIASDECL_H

#include <string>
#include "AstNode.h"

class Name;

enum AliasType
{
	NAME_ALIAS, TYPE_ALIAS
};

class AliasDecl : public AstNode
{
 public:
	AliasDecl();
	~AliasDecl() override;
	std::string ToString(bool nl) override;
	std::shared_ptr<Name> base_name = nullptr;
	std::shared_ptr<Name> alias_name = nullptr;
	AliasType alias_type;
};

#endif //RONASCRIPT_ALIASDECL_H