/*****************************************************************************
* File: RnBinary.h
* Description:
* Author: Malcolm Hall
* Date: 4/20/23
* Version: 1
*
*****************************************************************************/

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include "../codegen/RnInstruction.h"

class RnObject;

/*****************************************************************************/
/*****************************************************************************/
class BinaryWriter {
public:
    explicit BinaryWriter(std::string file);
    ~BinaryWriter();
    void SetInstructions(const InstructionBlock& instructions);
    bool Write();

private:
    std::string _outfile;
    InstructionBlock _instructions;
};

/*****************************************************************************/
/*****************************************************************************/
class BinaryReader {
public:
    explicit BinaryReader(std::string file);
    ~BinaryReader();
    bool Read(InstructionBlock& instructions);
    RnObject* ReadObject(std::fstream& fs);

private:
    std::string _infile;
};
