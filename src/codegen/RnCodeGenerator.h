/*****************************************************************************
* File: RnCodeGenerator.h
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
* Version: 1
*
******************************************************************************/

#pragma once

#include <memory>
#include "../common/RnInternment.h"
#include "RnCodeGenVisitor.h"

class Ast;

class RnCodeGenerator {
public:
    RnCodeGenerator();
    ~RnCodeGenerator();
    void Generate(Ast* ast);
    void Optimize();

    InstructionBlock GetInstructions() {
        return instructions;
    }

private:
    InstructionBlock instructions;
    RnCodeGenVisitor visitor;
};
