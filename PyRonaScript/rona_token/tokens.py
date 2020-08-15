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

from .token_type import TokenType
from . import lexeme_sets
from .token_map import TOKEN_MAP


class Token(object):
    def __init__(self, lexeme, tok_type=TokenType.UNDEFINED):
        self.lexeme: str = lexeme

        if tok_type == TokenType.UNDEFINED:
            if self.lexeme in TOKEN_MAP:
                self.token = TOKEN_MAP[self.lexeme]
            else:
                if self.lexeme.isnumeric():
                    self.token = TokenType.INT_LITERAL
                elif self.lexeme.replace('.', '').isnumeric():
                    self.token = TokenType.FLOAT_LITERAL
                else:
                    self.token = TokenType.IDENTIFIER
        else:
            self.token = tok_type

    def __repr__(self) -> str:
        return f"TOKEN[    {self.lexeme}{' ' * (12 - len(self.lexeme))}{self.token}{' ' * (26 - len(self.token.__repr__()))}]"

    def is_literal(self) -> bool:
        """
        Returns whether or not the rona_token is a literal value.

        :return: boolean
        """
        return self.token in {TokenType.STRING_LITERAL, TokenType.FLOAT_LITERAL, TokenType.INT_LITERAL}

    def is_binary_op(self) -> bool:
        """
        Returns whether the rona_token is a binary operator.

        :return: boolean
        """
        return self.lexeme in lexeme_sets.BINARY_OPS

    def is_unary_op(self) -> bool:
        """
        Returns whether the rona_token is a unary operator.

        :return: boolean
        """
        return self.lexeme in lexeme_sets.UNARY_OPS

    def is_type(self) -> bool:
        """
        Returns whether the rona_token is a possible type name.

        :return: boolean
        """
        return self.token in {TokenType.STRING, TokenType.FLOAT, TokenType.INT, TokenType.LIST, TokenType.IDENTIFIER}
