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
#include "../vm/RnObject.h"
#include "../vm/RnOpCode.h"

class RnInstruction {
public:
    RnInstruction() = default;

    explicit RnInstruction(RnOpCode opcode) {
        _opcode = opcode;
    }

    RnInstruction(RnOpCode opcode, uint32_t arg1) {
        _opcode = opcode;
        _arg1 = arg1;
    }

    RnInstruction(RnOpCode opcode, uint32_t arg1, uint32_t arg2) {
        _opcode = opcode;
        _arg1 = arg1;
        _arg2 = arg2;
    }

    RnInstruction(RnOpCode opcode, uint32_t arg1, uint32_t arg2, uint32_t arg3) {
        _opcode = opcode;
        _arg1 = arg1;
        _arg2 = arg2;
        _arg3 = arg3;
    }

    ~RnInstruction() = default;

    char* GetAsBytes() const;
    bool FromBytes(const char* bytes);
    std::string ToString();
    RnOpCode _opcode;
    uint32_t _arg1 = 0;
    uint32_t _arg2 = 0;
    uint32_t _arg3 = 0;
};

#endif  //RONASCRIPT_RNINSTRUCTION_H