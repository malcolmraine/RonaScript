/*****************************************************************************
* File: FileInfo.h
* Description:
* Author: Malcolm Hall
* Date: 5/16/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_FILEINFO_H
#define RONASCRIPT_FILEINFO_H

#include <fstream>
#include <memory>
#include <string>

#define RN_COMPILED_FILE_EXT "rnc"
#define RN_SOURCE_FILE_EXT "rn"

typedef long long FileSize;

class FileInfo {
public:
    FileInfo() = default;
    explicit FileInfo(const std::string& filepath);
    FileInfo(const FileInfo& other);
    ~FileInfo();
    void SetFilePath(const std::string& filepath);
    [[nodiscard]] std::string GetFilePath() const;
    void SetLineNum(FileSize linenum);
    [[nodiscard]] FileSize GetLineNum() const;
    void IncrementLineNum();
    void SetCharNum(FileSize charnum);
    [[nodiscard]] FileSize GetCharNum() const;
    void IncrementCharNum();
    std::string GetContextualBlock(bool formatted = true);
    std::string GetLine();
    std::string ToString() const;
    [[nodiscard]] bool IsOriginal() const;

    FileInfo& operator=(const FileInfo& file_info) {
        SetFilePath(file_info.GetFilePath());
        _current_line_start = file_info._current_line_start;
        _previous_line_start = file_info._previous_line_start;
        _next_line_start = file_info._next_line_start;
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

    void UpdateLineStartValues(FileSize current_line_start) {
        _previous_line_start = _current_line_start;
        _current_line_start = _next_line_start;
        _next_line_start = current_line_start;
    }

    [[nodiscard]] FileSize GetCurrentLineStart() const {
        return _current_line_start;
    }

    [[nodiscard]] FileSize GetCharCount() const {
        return _char_cnt;
    }

protected:
    static int CONTEXTUAL_BLOCK_TAB_LVL;
    static std::string GetContextualBlockTabStr();
    std::string GetLineAt(FileSize line_start, bool keep_open = false,
                          bool formatted = true);
    std::string GetFormattedLine(const std::string& line);
    std::shared_ptr<std::string> _filepath = nullptr;
    FileSize _linenum{};
    FileSize _charnum{};
    FileSize _previous_line_start = 0;
    FileSize _current_line_start = 0;
    FileSize _next_line_start = 0;
    FileSize _char_cnt = 0;
    bool _is_original = true;
    std::ifstream _file_obj;
    std::string _line_prefix = "\033[1;31m>\033[0m";
};

#endif  //RONASCRIPT_FILEINFO_H