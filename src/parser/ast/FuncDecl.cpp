/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "FuncDecl.h"
#include "ScopeNode.h"
#include "ArgDecl.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
FuncDecl::FuncDecl()
{
	node_type = AST_FUNC_DECL;
}

/*****************************************************************************/
FuncDecl::~FuncDecl()
{
	scope.reset();
}

/*****************************************************************************/
std::string FuncDecl::ToString(bool nl)
{
	std::string s = MakeTabStr() + "FuncDecl( " + id + ", " + type + " )";
	if (nl)
	{
		s += "\n";
	}

	for (auto& arg : args)
	{
		arg->nest_lvl = nest_lvl + 1;
		s += arg->ToString(true);
	}

	if (scope != nullptr)
	{
		scope->nest_lvl = nest_lvl + 1;
		s += scope->ToString(true);
	}

	return s;
}

