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
#include "Lexer.h"
#include <algorithm>
#include <utility>
#include "Token.h"
#include "../memory_mgmt/RnLinearAllocator.h"

#undef TOKEN_DEF
#undef RESERVED_WORD
#define TOKEN_DEF(token, lexeme) {lexeme, TokenType::token},
#define RESERVED_WORD TOKEN_DEF
std::unordered_map<std::string, TokenType> Lexer::_token_map = {RN_TOKEN_LIST};

#undef TOKEN_DEF
#undef RESERVED_WORD
#define TOKEN_DEF(token, lexeme)
#define RESERVED_WORD(token, lexeme) lexeme,

const std::unordered_set<std::string> Lexer::_reserved_words = {RN_TOKEN_LIST};

const std::unordered_set<std::string> Lexer::_compounds = {
    "++", "+=", "--", "-=", "&&", "%=", "||", "**", "&=", "|=", "/=", "*=", "~=",
    "^=", "!=", "->", "::", ">=", "<=", "==", ">>", "<<", "::", "??", "in"};

const std::unordered_set<std::string> Lexer::_binary_ops = {
    "+"
    "-"
    "=="
    ">="
    "<="
    "!="
    ">"
    "<"
    "&"
    "&&"
    "|"
    "||"
    "^"
    "/"
    "%"
    "*"
    "??"
    "->"
    "::",
    "in"};

/*****************************************************************************/
Lexer::Lexer() {
    file_info.SetFilePath(_file_path);
    FillBuffer('\0');
}

/*****************************************************************************/
Lexer::~Lexer() {
    _file_obj.close();
    for (auto& token : tokens)
        delete token;
}

/*****************************************************************************/
Token* Lexer::Emit(TokenType type) {
    auto token = MakeToken(_lexeme, type);

    // This is a little awkward, but it handles repeated unary operators
    if (token->GetType() == TokenType::NAME &&
        (token->GetLexeme()[0] == '+' || token->GetLexeme()[0] == '-')) {
        tokens.emplace_back(MakeToken(std::string(1, token->GetLexeme()[0])));
        token->SetLexeme(token->GetLexeme().substr(1));
    }
    _lexeme.clear();
    tokens.emplace_back(token);

    return token;
}

/*****************************************************************************/
Token* Lexer::MakeToken(const std::string& s, TokenType initial_type) const {
    auto token = new Token(s, initial_type);
    token->file_info = file_info;

    // A number can have an abitrary number of signs in front of it
    auto normalize_sign = [](const std::string& s) {
        size_t i = 0;
        int sign = 1;
        for (; i < s.length(); i++) {
            if (s[i] == '-') {
                sign *= -1;
                continue;
            } else if (s[i] == '+') {
                continue;
            }
            break;
        }
        if (sign == 1)
            return s.substr(i);
        return std::string(1, '-') + s.substr(i);
    };

    if (initial_type == TokenType::UNDEFINED) {
        if (_token_map.count(s) || (_token_map.count(s) && IsReservedWord(s))) {
            token->SetType(_token_map[s]);
        } else if (IsIntLiteral(s)) {
            token->SetType(TokenType::INT_LITERAL);
            token->SetLexeme(normalize_sign(s));
        } else if (IsFloatLiteral(s)) {
            token->SetType(TokenType::FLOAT_LITERAL);
            token->SetLexeme(normalize_sign(s));
        } else if (IsStrLiteral(s)) {
            token->SetType(TokenType::STRING_LITERAL);
        } else if (IsBoolLiteral(s)) {
            token->SetType(TokenType::BOOL_LITERAL);
        } else {
            token->SetType(TokenType::NAME);
        }
    }
    return token;
}

/*****************************************************************************/
bool Lexer::IsIntLiteral(const std::string& s) {
    size_t i = 0;
    char c;
    for (; i < s.length(); i++) {
        c = s[i];
        if (c != '+' && c != '-')
            break;
    }
    for (; i < s.length(); i++) {
        c = s[i];
        if (!std::isdigit(c))
            return false;
    }
    return true;
}

/*****************************************************************************/
bool Lexer::IsFloatLiteral(const std::string& s) {
    bool decimal_found = false;
    size_t i = 0;
    char c;
    for (; i < s.length(); i++) {
        c = s[i];
        if (c != '+' && c != '-')
            break;
    }

    for (; i < s.length(); ++i) {
        c = s[i];
        if (!std::isdigit(c)) {
            if (c == '.' && !decimal_found)
                decimal_found = true;
            else
                return false;
        }
    }
    return decimal_found;
}

/*****************************************************************************/
bool Lexer::IsHexLiteral(const std::string& s) {
    std::string prefix = std::string(1, s[0]) + std::string(1, s[1]);
    if (prefix == "0x" || prefix == "0X") {
        for (size_t i = 2; i < s.length(); ++i) {
            if (!std::isdigit(s[i]))
                return false;
        }
        return true;
    } else {
        return false;
    }
}

/*****************************************************************************/
bool Lexer::IsBoolLiteral(const std::string& s) {
    return s == "true" || s == "false";
}

/*****************************************************************************/
bool Lexer::IsStrLiteral(const std::string& s) {
    return (s[0] == '"' && s[s.length() - 1] == '"') ||
           (s[0] == '\'' && s[s.length() - 1] == '\'');
}

/*****************************************************************************/
bool Lexer::IsCompound() const {
    if (Peek() == '>') {
        return true;
    }
    if ((Current() == '-' || Current() == '+') &&
        (tokens.back()->IsLiteral() ||
         _binary_ops.find(std::string(1, Peek())) == _binary_ops.end()) &&
        Peek() != '=') {
        return false;
    }
    return _compounds.find(GetCompoundCandidate()) != _compounds.end();
}

/*****************************************************************************/
bool Lexer::IsReservedWord(const std::string& s) {
    return _reserved_words.find(s) != _reserved_words.end();
}

/*****************************************************************************/
std::string Lexer::GetCompoundCandidate(int n) const {
    std::string s = std::string(1, Current());

    for (int i = 1; i < n; ++i)
        s.append(std::string(1, Peek(i)));

    return s;
}

/*****************************************************************************/
Token* Lexer::ProcessReservedWord() {
    return Emit();
}

/*****************************************************************************/
Token* Lexer::ProcessDefault() {
    if (_lexeme.empty()) {
        _lexeme = std::string(1, Current());
        AdvanceBuffer(1);
    }
    return Emit();
}

/*****************************************************************************/
Token* Lexer::ProcessComment(bool is_block_comment) {
    if (_lexeme.empty()) {
        if (is_block_comment) {
            while (GetCompoundCandidate() != BLOCK_COMMENT_END && !EndOfSequence())
                AdvanceBuffer(1);
        } else {
            while (Current() != INLINE_COMMENT_END_N &&
                   Current() != INLINE_COMMENT_END_R && !EndOfSequence())
                AdvanceBuffer(1);
        }
        AdvanceBuffer(1);

        if (is_block_comment)
            AdvanceBuffer(1);
        return new Token("", TokenType::UNDEFINED);
    } else {
        return Emit();
    }
}

/*****************************************************************************/
Token* Lexer::ProcessOperator() {
    if (_lexeme.empty()) {
        if (IsCompound()) {
            _lexeme = GetCompoundCandidate();
            AdvanceBuffer(2);
        } else {
            _lexeme = std::string(1, Current());
            AdvanceBuffer(1);
        }
    }

    return Emit();
}

/*****************************************************************************/
Token* Lexer::ProcessStringLiteral() {
    if (_lexeme.empty()) {
        AdvanceBuffer(1);

        while (_lexeme.length() < STRING_LITERAL_MAX_LENGTH) {
            if (Current() == '\"' && Lookback() != '\\') {
                AdvanceBuffer(1);
                break;
            } else if (Current() == '\\') {
                switch (Peek()) {
                    case 'n':
                        _lexeme += '\n';
                        AdvanceBuffer(1);
                        break;
                    case 't':
                        _lexeme += '\t';
                        AdvanceBuffer(1);
                        break;
                    case 'r':
                        _lexeme += '\r';
                        AdvanceBuffer(1);
                        break;
                    default:
                        _lexeme += Current();
                        break;
                }
            } else {
                _lexeme += Current();
            }
            AdvanceBuffer(1);
        }
    }

    return Emit(TokenType::STRING_LITERAL);
}

/*****************************************************************************/
Token* Lexer::Consume() {
    switch (Current()) {
        case '\n':
        case '\t':
        case '\r':
        case ' ':
            if (!_lexeme.empty()) {
                AdvanceBuffer(1);
                return ProcessReservedWord();
            } else {
                while (IsWhiteSpace(Current()) && !EndOfSequence())
                    AdvanceBuffer(1);
                break;
            }
        case '{':
        case '}':
        case '[':
        case ']':
        case '(':
        case ')':
        case ',':
        case ';':
            return ProcessDefault();
        case '/':
            if (GetCompoundCandidate() == BLOCK_COMMENT_START)
                return ProcessComment(true);
            else if (GetCompoundCandidate() == INLINE_COMMENT_START)
                return ProcessComment(false);
            else
                return ProcessOperator();
        case '*':
        case '^':
        case '%':
        case '&':
        case '|':
        case '~':
        case '=':
        case '<':
        case '>':
        case '!':
        case ':':
            return ProcessOperator();
        case '\'':
        case '\"':
            return ProcessStringLiteral();
        case '+':
        case '-': {
            if (Peek() == Current() && tokens.back()->IsOneOf({TokenType::NAME})) {
                if (!_lexeme.empty()) {
                    Emit();
                }
                _lexeme = GetCompoundCandidate();
                AdvanceBuffer(2);
                return Emit();
            } else {
                if ((tokens.back()->GetType() != TokenType::R_PARAN &&
                     !tokens.back()->IsBinaryOp()) ||
                    !_lexeme.empty() || Peek() == '=' || Peek() == '>')
                    return ProcessOperator();
            }
        }
        default: {
            if (Current() != '\r' && Current() != '\t' && Current() != '\n')
                _lexeme += Current();
            AdvanceBuffer(1);
            break;
        }
    }
    return new Token("", TokenType::UNDEFINED);
}

/*****************************************************************************/
void Lexer::ProcessTokens() {
    while (!EndOfSequence())
        Consume();
    Consume();

    // Clear the last _lexeme
    if (!_lexeme.empty())
        Emit();
}

/*****************************************************************************/
void Lexer::LoadFile(const std::string& path) {
    file_info.SetFilePath(path);
    _file_obj.open(file_info.GetFilePath());

    if (_file_obj.fail())
        throw std::runtime_error("Failed to open " + file_info.GetFilePath());

    auto file_size = std::filesystem::file_size(path);
    _data = new char[file_size];
    std::memset(_data, 0, file_size);
    _file_obj.read(_data, file_size);
    _data_size = file_size;
    _file_obj.close();
    AdvanceBuffer(2);
}

/*****************************************************************************/
char Lexer::GetCurrentAsExpectedType() {
    return Current();
}

/*****************************************************************************/
void Lexer::RunAdvanceBufferSideEffects() {
    file_info.IncrementCharNum();

    if (Lookback() == '\n' || Lookback() == '\r') {
        file_info.IncrementLineNum();
        file_info.SetCharNum(1);
        file_info.UpdateLineStartValues(_current_line_start);
        _current_line_start = _data_idx;
    }
}

/*****************************************************************************/
bool Lexer::IsWhiteSpace(char c) {
    std::vector<char> ws_chars = {'\r', '\t', ' ', '\n'};
    return std::find(ws_chars.begin(), ws_chars.end(), c) != ws_chars.end();
}

/*****************************************************************************/
std::string Lexer::ItemToString(char item) {
    return {1, item};
}

/*****************************************************************************/
void Lexer::Reset() {
    for (auto token : tokens) {
        delete token;
    }
    tokens.clear();
}

/*****************************************************************************/
void Lexer::Run() {
    ProcessTokens();
}

/*****************************************************************************/
void Lexer::HandleUnexpectedItem() {
    std::string msg = "Unexpected character '" + std::string(1, Current()) + "'" +
                      " in " + file_info.ToString() + "\nExpected one of [";

    for (auto& expected_char : _expected_items)
        msg.append("'" + std::string(1, expected_char) + "', ");

    if (!_expected_items.empty())
        msg = msg.substr(0, msg.length() - 2);

    msg += "]\n\n";
    msg += file_info.GetContextualBlock();

    throw std::runtime_error(msg);
}
