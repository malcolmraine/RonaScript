/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "WhileLoop.h"
#include "ScopeNode.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
WhileLoop::WhileLoop()
{
	node_type = AST_WHILE_LOOP;
}

/*****************************************************************************/
WhileLoop::~WhileLoop() = default;

/*****************************************************************************/
std::string WhileLoop::ToString(bool nl)
{
	std::string s = MakeTabStr() + "WhileLoop( )";
	if (nl)
	{
		s += "\n";
	}
	test->nest_lvl = nest_lvl + 1;
	scope->nest_lvl = nest_lvl + 1;
	s += test->ToString(true);
	s += scope->ToString(true);

	return s;
}
