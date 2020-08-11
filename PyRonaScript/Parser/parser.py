#!/usr/bin/env python
"""
File: parser.py
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

from Stack.stack import Stack
from .tree import Tree
from .ast_nodes import *
from Lexer.tokens import TokenType
from Lexer.tokens import Token

"""

Expect - what token do we expect next?
Await - What token are we expecting to change scope or state (this should probably be a Stack)

HANDLERS:
 - require declaration
 - function declaration
 - class creation
 - variable declaration
 - Binary expressions
 
"""


class Parser(object):
    def __init__(self):
        self.ast: Tree = Tree()
        self.ast.root = Scope()
        self.scope_stack: Stack = Stack()  # Stack to keep track of scope changes
        self.paran_stack: Stack = Stack()  # Stack to keep track of matching parantheses
        self.current_node = self.ast.root
        self.current_scope: Scope = self.ast.root
        self.scope_parent: Scope = self.ast.root
        self.token_buf: list = []
        self.token_idx: int = 0
        self.f = 0

    def adv_buf(self, n=1):
        """

        :param n:
        :return:
        """
        for i in range(n):
            if self.token_idx < (len(self.token_buf) - 1):
                self.token_idx += 1
            else:
                self.token_idx = self.token_idx

    def lookback(self) -> Token:
        """

        :return:
        """
        if self.token_idx > 0:
            return self.token_buf[self.token_idx - 1]
        else:
            return self.token_buf[0]

    def current(self) -> Token:
        """

        :return:
        """
        return self.token_buf[self.token_idx]

    def peek(self) -> Token:
        """

        :return:
        """
        if self.token_idx < (len(self.token_buf) - 1):
            return self.token_buf[self.token_idx + 1]
        else:
            return self.token_buf[-1]

    # Handlers
    def require(self):
        """

        :return:
        """
        node = Require()
        node.source = self.peek().lexeme
        node.parent = self.current_scope
        self.adv_buf(2)
        return node

    def var_decl(self):
        """

        :return:
        """
        node = VarDecl()
        self.adv_buf()
        node.id = self.current().lexeme
        self.adv_buf(2)
        node.type = self.current().lexeme
        self.adv_buf()

        if self.current().token == TokenType.EQUAL:
            self.adv_buf()
            node.init_value = self.expr()
        return node

    def func_qualifier(self):
        valid_qualifiers = ["construct", "destruct", "public", "private", "static"]
        out = []

        while self.current() in valid_qualifiers:
            out.append(self.current())
            self.adv_buf()

        return out

    def func_decl(self):
        """

        :return:
        """
        node = FuncDecl()
        node.qualifiers = self.func_qualifier()
        node.id = self.current()
        self.adv_buf()

        while self.current().token != TokenType.R_PARAN:
            node.args.append(self.expr())

        self.adv_buf()
        node.scope = self.scope()

        return node

    def class_decl(self):
        """

        :return:
        """
        node = ClassDecl()
        self.adv_buf()
        node.id = self.current()
        self.adv_buf()
        node.scope = self.scope()

    def expr(self, stop_tok=None):
        expr_stack: Stack = Stack()
        expr_stack.push(Expr())

        while not expr_stack.empty():
            if (
                self.current().token in [TokenType.L_BRACE, TokenType.SEMICOLON]
                or self.current().lexeme == stop_tok
            ):

                # We should only get here at the end of an expression and at the
                # end of every expression we should be here
                if self.current().token == TokenType.SEMICOLON or self.current().lexeme == stop_tok:
                    self.adv_buf()

                    if self.peek().token == TokenType.SEMICOLON:
                        self.adv_buf()

                return expr_stack.pop()

            elif self.current().token == TokenType.R_PARAN:
                expr_stack.push(Expr())
                self.adv_buf()

            elif self.current().token == TokenType.L_PARAN:
                node = expr_stack.pop()

                # Check if the stack is empty and return the last popped node
                # if it is. Redundant parentheses can cause this condition so
                # we need to check before proceeding.
                if expr_stack.empty():
                    self.adv_buf()
                    return node

                # Remove any redundant expressions
                if isinstance(expr_stack.last(), Expr):
                    if expr_stack.size() == 1:
                        self.adv_buf()
                        t_node = expr_stack.pop()
                        t_node.expr = node
                        return t_node
                    else:
                        expr_stack.pop()

                if isinstance(expr_stack.last(), Expr):
                    if expr_stack.last().expr is None:
                        t_node = expr_stack.pop()

                        if expr_stack.empty():
                            expr_stack.push(node)
                        else:
                            t_node.expr = node
                            expr_stack.push(t_node)

                elif isinstance(expr_stack.last(), BinaryExpr):
                    if expr_stack.last().right is None:
                        t_node = expr_stack.pop()
                        t_node.right = node

                        try:
                            # If the binary expression is simply the child of another expression
                            # we can get rid of the parent expression, replacing it with the binary
                            # expression
                            if isinstance(expr_stack.last(), Expr):
                                expr_stack.pop()
                        except IndexError:
                            ...

                        expr_stack.push(t_node)

                self.adv_buf()

            elif self.current().token == TokenType.IDENTIFIER:
                if self.peek().token == TokenType.R_PARAN:
                    node = self.func_call()
                    expr_stack.push(node)

                elif self.peek().token == TokenType.R_BRACK:
                    # <indexed_id> ::= <id> '[' <expr> ']'
                    node = IndexedExpr()
                    node.expr = self.current().lexeme
                    self.adv_buf(2)
                    node.idx = self.expr(']')
                    expr_stack.push(node)

                else:
                    if isinstance(expr_stack.last(), BinaryExpr):
                        node = expr_stack.pop()
                        node.right = self.current().lexeme
                        expr_stack.push(node)
                    else:
                        expr_stack.push(self.current().lexeme)
                    self.adv_buf()

            elif self.current().is_literal():
                if self.current().token == TokenType.INT_LITERAL:
                    literal_tok = int(self.current().lexeme)

                elif self.current().token == TokenType.FLOAT_LITERAL:
                    literal_tok = float(self.current().lexeme)

                else:
                    literal_tok = self.current().lexeme

                if isinstance(expr_stack.last(), BinaryExpr):
                    node = expr_stack.pop()
                    node.right = literal_tok
                    expr_stack.push(node)
                else:
                    expr_stack.push(literal_tok)
                self.adv_buf()

            elif self.current().token == TokenType.EMPTY_LIST:
                expr_stack.push(self.current().lexeme)

            elif self.current().is_binary_op():
                node = BinaryExpr()
                node.left = expr_stack.pop()
                node.op = self.current().lexeme
                expr_stack.push(node)
                self.adv_buf()

            elif self.current().is_unary_op():
                node = UnaryExpr()
                node.op = self.current().lexeme

                if self.lookback().token == TokenType.IDENTIFIER:
                    if expr_stack.last() == self.lookback().lexeme:
                        node.id = expr_stack.pop()
                        self.adv_buf()

                elif self.peek().token == TokenType.IDENTIFIER:
                    node.id = self.peek().lexeme
                    self.adv_buf(2)

                elif self.peek().token == TokenType.R_PARAN:
                    self.adv_buf()
                    node.id = self.expr()
                    self.token_idx -= 1

                if isinstance(expr_stack.last(), BinaryExpr):
                    t_node = expr_stack.pop()
                    t_node.right = node
                    expr_stack.push(t_node)
                else:
                    expr_stack.push(node)

    # def unary_expr(self):
    #     node = UnaryExpr()
    #
    #     if self.current().is_unary_op():
    #         node.op = self.current().tok
    #         self.adv_buf()
    #         node.id = self.current().tok
    #     else:
    #         node.id = self.current().tok
    #         self.adv_buf()
    #         node.op = self.current().tok
    #     return node

    def binary_expr(self):
        node = BinaryExpr()
        node.left = Expr()
        self.adv_buf()
        node.right = Expr()

        return node

    def arg_decl(self):
        ...

    def break_stmt(self):
        return BreakStmt()

    def flow_stmt(self):
        return FlowStmt()

    def return_stmt(self):
        node = ReturnStmt()
        node.expr = self.expr()

        return node

    def assignment_stmt(self):
        node = AssignmentStmt()
        node.id = self.current()
        self.adv_buf(2)
        node.expr = self.expr()

        return node

    def if_stmt(self):
        node = IfStmt()
        self.adv_buf()

        if self.current().token == TokenType.L_PARAN:
            node.test = self.expr()
            node.consequent = self.scope()

            if self.current().token == TokenType.ELIF:
                node.alternative = self.elif_stmt()
            elif self.current().token == TokenType.ELSE:
                node.alternative = self.else_stmt()
            else:
                node.alternative = None

        return node

    def elif_stmt(self):
        node = ElifStmt()
        self.adv_buf()

        if self.current().token == TokenType.L_PARAN:
            node.test = self.expr()
            node.consequent = self.scope()

            if self.current().token == TokenType.ELIF:
                node.alternative = self.elif_stmt()
            elif self.current().token == TokenType.ELSE:
                node.alternative = self.else_stmt()
            else:
                node.alternative = None

        return node

    def else_stmt(self):
        self.adv_buf()
        node = self.scope()

        return node

    def block(self):
        ...

    def scope(self):
        node = Scope()

        return node

    def func_call(self):
        node = FuncCall()
        node.id = self.current().lexeme
        #print(node.id)
        #print("BREAK")

        self.adv_buf(1)

        if self.current().token == TokenType.R_PARAN:
            self.adv_buf()

        while self.current().token != TokenType.L_PARAN:
            # if self.current().type == TokenType.VAR:
            #self.adv_buf()
            #print("BEFORE ", self.current(), self.peek())
            node.args.append(self.expr(','))
            print("AFTER ", self.current(), self.peek())
            #self.adv_buf(1)
            print("CURRENT ", self.current())

        print(node.args)
        return node

    def scope_method_call(self):
        node = ScopeMethodCall()
        node.accessor = self.current().lexeme
        self.adv_buf(2)
        node.func_call = self.func_call()

        return node

    def scope_var_access(self):
        node = ScopeVarAccess()
        node.accessor = self.current().lexeme
        self.adv_buf(2)
        node.var = self.current().lexeme

        return node

    def while_loop(self):
        node = WhileLoop()
        node.test = self.expr()
        node.scope = self.scope()

        return node

    def for_loop(self):
        """

        :return:
        """
        node = ForLoop()
        node.init_var = self.var_decl()
        node.test = self.expr()
        node.advance_expr = self.expr()
        node.scope = self.scope()

        return node

    def alias_decl(self):
        node = AliasDecl()
        self.adv_buf()
        node.alias_id = self.current().lexeme
        self.adv_buf()

        if self.current().token == TokenType.IS:
            self.adv_buf()
            node.base_id = self.current().lexeme
            self.adv_buf(2)
        else:
            raise Exception("Invalid 'alias' declaration!")

        return node

    def parse(self):
        while self.token_idx < len(self.token_buf) - 1:
            if self.current().token == TokenType.COMMENT:
                self.adv_buf()

            elif self.current().token == TokenType.REQUIRE:
                self.current_scope.add_subtree(self.require())

            elif self.current().token == TokenType.R_BRACE:
                self.current_scope = self.current_scope.parent_scope

            elif self.current().token == TokenType.L_BRACE:
                new_scope = self.scope()
                self.current_scope.add_subtree(new_scope)
                new_scope.parent_scope = self.current_scope
                self.current_scope = new_scope

            elif self.current().token == TokenType.VAR:
                self.current_scope.add_subtree(self.var_decl())

                if self.current().token == TokenType.SEMICOLON:
                    self.adv_buf()

            elif self.current().token == TokenType.ALIAS:
                self.current_scope.add_subtree(self.alias_decl())

            elif self.current().token == TokenType.FUNCTION:
                self.current_scope.add_subtree(self.func_decl())

            elif self.current().token == TokenType.RETURN:
                self.current_scope.add_subtree(self.return_stmt())

            elif self.current().token == TokenType.CLASS:
                self.current_scope.add_subtree(self.class_decl())

            elif self.current().token == TokenType.IDENTIFIER:
                ...
