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
    RnInstruction();
    explicit RnInstruction(RnOpCode opcode);
    RnInstruction(RnOpCode opcode, uint32_t arg1);
    RnInstruction(RnOpCode opcode, uint32_t arg1, uint32_t arg2);
    RnInstruction(RnOpCode opcode, uint32_t arg1, uint32_t arg2, uint32_t arg3);
    ~RnInstruction();

    char* GetAsBytes() const;
    bool FromBytes(const char* bytes);
    std::string ToString();
    RnOpCode _opcode = RnOpCode::OP_NOP;
    uint32_t _arg1 = 0;
    uint32_t _arg2 = 0;
    uint32_t _arg3 = 0;
};

#endif  //RONASCRIPT_RNINSTRUCTION_H