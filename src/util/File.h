/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#pragma once

#include <string>

class File {
public:
    static std::string GetContents(const std::string& path);
    static size_t LineCount(const std::string& path);
    static size_t Size(const std::string& path);
    static bool Remove(const std::string& path);

private:
    File() = default;
    ~File() = default;
};
