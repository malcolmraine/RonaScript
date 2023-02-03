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

#ifndef RONASCRIPT_TOKEN_TYPE_H
#define RONASCRIPT_TOKEN_TYPE_H

enum TokenType : int {
    L_BRACE,         // {
    R_BRACE,         // }
    R_PARAN,         // (
    L_PARAN,         // )
    R_BRACK,         // [
    L_BRACK,         // ]
    R_CARAT,         // <
    L_CARAT,         // >
    SLASH,           // /
    PLUS,            // +
    STAR,            // *
    MINUS,           // -
    PERCENT,         // %
    AMPER,           // %
    BAR,             // |
    UP_ARROW,        // ^
    BLOCK_COMMENT,   // /*
    INLINE_COMMENT,  // //
    DOLLAR,          // $
    NOT,             // !
    EQUAL,           // =
    COMMA,           // ,
    DOT,             // .
    COLON,           // :
    SEMICOLON,       // ;
    DBL_QUOTE,       // "
    DBL_PLUS,        // ++
    DBL_MINUS,       // --
    DBL_EQUAL,       // ==
    NOT_EQUAL,       // !=
    DBL_AMPER,       // &&
    DBL_BAR,         // ||
    DBL_COLON,       // ::
    FLOAT,           // float
    INT,             // int
    STRING,          // string
    ARRAY,           // list
    CLASS,           // class
    CALLABLE,
    NAME,
    SLASH_EQUAL,    // /=
    MINUS_EQUAL,    // -=
    PLUS_EQUAL,     // +=
    PERCENT_EQUAL,  // %=
    AMPER_EQUAL,    // &=
    BAR_EQUAL,      // |=
    STAR_EQUAL,     // *=
    DBL_R_CARAT,    // >>
    DBL_L_CARAT,    // <<
    FUNC,           // func
    ROUTINE,        // routine
    SUB,            // subroutine
    VAR,            // var
    CONST,          // const
    PUBLIC,
    PROTECTED,
    PRIVATE,
    STATIC,
    LITERAL,    // Indicates always pass by value
    REFERENCE,  // Indicates always pass by reference
    IMPORT,     // import
    RETURN,     // return
    BREAK,      // break
    VOID,       // void
    STRING_LITERAL,
    DBL_STAR,  // **
    UNDEFINED,
    NULL_LITERAL,  // null
    BOOL,          // bool
    LEQ,           // <=
    GEQ,           // >=
    XOREQ,         // ^=
    EMPTY_LIST,    // []
    TILDE_EQUAL,   // ~=
    TILDE,         // ~
    CONSTRUCT,     // construct
    DESTRUCT,      // destruct
    R_ARROW,
    DOUBLE_COLON,  // ->
    IF,            // if
    ELIF,          // elif
    ELSE,          // else
    IS,            // is
    ALIAS,         // alias
    WHILE,         // while
    FOR,           // for
    INT_LITERAL,
    FLOAT_LITERAL,
    BOOL_LITERAL,
    CONTINUE,
    OBJECT,
    TYPE,
    TRY,
    CATCH,
    GLOBAL,
    LOCAL,
    QUESTION_MARK,
    DBL_QUESTION_MARK,
    DELETE,
    EXIT,
    EXTENDS,
    MODULE,
    RANGE,
};

#endif  //RONASCRIPT_TOKEN_TYPE_H