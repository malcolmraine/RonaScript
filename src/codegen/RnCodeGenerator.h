/*****************************************************************************
* File: RnCodeGenerator.h
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNCODEGENERATOR_H
#define RONASCRIPT_RNCODEGENERATOR_H

#include "../parser/ast/Ast.h"
#include "../vm/RnInternment.h"
#include "RnCodeGenVisitor.h"

class RnCodeGenerator {
public:
    RnCodeGenerator();
    ~RnCodeGenerator();
    void Generate(Ast* ast);
    void Optimize();

    InstructionBlock GetInstructions() { return instructions; }

private:
    InstructionBlock instructions;
    RnCodeGenVisitor visitor;
};

#endif  //RONASCRIPT_RNCODEGENERATOR_H