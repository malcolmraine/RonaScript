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

class ScopeNode;
class FileInfo;

class Ast {
public:
    Ast();
    ~Ast();
    AstNodePtr<ScopeNode> root = nullptr;
    FileInfo* file_info{};
};
