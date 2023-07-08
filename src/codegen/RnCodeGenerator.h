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
#include "../common/RnCompilerPhase.h"

class Ast;

class RnCodeGenerator : public RnCompilerPhase<Ast*, InstructionBlock>{
public:
    RnCodeGenerator();
    ~RnCodeGenerator() override;
    void Generate(Ast* ast);
    void Optimize();
    void Run() override;

private:
    RnCodeGenVisitor visitor;
};
