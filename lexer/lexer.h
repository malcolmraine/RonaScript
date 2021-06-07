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

#ifndef RONASCRIPT_LEXER_H
#define RONASCRIPT_LEXER_H

#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <sys/stat.h>
#include "token.h"
#include "token_type.h"

#define STRING_LITERAL_MAX_LENGTH 1000000
#define BLOCK_COMMENT_START "/*"
#define BLOCK_COMMENT_END "*/"
#define INLINE_COMMENT_START "//"
#define INLINE_COMMENT_END '\n'
#define HEX_LITERAL_PREFIX "0x"
#define IS_NEGATIVE_LITERAL(s) ((s)[0] == '-')

class Lexer {
public:
    Lexer();
    ~Lexer();
    char peek();
    char current();
    char lookback();
    void adv_buf(int n);
    Token *emit();
    void expect(char c1);
    Token *make_token(const std::string &s);
    static Token make_token(TokenType::TokenType_t type);
    static bool is_int_literal(std::string s);
    static bool is_float_literal(std::string s);
    static bool is_hex_literal(std::string s);
    static bool is_bool_literal(const std::string &s);
    static bool is_str_literal(const std::string &s);
    bool is_cmpnd(const std::string &s);
    bool is_reserved_word(const std::string &s);
    std::string get_cmpnd_candidate();
    Token *reserved_word_handler(std::string lexeme);
    Token *default_handler();
    Token *comment_handler(bool is_block_comment = false);
    Token *op_handler();
    Token *string_literal_handler();
    Token *get_token();
    void get_tokens();
    void load_file(const std::string &path);
    bool eof();

    std::vector<Token *> tokens;
    std::string lexeme;
    int line_num = -1;
    int char_num = -1;
    long file_char_cnt = -1;

protected:
    std::string _file_path;
    long char_idx = 0;
    std::ifstream file_obj;
    std::map<std::string, TokenType::TokenType_t> token_map;
    std::set<std::string> compounds;
    std::set<std::string> binary_ops;
    std::set<std::string> unary_ops;
    std::set<std::string> reserved_words;
    std::set<std::string> compound_ops;
    char _buf[3]{};
};

#endif //RONASCRIPT_LEXER_H