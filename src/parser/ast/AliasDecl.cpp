/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "AliasDecl.h"
#include "Name.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
AliasDecl::AliasDecl()
{
	node_type = AST_ALIAS_DECL;
}

/*****************************************************************************/
AliasDecl::~AliasDecl() = default;

/*****************************************************************************/
std::string AliasDecl::ToString(bool nl)
{
	std::string s =
		MakeTabStr() + "AliasDecl( " + alias_name->value + " -> " + base_name->value;
	s += ", ";
	s += (alias_type == NAME_ALIAS ? "NAME_ALIAS" : "TYPE_ALIAS");
	s += " )";

	if (nl)
	{
		s += "\n";
	}

	return s;
}
