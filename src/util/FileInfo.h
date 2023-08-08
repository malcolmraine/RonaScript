/*****************************************************************************
* File: RnFileInfo.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2021 Malcolm Hall
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

#include <fstream>
#include <memory>
#include <string>

#define RN_COMPILED_FILE_EXT "rnc"
#define RN_SOURCE_FILE_EXT "rn"

class FileInfo {
public:
    FileInfo() = default;
    explicit FileInfo(const std::string& filepath);
    FileInfo(const FileInfo& other);
    ~FileInfo();
    void SetFilePath(const std::string& filepath);
    [[nodiscard]] std::string GetFilePath() const;
    void SetLineNum(size_t linenum);
    [[nodiscard]] size_t GetLineNum() const;
    void IncrementLineNum();
    void SetCharNum(size_t charnum);
    [[nodiscard]] size_t GetCharNum() const;
    void IncrementCharNum();
    std::string GetContextualBlock(bool formatted = true);
    std::string GetLine();
    std::string ToString() const;
    [[nodiscard]] bool IsOriginal() const;

    FileInfo& operator=(const FileInfo& file_info) {
        SetFilePath(file_info.GetFilePath());
        _previous_line_start = file_info._previous_line_start;
        _linenum = file_info._linenum;
        _charnum = file_info._charnum;
        _char_cnt = file_info._char_cnt;
        _line_prefix = file_info._line_prefix;
        _is_original = false;

        return *this;
    }

    static void SetContextualBlockTabLvl(int n) {
        FileInfo::CONTEXTUAL_BLOCK_TAB_LVL = n;
    }

    static int GetContextualBlockTabLvl() {
        return FileInfo::CONTEXTUAL_BLOCK_TAB_LVL;
    }

    static bool IsCompiledFile(const std::string& file) {
        return file.ends_with(RN_COMPILED_FILE_EXT);
    }

    static bool IsSourceFile(const std::string& file) {
        return file.ends_with(RN_SOURCE_FILE_EXT);
    }

    void UpdateLineStartValues(size_t line_start) {
        _previous_line_start = line_start;
    }

    [[nodiscard]] size_t GetCharCount() const {
        return _char_cnt;
    }

    [[nodiscard]] size_t GetPreviousLineStart() const {
        return _previous_line_start;
    }

protected:
    static int CONTEXTUAL_BLOCK_TAB_LVL;
    static std::string GetContextualBlockTabStr();
    std::string GetLineAt(size_t line_start, bool keep_open = false,
                          bool formatted = true);
    std::string GetFormattedLine(const std::string& line);
    std::string _filepath;
    size_t _linenum = 0;
    size_t _charnum = 0;
    size_t _previous_line_start = 0;
    size_t _char_cnt = 0;
    bool _is_original = true;
    std::ifstream _file_obj;
    std::string _line_prefix = "\033[1;31m>";
    std::string _line_suffix = "\033[0m";
};
