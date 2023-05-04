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

#pragma once

#include <sys/stat.h>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../util/RnSequencer.h"
#include "TokenType.h"
#include "../util/FileInfo.h"

#define STRING_LITERAL_MAX_LENGTH 1000000
#define BLOCK_COMMENT_START "/*"
#define BLOCK_COMMENT_END "*/"
#define INLINE_COMMENT_START "//"
#define INLINE_COMMENT_END_N '\n'
#define INLINE_COMMENT_END_R '\r'
#define HEX_LITERAL_PREFIX "0x"
#define OCTAL_LITERAL_PREFIX "0o"
#define IS_NEGATIVE_LITERAL(s) ((s)[0] == '-')
#define IS_SIGNED_POSITIVE_LITERAL(s) ((s)[0] == '+')

class Token;

class Lexer : public RnSequencer<char, char> {
public:
    Lexer();
    ~Lexer();
    Token* Emit(TokenType type = TokenType::UNDEFINED);
    Token* MakeToken(const std::string& s,
                     TokenType initial_type = TokenType::UNDEFINED) const;
    static bool IsIntLiteral(std::string s);          // TODO: Unit test
    static bool IsFloatLiteral(std::string s);        // TODO: Unit test
    static bool IsHexLiteral(std::string s);          // TODO: Unit test
    static bool IsBoolLiteral(const std::string& s);  // TODO: Unit test
    static bool IsStrLiteral(const std::string& s);   // TODO: Unit test
    bool IsCompound() const;
    bool IsReservedWord(const std::string& s) const;
    std::string GetCompoundCandidate(int n = 2) const;
    Token* ProcessReservedWord();
    Token* ProcessDefault();
    Token* ProcessComment(bool is_block_comment = false);
    Token* ProcessOperator();
    Token* ProcessStringLiteral();
    Token* Consume();
    void ProcessTokens();
    void LoadFile(const std::string& path);
    char GetCurrentAsExpectedType() override;
    void HandleUnexpectedItem() override;
    void RunAdvanceBufferSideEffects() override;
    static bool IsWhiteSpace(char c) ;               // TODO: Unit test
    std::string ItemToString(char item) override;  // TODO: Unit test
    void Reset();

    std::vector<Token*> tokens;
    std::string _lexeme;
    FileInfo file_info;

protected:
    bool _use_loaded_string = false;
    std::string _file_path;
    std::ifstream _file_obj;
    static std::unordered_map<std::string, TokenType> _token_map;
    static const std::unordered_set<std::string> _compounds;
    static const std::unordered_set<std::string> _binary_ops;
    static const std::unordered_set<std::string> _reserved_words;
    std::vector<std::string> _error_messages;
    size_t _current_line_start = 0;
};
