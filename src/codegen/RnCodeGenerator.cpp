/*****************************************************************************
* File: RnCodeGenerator.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
* Version: 1
*
******************************************************************************/

#include "RnCodeGenerator.h"
#include "../parser/ast/Ast.h"
#include "RnCodeGenVisitor.h"
#include "../parser/ast/Module.h"

/*****************************************************************************/
RnCodeGenerator::RnCodeGenerator() = default;

/*****************************************************************************/
RnCodeGenerator::~RnCodeGenerator() = default;

/*****************************************************************************/
void RnCodeGenerator::Generate(Ast* ast) {
    for (auto& module : ast->modules) {
        InstructionBlock module_instructions = visitor.GeneralVisit(module.second);
        instructions.insert(instructions.end(), module_instructions.begin(),
                            module_instructions.end());
    }

    InstructionBlock root_instructions = visitor.GeneralVisit(ast->root);
    instructions.insert(instructions.end(), root_instructions.begin(),
                        root_instructions.end());
    instructions.emplace_back(new RnInstruction(OP_EXIT, 0));
}

/*****************************************************************************/
void RnCodeGenerator::Optimize() {}
