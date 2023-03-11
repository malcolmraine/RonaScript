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
#include "Lexer.h"
#include <algorithm>
#include <iostream>
#include <utility>
#include "Token.h"
#include "TokenType.h"

std::unordered_map<std::string, TokenType> Lexer::_token_map = {
    {"{", TokenType::R_BRACE},
    {"}", TokenType::L_BRACE},
    {"(", TokenType::R_PARAN},
    {")", TokenType::L_PARAN},
    {"[", TokenType::R_BRACK},
    {"]", TokenType::L_BRACK},
    {"<", TokenType::R_CARAT},
    {">", TokenType::L_CARAT},
    {"/", TokenType::SLASH},
    {"+", TokenType::PLUS},
    {"*", TokenType::STAR},
    {"-", TokenType::MINUS},
    {"%", TokenType::PERCENT},
    {"&", TokenType::AMPER},
    {"|", TokenType::BAR},
    {"^", TokenType::UP_ARROW},
    {"~", TokenType::TILDE},
    {"/*", TokenType::BLOCK_COMMENT},
    {"//", TokenType::INLINE_COMMENT},
    {"'$'", TokenType::DOLLAR},
    {"!", TokenType::NOT},
    {"=", TokenType::EQUAL},
    {",", TokenType::COMMA},
    {".", TokenType::DOT},
    {":", TokenType::COLON},
    {";", TokenType::SEMICOLON},
    {"\"", TokenType::DBL_QUOTE},
    {"++", TokenType::DBL_PLUS},
    {"--", TokenType::DBL_MINUS},
    {"==", TokenType::DBL_EQUAL},
    {"+=", TokenType::PLUS_EQUAL},
    {"-=", TokenType::MINUS_EQUAL},
    {"%=", TokenType::PERCENT_EQUAL},
    {"/=", TokenType::SLASH_EQUAL},
    {"!=", TokenType::NOT_EQUAL},
    {"&=", TokenType::AMPER_EQUAL},
    {"|=", TokenType::BAR_EQUAL},
    {"*=", TokenType::STAR_EQUAL},
    {"**", TokenType::DBL_STAR},
    {"~=", TokenType::TILDE_EQUAL},
    {"&&", TokenType::DBL_AMPER},
    {"||", TokenType::DBL_BAR},
    {">>", TokenType::DBL_R_CARAT},
    {"<<", TokenType::DBL_L_CARAT},
    {"<=", TokenType::LEQ},
    {">=", TokenType::GEQ},
    {"^=", TokenType::XOREQ},
    {"->", TokenType::R_ARROW},
    {"::", TokenType::DOUBLE_COLON},
    {"float", TokenType::FLOAT},
    {"bool", TokenType::BOOL},
    {"int", TokenType::INT},
    {"string", TokenType::STRING},
    {"object", TokenType::OBJECT},
    {"array", TokenType::ARRAY},
    {"class", TokenType::CLASS},
    {"callable", TokenType::CALLABLE},
    {"var", TokenType::VAR},
    {"const", TokenType::CONST},
    {"public", TokenType::PUBLIC},
    {"protected", TokenType::PROTECTED},
    {"private", TokenType::PRIVATE},
    {"static", TokenType::STATIC},
    {"literal", TokenType::LITERAL},
    {"reference", TokenType::REFERENCE},
    {"func", TokenType::FUNC},
    {"routine", TokenType::FUNC},
    {"sub", TokenType::FUNC},
    {"begin", TokenType::R_BRACE},
    {"end", TokenType::L_BRACE},
    {"import", TokenType::IMPORT},
    {"module", TokenType::MODULE},
    {"return", TokenType::RETURN},
    {"break", TokenType::BREAK},
    {"continue", TokenType::CONTINUE},
    {"delete", TokenType::DELETE},
    {"extends", TokenType::EXTENDS},
    {"exit", TokenType::EXIT},
    {"void", TokenType::VOID},
    {"null", TokenType::NULL_LITERAL},
    {"if", TokenType::IF},
    {"elif", TokenType::ELIF},
    {"else", TokenType::ELSE},
    {"is", TokenType::IS},
    {"alias", TokenType::ALIAS},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"type", TokenType::TYPE},
    {"try", TokenType::TRY},
    {"global", TokenType::GLOBAL},
    {"local", TokenType::LOCAL},
    {"?", TokenType::QUESTION_MARK},
    {"??", TokenType::DBL_QUESTION_MARK},
};

const std::unordered_set<std::string> Lexer::_reserved_words = {
    "float",    "string",  "int",       "bool",    "list",      "void",     "null",
    "class",    "object",  "alias",     "is",      "require",   "class",    "func",
    "routine",  "sub",     "begin",     "end",     "construct", "destruct", "var",
    "const",    "public",  "protected", "private", "return",    "break",    "continue",
    "inherits", "literal", "reference", "static",  "callable",  "type",     "try",
    "catch",    "global",  "local",     "exit",    "delete",    "extends",  "import",
    "module",
};

const std::unordered_set<std::string> Lexer::_compounds = {
    "++", "+=", "--", "-=", "&&", "%=", "||", "**", "&=", "|=", "/=", "*=",
    "~=", "^=", "!=", "->", "::", ">=", "<=", "==", ">>", "<<", "::", "??",
};

const std::unordered_set<std::string> Lexer::_compound_ops = {
    "+="
    "-="
    "%="
    "&="
    "|="
    "/="
    "*="
    "~="
    "^="
    ">="
    "<="};

const std::unordered_set<std::string> Lexer::_unary_ops = {
    "++"
    "--"
    "**"
    "!"
    "~"
    "-"
    "+"};

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
    "::"};

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
Token* Lexer::Emit() {
    auto token = MakeToken(_lexeme);

    // This is a little awkward but it handles repeated unary operators
    if (token->token_type == TokenType::NAME &&
        (token->lexeme[0] == '+' || token->lexeme[0] == '-')) {
        tokens.emplace_back(MakeToken(std::string(1, token->lexeme[0])));
        token->lexeme = token->lexeme.substr(1);
    }
    _lexeme.clear();
    tokens.emplace_back(token);

    return token;
}

/*****************************************************************************/
Token* Lexer::MakeToken(const std::string& s) {
    auto token = new Token(s, TokenType::UNDEFINED);
    token->file_info = FileInfo(file_info);
    token->file_pos.char_num = char_num;
    token->file_pos.line_num = line_num;

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

    if (_token_map.count(s) || (_token_map.count(s) && IsReservedWord(s))) {
        token->token_type = _token_map[s];
    } else if (IsIntLiteral(s)) {
        token->token_type = TokenType::INT_LITERAL;
        token->lexeme = normalize_sign(s);
    } else if (IsFloatLiteral(s)) {
        token->token_type = TokenType::FLOAT_LITERAL;
        token->lexeme = normalize_sign(s);
    } else if (IsStrLiteral(s)) {
        token->token_type = TokenType::STRING_LITERAL;
    } else if (IsBoolLiteral(s)) {
        token->token_type = TokenType::BOOL_LITERAL;
    } else {
        token->token_type = TokenType::NAME;
    }
    return token;
}

/*****************************************************************************/
bool Lexer::IsIntLiteral(std::string s) {
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
bool Lexer::IsFloatLiteral(std::string s) {
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
bool Lexer::IsHexLiteral(std::string s) {
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
bool Lexer::IsReservedWord(const std::string& s) const {
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
            while (GetCompoundCandidate() != BLOCK_COMMENT_END && !EndOfFile())
                AdvanceBuffer(1);
        } else {
            while (Current() != INLINE_COMMENT_END_N &&
                   Current() != INLINE_COMMENT_END_R && !EndOfFile())
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
        _lexeme.append(std::string(1, Current()));
        AdvanceBuffer(1);
        bool end_quote_found = false;

        while (!end_quote_found && _lexeme.length() < STRING_LITERAL_MAX_LENGTH) {
            if (Current() == '\"' && Lookback() != '\\')
                end_quote_found = true;

            _lexeme.append(std::string(1, Current()));
            AdvanceBuffer(1);
        }
    }

    return Emit();
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
                while (IsWhiteSpace(Current()) && !EndOfFile())
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
        case '-':
            if ((tokens.back()->token_type != TokenType::R_PARAN &&
                 !tokens.back()->IsBinaryOp()) ||
                !_lexeme.empty() || Peek() == '=' || Peek() == '>')
                return ProcessOperator();
        default: {
            if (Current() != '\r' && Current() != '\t' && Current() != '\n')
                _lexeme.append(std::string(1, Current()));
            AdvanceBuffer(1);
            break;
        }
    }
    return new Token("", TokenType::UNDEFINED);
}

/*****************************************************************************/
void Lexer::ProcessTokens() {
    while (!EndOfFile())
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

    // Get the number of characters in the file
    _file_obj.seekg(0, std::ios_base::end);
    std::streampos end_pos = _file_obj.tellg();
    file_char_cnt = end_pos;

    // Reset the file position
    _file_obj.seekg(0, std::ios_base::beg);

    // Initialize the buffer
    AdvanceBuffer(2);
}

/*****************************************************************************/
void Lexer::LoadString(const std::string& input) {
    _use_loaded_string = true;
    _data.clear();
    _data.reserve(input.length());
    for (auto c : input) {
        _data.push_back(c);
    }
}

/*****************************************************************************/
bool Lexer::EndOfFile() const {
    if (_use_loaded_string) {
        return _data_idx == _data.size() - 1;
    }
    return _file_obj.gcount() == 0;
}

/*****************************************************************************/
bool Lexer::EndOfSequence() const {
    return EndOfFile();
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
    }
}

/*****************************************************************************/
bool Lexer::IsWhiteSpace(char c) const {
    std::vector<char> ws_chars = {'\r', '\t', ' ', '\n'};
    return std::find(ws_chars.begin(), ws_chars.end(), c) != ws_chars.end();
}

/*****************************************************************************/
std::string Lexer::ItemToString(char item) {
    return {1, item};
}

/*****************************************************************************/
void Lexer::LoadNextItem() {
    if (_use_loaded_string) {
        _buffer[2] = _data[_data_idx++];
    } else {
        if (Current() == '\n') {
            file_info.IncrementLineNum();
            file_info.UpdateLineStartValues(file_info.GetCharCount());
            file_info.SetCharNum(0);
        } else {
            file_info.IncrementCharNum();
        }

        _file_obj.get(_buffer[2]);
    }
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
