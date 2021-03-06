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

#include "FloatLiteral.h"

/******************************************************************************
 * @brief
 */
FloatLiteral::FloatLiteral() {
    this->node_type = FLOAT_LITERAL;
    this->data = 0;
}

/******************************************************************************
 * @brief
 */
FloatLiteral::FloatLiteral(double value) {
    this->node_type = FLOAT_LITERAL;
    this->data = value;
}

/******************************************************************************
 * @brief
 */
FloatLiteral::~FloatLiteral() = default;

/******************************************************************************
 * @brief
 * @return
 */
std::string FloatLiteral::to_string(bool nl) {
    return make_tab_str() + "FloatLiteral( " + std::to_string(this->data) + " )\n";
}

FloatLiteral::FloatLiteral(const std::string &value) {

}

FloatLiteral::FloatLiteral(bool value) {

}
