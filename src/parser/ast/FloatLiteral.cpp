/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "FloatLiteral.h"
#include <iostream>
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
FloatLiteral::FloatLiteral() {
    node_type = AST_FLOAT_LITERAL;
    data = 0;
}

/*****************************************************************************/
FloatLiteral::FloatLiteral(double value) {
    node_type = AST_FLOAT_LITERAL;
    data = value;
}

/*****************************************************************************/
FloatLiteral::~FloatLiteral() = default;

/*****************************************************************************/
std::string FloatLiteral::ToString(bool nl) {
    std::string s = MakeTabStr() + "FloatLiteral( " + std::to_string(data) + " )";
    if (nl) {
        s += "\n";
    }
    return s;
}

/*****************************************************************************/
FloatLiteral::FloatLiteral(const std::string& value) {
    std::cout << value;
}

/*****************************************************************************/
FloatLiteral::FloatLiteral(bool value) {
    data = value ? 1.0 : 0.0;
}
