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
std::unordered_map<TokenType, std::string> Token::token_type_string_names{
    {TokenType::L_BRACE, "L_BRACE"},                // {
    {TokenType::R_BRACE, "R_BRACE"},                // }
    {TokenType::R_PARAN, "R_PARAN"},                // (
    {TokenType::L_PARAN, "L_PARAN"},                // )
    {TokenType::R_BRACK, "R_BRACK"},                // [
    {TokenType::L_BRACK, "L_BRACK"},                // ]
    {TokenType::R_CARAT, "R_CARAT"},                // <
    {TokenType::L_CARAT, "L_CARAT"},                // >
    {TokenType::SLASH, "SLASH"},                    // /
    {TokenType::PLUS, "PLUS"},                      // +
    {TokenType::STAR, "STAR"},                      // *
    {TokenType::MINUS, "MINUS"},                    // -
    {TokenType::PERCENT, "PERCENT"},                // %
    {TokenType::AMPER, "AMPER"},                    // %
    {TokenType::BAR, "BAR"},                        // |
    {TokenType::UP_ARROW, "UP_ARROW"},              // ^
    {TokenType::BLOCK_COMMENT, "BLOCK_COMMENT"},    // /*
    {TokenType::INLINE_COMMENT, "INLINE_COMMENT"},  // //
    {TokenType::DOLLAR, "DOLLAR"},                  // $
    {TokenType::NOT, "NOT"},                        // !
    {TokenType::EQUAL, "EQUAL"},                    // =
    {TokenType::COMMA, "COMMA"},                    // ,
    {TokenType::DOT, "DOT"},                        // .
    {TokenType::COLON, "COLON"},                    // :
    {TokenType::DOUBLE_COLON, "DOUBLE_COLON"},      // :

    {TokenType::SEMICOLON, "SEMICOLON"},  // ;
    {TokenType::DBL_QUOTE, "DBL_QUOTE"},  // "
    {TokenType::DBL_PLUS, "DBL_PLUS"},    // ++
    {TokenType::DBL_MINUS, "DBL_MINUS"},  // --
    {TokenType::DBL_EQUAL, "DBL_EQUAL"},  // ==
    {TokenType::NOT_EQUAL, "NOT_EQUAL"},  // !=
    {TokenType::DBL_AMPER, "DBL_AMPER"},  // &&
    {TokenType::DBL_BAR, "DBL_BAR"},      // ||
    {TokenType::DBL_COLON, "DBL_COLON"},  // ::
    {TokenType::FLOAT, "FLOAT"},          // float
    {TokenType::INT, "INT"},              // int
    {TokenType::STRING, "STRING"},        // string
    {TokenType::ARRAY, "ARRAY"},          // list
    {TokenType::CLASS, "CLASS"},          // class
    {TokenType::CALLABLE, "CALLABLE"},
    {TokenType::NAME, "NAME"},
    {TokenType::SLASH_EQUAL, "SLASH_EQUAL"},      // /=
    {TokenType::MINUS_EQUAL, "MINUS_EQUAL"},      // -=
    {TokenType::PLUS_EQUAL, "PLUS_EQUAL"},        // +=
    {TokenType::PERCENT_EQUAL, "PERCENT_EQUAL"},  // %=
    {TokenType::AMPER_EQUAL, "AMPER_EQUAL"},      // &=
    {TokenType::BAR_EQUAL, "BAR_EQUAL"},          // |=
    {TokenType::STAR_EQUAL, "STAR_EQUAL"},        // *=
    {TokenType::DBL_R_CARAT, "DBL_R_CARAT"},      // >>
    {TokenType::DBL_L_CARAT, "DBL_L_CARAT"},      // <<
    {TokenType::FUNC, "FUNC"},                    // func
    {TokenType::ROUTINE, "ROUTINE"},              // func
    {TokenType::VAR, "VAR"},                      // var
    {TokenType::LITERAL, "LITERAL"},              // literal
    {TokenType::CONST, "CONST"},                  // const
    {TokenType::IMPORT, "IMPORT"},                // import
    {TokenType::RETURN, "RETURN"},                // return
    {TokenType::BREAK, "BREAK"},                  // break
    {TokenType::VOID, "VOID"},                    // void
    {TokenType::STRING_LITERAL, "STRING_LITERAL"},
    {TokenType::DBL_STAR, "DBL_STAR"},  // **
    {TokenType::UNDEFINED, "UNDEFINED"},
    {TokenType::NULL_LITERAL, "NONE"},        // null
    {TokenType::BOOL, "BOOL"},                // bool
    {TokenType::LEQ, "LEQ"},                  // <=
    {TokenType::GEQ, "GEQ"},                  // >=
    {TokenType::XOREQ, "XOREQ"},              // ^=
    {TokenType::EMPTY_LIST, "EMPTY_LIST"},    // []
    {TokenType::TILDE_EQUAL, "TILDE_EQUAL"},  // ~=
    {TokenType::TILDE, "TILDE"},              // ~
    {TokenType::CONSTRUCT, "CONSTRUCT"},      // construct
    {TokenType::DESTRUCT, "DESTRUCT"},        // destruct
    {TokenType::R_ARROW, "R_ARROW"},          // ->
    {TokenType::IF, "IF"},                    // if
    {TokenType::ELIF, "ELIF"},                // elif
    {TokenType::ELSE, "ELSE"},                // else
    {TokenType::IS, "IS"},                    // is
    {TokenType::ALIAS, "ALIAS"},              // alias
    {TokenType::WHILE, "WHILE"},              // while
    {TokenType::FOR, "FOR"},                  // for
    {TokenType::INT_LITERAL, "INT_LITERAL"},
    {TokenType::FLOAT_LITERAL, "FLOAT_LITERAL"},
    {TokenType::BOOL_LITERAL, "BOOL_LITERAL"},
    {TokenType::CONTINUE, "CONTINUE"},
    {TokenType::OBJECT, "OBJECT"},
    {TokenType::TYPE, "TYPE"},
    {TokenType::TRY, "TRY"},
    {TokenType::CATCH, "CATCH"},
    {TokenType::GLOBAL, "GLOBAL"},
    {TokenType::LOCAL, "LOCAL"},
    {TokenType::QUESTION_MARK, "QUESTION_MARK"},
    {TokenType::DBL_QUESTION_MARK, "DBL_QUESTION_MARK"},
    {TokenType::DELETE, "DELETE"},
    {TokenType::EXTENDS, "EXTENDS"},
    {TokenType::IMPORT, "IMPORT"},
    {TokenType::MODULE, "MODULE"},
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
    return IsOneOf(
        {TokenType::PLUS,        TokenType::MINUS,       TokenType::STAR,
         TokenType::BAR,         TokenType::DBL_BAR,     TokenType::DBL_AMPER,
         TokenType::DBL_EQUAL,   TokenType::UP_ARROW,    TokenType::GEQ,
         TokenType::LEQ,         TokenType::R_CARAT,     TokenType::L_CARAT,
         TokenType::SLASH,       TokenType::AMPER,       TokenType::NOT_EQUAL,
         TokenType::PERCENT,     TokenType::UP_ARROW,    TokenType::DOUBLE_COLON,
         TokenType::DBL_L_CARAT, TokenType::DBL_R_CARAT, TokenType::R_ARROW,
         TokenType::DBL_STAR});
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
    return "Token('" + lexeme + "', " + Token::token_type_string_names[token_type] +
           ")";
}

/*****************************************************************************/
bool Token::IsOneOf(const std::unordered_set<TokenType>& tokens) const {
    return tokens.contains(token_type);
}
