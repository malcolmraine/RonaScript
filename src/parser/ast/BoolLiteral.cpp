/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "BoolLiteral.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
BoolLiteral::BoolLiteral() {
    node_type = AST_BOOL_LITERAL;
    _data = false;
}

/*****************************************************************************/
BoolLiteral::BoolLiteral(bool value) {
    node_type = AST_BOOL_LITERAL;
    _data = static_cast<bool>(value);
}

/*****************************************************************************/
BoolLiteral::BoolLiteral(long value) {
    node_type = AST_BOOL_LITERAL;
    _data = static_cast<bool>(value);
}

/*****************************************************************************/
BoolLiteral::BoolLiteral(const std::string& value) {
    node_type = AST_BOOL_LITERAL;
    _data = value.empty();
}

/*****************************************************************************/
BoolLiteral::BoolLiteral(double value) {
    node_type = AST_BOOL_LITERAL;
    _data = static_cast<bool>(value);
}

/*****************************************************************************/
BoolLiteral::~BoolLiteral() = default;

/*****************************************************************************/
std::string BoolLiteral::ToString(bool nl) {
    std::string s = MakeTabStr() + "BoolLiteral( " + (_data ? "true" : "false") + " )";
    if (nl) {
        s += "\n";
    }
    return s;
}
