/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef VARSTACKTEST_TRY_BLOCK_H
#define VARSTACKTEST_TRY_BLOCK_H

#include "AstNode.h"

class ScopeNode;
class Name;
class CatchBlock;

class TryBlock : public AstNode {
public:
    TryBlock();
    ~TryBlock();
    std::string ToString(bool nl) override;
    std::vector<std::shared_ptr<Name>> exception_ids;
    std::shared_ptr<ScopeNode> scope = nullptr;
    std::shared_ptr<CatchBlock> catch_block = nullptr;
};

#endif  //VARSTACKTEST_TRY_BLOCK_H
