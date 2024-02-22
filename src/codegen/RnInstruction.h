/*****************************************************************************
* File: RnInstruction.h
* Description:
* Author: Malcolm Hall
* Date: 6/1/22
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

#pragma once

#include <memory>
#include <vector>
#include "../vm/RnOpCode.h"

typedef uint32_t RnInstructionArg;
constexpr size_t RN_INSTRUCTION_OP_WIDTH = sizeof(RnOpCode);
constexpr size_t RN_INSTRUCTION_ARG_WIDTH = sizeof(RnInstructionArg);


class RnInstruction {
public:
    explicit RnInstruction(RnOpCode opcode = OP_NOP, RnInstructionArg arg1 = 0,
                           RnInstructionArg arg2 = 0, RnInstructionArg arg3 = 0);
    ~RnInstruction();
    std::string ToString();

    [[nodiscard]] inline RnOpCode GetOpcode() const {
        return _opcode;
    }

    [[nodiscard]] inline RnInstructionArg GetArg1() const {
        return _arg1;
    }

    void inline SetArg1(RnInstructionArg arg1) {
        _arg1 = arg1;
    }

    [[nodiscard]] inline RnInstructionArg GetArg2() const {
        return _arg2;
    }

    void inline SetArg2(RnInstructionArg arg2) {
        _arg2 = arg2;
    }

    [[nodiscard]] inline RnInstructionArg GetArg3() const {
        return _arg3;
    }

    void inline SetArg3(RnInstructionArg arg3) {
        _arg3 = arg3;
    }

private:
    RnOpCode _opcode = OP_NOP;
    RnInstructionArg _arg1 = 0;
    RnInstructionArg _arg2 = 0;
    RnInstructionArg _arg3 = 0;
};

typedef std::vector<RnInstruction*> InstructionBlock;
