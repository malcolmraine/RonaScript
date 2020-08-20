#!/usr/bin/env python
"""
File: ast_nodes.py
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

from uuid import uuid4
from .ast_graph import AstGraph

_tab_lvl: int = 0
graph_visual = AstGraph()
current_parent = graph_visual.name


class AstNode(object):
    instance_num: int = 0

    def __init__(self):
        self.parent = None
        self.instance_num += 1
        self.file_pos = ()

    @staticmethod
    def tab_str() -> str:
        """
        Gets the correct sized tab string for printing the AST nodes.

        :return: String containing only \t characters.
        """
        global _tab_lvl

        if _tab_lvl == 0:
            return "\t" * _tab_lvl
        else:
            return "|\t" * _tab_lvl

    @staticmethod
    def increment_lvl(n=1) -> None:
        """
        Increments the global tab level for printing purposes

        :return: No return value
        """
        global _tab_lvl
        _tab_lvl += n

    @staticmethod
    def decrement_lvl(n=1) -> None:
        """
        Decrements the global tab level for printing purposes

        :return: No return value
        """
        global _tab_lvl
        _tab_lvl -= n

    def class_name(self):
        return f"{type(self).__name__}_{self.instance_num}"


class BinaryExpr(AstNode):
    def __init__(self):
        super().__init__()
        self.op = None
        self.left = None
        self.right = None

    def __str__(self):
        global _tab_lvl
        
        string = f"{self.tab_str()}BinaryExpr( {self.op} )\n"
        self.increment_lvl()

        if isinstance(self.left, (UnaryExpr, BinaryExpr, Expr, IndexedExpr)):
            string += str(self.left)
        else:
            string += f"{self.tab_str()}Expr( {self.left} )\n"

        if isinstance(self.right, (UnaryExpr, BinaryExpr, Expr, IndexedExpr)):
            string += str(self.right)
        else:
            string += f"{self.tab_str()}Expr( {self.right} )\n"

        self.decrement_lvl()
        
        return string


class UnaryExpr(AstNode):
    def __init__(self):
        super().__init__()
        self.op = None
        self.id = None

    def __str__(self):
        string = f"{self.tab_str()}UnaryExpr( {self.op} )\n"
        self.increment_lvl()

        if isinstance(self.id, str):
            string += f"{self.tab_str()}Expr( {self.id} )\n"
        else:
            string += f"{self.id}"
        self.decrement_lvl()
        return string


class Expr(AstNode):
    def __init__(self):
        super().__init__()
        self.expr = None
        self.is_literal = False
        
    def __str__(self):
        string = f"{self.tab_str()}Expr( )\n"
        self.increment_lvl()
        string += str(self.expr)
        self.decrement_lvl()
        return string


class IndexedExpr(Expr):
    def __init__(self):
        super().__init__()
        self.idx = None
        
    def __str__(self):
        string = f"{self.tab_str()}IndexedExpr( )\n"
        self.increment_lvl()

        if isinstance(self.expr, (Expr, BinaryExpr, UnaryExpr)):
            string += str(self.expr)
        else:
            string += f"{self.tab_str()}Expr( {self.expr} )\n"

        if isinstance(self.idx, (Expr, BinaryExpr, UnaryExpr)):
            self.increment_lvl()
            string += f"{self.tab_str()}Index( )\n" + str(self.idx)
            self.decrement_lvl()
        else:
            string += f"{self.tab_str()}Index( {self.idx} )\n"

        self.decrement_lvl()
        return string


class Require(AstNode):
    def __init__(self):
        super().__init__()
        self.source = None
        
    def __str__(self):
        string = f"{self.tab_str()}Require( {self.source} )\n"
        return string


class ArgDecl(AstNode):
    def __init__(self):
        super().__init__()
        self.id = None
        self.type = None


class FuncQualifier(AstNode):
    def __init__(self):
        super().__init__()
        self.value = None

    def __str__(self):
        return f"{self.tab_str()}FuncQualifier( {self.value} )\n"


class BreakStmt(AstNode):
    def __init__(self):
        super().__init__()
        self.scope_id = None

    def __str__(self):
        return f"{self.tab_str()}BreakStmt( {self.scope_id} )\n"


class FlowStmt(AstNode):
    def __init__(self):
        super().__init__()
        self.id = None
        self.scope_id = None


class ReturnStmt(AstNode):
    def __init__(self):
        super().__init__()
        self.expr: Expr or None = None
        self.scope_id = None


class AssignmentStmt(AstNode):
    def __init__(self):
        super().__init__()
        self.id = None
        self.expr: Expr or None = None

    def __str__(self):
        string = f"{self.tab_str()}AssignmentStmt( {self.id} )\n"
        self.increment_lvl()

        if isinstance(self.expr, (int, float, str)):
            string += f"{self.tab_str()}Expr( {self.expr} )\n"
        else:
            string += str(self.expr)

        self.decrement_lvl()

        return string


class IfStmt(AstNode):
    def __init__(self):
        super().__init__()
        self.test: Expr or None = None
        self.consequent: Scope or None = None
        self.alternative: ElifStmt or ElseStmt = None

    def __str__(self):
        string = f"{self.tab_str()}IfStmt( )\n"
        self.increment_lvl()
        string += f"{self.tab_str()}Test( )\n"
        self.increment_lvl()
        string += str(self.test)
        self.decrement_lvl(2)

        string += f"{self.tab_str()}Consequent( )\n"
        self.increment_lvl()
        string += str(self.consequent)
        self.decrement_lvl()

        if self.alternative is not None:
            string += f"{self.tab_str()}Alternative( )\n"
            self.increment_lvl()
            string += str(self.alternative)
            self.decrement_lvl()

        return string


class ElifStmt(IfStmt):
    def __init__(self):
        super().__init__()

    def __str__(self):
        string = f"{self.tab_str()}ElifStmt( )\n"
        self.increment_lvl()
        string += f"{self.tab_str()}Test( )\n"
        self.increment_lvl()
        string += str(self.test)
        self.decrement_lvl(2)

        string += f"{self.tab_str()}Consequent( )\n"
        self.increment_lvl()
        string += str(self.consequent)
        self.decrement_lvl()

        if self.alternative is not None:
            string += f"{self.tab_str()}Alternative( )\n"
            self.increment_lvl()
            string += str(self.alternative)
            self.decrement_lvl()

        return string


class ElseStmt(AstNode):
    def __init__(self):
        super().__init__()
        self.scope: Scope or None = None

    def __str__(self):
        string = f"{self.tab_str()}ElseStmt( )\n"
        self.increment_lvl()
        string += f"{self.tab_str()}Consequent( )\n"
        self.increment_lvl()
        string += str(self.scope)
        self.decrement_lvl(2)

        return string


class Scope(AstNode):
    def __init__(self):
        super().__init__()
        self.children: list = []
        self.parent_scope: Scope or None = None
        self.id = uuid4()

    def add_subtree(self, root_node):
        self.children.append(root_node)
        
    def __str__(self):
        string = f"{self.tab_str()}Scope( {self.id} )\n"

        for child in self.children:
            self.increment_lvl()
            string += str(child)
            self.decrement_lvl()

        return string


class VarDecl(AstNode):
    def __init__(self):
        super().__init__()
        self.id = None
        self.type = None
        self.init_value = None
        
    def __str__(self):
        string = f"{self.tab_str()}VarDecl( {self.type} , {self.id} )\n"

        if self.init_value is not None:
            self.increment_lvl()
            if isinstance(self.init_value, (int, float, str)):
                string += f"{self.tab_str()}Expr( {self.init_value} )\n"
            else:
                string += str(self.init_value)
            self.decrement_lvl()

        return string


class FuncDecl(AstNode):
    def __init__(self):
        super().__init__()
        self.qualifiers = []
        self.id = None
        self.args: list = []
        self.type = None
        self.scope: Scope or None = None

    def __str__(self):
        string = f"{self.tab_str()}FuncDecl( {self.type} , {self.id} )\n"

        self.increment_lvl()
        string += f"{self.tab_str()}Qualifiers( {self.qualifiers} )\n"
        string += f"{self.tab_str()}Arguments( {self.args} )\n"
        string += f"{self.scope}"
        self.decrement_lvl()

        return string


class ClassDecl(AstNode):
    def __init__(self):
        super().__init__()
        self.id = None
        self.scope: Scope or None = None

    def __str__(self):
        string = f"{self.tab_str()}ClassDecl( {self.id} )\n"
        self.increment_lvl()
        string += str(self.scope)
        self.decrement_lvl()

        return string


class FuncCall(AstNode):
    def __init__(self):
        super().__init__()
        self.id = None
        self.args: list = []

    def __str__(self):
        string = f"{self.tab_str()}FuncCall( {self.id} )\n"

        self.increment_lvl()

        for arg in self.args:
            if isinstance(arg, (int, float, str)):
                string += f"{self.tab_str()}Expr( {arg} )\n"
            else:
                string += str(arg)

        self.decrement_lvl()

        return string


class ScopeMethodCall(AstNode):
    def __init__(self):
        super().__init__()
        self.accessor = None
        self.func_call: FuncCall or None = None


class ScopeVarAccess(AstNode):
    def __init__(self):
        super().__init__()
        self.scope_id = None
        self.var = None


class WhileLoop(AstNode):
    def __init__(self):
        super().__init__()
        self.test: Expr or None = None
        self.scope: Scope or None = None

    def __str__(self):
        string = f"{self.tab_str()}WhileLoop( )\n"
        self.increment_lvl()
        string += f"{self.tab_str()}Test( )\n"
        self.increment_lvl()
        string += f"{self.tab_str()}{self.test}\n"
        self.decrement_lvl()
        self.increment_lvl()
        string += f"{self.tab_str()}{self.scope}\n"
        self.decrement_lvl(2)

        return string


class ForLoop(AstNode):
    def __init__(self):
        super().__init__()
        self.init_var: VarDecl or None = None
        self.test: Expr or None = None
        self.advance_expr: Expr or None = None
        self.scope: Scope or None = None

    def __str__(self):
        string = f"{self.tab_str()}ForLoop( )\n"
        self.increment_lvl()
        string += f"{self.tab_str()}InitVar( )\n"
        self.increment_lvl()
        string += str(self.init_var)
        self.decrement_lvl()
        string += f"{self.tab_str()}Test( )\n"
        self.increment_lvl()
        string += str(self.test)
        self.decrement_lvl()
        string += f"{self.tab_str()}Advance( )\n"
        self.increment_lvl()
        string += str(self.advance_expr)
        self.decrement_lvl()
        string += str(self.scope)

        return string


class AliasDecl(AstNode):
    def __init__(self):
        super().__init__()
        self.alias_id = None
        self.base_id = None

    def __str__(self):
        string = f"{self.tab_str()}AliasDecl( {self.base_id} -> {self.alias_id} )"
        return string
