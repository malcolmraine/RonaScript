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

from cpp_helpers import tabs

tab_lvl = 0


class AstNode(object):
    def __init__(self):
        ...
        
    @staticmethod
    def get_tab_str():
        global tab_lvl

        if tab_lvl == 0:
            return "\t" * tab_lvl
        else:
            return "|\t" * tab_lvl

    @staticmethod
    def increment_lvl():
        global tab_lvl
        tab_lvl += 1

    @staticmethod
    def decrement_lvl():
        global tab_lvl
        tab_lvl -= 1


class BinaryExpr(AstNode):
    def __init__(self):
        super().__init__()
        self.op = None
        self.left = None
        self.right = None

    def __str__(self):
        global tab_lvl
        
        string = f"{self.get_tab_str()}BinaryExpr( {self.op} )\n"
        self.increment_lvl()

        if isinstance(self.left, (UnaryExpr, BinaryExpr, Expr, IndexedExpr)):
            string += str(self.left)
        else:
            string += f"{self.get_tab_str()}Expr( {self.left} )\n"

        if isinstance(self.right, (UnaryExpr, BinaryExpr, Expr, IndexedExpr)):
            string += str(self.right)
        else:
            string += f"{self.get_tab_str()}Expr( {self.right} )\n"

        self.decrement_lvl()
        
        return string


class UnaryExpr(AstNode):
    def __init__(self):
        super().__init__()
        self.op = None
        self.id = None

    def __str__(self):
        global tab_lvl
        
        string = f"{self.get_tab_str()}UnaryExpr( {self.op} )\n"
        self.increment_lvl()

        if isinstance(self.id, str):
            string += f"{self.get_tab_str()}Expr( {self.id} )\n"
        else:
            string += f"{self.id}"
        self.decrement_lvl()
        return string


class Expr(AstNode):
    def __init__(self):
        super().__init__()
        self.expr = None
        
    def __str__(self):
        global tab_lvl
        
        string = f"{self.get_tab_str()}Expr( )\n"
        self.increment_lvl()
        string += str(self.expr)
        self.decrement_lvl()
        return string


class IndexedExpr(Expr):
    def __init__(self):
        super().__init__()
        self.idx = None
        
    def __str__(self):
        string = f"{self.get_tab_str()}IndexedExpr( )\n"
        self.increment_lvl()

        if isinstance(self.expr, (Expr, BinaryExpr, UnaryExpr)):
            string += str(self.expr)
        else:
            string += f"{self.get_tab_str()}Expr( {self.expr} )\n"

        if isinstance(self.idx, (Expr, BinaryExpr, UnaryExpr)):
            self.increment_lvl()
            string += f"{self.get_tab_str()}Index( )\n" + str(self.idx)
            self.decrement_lvl()
        else:
            string += f"{self.get_tab_str()}Index( {self.idx} )\n"

        self.decrement_lvl()
        return string


class Require(AstNode):
    def __init__(self):
        super().__init__()
        self.source = None
        
    def __str__(self):
        string = f"{self.get_tab_str()}Require( {self.source} )\n"
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
        return f"{self.get_tab_str()}FuncQualifier( {self.value} )\n"


class BreakStmt(AstNode):
    def __init__(self):
        super().__init__()
        self.scope = None


class FlowStmt(AstNode):
    def __init__(self):
        super().__init__()
        self.id = None


class ReturnStmt(AstNode):
    def __init__(self):
        super().__init__()
        self.expr: Expr = None


class AssignmentStmt(AstNode):
    def __init__(self):
        super().__init__()
        self.id = None
        self.expr: Expr = None


class IfStmt(AstNode):
    def __init__(self):
        super().__init__()
        self.test: Expr = None
        self.consequent: Scope = None
        self.alternative: ElifStmt or ElseStmt = None


class ElifStmt(IfStmt):
    def __init__(self):
        super().__init__()


class ElseStmt(AstNode):
    def __init__(self):
        super().__init__()
        self.scope: Scope = None


class ConditionalBlock(AstNode):
    def __init__(self):
        super().__init__()
        self.stmt: IfStmt = None


class Block(AstNode):
    def __init__(self):
        super().__init__()
        self.children: list = []
        
    def __str__(self):
        string = f"{self.get_tab_str()}Block( )\n"
        
        for child in self.children:
            self.increment_lvl()
            string += str(child)
            self.decrement_lvl()
        
        return string


class Scope(AstNode):
    def __init__(self):
        super().__init__()
        self.block: Block = Block()
        self.parent_scope: Scope = None

    def add_subtree(self, root_node):
        self.block.children.append(root_node)
        
    def __str__(self):
        string = f"{self.get_tab_str()}Scope( )\n"
        self.increment_lvl()
        string += str(self.block)
        return string


class VarDecl(AstNode):
    def __init__(self):
        super().__init__()
        self.id = None
        self.type = None
        self.init_value = None
        
    def __str__(self):
        string = f"{self.get_tab_str()}VarDecl( {self.type} , {self.id} )\n"

        if self.init_value is not None:
            self.increment_lvl()
            if isinstance(self.init_value, (int, float, str)):
                string += f"{self.get_tab_str()}Expr( {self.init_value} )\n"
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
        self.return_type = None
        self.scope: Scope = None

    def __str__(self):
        ...


class ClassDecl(AstNode):
    def __init__(self):
        super().__init__()
        self.id = None
        self.scope: Scope = None


class FuncCall(AstNode):
    def __init__(self):
        super().__init__()
        self.id = None
        self.args: list = []

    def __str__(self):
        string = f"{self.get_tab_str()}FuncCall( {self.id} )\n"

        for arg in self.args:
            string += arg

        return string


class ScopeMethodCall(AstNode):
    def __init__(self):
        super().__init__()
        self.accessor = None
        self.func_call: FuncCall = None


class ScopeVarAccess(AstNode):
    def __init__(self):
        super().__init__()
        self.accessor = None
        self.var = None


class WhileLoop(AstNode):
    def __init__(self):
        super().__init__()
        self.test: Expr = None
        self.scope: Scope = None


class ForLoop(AstNode):
    def __init__(self):
        super().__init__()
        self.init_var: VarDecl = None
        self.test: Expr = None
        self.advance_expr: Expr = None
        self.scope: Scope = None


class AliasDecl(AstNode):
    def __init__(self):
        super().__init__()
        self.alias_id = None
        self.base_id = None

    def __str__(self):
        string = f"{self.get_tab_str()}AliasDecl( {self.base_id} -> {self.alias_id} )"
        return string