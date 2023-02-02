/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_CLASSDECL_H
#define RONASCRIPT_CLASSDECL_H

#include <string>
#include "AstNode.h"

class ScopeNode;

class RnCodeGenVisitor;

class Name;

class ClassDecl : public AstNode {
public:
    ClassDecl();
    ~ClassDecl();
    std::string ToString(bool nl) override;

    std::string id;
    std::shared_ptr<ScopeNode> scope = nullptr;
    std::vector<std::shared_ptr<Name>>
        parent_classes;  // Names of classes to inherit. Names are resolved at code generation time
};

#endif  //RONASCRIPT_CLASSDECL_H
