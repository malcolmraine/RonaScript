/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
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

#include "CatchBlock.h"
#include "../../codegen/RnCodeGenVisitor.h"
#include "Name.h"
#include "ScopeNode.h"

/*****************************************************************************/
CatchBlock::CatchBlock() = default;

/*****************************************************************************/
CatchBlock::~CatchBlock() = default;

/*****************************************************************************/
std::string CatchBlock::ToString(bool nl) {
    std::string s = MakeTabStr() + "Catch( ";
    for (auto& exception_id : _exception_ids) {
        s += exception_id->value + " ";
    }

    s += ")";
    if (nl) {
        s += "\n";
    }
    _scope->nest_lvl = nest_lvl + 1;
    s += _scope->ToString(true);

    return s;
}
