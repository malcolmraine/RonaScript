/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "File.h"
#include <fstream>
#include <ostream>

/*****************************************************************************/
std::string File::GetContents(const std::string& path) {
    std::string contents;
    std::ifstream f;
    f.open(path, std::ios::in);
    f >> contents;
    f.close();

    return contents;
}

/*****************************************************************************/
size_t File::LineCount(const std::string& path) {
    size_t line_count = 0;

    for (char c : GetContents(path)) {
        if (c == '\n') {
            line_count++;
        }
    }
    return line_count;
}

/*****************************************************************************/
size_t File::Size(const std::string& path) {
    std::streampos begin, end;
    std::ifstream f(path, std::ios::binary);
    begin = f.tellg();
    f.seekg(0, std::ios::end);
    end = f.tellg();
    f.close();

    return end - begin;
}

/*****************************************************************************/
bool File::Remove(const std::string& path) {
    return std::filesystem::remove(path);
}