//
// Created by Malcolm Hall on 4/17/21.
//

#include "catch_block.h"

/******************************************************************************
 * @brief
 */
CatchBlock::CatchBlock() = default;

/******************************************************************************
 * @brief
 */
CatchBlock::~CatchBlock() {
    delete this->scope;
}

/******************************************************************************
 * @brief
 * @param nl
 * @return
 */
std::string CatchBlock::to_string(bool nl) {
    std::string output = make_tab_str() + "Catch( ";

    for (auto &exception_id : this->exception_ids) {
        output += exception_id->value + " ";
    }

    output += ")\n";
    this->scope->nest_lvl = this->nest_lvl + 1;
    output += this->scope->to_string();

    return output;
}
