/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_AST_H
#define RONASCRIPT_AST_H

#include "../../util/FileInfo.h"
#include <map>

class Module;

class ScopeNode;

class Ast
{
 public:
	Ast();
	~Ast();
	std::map<std::string, std::shared_ptr<Module>> modules;
	std::shared_ptr<ScopeNode> root = nullptr;
	FileInfo* file_info{};
};

#endif //RONASCRIPT_AST_H
