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

#ifndef RONASCRIPT_TOKEN_H
#define RONASCRIPT_TOKEN_H

#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../util/FileInfo.h"
#include "TokenType.h"

struct FilePosition {
    int line_num = 1;
    int char_num = 1;
};

class Token {
public:
    static std::unordered_map<TokenType, std::string> token_type_string_names;
    Token(std::string s, TokenType token, int line_num = -1, int char_num = -1);
    ~Token() = default;
    [[nodiscard]] bool IsLiteral() const;
    [[nodiscard]] bool IsBinaryOp() const;
    [[nodiscard]] bool IsUnaryOp() const;
    [[nodiscard]] bool IsCompoundOp() const;
    [[nodiscard]] bool IsOperator() const;
    [[nodiscard]] bool IsType() const;
    [[nodiscard]] bool IsQualifier() const;
    [[nodiscard]] std::string ToString() const;
    bool IsOneOf(const std::unordered_set<TokenType>& tokens) const;

    TokenType token_type = TokenType::UNDEFINED;
    std::string lexeme;
    FilePosition file_pos;
    FileInfo file_info;
};

#endif  //RONASCRIPT_TOKEN_H
