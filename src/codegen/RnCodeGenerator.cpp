/*****************************************************************************
* File: RnCodeGenerator.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2021 Malcolm Hall
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************/

#include "RnCodeGenerator.h"
#include "../parser/ast/Module.h"
#include "RnCodeGenVisitor.h"

/*****************************************************************************/
RnCodeGenerator::RnCodeGenerator() = default;

/*****************************************************************************/
RnCodeGenerator::~RnCodeGenerator() = default;

/*****************************************************************************/
void RnCodeGenerator::Generate(Ast* ast) {
    _result.clear();
    for (auto& module : ast->modules) {
        InstructionBlock module_instructions = visitor.GeneralVisit(module.second);
        _result.insert(_result.end(), module_instructions.begin(),
                       module_instructions.end());
    }

    InstructionBlock root_instructions = visitor.GeneralVisit(ast->root);
    _result.insert(_result.end(), root_instructions.begin(), root_instructions.end());
    _result.emplace_back(new RnInstruction(OP_EXIT, 0));
}

/*****************************************************************************/
void RnCodeGenerator::Optimize() {}

/*****************************************************************************/
void RnCodeGenerator::Run() {
    Generate(_input);
}