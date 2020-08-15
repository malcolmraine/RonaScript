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

from stack import Stack
from .ast_nodes import *
from .parser_base import ParserBase
from token import TokenType
from uuid import uuid4
import os


class Parser(ParserBase):
    def __init__(self):
        super().__init__()
        self.ast.root = Scope()
        self.current_scope: Scope = self.ast.root
        self.parent_scope: Scope = self.ast.root

    # Handlers
    def require(self):
        """

        :return:
        """
        node: AstNode = Require()
        node.source = self.peek().lexeme
        node.parent = self.current_scope
        self.adv_buf(2)
        return node

    def var_decl(self) -> AstNode:
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

    def func_qualifier(self) -> list:
        """

        :return:
        """
        valid_qualifiers = ["construct", "destruct", "public", "protected", "private", "static"]
        out = []

        while self.current() in valid_qualifiers:
            out.append(self.current())
            self.adv_buf()

        return out

    def func_decl(self) -> FuncDecl:
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

    def class_decl(self) -> ClassDecl:
        """

        :return:
        """
        node = ClassDecl()
        self.adv_buf()
        node.id = self.current()
        self.adv_buf()
        node.scope = self.scope()

        return node

    def expr(self, stop_tok=None, paran_stack=Stack()) -> Expr:
        """

        :param stop_tok:
        :param paran_stack:
        :return:
        """
        p_stack = paran_stack
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
                p_stack.push(1)
                self.adv_buf()

            elif self.current().token == TokenType.L_PARAN:
                node = expr_stack.pop()

                if p_stack.empty():
                    return node
                else:
                    p_stack.pop()

                # Check if the stack is empty and return the last popped node
                # if it is. Redundant parentheses can cause this condition so
                # we need to check before proceeding.
                if expr_stack.empty():
                    self.adv_buf()
                    return node

                # Remove any redundant expressions
                if isinstance(expr_stack.top(), Expr):
                    if expr_stack.size() == 1:
                        self.adv_buf()
                        t_node = expr_stack.pop()
                        t_node.expr = node
                        return t_node
                    else:
                        expr_stack.pop()

                if isinstance(expr_stack.top(), Expr):
                    if expr_stack.top().expr is None:
                        t_node = expr_stack.pop()

                        if expr_stack.empty():
                            expr_stack.push(node)
                        else:
                            t_node.expr = node
                            expr_stack.push(t_node)

                elif isinstance(expr_stack.top(), BinaryExpr):
                    if expr_stack.top().right is None:
                        t_node = expr_stack.pop()
                        t_node.right = node

                        try:
                            # If the binary expression is simply the child of another expression
                            # we can get rid of the parent expression, replacing it with the binary
                            # expression
                            if isinstance(expr_stack.top(), Expr):
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
                    if isinstance(expr_stack.top(), BinaryExpr):
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

                if isinstance(expr_stack.top(), BinaryExpr):
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
                self.unary_expr(expr_stack)

    def unary_expr(self, expr_stack: Stack) -> None:
        """

        :param expr_stack:
        :return:
        """
        node = UnaryExpr()
        node.op = self.current().lexeme

        if self.lookback().token == TokenType.IDENTIFIER:
            if expr_stack.top() == self.lookback().lexeme:
                node.id = expr_stack.pop()
                self.adv_buf()

        elif self.peek().token == TokenType.IDENTIFIER:
            node.id = self.peek().lexeme
            self.adv_buf(2)

        elif self.peek().token == TokenType.R_PARAN:
            self.adv_buf()
            node.id = self.expr()
            self.token_idx -= 1

        if isinstance(expr_stack.top(), BinaryExpr):
            t_node = expr_stack.pop()
            t_node.right = node
            expr_stack.push(t_node)
        else:
            expr_stack.push(node)

    def binary_expr(self) -> BinaryExpr:
        """

        :return:
        """
        node = BinaryExpr()
        node.left = Expr()
        self.adv_buf()
        node.right = Expr()

        return node

    def arg_decl(self) -> ArgDecl:
        """

        :return:
        """
        ...

    def break_stmt(self) -> BreakStmt:
        """

        :return:
        """
        return BreakStmt()

    def flow_stmt(self) -> FlowStmt:
        """

        :return:
        """
        return FlowStmt()

    def return_stmt(self) -> ReturnStmt:
        """

        :return:
        """
        node = ReturnStmt()
        node.expr = self.expr()

        return node

    def assignment_stmt(self) -> AssignmentStmt:
        """

        :return:
        """
        node = AssignmentStmt()
        node.id = self.current()
        self.adv_buf(2)
        node.expr = self.expr()

        return node

    def if_stmt(self) -> IfStmt:
        """

        :return:
        """
        node = IfStmt()
        self.adv_buf()

        if self.current().token == TokenType.R_PARAN:
            node.test = self.expr()
            node.consequent = self.scope()

            if self.current().token == TokenType.ELIF:
                node.alternative = self.elif_stmt()

            elif self.current().token == TokenType.ELSE:
                node.alternative = self.else_stmt()

            else:
                node.alternative = None

        return node

    def elif_stmt(self) -> ElifStmt:
        """

        :return:
        """
        node = ElifStmt()
        self.adv_buf()

        if self.current().token == TokenType.R_PARAN:
            node.test = self.expr()
            node.consequent = self.scope()

            if self.current().token == TokenType.ELIF:
                node.alternative = self.elif_stmt()
            elif self.current().token == TokenType.ELSE:
                node.alternative = self.else_stmt()
            else:
                node.alternative = None

        return node

    def else_stmt(self) -> ElseStmt:
        """

        :return:
        """
        node = ElseStmt()
        node.scope = self.scope()

        return node

    def block(self) -> Block:
        """

        :return:
        """
        node = Block()

        return node

    def scope(self) -> Scope:
        """

        :return:
        """
        node = Scope()
        node.block = Block()
        self.convert_scope(node)
        self.adv_buf()
        self.parse()

        return node

    def func_call(self) -> FuncCall:
        """

        :return:
        """
        node = FuncCall()
        node.id = self.current().lexeme
        self.adv_buf(1)

        if self.current().token == TokenType.R_PARAN:
            self.adv_buf()

        while self.current().token not in [TokenType.L_PARAN, TokenType.SEMICOLON]:
            node.args.append(self.expr(','))

        self.adv_buf()
        return node

    def scope_method_call(self) -> ScopeMethodCall:
        """

        :return:
        """
        node = ScopeMethodCall()
        node.accessor = self.current().lexeme
        self.adv_buf(2)
        node.func_call = self.func_call()

        return node

    def scope_var_access(self) -> ScopeVarAccess:
        """

        :return:
        """
        node = ScopeVarAccess()
        node.accessor = self.current().lexeme
        self.adv_buf(2)
        node.var = self.current().lexeme

        return node

    def while_loop(self) -> WhileLoop:
        """

        :return:
        """
        node = WhileLoop()
        node.test = self.expr()
        node.scope = self.scope()

        return node

    def for_loop(self) -> ForLoop:
        """

        :return:
        """
        node = ForLoop()
        node.init_var = self.var_decl()
        node.test = self.expr()
        node.advance_expr = self.expr()
        node.scope = self.scope()

        return node

    def alias_decl(self) -> AliasDecl:
        """

        :return:
        """
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

    def revert_scope(self) -> None:
        """
        Changes the current scope to the parent scope.

        :return: No return value.
        """
        if self.current_scope.parent_scope is not None:
            self.current_scope = self.current_scope.parent_scope

    def convert_scope(self, scope: Scope) -> None:
        """
        Changes the current scope to a new scope and the parent scope to the current scope.

        :param scope: New scope to change to.
        :return: No return value.
        """
        scope.parent_scope = self.current_scope
        self.current_scope = scope

    def dump_ast(self, name=uuid4()) -> str:
        """
        Dumps the AST to a file.

        :return: Absolute path to AST dump file.
        """
        file_name: str = f"{name}.ast"

        with open(file_name, 'w') as file:
            file.write(str(self.ast.root))

        return file_name

    def parse(self) -> None:
        """

        :return:
        """
        while self.token_idx < len(self.token_buf) - 1:

            if self.current().token == TokenType.COMMENT:
                self.adv_buf()

            elif self.current().token == TokenType.REQUIRE:
                self.current_scope.add_subtree(self.require())

            elif self.current().token == TokenType.L_BRACE:
                self.revert_scope()
                self.adv_buf()

            elif self.current().token == TokenType.R_BRACE:
                new_scope = self.scope()
                self.current_scope.add_subtree(new_scope)

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

            elif self.current().token == TokenType.IF:
                self.current_scope.add_subtree(self.if_stmt())

            elif self.current().token == TokenType.ELIF:
                self.current_scope.add_subtree(self.elif_stmt())

            elif self.current().token == TokenType.ELSE:
                self.current_scope.add_subtree(self.else_stmt())
