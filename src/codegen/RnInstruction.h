/*****************************************************************************
* File: RnInstruction.h
* Description:
* Author: Malcolm Hall
* Date: 6/1/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNINSTRUCTION_H
#define RONASCRIPT_RNINSTRUCTION_H

#include <memory>
#include <vector>
#include "../vm/RnOpCode.h"

class RnInstruction {
public:
    RnInstruction();
    explicit RnInstruction(RnOpCode opcode);
    RnInstruction(RnOpCode opcode, uint32_t arg1);
    RnInstruction(RnOpCode opcode, uint32_t arg1, uint32_t arg2);
    RnInstruction(RnOpCode opcode, uint32_t arg1, uint32_t arg2, uint32_t arg3);
    ~RnInstruction();
    [[nodiscard]] char* GetAsBytes() const;
    std::string ToString();

    [[nodiscard]] inline RnOpCode GetOpcode() const {
        return _opcode;
    }

    void inline SetOpcode(RnOpCode opcode) {
        _opcode = opcode;
    }

    [[nodiscard]] inline uint32_t GetArg1() const {
        return _arg1;
    }

    void inline SetArg1(uint32_t arg1) {
        _arg1 = arg1;
    }

    [[nodiscard]] inline uint32_t GetArg2() const {
        return _arg2;
    }

    void inline SetArg2(uint32_t arg2) {
        _arg2 = arg2;
    }

    [[nodiscard]] inline uint32_t GetArg3() const {
        return _arg3;
    }

    void inline SetArg3(uint32_t arg3) {
        _arg3 = arg3;
    }

private:
    RnOpCode _opcode = OP_NOP;
    uint32_t _arg1 = 0;
    uint32_t _arg2 = 0;
    uint32_t _arg3 = 0;
};

#endif  //RONASCRIPT_RNINSTRUCTION_H