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
#include <iostream>
#include <utility>

/*****************************************************************************/
Lexer::Lexer() {
    file_info.SetFilePath(_file_path);
    // Initialize the buffer with EOF characters.
    FillBuffer('\0');

    // Set up the token map.rn
    _token_map["{"] = TokenType::R_BRACE;
    _token_map["}"] = TokenType::L_BRACE;
    _token_map["("] = TokenType::R_PARAN;
    _token_map[")"] = TokenType::L_PARAN;
    _token_map["["] = TokenType::R_BRACK;
    _token_map["]"] = TokenType::L_BRACK;
    _token_map["<"] = TokenType::R_CARAT;
    _token_map[">"] = TokenType::L_CARAT;
    _token_map["/"] = TokenType::SLASH;
    _token_map["+"] = TokenType::PLUS;
    _token_map["*"] = TokenType::STAR;
    _token_map["-"] = TokenType::MINUS;
    _token_map["%"] = TokenType::PERCENT;
    _token_map["&"] = TokenType::AMPER;
    _token_map["|"] = TokenType::BAR;
    _token_map["^"] = TokenType::UP_ARROW;
    _token_map["~"] = TokenType::TILDE;
    _token_map["/*"] = TokenType::BLOCK_COMMENT;
    _token_map["//"] = TokenType::INLINE_COMMENT;
    _token_map["'$'"] = TokenType::DOLLAR;
    _token_map["!"] = TokenType::NOT;
    _token_map["="] = TokenType::EQUAL;
    _token_map[","] = TokenType::COMMA;
    _token_map["."] = TokenType::DOT;
    _token_map[":"] = TokenType::COLON;
    _token_map[";"] = TokenType::SEMICOLON;
    _token_map["\""] = TokenType::DBL_QUOTE;
    _token_map["++"] = TokenType::DBL_PLUS;
    _token_map["--"] = TokenType::DBL_MINUS;
    _token_map["=="] = TokenType::DBL_EQUAL;
    _token_map["+="] = TokenType::PLUS_EQUAL;
    _token_map["-="] = TokenType::MINUS_EQUAL;
    _token_map["%="] = TokenType::PERCENT_EQUAL;
    _token_map["/="] = TokenType::SLASH_EQUAL;
    _token_map["!="] = TokenType::NOT_EQUAL;
    _token_map["&="] = TokenType::AMPER_EQUAL;
    _token_map["|="] = TokenType::BAR_EQUAL;
    _token_map["*="] = TokenType::STAR_EQUAL;
    _token_map["**"] = TokenType::DBL_STAR;
    _token_map["~="] = TokenType::TILDE_EQUAL;
    _token_map["&&"] = TokenType::DBL_AMPER;
    _token_map["||"] = TokenType::DBL_BAR;
    _token_map[">>"] = TokenType::DBL_R_CARAT;
    _token_map["<<"] = TokenType::DBL_L_CARAT;
    _token_map["<="] = TokenType::LEQ;
    _token_map[">="] = TokenType::GEQ;
    _token_map["^="] = TokenType::XOREQ;
    _token_map["->"] = TokenType::R_ARROW;
    _token_map["::"] = TokenType::DOUBLE_COLON;
    _token_map["float"] = TokenType::FLOAT;
    _token_map["bool"] = TokenType::BOOL;
    _token_map["int"] = TokenType::INT;
    _token_map["string"] = TokenType::STRING;
    _token_map["object"] = TokenType::OBJECT;
    _token_map["array"] = TokenType::ARRAY;
    _token_map["class"] = TokenType::CLASS;
    _token_map["callable"] = TokenType::CALLABLE;
    _token_map["var"] = TokenType::VAR;
    _token_map["const"] = TokenType::CONST;
    _token_map["public"] = TokenType::PUBLIC;
    _token_map["protected"] = TokenType::PROTECTED;
    _token_map["private"] = TokenType::PRIVATE;
    _token_map["static"] = TokenType::STATIC;
    _token_map["literal"] = TokenType::LITERAL;
    _token_map["reference"] = TokenType::REFERENCE;
    _token_map["func"] = TokenType::FUNC;
    _token_map["routine"] = TokenType::FUNC;
    _token_map["sub"] = TokenType::FUNC;
    _token_map["begin"] = TokenType::R_BRACE;
    _token_map["end"] = TokenType::L_BRACE;
    _token_map["import"] = TokenType::IMPORT;
    _token_map["module"] = TokenType::MODULE;
    _token_map["return"] = TokenType::RETURN;
    _token_map["break"] = TokenType::BREAK;
    _token_map["continue"] = TokenType::CONTINUE;
    _token_map["delete"] = TokenType::DELETE;
    _token_map["extends"] = TokenType::EXTENDS;
    _token_map["exit"] = TokenType::EXIT;
    _token_map["void"] = TokenType::VOID;
    _token_map["null"] = TokenType::NULL_LITERAL;
    _token_map["if"] = TokenType::IF;
    _token_map["elif"] = TokenType::ELIF;
    _token_map["else"] = TokenType::ELSE;
    _token_map["is"] = TokenType::IS;
    _token_map["alias"] = TokenType::ALIAS;
    _token_map["while"] = TokenType::WHILE;
    _token_map["for"] = TokenType::FOR;
    _token_map["type"] = TokenType::TYPE;
    _token_map["try"] = TokenType::TRY;
    _token_map["global"] = TokenType::GLOBAL;
    _token_map["local"] = TokenType::LOCAL;
    _token_map["?"] = TokenType::QUESTION_MARK;
    _token_map["??"] = TokenType::DBL_QUESTION_MARK;

    // Setup reserved words set
    _reserved_words.insert("float");
    _reserved_words.insert("string");
    _reserved_words.insert("int");
    _reserved_words.insert("bool");
    _reserved_words.insert("list");
    _reserved_words.insert("void");
    _reserved_words.insert("null");
    _reserved_words.insert("class");
    _reserved_words.insert("object");
    _reserved_words.insert("alias");
    _reserved_words.insert("is");
    _reserved_words.insert("require");
    _reserved_words.insert("class");
    _reserved_words.insert("func");
    _reserved_words.insert("routine");
    _reserved_words.insert("sub");
    _reserved_words.insert("begin");
    _reserved_words.insert("end");
    _reserved_words.insert("construct");
    _reserved_words.insert("destruct");
    _reserved_words.insert("var");
    _reserved_words.insert("const");
    _reserved_words.insert("public");
    _reserved_words.insert("protected");
    _reserved_words.insert("private");
    _reserved_words.insert("return");
    _reserved_words.insert("break");
    _reserved_words.insert("continue");
    _reserved_words.insert("inherits");
    _reserved_words.insert("literal");
    _reserved_words.insert("reference");
    _reserved_words.insert("static");
    _reserved_words.insert("callable");
    _reserved_words.insert("type");
    _reserved_words.insert("try");
    _reserved_words.insert("catch");
    _reserved_words.insert("global");
    _reserved_words.insert("local");
    _reserved_words.insert("exit");
    _reserved_words.insert("delete");
    _reserved_words.insert("extends");
    _reserved_words.insert("import");
    _reserved_words.insert("module");

    _compounds.insert("++");
    _compounds.insert("+=");
    _compounds.insert("--");
    _compounds.insert("-=");
    _compounds.insert("&&");
    _compounds.insert("%=");
    _compounds.insert("||");
    _compounds.insert("**");
    _compounds.insert("&=");
    _compounds.insert("|=");
    _compounds.insert("/=");
    _compounds.insert("*=");
    _compounds.insert("~=");
    _compounds.insert("^=");
    _compounds.insert("!=");
    _compounds.insert("->");
    _compounds.insert("::");
    _compounds.insert(">=");
    _compounds.insert("<=");
    _compounds.insert("==");
    //    _compounds.insert("[]");
    _compounds.insert(">>");
    _compounds.insert("<<");
    _compounds.insert("::");
    _compounds.insert("??");

    // Setup compound operators set
    _compound_ops.insert("+=");
    _compound_ops.insert("-=");
    _compound_ops.insert("%=");
    _compound_ops.insert("&=");
    _compound_ops.insert("|=");
    _compound_ops.insert("/=");
    _compound_ops.insert("*=");
    _compound_ops.insert("~=");
    _compound_ops.insert("^=");
    _compound_ops.insert(">=");
    _compound_ops.insert("<=");

    _unary_ops.insert("++");
    _unary_ops.insert("--");
    _unary_ops.insert("**");
    _unary_ops.insert("!");
    _unary_ops.insert("~");
    _unary_ops.insert("-");
    _unary_ops.insert("+");

    // Setup binary operators set
    _binary_ops.insert("+");
    _binary_ops.insert("-");
    _binary_ops.insert("==");
    _binary_ops.insert(">=");
    _binary_ops.insert("<=");
    _binary_ops.insert("!=");
    _binary_ops.insert(">");
    _binary_ops.insert("<");
    _binary_ops.insert("&");
    _binary_ops.insert("&&");
    _binary_ops.insert("|");
    _binary_ops.insert("||");
    _binary_ops.insert("^");
    _binary_ops.insert("/");
    _binary_ops.insert("%");
    _binary_ops.insert("*");
    _binary_ops.insert("??");
    _binary_ops.insert("->");
    _binary_ops.insert("::");
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

    if (_token_map.count(s) || (_token_map.count(s) && IsReservedWord(s))) {
        token->token_type = _token_map[s];
    } else if (IsIntLiteral(s)) {
        token->token_type = TokenType::INT_LITERAL;
    } else if (IsFloatLiteral(s)) {
        token->token_type = TokenType::FLOAT_LITERAL;
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
    for (size_t i = IS_NEGATIVE_LITERAL(s) ? 1 : 0; i < s.length(); ++i) {
        if (!std::isdigit(s[i]))
            return false;
    }
    return true;
}

/*****************************************************************************/
bool Lexer::IsFloatLiteral(std::string s) {
    bool decimal_found = false;
    for (size_t i = IS_NEGATIVE_LITERAL(s) ? 1 : 0; i < s.length(); ++i) {
        if (!std::isdigit(s[i])) {
            if (s[i] == '.' && !decimal_found)
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
    return s[0] == '"' && s[s.length() - 1] == '"';
}

/*****************************************************************************/
bool Lexer::IsCompound() const {
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
        case '+':
        case '-':
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
bool Lexer::EndOfFile() const {
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
    if (Current() == '\n') {
        file_info.IncrementLineNum();
        file_info.UpdateLineStartValues(file_info.GetCharCount());
        file_info.SetCharNum(0);
    } else {
        file_info.IncrementCharNum();
    }

    _file_obj.get(_buffer[2]);
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
