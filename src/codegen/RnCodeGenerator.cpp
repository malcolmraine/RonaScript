/*****************************************************************************
* File: RnCodeGenerator.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
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
#include "../parser/ast/ScopeNode.h"
#include "RnCodeGenVisitor.h"

/*****************************************************************************/
RnCodeGenerator::RnCodeGenerator() = default;

/*****************************************************************************/
RnCodeGenerator::~RnCodeGenerator() = default;

/*****************************************************************************/
void RnCodeGenerator::Generate(Ast* ast) {
    //    _result.clear();
    _result = RnCodeFrame::CreateEmpty();
    visitor.SetCurrentFrame(_result);
    InstructionBlock root_instructions = visitor.GeneralVisit(ast->root);
    for (auto instruction : root_instructions) {
        _result->AddInstruction(instruction->GetOpcode(), instruction->GetArg1(),
                                instruction->GetArg2(), instruction->GetArg3());
    }
    //    _result.insert(_result.end(), root_instructions.begin(), root_instructions.end());

    //    if (_result.empty() || _result.back()->GetOpcode() != OP_EXIT) {
    _result->AddInstruction(OP_LOAD_LITERAL,
                            RnConstStore::InternValue(static_cast<RnIntNative>(0)));
    _result->AddInstruction(OP_EXIT, 0);
    //    }
}

/*****************************************************************************/
void RnCodeGenerator::Optimize() {}

/*****************************************************************************/
void RnCodeGenerator::Run() {
    Generate(_input);
}