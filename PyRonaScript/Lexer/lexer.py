#!/usr/bin/env python
"""
File: Lexer.py
Description:
Author: Malcolm Hall
Version: 1

MIT License

Copyright (c) 2020 Malcolm Hall

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

from Lexer.tokens import Token, TokenType
from cpp_helpers import *
from .lexer_base import LexerBase

"""

FILE FORMATS:
    - *.rn Source code file
    - *.rnc Compiled byte code file
        * running a byte code file will skip the Lexer, Parser, and code generation stages, 
            jumping directly to the runtime stage
HANDLERS:
    - string literals
    - bool constants
    - int constants
    - double constants
    - reserved words
    - default handler
    - compound operators
    - comment
    

PARSER NOTES:
    - objects can have an associated type (function, var, etc)
    - scope state will need to have transition functions
    - need scope cleanup function 
    - should have an expect() function to predict the next token
        * raise exception if prediction is wrong
    - Object classification, object table creation, scoping, import files list, 
    
    
BUILT-IN FUNCTIONS:
    - print
    - input
    - range
    - len
    - 

SCOPING:
    - Objects are declared they will be given a scope level  
    - The object store/lookup will go like this:
        scope->name
    - Conflicting names in different scopes will defer to the closest scope
    - When referencing a variable, the scope level will be checked for the variable name
        then, if the variable exists, it will be used.
    - A scope can access resources in all scopes with a lower scope level. Attempting to access
        a higher level will result in an error.
        
    
"""


class Lexer(LexerBase):
    def __init__(self):
        super().__init__()
        self.reserved_words = [
            "float",
            "string",
            "alias",
            "is"
            "list",
            "int",
            "bool",
            "void",
            "require",
            "class",
            "function" "var",
            "return",
            "null",
        ]
        self.compounds = [
            "++",
            "+=",
            "--",
            "-=",
            "&&",
            "%=",
            "||",
            "**",
            "&=",
            "|=",
            "/=",
            "*=",
            "~=",
            "^=",
            "!=",
            "->",
            ">=",
            "<=",
            "[]",
        ]
        self.ignore_comments = True

    def emit(self, token, n=1):
        """

        :param token:
        :param n:
        :return:
        """
        self.tok_str = ""
        self.adv_buf(n)
        if isinstance(token, Token):
            self.tokens.append(token)
        else:
            self.tokens.append(Token(token))
        return self.tokens[-1]

    def is_cmpnd(self):
        """

        :return:
        """
        return in_list(self.get_cmpnd_candidate(), self.compounds)

    def get_cmpnd_candidate(self):
        """

        :return:
        """
        return self.current() + self.peek()

    def reserved_word_handler(self, token):
        """

        :param token:
        :return:
        """
        return self.emit(token, 0)

    def default_handler(self, token):
        """

        :param token:
        :return:
        """
        if self.tok_str == "":
            return self.emit(token)
        else:
            return self.emit(self.tok_str, 0)

    def comment_handler(self, token):
        """

        :param token:
        :return:
        """
        if self.tok_str == "":
            if self.peek() == "/":
                while self.current() != "\n":
                    self.tok_str += self.current()
                    self.adv_buf()
            else:
                while (self.current() + self.peek()) != "*/":
                    self.tok_str += self.current()
                    self.adv_buf()
                self.tok_str += self.current()
                self.adv_buf()
                self.tok_str += self.current()
            return self.emit(Token(self.tok_str, TokenType.COMMENT))
        else:
            return self.emit(self.tok_str)

    def cmpnd_handler(self, token):
        """

        :param token:
        :return:
        """
        if self.tok_str == "":
            if self.is_cmpnd():
                return self.emit(self.get_cmpnd_candidate(), 2)

            elif token == "[" and self.peek() == "]":
                return self.emit("[]", 2)

            else:
                return self.emit(token)

        else:
            return self.emit(self.tok_str, 0)

    def string_literal_handler(self, token):
        """

        :param token:
        :return:
        """
        if self.tok_str == "":
            self.tok_str += token
            self.adv_buf()
            while self.current() != '"':
                self.tok_str += self.current()
                self.adv_buf()
            self.tok_str += self.current()
            self.adv_buf()

            return self.emit(Token(self.tok_str, TokenType.STRING_LITERAL))
        else:
            return self.emit(self.tok_str)

    def get_token(self):
        """

        :return:
        """
        while not self.eof:
            if self.current() == " ":
                if self.tok_str != "":
                    return self.reserved_word_handler(self.tok_str)
                else:
                    while self.current() == " ":
                        self.adv_buf()

            elif self.current() == "{":
                return self.default_handler(self.current())

            elif self.current() == "}":
                return self.default_handler(self.current())

            elif self.current() == "(":
                return self.default_handler(self.current())

            elif self.current() == ")":
                return self.default_handler(self.current())

            elif self.current() == "[":
                return self.default_handler(self.current())

            elif self.current() == "]":
                return self.default_handler(self.current())

            elif self.current() == "<":
                return self.default_handler(self.current())

            elif self.current() == ">":
                return self.default_handler(self.current())

            elif self.current() == "/":
                if self.peek() == "*" or self.peek() == "/":
                    return self.comment_handler(self.current())
                else:
                    return self.cmpnd_handler(self.current())

            elif self.current() == "+":
                return self.cmpnd_handler(self.current())

            elif self.current() == "-":
                return self.cmpnd_handler(self.current())

            elif self.current() == "*":
                return self.cmpnd_handler(self.current())

            elif self.current() == "%":
                return self.cmpnd_handler(self.current())

            elif self.current() == "&":
                return self.cmpnd_handler(self.current())

            elif self.current() == "|":
                return self.cmpnd_handler(self.current())

            elif self.current() == "^":
                return self.default_handler(self.current())

            elif self.current() == "~":
                return self.cmpnd_handler(self.current())

            elif self.current() == "!":
                return self.cmpnd_handler(self.current())

            elif self.current() == "-":
                return self.cmpnd_handler(self.current())

            elif self.current() == "$":
                return self.default_handler(self.current())

            elif self.current() == "/":
                return self.comment_handler(self.current())

            elif self.current() == "=":
                return self.cmpnd_handler(self.current())

            elif self.current() == ",":
                return self.default_handler(self.current())

            elif self.current() == ".":
                return self.default_handler(self.current())

            elif self.current() == ":":
                return self.default_handler(self.current())

            elif self.current() == ";":
                return self.default_handler(self.current())

            elif self.current() == '"':
                return self.string_literal_handler(self.current())

            else:
                if self.current() not in ["\r", "\t", "\n"]:
                    self.tok_str += self.current()
                    if self.current().isnumeric():
                        if self.peek() == '.':
                            self.adv_buf()
                            self.tok_str += self.current()
                            self.adv_buf()

                            if self.current().isnumeric():
                                self.tok_str += self.current()

                self.adv_buf()

    def load_tokens(self):
        while not self.eof:
            self.get_token()

        eof_tok = Token(None, TokenType.EOF)
        eof_tok.token = TokenType.EOF
        eof_tok.lexeme = ""
