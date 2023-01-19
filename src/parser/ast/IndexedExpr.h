/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_INDEXEDEXPR_H
#define RONASCRIPT_INDEXEDEXPR_H

#include <string>
#include "AstNode.h"
#include "Expr.h"

class IndexedExpr : public Expr
{
 public:
	IndexedExpr();
	~IndexedExpr();
	std::string ToString(bool nl) override;
	std::shared_ptr<AstNode> idx = nullptr;
	bool is_access = false; // For handling access vs assignment indexing

};

#endif //RONASCRIPT_INDEXEDEXPR_H
