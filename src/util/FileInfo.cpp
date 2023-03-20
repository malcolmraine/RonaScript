/*****************************************************************************
* File: FileInfo.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/16/22
* Version: 1
*
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
    std::string block = "\033[0m" + GetContextualBlockTabStr() +
                        GetLineAt(_previous_line_start, true, false);
    std::string line;

    if (std::getline(_file_obj, line, '\r')) {
        if (formatted) {
            block += GetFormattedLine(line);
        } else {
            block += line + "\n";
        }

        if (std::getline(_file_obj, line, '\r')) {
            if (formatted) {
                block += GetContextualBlockTabStr();
            }
            block += line + "\n";
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
bool FileInfo::IsOriginal() const {
    return _is_original;
}

/*****************************************************************************/
std::string FileInfo::GetLineAt(size_t line_start, bool keep_open, bool formatted) {
    if (!_file_obj.is_open()) {
        _file_obj.open(GetFilePath(), std::ios::in);
    }
    _file_obj.seekg(line_start);
    std::string line;
    std::getline(_file_obj, line, '\r');

    if (!keep_open) {
        _file_obj.close();
    }

    if (formatted) {
        return GetFormattedLine(line) + "\n";
    } else {
        return line + "\n";
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
    return _line_prefix + GetContextualBlockTabStr().substr(CONTEXTUAL_BLOCK_TAB_LVL + 1) +
           line + _line_suffix + "\n";
}
