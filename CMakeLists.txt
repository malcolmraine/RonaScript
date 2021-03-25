cmake_minimum_required(VERSION 3.16)
project(VarStackTest)

set(CMAKE_CXX_STANDARD 17)

add_executable(VarStackTest
        main.cpp
        vm/rona_object.cpp
        vm/rona_object.h
        vm/memory.cpp
        vm/memory.h
        vm/scope.cpp
        vm/scope.h
        vm/rona_function.cpp
        vm/rona_function.h
        vm/instruction.cpp
        vm/instruction.h
        vm/op_codes.h
        vm/rona_class.cpp
        vm/rona_class.h
        vm/virtual_machine.cpp
        vm/virtual_machine.h
        vm/builtins.cpp
        vm/builtins.h
        vm/rona_type.h
        vm/rona_type.cpp exceptions/TypeAssignmentError.cpp exceptions/TypeAssignmentError.h exceptions/InvalidIndexError.cpp exceptions/InvalidIndexError.h vm/rona_class_decl.cpp vm/rona_class_decl.h exceptions/TypeCastError.cpp exceptions/TypeCastError.h exceptions/UnsupportedOperationError.cpp exceptions/UnsupportedOperationError.h lexer/lexer.cpp lexer/lexer.h parser/parser.cpp parser/parser.h lexer/token.cpp lexer/token.h parser/ast/AstNode.cpp parser/ast/AstNode.h parser/ast/NodeType.h lexer/token_type.h exceptions/UnexpectedSymbolError.cpp exceptions/UnexpectedSymbolError.h exceptions/NameNotFoundError.cpp exceptions/NameNotFoundError.h exceptions/RuntimeError.cpp exceptions/RuntimeError.h exceptions/MemoryError.cpp exceptions/MemoryError.h codegen/code_generator.cpp codegen/code_generator.h parser/ast/Ast.cpp parser/ast/Ast.h exceptions/SourceNotFoundError.cpp exceptions/SourceNotFoundError.h exceptions/BaseException.cpp exceptions/BaseException.h parser/ast/StringLiteral.cpp parser/ast/StringLiteral.h parser/ast/FloatLiteral.cpp parser/ast/FloatLiteral.h parser/ast/IntLiteral.cpp parser/ast/IntLiteral.h parser/ast/ListLiteral.cpp parser/ast/ListLiteral.h parser/ast/Expr.cpp parser/ast/Expr.h parser/ast/BinaryExpr.cpp parser/ast/BinaryExpr.h parser/ast/UnaryExpr.cpp parser/ast/UnaryExpr.h parser/ast/IndexedExpr.cpp parser/ast/IndexedExpr.h parser/ast/ForLoop.cpp parser/ast/ForLoop.h parser/ast/AliasDecl.cpp parser/ast/AliasDecl.h parser/ast/ScopeNode.cpp parser/ast/ScopeNode.h parser/ast/Require.cpp parser/ast/Require.h parser/ast/FlowStmt.cpp parser/ast/FlowStmt.h parser/ast/IfStmt.cpp parser/ast/IfStmt.h parser/ast/ElifStmt.cpp parser/ast/ElifStmt.h parser/ast/VarDecl.cpp parser/ast/VarDecl.h parser/ast/FuncDecl.cpp parser/ast/FuncDecl.h parser/ast/WhileLoop.cpp parser/ast/WhileLoop.h parser/ast/ReturnStmt.cpp parser/ast/ReturnStmt.h parser/ast/AssignmentStmt.cpp parser/ast/AssignmentStmt.h parser/ast/BreakStmt.cpp parser/ast/BreakStmt.h parser/ast/ArgDecl.cpp parser/ast/ArgDecl.h parser/ast/ClassDecl.cpp parser/ast/ClassDecl.h parser/ast/FuncCall.cpp parser/ast/FuncCall.h parser/ast/ElseStmt.cpp parser/ast/ElseStmt.h parser/ast/Name.cpp parser/ast/Name.h support/RnStack.h parser/ast/BoolLiteral.cpp parser/ast/BoolLiteral.h support/Log.cpp support/Log.h memory_brainstorm/memory_manager.cpp memory_brainstorm/memory_manager.h memory_brainstorm/memory_interface.cpp memory_brainstorm/memory_interface.h)