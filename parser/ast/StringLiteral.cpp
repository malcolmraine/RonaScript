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

#include "StringLiteral.h"

#include <utility>

/******************************************************************************
 * @brief
 */
StringLiteral::StringLiteral() {
    this->node_type = STRING_LITERAL;
}

/******************************************************************************
 * @brief
 */
StringLiteral::StringLiteral(std::string value) {
    this->node_type = STRING_LITERAL;
    this->data = std::move(value);
}

/******************************************************************************
 * @brief
 * @param value
 */
StringLiteral::StringLiteral(bool value) {
    this->node_type = STRING_LITERAL;
    this->data = std::to_string(value);
}

/******************************************************************************
 * @brief
 * @param value
 */
StringLiteral::StringLiteral(double value) {
    this->node_type = STRING_LITERAL;
    this->data = std::to_string(value);
}

/******************************************************************************
 * @brief
 */
StringLiteral::~StringLiteral() = default;

/******************************************************************************
 * @brief
 * @return
 */
std::string StringLiteral::to_string(bool nl) {
    return make_tab_str() + "StringLiteral( " + this->data + " )\n";
}


