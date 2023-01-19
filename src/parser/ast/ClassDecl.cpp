/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "ClassDecl.h"
#include "Name.h"
#include "ScopeNode.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
ClassDecl::ClassDecl()
{
	node_type = AST_CLASS_DECL;
}

/*****************************************************************************/
ClassDecl::~ClassDecl() = default;

/*****************************************************************************/
std::string ClassDecl::ToString(bool nl)
{
	std::string s = MakeTabStr() + "ClassDecl( " + id + ", inherits [ ";

	for (auto& parent : parent_classes)
	{
		s += parent->value + " ";
	}
	s += "] )";
	if (nl)
	{
		s += "\n";
	}

	scope->nest_lvl = nest_lvl + 1;
	s += scope->ToString(nl);

	return s;
}

