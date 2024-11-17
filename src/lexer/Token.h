/******************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
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

#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../util/FileInfo.h"

// token, lexeme
#define RN_TOKEN_LIST                     \
    TOKEN_DEF(R_BRACE, "{")               \
    TOKEN_DEF(L_BRACE, "}")               \
    TOKEN_DEF(BEGIN, "begin")             \
    TOKEN_DEF(END, "end")                 \
    TOKEN_DEF(R_PARAN, "(")               \
    TOKEN_DEF(L_PARAN, ")")               \
    TOKEN_DEF(R_BRACK, "[")               \
    TOKEN_DEF(L_BRACK, "]")               \
    TOKEN_DEF(R_CARAT, "<")               \
    TOKEN_DEF(L_CARAT, ">")               \
    TOKEN_DEF(SLASH, "/")                 \
    TOKEN_DEF(PLUS, "+")                  \
    TOKEN_DEF(STAR, "*")                  \
    TOKEN_DEF(MINUS, "-")                 \
    TOKEN_DEF(PERCENT, "%")               \
    TOKEN_DEF(AMPER, "&")                 \
    TOKEN_DEF(BAR, "|")                   \
    TOKEN_DEF(UP_ARROW, "^")              \
    TOKEN_DEF(BLOCK_COMMENT, "/*")        \
    TOKEN_DEF(INLINE_COMMENT, "//")       \
    TOKEN_DEF(DOLLAR, "$")                \
    TOKEN_DEF(NOT, "!")                   \
    TOKEN_DEF(EQUAL, "=")                 \
    TOKEN_DEF(COMMA, ",")                 \
    TOKEN_DEF(DOT, ".")                   \
    TOKEN_DEF(COLON, ":")                 \
    TOKEN_DEF(SEMICOLON, ";")             \
    TOKEN_DEF(DBL_QUOTE, "\"")            \
    TOKEN_DEF(DBL_PLUS, "++")             \
    TOKEN_DEF(DBL_MINUS, "--")            \
    TOKEN_DEF(DBL_EQUAL, "==")            \
    TOKEN_DEF(NOT_EQUAL, "!=")            \
    TOKEN_DEF(DBL_AMPER, "&&")            \
    TOKEN_DEF(DBL_BAR, "||")              \
    TOKEN_DEF(DBL_COLON, "::")            \
    RESERVED_WORD(FLOAT, "float")         \
    RESERVED_WORD(AND, "and")             \
    RESERVED_WORD(OR, "or")               \
    RESERVED_WORD(XOR, "xor")             \
    RESERVED_WORD(INT, "int")             \
    RESERVED_WORD(STRING, "string")       \
    RESERVED_WORD(ARRAY, "array")         \
    RESERVED_WORD(CLASS, "class")         \
    RESERVED_WORD(CALLABLE, "callable")   \
    TOKEN_DEF(NAME, "")                   \
    TOKEN_DEF(SLASH_EQUAL, "/=")          \
    TOKEN_DEF(MINUS_EQUAL, "-=")          \
    TOKEN_DEF(PLUS_EQUAL, "+=")           \
    TOKEN_DEF(PERCENT_EQUAL, "%=")        \
    TOKEN_DEF(AMPER_EQUAL, "&=")          \
    TOKEN_DEF(BAR_EQUAL, "|=")            \
    TOKEN_DEF(STAR_EQUAL, "*=")           \
    TOKEN_DEF(DBL_R_CARAT, ">>")          \
    TOKEN_DEF(DBL_L_CARAT, "<<")          \
    RESERVED_WORD(ROUTINE, "routine")     \
    RESERVED_WORD(VAR, "var")             \
    RESERVED_WORD(LITERAL, "literal")     \
    RESERVED_WORD(CONST, "const")         \
    RESERVED_WORD(IMPORT, "import")       \
    RESERVED_WORD(RETURN, "return")       \
    RESERVED_WORD(BREAK, "break")         \
    RESERVED_WORD(VOID, "void")           \
    TOKEN_DEF(STRING_LITERAL, "")         \
    TOKEN_DEF(DBL_STAR, "**")             \
    TOKEN_DEF(UNDEFINED, "")              \
    RESERVED_WORD(NULL_LITERAL, "null")   \
    RESERVED_WORD(BOOL, "bool")           \
    TOKEN_DEF(LEQ, "<=")                  \
    TOKEN_DEF(GEQ, ">=")                  \
    TOKEN_DEF(XOREQ, "^=")                \
    TOKEN_DEF(EMPTY_LIST, "[]")           \
    TOKEN_DEF(TILDE_EQUAL, "~=")          \
    TOKEN_DEF(TILDE, "~")                 \
    RESERVED_WORD(CONSTRUCT, "construct") \
    RESERVED_WORD(DESTRUCT, "destruct")   \
    TOKEN_DEF(R_ARROW, "->")              \
    RESERVED_WORD(IF, "if")               \
    RESERVED_WORD(ELIF, "elif")           \
    RESERVED_WORD(ELSE, "else")           \
    RESERVED_WORD(IS, "is")               \
    RESERVED_WORD(ALIAS, "alias")         \
    RESERVED_WORD(WHILE, "while")         \
    RESERVED_WORD(FOR, "for")             \
    TOKEN_DEF(INT_LITERAL, "")            \
    TOKEN_DEF(FLOAT_LITERAL, "")          \
    TOKEN_DEF(BOOL_LITERAL, "")           \
    RESERVED_WORD(CONTINUE, "continue")   \
    RESERVED_WORD(OBJECT, "object")       \
    TOKEN_DEF(TYPE, "")                   \
    RESERVED_WORD(TRY, "try")             \
    RESERVED_WORD(CATCH, "catch")         \
    RESERVED_WORD(GLOBAL, "global")       \
    RESERVED_WORD(LOCAL, "local")         \
    TOKEN_DEF(QUESTION_MARK, "?")         \
    TOKEN_DEF(DBL_QUESTION_MARK, "??")    \
    RESERVED_WORD(DELETE, "delete")       \
    RESERVED_WORD(EXIT, "exit")           \
    RESERVED_WORD(EXTENDS, "extends")     \
    RESERVED_WORD(MODULE, "module")       \
    RESERVED_WORD(RANGE, "range")         \
    RESERVED_WORD(ANY, "any")             \
    RESERVED_WORD(IN, "in")

#define RESERVED_WORD TOKEN_DEF
#define TOKEN_DEF(token, lexeme) token,

enum TokenType : uint8_t {INVALID_TOKEN = 0, RN_TOKEN_LIST };

class Token {
public:
    Token(std::string s, TokenType token, int line_num = -1, int char_num = -1);
    ~Token() = default;
    [[nodiscard]] bool IsLiteral() const;
    [[nodiscard]] bool IsBinaryOp() const;
    [[nodiscard]] bool IsUnaryOp() const;
    [[nodiscard]] bool IsCompoundOp() const;
    [[nodiscard]] bool IsOperator() const;
    [[nodiscard]] bool IsType() const;
    [[nodiscard]] std::string ToString() const;

    [[nodiscard]] inline bool IsOneOf(
        const std::unordered_set<TokenType>& tokens) const {
        return tokens.contains(_token_type);
    }

    const std::string& GetLexeme() const {
        return _lexeme;
    }

    void SetLexeme(const std::string& value) {
        _lexeme = value;
    }

    TokenType GetType() const {
        return _token_type;
    }

    void SetType(TokenType type) {
        _token_type = type;
    }

    FileInfo file_info;

private:
    std::string _lexeme;
    TokenType _token_type = TokenType::UNDEFINED;
    static std::unordered_map<TokenType, std::string> token_name_map;
};
