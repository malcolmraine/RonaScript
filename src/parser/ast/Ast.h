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

class Module;
class ScopeNode;
class FileInfo;

class Ast {
public:
    Ast();
    ~Ast();
    std::map<std::string, std::shared_ptr<Module>> modules;
    std::shared_ptr<ScopeNode> root = nullptr;
    FileInfo* file_info{};
};
