#!/usr/bin/env python
"""
File: stack.py
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


class Stack(object):
    def __init__(self, type_enforcement=None):
        self._obj_store: list = []
        self.enforcement = type_enforcement

    def __str__(self):
        return str(self._obj_store)

    def size(self) -> int:
        """
        Returns the current size of the stack

        :return: int
        """
        return len(self._obj_store)

    def empty(self) -> bool:
        """
        Checks whether the stack is empty

        :return:
        """
        return len(self._obj_store) == 0

    def push(self, obj) -> None:
        """
        Push an object onto the stack

        :param obj: object to push
        :return: None
        """
        if self.enforcement is not None:
            if isinstance(obj, self.enforcement):
                self._obj_store.append(obj)
            else:
                raise Exception("Illegal object type for this stack!")
        else:
            self._obj_store.append(obj)

    def pop(self):
        """
        Pop an object off the top of the stack

        :return: object
        """
        return self._obj_store.pop()

    def top(self):
        return self._obj_store[-1]

