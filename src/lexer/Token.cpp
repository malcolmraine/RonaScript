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
#include "Token.h"
#include <utility>

/*****************************************************************************/

#undef TOKEN_DEF
#undef RESERVED_WORD
#define TOKEN_DEF(token, lexeme) {TokenType::token, #token},
#define RESERVED_WORD TOKEN_DEF
std::unordered_map<TokenType, std::string> Token::token_name_map{
    RN_TOKEN_LIST
};

/*****************************************************************************/
Token::Token(std::string s, TokenType token, int line_num, int char_num) {
    lexeme = std::move(s);
    file_info.SetCharNum(char_num);
    file_info.SetLineNum(line_num);
    token_type = token;
}

/*****************************************************************************/
bool Token::IsLiteral() const {
    return IsOneOf({TokenType::FLOAT_LITERAL, TokenType::INT_LITERAL,
                    TokenType::BOOL_LITERAL, TokenType::NULL_LITERAL,
                    TokenType::STRING_LITERAL});
}

/*****************************************************************************/
bool Token::IsBinaryOp() const {
    return token_type == TokenType::DBL_COLON || IsOneOf(
        {TokenType::PLUS,        TokenType::MINUS,       TokenType::STAR,
         TokenType::BAR,         TokenType::DBL_BAR,     TokenType::DBL_AMPER,
         TokenType::DBL_EQUAL,   TokenType::UP_ARROW,    TokenType::GEQ,
         TokenType::LEQ,         TokenType::R_CARAT,     TokenType::L_CARAT,
         TokenType::SLASH,       TokenType::AMPER,       TokenType::NOT_EQUAL,
         TokenType::PERCENT,     TokenType::UP_ARROW,    TokenType::DOUBLE_COLON,
         TokenType::DBL_L_CARAT, TokenType::DBL_R_CARAT, TokenType::R_ARROW,
         TokenType::DBL_STAR, TokenType::OR, TokenType::AND, TokenType::XOR});
}

/*****************************************************************************/
bool Token::IsUnaryOp() const {
    return IsOneOf({TokenType::DBL_PLUS, TokenType::DBL_MINUS, TokenType::MINUS,
                    TokenType::TILDE, TokenType::NOT});
}

/*****************************************************************************/
bool Token::IsCompoundOp() const {
    return IsOneOf({TokenType::PLUS_EQUAL, TokenType::MINUS_EQUAL,
                    TokenType::STAR_EQUAL, TokenType::SLASH_EQUAL,
                    TokenType::AMPER_EQUAL, TokenType::XOREQ, TokenType::BAR_EQUAL,
                    TokenType::PERCENT_EQUAL});
}

/*****************************************************************************/
bool Token::IsOperator() const {
    return IsBinaryOp();
}

/*****************************************************************************/
bool Token::IsType() const {
    return IsOneOf({TokenType::FLOAT, TokenType::INT, TokenType::STRING,
                    TokenType::ARRAY, TokenType::OBJECT, TokenType::BOOL,
                    TokenType::CALLABLE, TokenType::ANY});
}

/*****************************************************************************/
std::string Token::ToString() const {
    return "Token('" + lexeme + "', " + Token::token_name_map[token_type] +
           ")";
}