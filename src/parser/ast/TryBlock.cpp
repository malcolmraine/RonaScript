/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "TryBlock.h"
#include "../../codegen/RnCodeGenVisitor.h"
#include "CatchBlock.h"
#include "ScopeNode.h"

/*****************************************************************************/
TryBlock::TryBlock() = default;

/*****************************************************************************/
TryBlock::~TryBlock() {
    scope.reset();
}

/*****************************************************************************/
std::string TryBlock::ToString(bool nl) {
    std::string s = MakeTabStr() + "Try( )";
    if (nl) {
        s += "\n";
    }
    scope->nest_lvl = nest_lvl + 1;
    catch_block->nest_lvl = nest_lvl + 1;
    s += scope->ToString(true);
    s += catch_block->ToString(true);

    return s;
}