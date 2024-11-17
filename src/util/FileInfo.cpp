/*****************************************************************************
* File: FileInfo.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/16/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
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

#include "FileInfo.h"
#include <utility>

int FileInfo::CONTEXTUAL_BLOCK_TAB_LVL = 2;

/*****************************************************************************/
FileInfo::FileInfo(const std::string& filepath) {
    _filepath = filepath;
}

/*****************************************************************************/
FileInfo::FileInfo(const FileInfo& other) {
    SetFilePath(other.GetFilePath());
    SetCharNum(other.GetCharNum());
    SetLineNum(other.GetLineNum());
    _previous_line_start = other.GetPreviousLineStart();
    _is_original = false;
}

/*****************************************************************************/
FileInfo::~FileInfo() = default;

/*****************************************************************************/
void FileInfo::SetFilePath(const std::string& filepath) {
    _filepath = filepath;
}

/*****************************************************************************/
std::string FileInfo::GetFilePath() const {
    return _filepath;
}

/*****************************************************************************/
void FileInfo::SetLineNum(size_t linenum) {
    _linenum = linenum;
}

/*****************************************************************************/
size_t FileInfo::GetLineNum() const {
    return _linenum;
}

/*****************************************************************************/
void FileInfo::IncrementLineNum() {
    ++_linenum;
}

/*****************************************************************************/
void FileInfo::SetCharNum(size_t charnum) {
    _charnum = charnum;
}

/*****************************************************************************/
size_t FileInfo::GetCharNum() const {
    return _charnum;
}

/*****************************************************************************/
void FileInfo::IncrementCharNum() {
    ++_charnum;
    ++_char_cnt;
}

/*****************************************************************************/
std::string FileInfo::GetContextualBlock(bool formatted) {
    size_t line_start_index = _previous_line_start > 0 ? (_previous_line_start - 1) : 0;
    std::string block = _line_prefix + " " + GetContextualBlockTabStr() +
                        GetLineAt(line_start_index, true, false) + "\n";
    std::string line;

    if (std::getline(_file_obj, line, '\r')) {
        if (formatted) {
            block += ">" + GetFormattedLine(line) + "\n";
        } else {
            block += " " + line + "\n";
        }

        if (std::getline(_file_obj, line, '\r')) {
            if (formatted) {
                block += " " + GetFormattedLine(line) + '\n';
            } else {
                block += " " + line + "\n";
            }
        }
    }
    return block;
}

/*****************************************************************************/
std::string FileInfo::ToString() const {
    return ": " + GetFilePath() + "  " + std::to_string(_linenum + 1) + ":" +
           std::to_string(_charnum + 1);
}

/*****************************************************************************/
std::string FileInfo::GetLineAt(size_t line_start, bool keep_open, bool formatted) {
    if (!_file_obj.is_open()) {
        _file_obj.open(GetFilePath(), std::ios::in);
    }
    _file_obj.seekg(
        static_cast<std::streamoff>(line_start == 0 ? line_start : line_start - 1));
    std::string line;
    std::getline(_file_obj, line, '\r');

    if (!keep_open) {
        _file_obj.close();
    }

    if (formatted) {
        return GetFormattedLine(line);
    } else {
        return line;
    }
}

/*****************************************************************************/
std::string FileInfo::GetContextualBlockTabStr() {
    std::string s;

    for (int i = 0; i < FileInfo::CONTEXTUAL_BLOCK_TAB_LVL; ++i) {
        s += "    ";
    }

    return s;
}

/*****************************************************************************/
std::string FileInfo::GetFormattedLine(const std::string& line) {
    return _line_prefix + GetContextualBlockTabStr() + line + _line_suffix;
}
