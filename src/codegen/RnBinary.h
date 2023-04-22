/*****************************************************************************
* File: RnBinary.h
* Description:
* Author: Malcolm Hall
* Date: 4/20/23
* Version: 1
*
*****************************************************************************/

#ifndef RONASCRIPT_SRC_CODEGEN_RNBINARY_H_
#define RONASCRIPT_SRC_CODEGEN_RNBINARY_H_

#include <string>
#include "../codegen/RnInstruction.h"
#include <fstream>
#include <iostream>

class RnObject;

/*****************************************************************************/
/*****************************************************************************/
class BinaryWriter {
public:
    explicit BinaryWriter(std::string  file);
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
    explicit BinaryReader(std::string  file);
    ~BinaryReader();
    bool Read(InstructionBlock& instructions);
    RnObject* ReadObject(std::fstream& fs);

private:
    std::string _infile;
};



#endif  //RONASCRIPT_SRC_CODEGEN_RNBINARY_H_