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

#include <utility>
#include "token.h"

/******************************************************************************
 * @brief
 * @param s
 * @param token
 * @param line_num
 * @param char_num
 */
Token::Token(std::string s, TokenType::TokenType_t token, int line_num, int char_num) {
    this->lexeme = std::move(s);
    this->file_pos.line_num = line_num;
    this->file_pos.char_num = char_num;
}

/******************************************************************************
 * @brief
 * @return
 */
bool Token::is_literal() {
    return this->tok_type == TokenType::FLOAT_LITERAL ||
           this->tok_type == TokenType::INT_LITERAL ||
           this->tok_type == TokenType::BOOL_LITERAL ||
           this->tok_type == TokenType::STRING_LITERAL;
}

/******************************************************************************
 * @brief Returns true if the token is a binary operator.
 * @return bool
 */
bool Token::is_binary_op() {
    return this->tok_type == TokenType::PLUS
           || this->tok_type == TokenType::MINUS
           || this->tok_type == TokenType::STAR
           || this->tok_type == TokenType::BAR
           || this->tok_type == TokenType::DBL_BAR
           || this->tok_type == TokenType::DBL_AMPER
           || this->tok_type == TokenType::DBL_EQUAL
           || this->tok_type == TokenType::UP_ARROW
           || this->tok_type == TokenType::GEQ
           || this->tok_type == TokenType::LEQ
           || this->tok_type == TokenType::R_CARAT
           || this->tok_type == TokenType::L_CARAT
           || this->tok_type == TokenType::SLASH
           || this->tok_type == TokenType::AMPER
           || this->tok_type == TokenType::NOT_EQUAL
           || this->tok_type == TokenType::PERCENT;
}

/******************************************************************************
 * @brief Returns true if the token is a unary operator.
 * @return bool
 */
bool Token::is_unary_op() {
    return this->tok_type == TokenType::DBL_PLUS
           || this->tok_type == TokenType::DBL_MINUS
           || this->tok_type == TokenType::DBL_STAR
           || this->tok_type == TokenType::MINUS
           || this->tok_type == TokenType::PLUS
           || this->tok_type == TokenType::UP_ARROW
           || this->tok_type == TokenType::TILDE
           || this->tok_type == TokenType::NOT;
}

/******************************************************************************
 * @brief Returns true if the token is a compound operator.
 * @return bool
 */
bool Token::is_cmpnd_op() {
    return this->tok_type == TokenType::PLUS_EQUAL
           || this->tok_type == TokenType::MINUS_EQUAL
           || this->tok_type == TokenType::STAR_EQUAL
           || this->tok_type == TokenType::SLASH_EQUAL
           || this->tok_type == TokenType::AMPER_EQUAL
           || this->tok_type == TokenType::XOREQ
           || this->tok_type == TokenType::BAR_EQUAL
           || this->tok_type == TokenType::PERCENT_EQUAL;
}

/******************************************************************************
 * @brief Returns true if the token is an operator.
 * @return bool
 */
bool Token::is_operator() {
    return is_binary_op();
}

/******************************************************************************
 * @brief Returns true if the token is a type name.
 * @return bool
 */
bool Token::is_type() {
    return this->tok_type == TokenType::FLOAT
           || this->tok_type == TokenType::INT
           || this->tok_type == TokenType::STRING
           || this->tok_type == TokenType::ARRAY
           || this->tok_type == TokenType::OBJECT;
}

/******************************************************************************
 * @brief
 * @return
 */
std::string Token::to_string() {
    return "Token('" + this->lexeme + "', " + std::to_string(tok_type) + ")";
}

/******************************************************************************
 * @brief
 * @return
 */
bool Token::is_qualifier() {
    return this->tok_type == TokenType::PUBLIC
           || this->tok_type == TokenType::PROTECTED
           || this->tok_type == TokenType::PRIVATE
           || this->tok_type == TokenType::STATIC
           || this->tok_type == TokenType::LITERAL
           || this->tok_type == TokenType::REFERENCE
           || this->tok_type == TokenType::CONST;
}