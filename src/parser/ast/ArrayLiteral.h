/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_LISTLITERAL_H
#define RONASCRIPT_LISTLITERAL_H

#include <string>
#include <vector>
#include "AstNode.h"

class ArrayLiteral : public AstNode {
public:
    ArrayLiteral();
    ~ArrayLiteral();
    std::string ToString(bool nl) override;

public:
    std::vector<std::shared_ptr<AstNode>> items;
};

#endif  //RONASCRIPT_LISTLITERAL_H
