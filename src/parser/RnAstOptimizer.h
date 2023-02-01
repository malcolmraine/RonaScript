/*****************************************************************************
* File: RnAstOptimizer.h
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNASTOPTIMIZER_H
#define RONASCRIPT_RNASTOPTIMIZER_H

class Ast;
class AstNode;
class FuncDecl;
class BinaryExpr;

class RnAstOptimizer
{
 public:
	explicit RnAstOptimizer(Ast* ast);
	~RnAstOptimizer();
	void Optimize();
	void TailCallOpt(FuncDecl* node);
	AstNode* SimplifyBinaryExpr(
		BinaryExpr* node);  // order of execution, duplicate operations, etc. Should return a BinaryExpr
	AstNode* FoldConstants(
		BinaryExpr* node); // Compile time arithmetic, should return a literal

 protected:
	Ast* ast = nullptr;
};

#endif //RONASCRIPT_RNASTOPTIMIZER_H