/*****************************************************************************
* File: RnCodeFrame.cpp
* Description:
* Author: Malcolm Hall
* Date: 2/20/24
* Version: 1
*
*****************************************************************************/

#include "RnCodeFrame.h"
#include <chrono>
#include <memory>
#include "RnInstruction.h"

#define CODEFRAME_END_MARKER "RN_CODE_FRAME_END"

/*****************************************************************************/
RnCodeFrame::RnCodeFrame() {
    SetModulePath("");
    const auto epoch = std::chrono::system_clock::now().time_since_epoch();
    _timestamp = static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::seconds>(epoch).count());
}

/*****************************************************************************/
RnCodeFrame::~RnCodeFrame() {}

/*****************************************************************************/
RnCodeFrame* RnCodeFrame::CreateFromFile(const std::string& file) {
    auto frame = CreateEmpty();
    frame->ReadFromFile(file);
    return frame;
}

/*****************************************************************************/
RnCodeFrame* RnCodeFrame::CreateEmpty() {
    auto frame = new RnCodeFrame();
    return frame;
}

/*****************************************************************************/
RnCodeFrame* RnCodeFrame::GetSubframe(uint32_t index, LOAD_POLICY load_policy) const {
    assert(index <= _subframe_cnt);
    auto frame = const_cast<RnCodeFrame*>(_subframes.data() + index);
    assert(frame);

    switch (load_policy) {
        case LOAD_POLICY::LAZY:
            // Load only the requested frame
            if (frame->GetIsExternal() && !frame->_is_loaded) {
                frame->ReadFromFile(frame->GetModulePath());
            }
            frame->_is_loaded = true;
            break;
        case LOAD_POLICY::EAGER:
            // Load the requested frame and all its subframes immediately
            if (frame->GetIsExternal() && !frame->_is_loaded) {
                frame->ReadFromFile(frame->GetModulePath());
            }
            for (uint32_t i = 0; i < frame->GetSubframeCount(); i++) {
                frame->GetSubframe(i, load_policy);
            }
            frame->_is_loaded = true;
            break;
        case LOAD_POLICY::NONE:
            // Get the requested subframe without loading
            break;
        default:
            assert(false);
            break;
    }

    return frame;
}

///*****************************************************************************/
//RnInstruction* RnCodeFrame::GetInstruction(uint32_t index) const {
//    assert(index < _instruction_cnt);
//    auto instruction = const_cast<RnInstruction*>(_instructions.data() + index);
//    assert(instruction);
//    return instruction;
//}

/*****************************************************************************/
RnCodeFrame* RnCodeFrame::AddSubframe() {
    _subframes.emplace_back();
    auto frame = const_cast<RnCodeFrame*>(_subframes.data() + _subframe_cnt);
    _subframe_cnt += 1;
    return frame;
}

/*****************************************************************************/
RnInstruction* RnCodeFrame::AddInstruction(RnOpCode opcode, RnInstructionArg arg1,
                                           RnInstructionArg arg2,
                                           RnInstructionArg arg3) {
    _instructions.emplace_back(opcode, arg1, arg2, arg3);
    auto instruction =
        const_cast<RnInstruction*>(_instructions.data() + _instruction_cnt);
    _instruction_cnt += 1;
    return instruction;
}

#include <iostream>
/*****************************************************************************/
void RnCodeFrame::ReadFrame(std::ifstream& fs, RnCodeFrame* frame) {
    delete frame->_module_path;
    frame->_module_path = nullptr;
    frame->_instructions.clear();
    frame->_subframes.clear();
    frame->_int_width = 0;
    frame->_bool_width = 0;
    frame->_float_width = 0;
    frame->_module_path_length = 0;
    frame->_instruction_cnt = 0;
    frame->_subframe_cnt = 0;
    frame->_is_external = 0;

    fs.read(reinterpret_cast<char*>(&frame->_timestamp), sizeof(frame->_timestamp));
    fs.read(reinterpret_cast<char*>(&frame->_is_external), sizeof(frame->_is_external));
    fs.read(reinterpret_cast<char*>(&frame->_float_width), sizeof(frame->_float_width));
    fs.read(reinterpret_cast<char*>(&frame->_bool_width), sizeof(frame->_bool_width));
    fs.read(reinterpret_cast<char*>(&frame->_module_path_length),
            sizeof(frame->_module_path_length));

    frame->_module_path = new char[frame->_module_path_length];
    std::memset(frame->_module_path, 0, frame->_module_path_length);
    fs.read(frame->_module_path, frame->_module_path_length);

    uint32_t subframe_count = 0;
    fs.read(reinterpret_cast<char*>(&subframe_count), sizeof(subframe_count));

    uint32_t instruction_count = 0;
    fs.read(reinterpret_cast<char*>(&instruction_count), sizeof(instruction_count));
    frame->_instructions.reserve(instruction_count);

    for (uint32_t i = 0; i < instruction_count; i++) {
        RnOpCode opcode = OP_NOP;
        RnInstructionArg arg1 = 0;
        RnInstructionArg arg2 = 0;
        RnInstructionArg arg3 = 0;
        fs.read(reinterpret_cast<char*>(&opcode), RN_INSTRUCTION_OP_WIDTH);
        fs.read(reinterpret_cast<char*>(&arg1), RN_INSTRUCTION_ARG_WIDTH);
        fs.read(reinterpret_cast<char*>(&arg2), RN_INSTRUCTION_ARG_WIDTH);
        fs.read(reinterpret_cast<char*>(&arg3), RN_INSTRUCTION_ARG_WIDTH);
        frame->AddInstruction(opcode, arg1, arg2, arg3);
    }

    frame->_instructions.reserve(subframe_count);
    for (uint32_t i = 0; i < subframe_count; i++) {
        auto subframe = frame->AddSubframe();
        ReadFrame(fs, subframe);
    }
    assert(frame->_subframe_cnt == subframe_count);

    char* frameEndMarker = new char[std::string(CODEFRAME_END_MARKER).size()];
    fs.read(frameEndMarker, 17);
    if (frameEndMarker != std::string(CODEFRAME_END_MARKER)) {
        throw std::runtime_error("Failed to read code frame: no end marker found.");
    }
}

/*****************************************************************************/
void RnCodeFrame::WriteFrame(std::ofstream& fs, RnCodeFrame* frame) {
    fs.write(reinterpret_cast<char*>(&frame->_timestamp), sizeof(frame->_timestamp));
    fs.write(reinterpret_cast<char*>(&frame->_is_external),
             sizeof(frame->_is_external));
    fs.write(reinterpret_cast<char*>(&frame->_int_width), sizeof(frame->_int_width));
    fs.write(reinterpret_cast<char*>(&frame->_float_width),
             sizeof(frame->_float_width));
    fs.write(reinterpret_cast<char*>(&frame->_bool_width), sizeof(frame->_bool_width));
    fs.write(reinterpret_cast<char*>(&frame->_module_path_length),
             sizeof(frame->_module_path_length));
    fs.write(frame->_module_path, frame->_module_path_length);
    fs.write(reinterpret_cast<char*>(&frame->_subframe_cnt),
             sizeof(frame->_subframe_cnt));
    fs.write(reinterpret_cast<char*>(&frame->_instruction_cnt),
             sizeof(_instruction_cnt));

    for (uint32_t i = 0; i < frame->_instruction_cnt; i++) {
        auto instruction = frame->GetInstruction(i);
        RnOpCode opcode = instruction->GetOpcode();
        RnInstructionArg arg1 = instruction->GetArg1();
        RnInstructionArg arg2 = instruction->GetArg2();
        RnInstructionArg arg3 = instruction->GetArg3();
        fs.write(reinterpret_cast<char*>(&opcode), RN_INSTRUCTION_OP_WIDTH);
        fs.write(reinterpret_cast<char*>(&arg1), RN_INSTRUCTION_ARG_WIDTH);
        fs.write(reinterpret_cast<char*>(&arg2), RN_INSTRUCTION_ARG_WIDTH);
        fs.write(reinterpret_cast<char*>(&arg3), RN_INSTRUCTION_ARG_WIDTH);
    }

    for (uint32_t i = 0; i < frame->_subframe_cnt; i++) {
        WriteFrame(fs, frame->GetSubframe(i));
    }

    std::string frameEndMarker = CODEFRAME_END_MARKER;
    fs.write(frameEndMarker.c_str(),
             static_cast<std::streamsize>(frameEndMarker.size()));
}

/*****************************************************************************/
bool RnCodeFrame::WriteToFile(const std::string& file) {
    std::ofstream fs;
    fs.open(file.c_str(), std::ios_base::in | std::ios_base::out |
                              std::ios_base::trunc | std::ios_base::binary);

    if (fs.fail()) {
        throw std::runtime_error("Failed to open file: " + file);
    }

    WriteFrame(fs, this);
    fs.flush();
    fs.close();

    return true;
}

/*****************************************************************************/
void RnCodeFrame::ReadFromFile(const std::string& file) {
    std::ifstream fs;
    fs.open(file.c_str(), std::ios_base::in | std::ios_base::binary);

    if (fs.fail()) {
        throw std::runtime_error("Failed to open file: " + file);
    }

    ReadFrame(fs, this);
}

/*****************************************************************************/
std::string RnCodeFrame::ToString() const {
    std::string str;
    str += std::string(80, '=') + "\n";
    str += "BEGIN Code Frame\n";
    str += std::string(80, '=') + "\n";
    str += "TIMESTAMP:         " + std::to_string(_timestamp) + "\n";
    str += "INT_WIDTH:         " + std::to_string(_int_width) + "\n";
    str += "FLOAT_WIDTH:       " + std::to_string(_float_width) + "\n";
    str += "BOOL_WIDTH:        " + std::to_string(_bool_width) + "\n";
    str += "MODULE_PATH:       " + std::string(_module_path ? _module_path : "") + "\n";
    str += "SUBFRAME COUNT:    " + std::to_string(_subframe_cnt) + "\n";
    str += "INSTRUCTION COUNT: " + std::to_string(_instruction_cnt) + "\n";

    str += "INSTRUCTIONS:\n";
    for (uint32_t i = 0; i < _instruction_cnt; i++) {
        str += "    " + GetInstruction(i)->ToString() + "\n";
    }

    str += "SUBFRAMES: ";
    if (_subframe_cnt == 0) {
        str += "N/A\n";
    } else {
        str += "\n";
        for (uint32_t i = 0; i < _subframe_cnt; i++) {
            str += GetSubframe(i)->ToString();
        }
    }

    str += std::string(80, '=') + "\n";
    str += "END Code Frame\n";
    str += std::string(80, '=') + "\n";
    return str;
}

/*****************************************************************************/
void RnCodeFrame::PrependInstructionBlock(const std::vector<RnInstruction>& block) {
    _instructions.insert(_instructions.begin(), block.begin(), block.end());
}

/*****************************************************************************/
void RnCodeFrame::AppendInstructionBlock(const std::vector<RnInstruction>& block) {
    _instructions.insert(_instructions.end(), block.begin(), block.end());
}