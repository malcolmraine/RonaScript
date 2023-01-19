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
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sys/stat.h>
#include "Token.h"
#include "TokenType.h"
#include "../util/FileInfo.h"
#include "../util/RonaSequencer.h"

#define STRING_LITERAL_MAX_LENGTH 1000000
#define BLOCK_COMMENT_START "/*"
#define BLOCK_COMMENT_END "*/"
#define INLINE_COMMENT_START "//"
#define INLINE_COMMENT_END_N '\n'
#define INLINE_COMMENT_END_R '\r'
#define HEX_LITERAL_PREFIX "0x"
#define OCTAL_LITERAL_PREFIX "0o"
#define IS_NEGATIVE_LITERAL(s) ((s)[0] == '-')

class Lexer : public RonaSequencer<char, char>
{
 public:
	Lexer();
	~Lexer();
	void LoadNextItem() override;
	Token* Emit();
	Token* MakeToken(TokenType type);
	Token* MakeToken(const std::string& s);
	static bool IsIntLiteral(std::string s);
	static bool IsFloatLiteral(std::string s);
	static bool IsHexLiteral(std::string s);
	static bool IsBoolLiteral(const std::string& s);
	static bool IsStrLiteral(const std::string& s);
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
	bool EndOfFile() const;  // TODO: Use RonaSequencer
	bool EndOfSequence() const override;
	char GetCurrentAsExpectedType() override;
	void HandleUnexpectedItem() override;
	void RunAdvanceBufferSideEffects() override;
	bool IsWhiteSpace(char c) const;
	std::string ItemToString(char item) override;

	std::vector<Token*> tokens;
	std::string _lexeme;

	int line_num = 1;
	int char_num = 1;
	FileInfo file_info;
	long file_char_cnt = -1;

 protected:
	std::string _file_path;
	long _char_idx = 0;
	std::ifstream _file_obj;
	std::unordered_map<std::string, TokenType> _token_map{};
	std::unordered_set<std::string> _compounds;
	std::unordered_set<std::string> _binary_ops;
	std::unordered_set<std::string> _unary_ops;
	std::unordered_set<std::string> _reserved_words;
	std::unordered_set<std::string> _compound_ops;
	std::vector<std::string> _error_messages;
	char _buf[3]{};
};

#endif //RONASCRIPT_LEXER_H