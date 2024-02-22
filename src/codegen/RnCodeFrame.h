/*****************************************************************************
* File: RnCodeFrame.cpp
* Description:
* Author: Malcolm Hall
* Date: 2/20/24
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2024 Malcolm Hall
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

#include <stdint.h>
#include <string>
#include "../vm/RnOpCode.h"
#include "../common/RnType.h"
#include <fstream>

class RnInstruction;

class RnCodeFrame {
public:
    RnCodeFrame();
    ~RnCodeFrame();

    static RnCodeFrame* CreateFromFile(const std::string& file);
    static RnCodeFrame* CreateEmpty();
    [[nodiscard]] RnCodeFrame* GetSubframe(uint32_t index) const;
    [[nodiscard]] RnInstruction* GetInstruction(uint32_t index) const;
    RnCodeFrame* AddSubframe();
    RnInstruction* AddInstruction(RnOpCode opcode = OP_NOP, uint32_t arg1 = 0,
                                  uint32_t arg2 = 0, uint32_t arg3 = 0);
    bool WriteToFile(const std::string& file);
    void ReadFromFile(const std::string& file);
    [[nodiscard]] std::string ToString() const;

    void SetModulePath(const std::string& path) {
        _module_path_length = path.size();
        _module_path = new char[_module_path_length];
        std::memset(_module_path, 0, _module_path_length);
        std::memcpy(_module_path, path.c_str(), _module_path_length);
    }

    [[nodiscard]] uint32_t GetTimeStamp() const {
        return _timestamp;
    }

    [[nodiscard]] uint8_t GetFloatWidth() const {
        return _float_width;
    }

    [[nodiscard]] uint8_t GetIntWidth() const {
        return _int_width;
    }

    [[nodiscard]] uint8_t GetBoolWidth() const {
        return _bool_width;
    }

    [[nodiscard]] const char* GetModulePath() const {
        return _module_path;
    }

    [[nodiscard]] uint32_t GetSubframeCount() const {
        return _subframe_cnt;
    }

    [[nodiscard]] uint32_t GetInstructionCount() const {
        return _instruction_cnt;
    }

private:
    static void ReadFrame(std::ifstream& fs, RnCodeFrame* frame);
    static void WriteFrame(std::ofstream& fs, RnCodeFrame* frame);

private:
    uint64_t _timestamp = 0;
    uint8_t _int_width = sizeof(RnIntNative);
    uint8_t _float_width = sizeof(RnFloatNative);
    uint8_t _bool_width = sizeof(RnBoolNative);
    uint32_t _module_path_length = 0;
    char* _module_path = nullptr;
    uint32_t _subframe_cnt = 0;
    uint32_t _instruction_cnt = 0;
    RnInstruction* _instructions = nullptr;
    RnCodeFrame* _subframes = nullptr;
};
