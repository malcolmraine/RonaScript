"""
File: lexer_base.py
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


class LexerBase(object):
    def __init__(self):
        self.tokens: list = []
        self.file = None
        self.buf: list = ["", "", ""]
        self.tok_str: str = ""
        self.eof: bool = False
        self.line_cnt = 1
        self.char_cnt = 1
        self.line_char_cnt = 1

    def __del__(self) -> None:
        """
        Overrides default __del__ method to make sure file gets closed.

        :return: No return value.
        """
        if self.file is not None:
            try:
                self.file.close()
            except:
                pass

    def load_file(self, file: str) -> None:
        """
        Load a file into the rona_lexer.

        :param file: File to be loaded.
        :return: No return value.
        """
        self.file = open(file, "r")
        self.buf[0] = ""
        self.buf[1] = self.file.read(1)
        self.buf[2] = self.file.read(1)

    def adv_buf(self, n=1) -> None:
        """
        Advance the buffer by 1 or more characters.

        :param n: Number of advancements to make.
        :return: No return value
        """
        for i in range(n):
            self.buf[0] = self.buf[1]
            self.buf[1] = self.buf[2]
            ch = self.file.read(1)
            self.buf[2] = ch
            self.char_cnt += 1
            self.line_char_cnt += 1

        if self.current() == '\n':
            self.line_cnt += 1
            self.line_char_cnt = 1

        if self.current() == "\0" or self.current() == "":
            self.eof = True

    def peek(self) -> str:
        """
        Get the next character.

        :return: string containing character
        """
        return self.buf[2]

    def current(self) -> str:
        """
        Get the current character.

        :return: string containing character
        """
        return self.buf[1]

    def lookback(self) -> str:
        """
        Get the previous character.

        :return: string containing character
        """
        return self.buf[0]
