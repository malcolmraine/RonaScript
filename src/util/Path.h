/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_SRC_UTIL_PATH_H
#define RONASCRIPT_SRC_UTIL_PATH_H

#include <string>
#include <vector>

class Path {
public:
    static std::vector<std::string> Split(const std::string& path);
    static std::string UNC(const std::string& path);
    static std::string Dir(const std::string& path);
    static std::string Name(const std::string& path);
    static std::string Ext(const std::string& path);
    static bool Exists(const std::string& path);
    static bool IsDir(const std::string& path);
    static bool IsFile(const std::string& path);
    static std::string IncrementName(const std::string& path);

private:
    Path() = default;
    ~Path() = default;
};

#endif  //RONASCRIPT_SRC_UTIL_PATH_H