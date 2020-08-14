"""
File: token_type.py
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


class TokenType(Enum):
    R_BRACE = auto()        # {
    L_BRACE = auto()        # }
    R_PARAN = auto()        # (
    L_PARAN = auto()        # )
    R_BRACK = auto()        # [
    L_BRACK = auto()        # ]
    R_CARAT = auto()        # <
    L_CARAT = auto()        # >
    SLASH = auto()          # /
    PLUS = auto()           # +
    STAR = auto()           # *
    MINUS = auto()          # -
    PERCENT = auto()        # %
    AMPER = auto()          # %
    BAR = auto()            # |
    UP_ARROW = auto()       # ^
    COMMENT = auto()
    DOLLAR = auto()         # $
    NOT = auto()            # !
    EQUAL = auto()          # =
    COMMA = auto()          # ,
    DOT = auto()            # .
    COLON = auto()          # :
    SEMICOLON = auto()      # ;
    DBL_QUOTE = auto()      # "
    DBL_PLUS = auto()       # ++
    DBL_MINUS = auto()      # --
    DBL_EQUAL = auto()      # ==
    NOT_EQUAL = auto()      # !=
    DBL_AMPER = auto()      # &&
    DBL_BAR = auto()        # ||
    FLOAT = auto()          # float
    INT = auto()            # int
    STRING = auto()         # string
    LIST = auto()           # list
    CLASS = auto()          # class
    IDENTIFIER = auto()
    SLASH_EQUAL = auto()    # /=
    MINUS_EQUAL = auto()    # -=
    PLUS_EQUAL = auto()     # +=
    PERCENT_EQUAL = auto()  # %=
    AMPER_EQUAL = auto()    # &=
    BAR_EQUAL = auto()      # |=
    STAR_EQUAL = auto()     # *=
    FUNCTION = auto()       # func
    VAR = auto()            # var
    REQUIRE = auto()        # require
    RETURN = auto()         # return
    VOID = auto()           # void
    STRING_LITERAL = auto()
    DBL_STAR = auto()       # **
    UNDEFINED = auto()
    NULL = auto()           # null
    BOOL = auto()           # bool
    LEQ = auto()            # <=
    GEQ = auto()            # >=
    XOREQ = auto()          # ^=
    EMPTY_LIST = auto()     # []
    TILDE_EQUAL = auto()    # ~=
    TILDE = auto()          # ~
    CONSTRUCT = auto()      # construct
    DESTRUCT = auto()       # destruct
    R_ARROW = auto()        # ->
    IF = auto()             # if
    ELIF = auto()           # elif
    ELSE = auto()           # else
    IS = auto()             # is
    ALIAS = auto()          # alias
    INT_LITERAL = auto()
    FLOAT_LITERAL = auto()
    EOF = auto()