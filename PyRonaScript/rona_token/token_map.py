"""
File: token_map.py
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

from .token_type import TokenType

TOKEN_MAP: dict = {
    "{": TokenType.R_BRACE,
    "}": TokenType.L_BRACE,
    "(": TokenType.R_PARAN,
    ")": TokenType.L_PARAN,
    "[": TokenType.R_BRACK,
    "]": TokenType.L_BRACK,
    "<": TokenType.R_CARAT,
    ">": TokenType.L_CARAT,
    "/": TokenType.SLASH,
    "+": TokenType.PLUS,
    "*": TokenType.STAR,
    "-": TokenType.MINUS,
    "%": TokenType.PERCENT,
    "&": TokenType.AMPER,
    "|": TokenType.BAR,
    "^": TokenType.UP_ARROW,
    "~": TokenType.TILDE,
    "/*": TokenType.COMMENT,
    "'$'": TokenType.DOLLAR,
    "!": TokenType.NOT,
    "=": TokenType.EQUAL,
    ",": TokenType.COMMA,
    ".": TokenType.DOT,
    ":": TokenType.COLON,
    ";": TokenType.SEMICOLON,
    '"': TokenType.DBL_QUOTE,
    "++": TokenType.DBL_PLUS,
    "--": TokenType.DBL_MINUS,
    "==": TokenType.DBL_EQUAL,
    "+=": TokenType.PLUS_EQUAL,
    "-=": TokenType.MINUS_EQUAL,
    "%=": TokenType.PERCENT_EQUAL,
    "/=": TokenType.SLASH_EQUAL,
    "!=": TokenType.NOT_EQUAL,
    "&=": TokenType.AMPER_EQUAL,
    "|=": TokenType.BAR_EQUAL,
    "*=": TokenType.STAR_EQUAL,
    "**": TokenType.DBL_STAR,
    "~=": TokenType.TILDE_EQUAL,
    "&&": TokenType.DBL_AMPER,
    "||": TokenType.DBL_BAR,
    "[]": TokenType.EMPTY_LIST,
    "<=": TokenType.LEQ,
    ">=": TokenType.GEQ,
    "^=": TokenType.XOREQ,
    "->": TokenType.R_ARROW,
    "float": TokenType.FLOAT,
    "bool": TokenType.BOOL,
    "int": TokenType.INT,
    "string": TokenType.STRING,
    "list": TokenType.LIST,
    "class": TokenType.CLASS,
    "var": TokenType.VAR,
    "func": TokenType.FUNCTION,
    "require": TokenType.REQUIRE,
    "return": TokenType.RETURN,
    "break": TokenType.BREAK,
    "void": TokenType.VOID,
    "null": TokenType.NULL,
    "construct": TokenType.CONSTRUCT,
    "destruct": TokenType.DESTRUCT,
    "if": TokenType.IF,
    "elif": TokenType.ELIF,
    "else": TokenType.ELSE,
    "is": TokenType.IS,
    "alias": TokenType.ALIAS,
    "while": TokenType.WHILE,
    "for": TokenType.FOR,
}