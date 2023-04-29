/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
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