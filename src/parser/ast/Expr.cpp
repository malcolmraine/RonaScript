/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "Expr.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
Expr::Expr()
{
	node_type = AST_EXPR;
}

/*****************************************************************************/
Expr::~Expr()
{
	expr.reset();
}

/*****************************************************************************/
std::string Expr::ToString(bool nl)
{
	expr->nest_lvl = nest_lvl;
	return expr->ToString(nl);
}

