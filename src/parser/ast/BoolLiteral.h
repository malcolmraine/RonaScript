/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef VARSTACKTEST_BOOLLITERAL_H
#define VARSTACKTEST_BOOLLITERAL_H

#include <string>
#include "AstNode.h"

class BoolLiteral : public AstNode {
public:
    BoolLiteral();
    explicit BoolLiteral(bool value);
    explicit BoolLiteral(long value);
    explicit BoolLiteral(double value);
    explicit BoolLiteral(const std::string& value);
    ~BoolLiteral();
    std::string ToString(bool nl) override;
    bool data = false;
};

#endif  //VARSTACKTEST_BOOLLITERAL_H
