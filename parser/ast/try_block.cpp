//
// Created by Malcolm Hall on 4/17/21.
//

#include "try_block.h"

/******************************************************************************
 * @brief
 */
TryBlock::TryBlock() = default;

/******************************************************************************
 * @brief
 */
TryBlock::~TryBlock() {
    delete this->scope;
}

/******************************************************************************
 * @brief
 * @param nl
 * @return
 */
std::string TryBlock::to_string(bool nl) {
    std::string output = make_tab_str() + "Try( )\n";
    this->scope->nest_lvl = this->nest_lvl + 1;
    this->catch_block->nest_lvl = this->nest_lvl + 1;
    output += this->scope->to_string();
    output += this->catch_block->to_string();

    return output;
}
