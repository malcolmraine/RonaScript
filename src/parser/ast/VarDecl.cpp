/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "VarDecl.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
VarDecl::VarDecl() : type(RnTypeComposite(RnType::RN_NULL))
{
	node_type = AST_VAR_DECL;
}

/*****************************************************************************/
VarDecl::~VarDecl()
{
}

/*****************************************************************************/
std::string VarDecl::ToString(bool nl)
{
	std::string s = MakeTabStr();

	if (is_const)
	{
		s += "Const";
	}
	else if (is_local)
	{
		s += "LocalVarDecl";
	}
	else if (is_global)
	{
		s += "GlobalVarDecl";
	}
	else
	{
		s += "VarDecl";
	}

	s += "( " + id + ", " + type.ToString();

	for (auto& qualifier : qualifiers)
	{
		s += ", " + qualifier->lexeme;
	}

	s += " )";
	if (nl)
	{
		s += "\n";
	}

	if (init_value != nullptr)
	{
		init_value->nest_lvl = nest_lvl + 1;
		s += MakeTabStr() + "AssignmentStmt( " + id + " )";
		if (nl)
		{
			s += "\n";
		}
		s += init_value->ToString(nl);
	}

	return s;
}

