/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#pragma once

#include <string>
#include "../../lexer/Token.h"
#include "NodeType.h"

class RnCodeGenVisitor;
class FileInfo;

class AstNode {
public:
    AstNode() = default;
    virtual ~AstNode() = default;
    [[nodiscard]] bool IsLiteral() const;
    virtual std::string ToString(bool nl);
    FilePosition file_pos;
    NodeType_t node_type = AST_DEFAULT;
    int nest_lvl = 0;  // For adding \t characters to string output
    FileInfo* file_info = nullptr;

protected:
    std::string MakeTabStr();
};

