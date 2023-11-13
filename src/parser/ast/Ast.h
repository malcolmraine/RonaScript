/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#pragma once

#include <map>
#include "AstNode.h"

class Module;
class ScopeNode;
class FileInfo;

class Ast {
public:
    Ast();
    ~Ast();
    std::map<std::string, AstNodePtr<Module>> modules;
    AstNodePtr<ScopeNode> root = nullptr;
    FileInfo* file_info{};
};
