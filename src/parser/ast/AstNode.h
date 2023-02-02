/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_ASTNODE_H
#define RONASCRIPT_ASTNODE_H

#include <string>
#include "../../codegen/RnInstruction.h"
#include "../../lexer/Token.h"
#include "../../util/FileInfo.h"
#include "NodeType.h"

class RnCodeGenVisitor;

class AstNode {
public:
    AstNode() = default;
    virtual ~AstNode() = default;
    virtual std::string ToString(bool nl);
    FilePosition file_pos;
    NodeType_t node_type = AST_DEFAULT;
    int nest_lvl = 0;  // For adding \t characters to string output
    FileInfo* file_info{};

protected:
    std::string MakeTabStr();
};

#endif  //RONASCRIPT_ASTNODE_H
