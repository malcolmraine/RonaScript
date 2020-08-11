#!/usr/bin/env python
"""
File: tokens.py
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

from enum import Enum, unique, auto


"""
{   R_BRACE
}   L_BRACE
(   R_PARAN
)   L_PARAN
[   R_BRACK
]   L_BRACK
<   R_CARAT
>   L_CARAT
/   SLASH
+   PLUS
*   STAR
-   MINUS
%   PERCENT
&   AMPER
|   BAR
^   UP_ARROW
#   POUND
$   DOLLAR
!   NOT
=   EQUAL
,   COMMA
.   DOT
:   COLON
;   SEMICOLON
"   DBL_QOTE

++  DBL_PLUS
--  DBL_MINUS
==  DBL_EQUAL
!=  NOT_EQUAL
&&  DBL_AMPER
||  DBL_BAR
[]  EMPTY_LIST

float   FLOAT
int     INT
string  STRING
list    LIST
class   CLASS

"""


class TokenType(Enum):
    L_BRACE = auto()
    R_BRACE = auto()
    R_PARAN = auto()
    L_PARAN = auto()
    R_BRACK = auto()
    L_BRACK = auto()
    R_CARAT = auto()
    L_CARAT = auto()
    SLASH = auto()
    PLUS = auto()
    STAR = auto()
    MINUS = auto()
    PERCENT = auto()
    AMPER = auto()
    BAR = auto()
    UP_ARROW = auto()
    COMMENT = auto()
    DOLLAR = auto()
    NOT = auto()
    EQUAL = auto()
    COMMA = auto()
    DOT = auto()
    COLON = auto()
    SEMICOLON = auto()
    DBL_QUOTE = auto()
    DBL_PLUS = auto()
    DBL_MINUS = auto()
    DBL_EQUAL = auto()
    NOT_EQUAL = auto()
    DBL_AMPER = auto()
    DBL_BAR = auto()
    FLOAT = auto()
    INT = auto()
    STRING = auto()
    LIST = auto()
    CLASS = auto()
    IDENTIFIER = auto()
    SLASH_EQUAL = auto()
    MINUS_EQUAL = auto()
    PLUS_EQUAL = auto()
    PERCENT_EQUAL = auto()
    AMPER_EQUAL = auto()
    BAR_EQUAL = auto()
    STAR_EQUAL = auto()
    FUNCTION = auto()
    VAR = auto()
    REQUIRE = auto()
    RETURN = auto()
    VOID = auto()
    STRING_LITERAL = auto()
    DBL_STAR = auto()
    UNDEFINED = auto()
    NULL = auto()
    BOOL = auto()
    LEQ = auto()
    GEQ = auto()
    XOREQ = auto()
    EMPTY_LIST = auto()
    TILDE_EQUAL = auto()
    TILDE = auto()
    CONSTRUCT = auto()
    DESTRUCT = auto()
    R_ARROW = auto()
    IF = auto()
    ELIF = auto()
    ELSE = auto()
    IS = auto()
    ALIAS = auto()
    INT_LITERAL = auto()
    FLOAT_LITERAL = auto()
    EOF = auto()



token_map = {
    "{": TokenType.L_BRACE,
    "}": TokenType.R_BRACE,
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
    "void": TokenType.VOID,
    "null": TokenType.NULL,
    "construct": TokenType.CONSTRUCT,
    "destruct": TokenType.DESTRUCT,
    "if": TokenType.IF,
    "elif": TokenType.ELIF,
    "else": TokenType.ELSE,
    "is": TokenType.IS,
    "alias": TokenType.ALIAS
}


class Token(object):
    def __init__(self, lexeme, tok_type=TokenType.UNDEFINED):
        self.lexeme: str = lexeme

        if tok_type == TokenType.UNDEFINED:
            if self.lexeme in token_map:
                self.token = token_map[self.lexeme]
            else:
                if self.lexeme.isnumeric():
                    self.token = TokenType.INT_LITERAL
                elif self.lexeme.replace('.', '').isnumeric():
                    self.token = TokenType.FLOAT_LITERAL
                else:
                    self.token = TokenType.IDENTIFIER
        else:
            self.token = tok_type

    def __repr__(self):
        return f"TOKEN[    {self.lexeme}{' ' * (12 - len(self.lexeme))}{self.token}{' ' * (26 - len(self.token.__repr__()))}]"

    def is_literal(self):
        return self.token in {TokenType.STRING_LITERAL, TokenType.FLOAT_LITERAL, TokenType.INT_LITERAL}

    def is_binary_op(self):
        return self.lexeme in {"+", "-", "==", "<", ">", "<=", ">=", "&", "&&", "|", "||", "^", "/", "%", "*"}

    def is_unary_op(self):
        return self.lexeme in {"++", "--", "**", "!", "~", "-", "+"}

    def is_type(self):
        return self.token in {TokenType.STRING, TokenType.FLOAT, TokenType.INT, TokenType.LIST, TokenType.IDENTIFIER}


# for key in token_map:
#     print('{"' + key + '", TOK_' + token_map[key].name + "},")
