"""
File: parser_base.py
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

from .tree import Tree
from Lexer.tokens import Token


class ParserBase(object):
    def __init__(self):
        self.token_buf: list = []
        self.token_idx: int = 0
        self.ast: Tree = Tree()

    def adv_buf(self, n=1) -> None:
        """
        Advances the token buffer by n tokens.

        :param n: Number of advancements to make.
        :return: No return value.
        """
        for i in range(n):
            if self.token_idx < (len(self.token_buf) - 1):
                self.token_idx += 1
            else:
                self.token_idx = self.token_idx

    def lookback(self) -> Token:
        """
        Gets the last token in the buffer.

        :return: Token object
        """
        if self.token_idx > 0:
            return self.token_buf[self.token_idx - 1]
        else:
            return self.token_buf[0]

    def current(self) -> Token:
        """
        Gets the current token in the buffer.

        :return: Token object
        """
        return self.token_buf[self.token_idx]

    def peek(self) -> Token:
        """
        Gets the next token in the buffer.

        :return: Token object
        """
        if self.token_idx < (len(self.token_buf) - 1):
            return self.token_buf[self.token_idx + 1]
        else:
            return self.token_buf[-1]