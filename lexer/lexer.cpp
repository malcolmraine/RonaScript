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

#include "lexer.h"
#include <iostream>
#include <utility>
#include "../exceptions/SourceNotFoundError.h"

/******************************************************************************
 * @brief
 */
Lexer::Lexer() {
    // Initialize the buffer with EOF characters. This will be overwritten when load_file is called
    this->_buf[0] = '\0';
    this->_buf[1] = '\0';
    this->_buf[2] = '\0';

    // Setup the token map.rn
    this->token_map["{"] = TokenType::R_BRACE;
    this->token_map["}"] = TokenType::L_BRACE;
    this->token_map["("] = TokenType::R_PARAN;
    this->token_map[")"] = TokenType::L_PARAN;
    this->token_map["["] = TokenType::R_BRACK;
    this->token_map["]"] = TokenType::L_BRACK;
    this->token_map["<"] = TokenType::R_CARAT;
    this->token_map[">"] = TokenType::L_CARAT;
    this->token_map["/"] = TokenType::SLASH;
    this->token_map["+"] = TokenType::PLUS;
    this->token_map["*"] = TokenType::STAR;
    this->token_map["-"] = TokenType::MINUS;
    this->token_map["%"] = TokenType::PERCENT;
    this->token_map["&"] = TokenType::AMPER;
    this->token_map["|"] = TokenType::BAR;
    this->token_map["^"] = TokenType::UP_ARROW;
    this->token_map["~"] = TokenType::TILDE;
    this->token_map["/*"] = TokenType::BLOCK_COMMENT;
    this->token_map["//"] = TokenType::INLINE_COMMENT;
    this->token_map["'$'"] = TokenType::DOLLAR;
    this->token_map["!"] = TokenType::NOT;
    this->token_map["="] = TokenType::EQUAL;
    this->token_map[","] = TokenType::COMMA;
    this->token_map["."] = TokenType::DOT;
    this->token_map[":"] = TokenType::COLON;
    this->token_map[";"] = TokenType::SEMICOLON;
    this->token_map["\""] = TokenType::DBL_QUOTE;
    this->token_map["++"] = TokenType::DBL_PLUS;
    this->token_map["--"] = TokenType::DBL_MINUS;
    this->token_map["=="] = TokenType::DBL_EQUAL;
    this->token_map["+="] = TokenType::PLUS_EQUAL;
    this->token_map["-="] = TokenType::MINUS_EQUAL;
    this->token_map["%="] = TokenType::PERCENT_EQUAL;
    this->token_map["/="] = TokenType::SLASH_EQUAL;
    this->token_map["!="] = TokenType::NOT_EQUAL;
    this->token_map["&="] = TokenType::AMPER_EQUAL;
    this->token_map["|="] = TokenType::BAR_EQUAL;
    this->token_map["*="] = TokenType::STAR_EQUAL;
    this->token_map["**"] = TokenType::DBL_STAR;
    this->token_map["~="] = TokenType::TILDE_EQUAL;
    this->token_map["&&"] = TokenType::DBL_AMPER;
    this->token_map["||"] = TokenType::DBL_BAR;
    this->token_map[">>"] = TokenType::DBL_R_CARAT;
    this->token_map["<<"] = TokenType::DBL_L_CARAT;
    this->token_map["<="] = TokenType::LEQ;
    this->token_map[">="] = TokenType::GEQ;
    this->token_map["^="] = TokenType::XOREQ;
    this->token_map["->"] = TokenType::R_ARROW;
    this->token_map["float"] = TokenType::FLOAT;
    this->token_map["bool"] = TokenType::BOOL;
    this->token_map["int"] = TokenType::INT;
    this->token_map["string"] = TokenType::STRING;
    this->token_map["object"] = TokenType::OBJECT;
    this->token_map["array"] = TokenType::ARRAY;
    this->token_map["class"] = TokenType::CLASS;
    this->token_map["callable"] = TokenType::CALLABLE;
    this->token_map["var"] = TokenType::VAR;
    this->token_map["const"] = TokenType::CONST;
    this->token_map["public"] = TokenType::PUBLIC;
    this->token_map["protected"] = TokenType::PROTECTED;
    this->token_map["private"] = TokenType::PRIVATE;
    this->token_map["static"] = TokenType::STATIC;
    this->token_map["literal"] = TokenType::LITERAL;
    this->token_map["reference"] = TokenType::REFERENCE;
    this->token_map["func"] = TokenType::FUNC;
    this->token_map["require"] = TokenType::REQUIRE;
    this->token_map["return"] = TokenType::RETURN;
    this->token_map["break"] = TokenType::BREAK;
    this->token_map["continue"] = TokenType::CONTINUE;
    this->token_map["void"] = TokenType::VOID;
    this->token_map["null"] = TokenType::NONE;
    this->token_map["if"] = TokenType::IF;
    this->token_map["elif"] = TokenType::ELIF;
    this->token_map["else"] = TokenType::ELSE;
    this->token_map["is"] = TokenType::IS;
    this->token_map["alias"] = TokenType::ALIAS;
    this->token_map["while"] = TokenType::WHILE;
    this->token_map["for"] = TokenType::FOR;
    this->token_map["type"] = TokenType::TYPE;
    this->token_map["try"] = TokenType::TRY;
    this->token_map["catch"] = TokenType::CATCH;

    // Setup reserved words set
    this->reserved_words.insert("float");
    this->reserved_words.insert("string");
    this->reserved_words.insert("int");
    this->reserved_words.insert("bool");
    this->reserved_words.insert("list");
    this->reserved_words.insert("void");
    this->reserved_words.insert("null");
    this->reserved_words.insert("class");
    this->reserved_words.insert("object");
    this->reserved_words.insert("alias");
    this->reserved_words.insert("is");
    this->reserved_words.insert("require");
    this->reserved_words.insert("class");
    this->reserved_words.insert("func");
    this->reserved_words.insert("construct");
    this->reserved_words.insert("destruct");
    this->reserved_words.insert("var");
    this->reserved_words.insert("const");
    this->reserved_words.insert("public");
    this->reserved_words.insert("protected");
    this->reserved_words.insert("private");
    this->reserved_words.insert("return");
    this->reserved_words.insert("break");
    this->reserved_words.insert("continue");
    this->reserved_words.insert("inherits");
    this->reserved_words.insert("literal");
    this->reserved_words.insert("reference");
    this->reserved_words.insert("static");
    this->reserved_words.insert("callable");
    this->reserved_words.insert("type");
    this->reserved_words.insert("try");
    this->reserved_words.insert("catch");

    this->compounds.insert("++");
    this->compounds.insert("+=");
    this->compounds.insert("--");
    this->compounds.insert("-=");
    this->compounds.insert("&&");
    this->compounds.insert("%=");
    this->compounds.insert("||");
    this->compounds.insert("**");
    this->compounds.insert("&=");
    this->compounds.insert("|=");
    this->compounds.insert("/=");
    this->compounds.insert("*=");
    this->compounds.insert("~=");
    this->compounds.insert("^=");
    this->compounds.insert("!=");
    this->compounds.insert("->");
    this->compounds.insert(">=");
    this->compounds.insert("<=");
    this->compounds.insert("==");
//    this->compounds.insert("[]");
    this->compounds.insert(">>");
    this->compounds.insert("<<");
    this->compounds.insert("::");

    // Setup compound operators set
    this->compound_ops.insert("+=");
    this->compound_ops.insert("-=");
    this->compound_ops.insert("%=");
    this->compound_ops.insert("&=");
    this->compound_ops.insert("|=");
    this->compound_ops.insert("/=");
    this->compound_ops.insert("*=");
    this->compound_ops.insert("~=");
    this->compound_ops.insert("^=");
    this->compound_ops.insert(">=");
    this->compound_ops.insert("<=");

    // Setup compound operators set
    this->unary_ops.insert("++");
    this->unary_ops.insert("--");
    this->unary_ops.insert("**");
    this->unary_ops.insert("!");
    this->unary_ops.insert("~");
    this->unary_ops.insert("-");
    this->unary_ops.insert("+");

    // Setup binary operators set
    this->binary_ops.insert("+");
    this->binary_ops.insert("-");
    this->binary_ops.insert("==");
    this->binary_ops.insert(">=");
    this->binary_ops.insert("<=");
    this->binary_ops.insert("!=");
    this->binary_ops.insert(">");
    this->binary_ops.insert("<");
    this->binary_ops.insert("&");
    this->binary_ops.insert("&&");
    this->binary_ops.insert("|");
    this->binary_ops.insert("||");
    this->binary_ops.insert("^");
    this->binary_ops.insert("/");
    this->binary_ops.insert("%");
    this->binary_ops.insert("*");
}

/******************************************************************************
 * @brief
 */
Lexer::~Lexer() {
    file_obj.close();

    for (auto &token : this->tokens) {
        delete token;
    }
    tokens.clear();
}

/******************************************************************************
 * @brief Get the next character without advancing the buffer.
 * @return
 */
char Lexer::peek() {
    return this->_buf[2];
}

/******************************************************************************
 * @brief Get the current character
 * @return
 */
char Lexer::current() {
    return this->_buf[1];
}

/******************************************************************************
 * @brief Get the previous character
 * @return
 */
char Lexer::lookback() {
    return this->_buf[0];
}

/******************************************************************************
 * @brief Advance the buffer n steps
 * @param n Number of characters to advance
 */
void Lexer::adv_buf(int n) {
    for (int i = 0; i < n; i++) {
        this->_buf[0] = this->_buf[1];
        this->_buf[1] = this->_buf[2];
        this->file_obj.get(this->_buf[2]);
        this->char_idx++;

        if (current() == '\n') {
            this->line_num++;
            this->char_num = 0;
        } else {
            this->char_num++;
        }
    }
}

/******************************************************************************
 * @brief Process and add a token to the token set
 * @return
 */
Token *Lexer::emit() {
    auto token = make_token(this->lexeme);
    this->lexeme.clear();
    this->tokens.emplace_back(token);

    return token;
}

/******************************************************************************
 * @brief
 * @param c1
 */
void Lexer::expect(char c1) {

}

/******************************************************************************
 * @brief Make a new Token object from a string.
 * @param s Input string
 * @return Token instance.
 */
Token *Lexer::make_token(const std::string &s) {
    auto token = new Token(s, TokenType::UNDEFINED);
    token->file_pos.char_num = char_num;
    token->file_pos.line_num = line_num;

    if (token_map.count(s) || (token_map.count(s) && is_reserved_word(s))) {
        token->tok_type = token_map[s];
    } else if (is_int_literal(s)) {
        token->tok_type = TokenType::INT_LITERAL;
    } else if (is_float_literal(s)) {
        token->tok_type = TokenType::FLOAT_LITERAL;
    } else if (is_str_literal(s)) {
        token->tok_type = TokenType::STRING_LITERAL;
    } else if (is_bool_literal(s)) {
        token->tok_type = TokenType::BOOL_LITERAL;
    } else {
        token->tok_type = TokenType::NAME;
    }
    return token;
}

/******************************************************************************
 * @brief Make a lexeme-free Token instance.
 * @param type Token type to use.
 * @return Token instance.
 */
Token Lexer::make_token(TokenType::TokenType_t type) {
    return Token("", type);
}

/******************************************************************************
 * @brief Determine if a string contains an integer representation.
 * @param s
 * @return bool
 */
bool Lexer::is_int_literal(std::string s) {
    for (int i = IS_NEGATIVE_LITERAL(s) ? 1 : 0; i < s.length(); i++) {
        if (!std::isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

/******************************************************************************
 * @brief Determine if a string contains a float representation.
 * @param s
 * @return bool
 */
bool Lexer::is_float_literal(std::string s) {
    bool decimal_found = false;

    for (int i = IS_NEGATIVE_LITERAL(s) ? 1 : 0; i < s.length(); i++) {
        if (!std::isdigit(s[i])) {
            if (s[i] == '.' && !decimal_found) {
                decimal_found = true;
            } else {
                return false;
            }
        }
    }
    return decimal_found;
}

/******************************************************************************
 * @brief Determine if a string contains a hex representation.
 * @param s
 * @return bool
 */
bool Lexer::is_hex_literal(std::string s) {
    if (std::string(1, s[0]) + std::string(1, s[1]) == HEX_LITERAL_PREFIX) {
        for (int i = 2; i < s.length(); i++) {
            if (!std::isdigit(s[i])) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

/******************************************************************************
 * @brief Determine if a string is a boolean literal value.
 * @param s
 * @return bool
 */
bool Lexer::is_bool_literal(const std::string &s) {
    return s == "true" || s == "false";
}

/******************************************************************************
 * @brief
 * @param s
 * @return bool
 */
bool Lexer::is_str_literal(const std::string &s) {
    return s[0] == '"' && s[s.length() - 1] == '"';
}

/******************************************************************************
 * @brief
 * @param s
 * @return bool
 */
bool Lexer::is_cmpnd(const std::string &s) {
    return this->compounds.find(get_cmpnd_candidate()) != this->compounds.end();
}

/******************************************************************************
 * @brief
 * @param s
 * @return bool
 */
bool Lexer::is_reserved_word(const std::string &s) {
    return this->reserved_words.find(s) != this->reserved_words.end();
}

/******************************************************************************
 * @brief
 * @return
 */
std::string Lexer::get_cmpnd_candidate() {
    return std::string(1, current()) + std::string(1, peek());
}

/******************************************************************************
 * @brief
 * @param lexeme
 * @return
 */
Token *Lexer::reserved_word_handler(std::string lexeme) {
    return emit();
}

/******************************************************************************
 * @brief
 * @return
 */
Token *Lexer::default_handler() {
    if (this->lexeme.empty()) {
        this->lexeme = std::string(1, current());
        adv_buf(1);
    }
    return emit();
}

/******************************************************************************
 * @brief
 * @param is_block_comment
 * @return
 */
Token *Lexer::comment_handler(bool is_block_comment) {
    if (this->lexeme.empty()) {
        if (is_block_comment) {
            while (get_cmpnd_candidate() != BLOCK_COMMENT_END && !eof()) {
                adv_buf(1);
            }
        } else {
            while (current() != INLINE_COMMENT_END) {
                adv_buf(1);
            }
        }
        adv_buf(1);

        return new Token("", TokenType::UNDEFINED);
    } else {
        return emit();
    }
}


/******************************************************************************
 * @brief
 * @return
 */
Token *Lexer::op_handler() {
    if (this->lexeme.empty()) {
        if (is_cmpnd(get_cmpnd_candidate())) {
            this->lexeme = get_cmpnd_candidate();
            adv_buf(2);
        } else {
            this->lexeme = std::string(1, current());
            adv_buf(1);
        }
    }

    return emit();
}

/******************************************************************************
 * @brief
 * @param s
 * @return
 */
Token *Lexer::string_literal_handler() {
    if (this->lexeme.empty()) {
        this->lexeme += std::string(1, current());
        adv_buf(1);
        bool end_quote_found = false;

        while (!end_quote_found && lexeme.length() < STRING_LITERAL_MAX_LENGTH) {
            if (current() == '\"' && lookback() != '\\') {
                end_quote_found = true;
            }
            this->lexeme += std::string(1, current());
            adv_buf(1);
        }
    }

    return emit();
}

/******************************************************************************
 * @brief
 * @return
 */
Token *Lexer::get_token() {
    switch (current()) {
        case ' ':
            if (!this->lexeme.empty()) {

                return reserved_word_handler(this->lexeme);
            } else {
                while (current() == ' ') {
                    adv_buf(1);
                }
                break;
            }
        case '{':
        case '}':
        case '[':
        case ']':
        case '(':
        case ')':
        case ',':
        case ':':
        case ';':
            return default_handler();
        case '/':
            if (get_cmpnd_candidate() == BLOCK_COMMENT_START) {
                return comment_handler(true);
            } else if (get_cmpnd_candidate() == INLINE_COMMENT_START) {
                return comment_handler(false);
            } else {
                return op_handler();
            }
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
            return op_handler();
        case '\'':
        case '\"':
            return string_literal_handler();
        default:
            if (current() != '\r' && current() != '\t' && current() != '\n') {
                this->lexeme += std::string(1, current());
            }
            adv_buf(1);
            break;
    }
    return new Token("", TokenType::UNDEFINED);
}

/******************************************************************************
 * @brief
 */
void Lexer::get_tokens() {
    while (!eof()) {
        get_token();
    }

    // Clear the last lexeme
    if (!this->lexeme.empty()) {
        emit();
    }
}

/******************************************************************************
 * @brief Setup the file object for reading character into the buffer
 * @param path
 */
void Lexer::load_file(const std::string &path) {
    this->_file_path = path;
    this->file_obj.open(this->_file_path);

    if (file_obj.fail()) {
        throw SourceNotFoundError(path);
    }

    // Get the number of characters in the file
    this->file_obj.seekg(0, std::ios_base::end);
    std::streampos end_pos = this->file_obj.tellg();
    this->file_char_cnt = end_pos;

    // Reset the file position
    this->file_obj.seekg(0, std::ios_base::beg);

    // Initialize the buffer
    adv_buf(2);
}

/******************************************************************************
 * @brief Checks if the end of file has been reached.
 * @return
 */
bool Lexer::eof() {
    return this->char_idx >= this->file_char_cnt + 2;
}