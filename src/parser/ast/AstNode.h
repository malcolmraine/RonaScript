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
#include <vector>
#include "../../util/FileInfo.h"
#include "NodeType.h"

class RnCodeGenVisitor;

class AstNode {
public:
    AstNode() = default;
    virtual ~AstNode() = default;
    [[nodiscard]] bool IsLiteral() const;
    virtual std::string ToString(bool nl);
    NodeType_t node_type = AST_DEFAULT;
    int nest_lvl = 0;  // For adding \t characters to string output
    FileInfo file_info;

protected:
    std::string MakeTabStr();
};
