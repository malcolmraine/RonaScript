/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_ARGDECL_H
#define RONASCRIPT_ARGDECL_H

#include <memory>
#include <string>
#include "AstNode.h"

class Name;
class RnTypeComposite;

class ArgDecl : public AstNode {
public:
    ArgDecl();
    ~ArgDecl() override;
    std::string ToString(bool nl) override;
    std::shared_ptr<Name> id{};
    std::shared_ptr<RnTypeComposite> type;
};

#endif  //RONASCRIPT_ARGDECL_H
