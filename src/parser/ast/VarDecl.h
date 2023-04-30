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
#include "../../common/RnType.h"
#include "AstNode.h"
#include "../../lexer/Token.h"

class RnCodeGenVisitor;
class RnInstruction;
class RnInstruction;

class VarDecl : public AstNode {
public:
    VarDecl();
    ~VarDecl() override;
    std::string ToString(bool nl) override;
    std::string id;
    std::shared_ptr<RnTypeComposite> type;
    std::vector<Token*> qualifiers;
    std::shared_ptr<AstNode> init_value = nullptr;
    bool is_const = false;
    bool is_local = false;
    bool is_global = false;
    bool is_literal = false;
};
