/******************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Version: 1
*
* MIT License
*
* Copyright (c) 2021 Malcolm Hall
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*******************************************************************************/

#include "IfStmt.h"

/******************************************************************************
 * @brief
 */
IfStmt::IfStmt() {
    this->node_type = IF_STMT;
}

/******************************************************************************
 * @brief
 */
IfStmt::~IfStmt() {
    delete this->test;
    delete this->consequent;
    delete this->alternative;
}

/******************************************************************************
 * @brief
 * @return
 */
std::string IfStmt::to_string(bool nl) {
    std::string output = make_tab_str() + "IfStmt( )\n";
    output += make_tab_str() + "\tTest( )\n";
    this->test->nest_lvl = this->nest_lvl + 1;
    output += this->test->to_string();

    output += make_tab_str() + "\tConsequent( )\n";
    this->consequent->nest_lvl = this->nest_lvl + 1;
    output += this->consequent->to_string();

    if (this->alternative != nullptr) {
        output += make_tab_str() + "\tAlternative( )\n";
        this->alternative->nest_lvl = this->nest_lvl + 1;
        output += this->alternative->to_string();
    }

    return output;
}
