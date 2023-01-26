/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "AstNode.h"
#include "../../codegen/RnCodeGenVisitor.h"

#define TAB_CHAR "  "

/*****************************************************************************/
std::string AstNode::ToString(bool nl)
{
	return nl ? "\n" : "";
}

/*****************************************************************************/
std::string AstNode::MakeTabStr()
{
	std::string outp_str;

	for (int i = 0; i < nest_lvl; ++i)
	{
		outp_str.append("|");
		outp_str.append(TAB_CHAR);
	}

	return outp_str;
}

