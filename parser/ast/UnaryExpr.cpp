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

#include "UnaryExpr.h"

/******************************************************************************
 * @brief
 */
UnaryExpr::UnaryExpr() {
    this->node_type = UNARY_EXPR;
}

/******************************************************************************
 * @brief
 */
UnaryExpr::~UnaryExpr() {
    delete this->expr;
}

/******************************************************************************
 * @brief
 * @return
 */
std::string UnaryExpr::to_string(bool nl) {
    std::string output = make_tab_str() + "UnaryExpr( " + this->op + ", " + this->expr->to_string(false) + " )\n";
    this->expr->nest_lvl = this->nest_lvl + 1;

    return output;
}
