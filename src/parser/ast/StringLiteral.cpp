/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "StringLiteral.h"
#include <utility>

/*****************************************************************************/
StringLiteral::StringLiteral() {
    node_type = AST_STRING_LITERAL;
}

/*****************************************************************************/
StringLiteral::StringLiteral(std::string value) {
    node_type = AST_STRING_LITERAL;

    data = std::move(value);
}

/*****************************************************************************/
StringLiteral::StringLiteral(bool value) {
    node_type = AST_STRING_LITERAL;
    data = std::to_string(value);
}

/*****************************************************************************/
StringLiteral::StringLiteral(double value) {
    node_type = AST_STRING_LITERAL;
    data = std::to_string(value);
}

/*****************************************************************************/
StringLiteral::~StringLiteral() = default;

/*****************************************************************************/
std::string StringLiteral::ToString(bool nl) {
    std::string s = MakeTabStr() + "StringLiteral( " + data + " )";
    if (nl) {
        s += "\n";
    }
    return s;
}
