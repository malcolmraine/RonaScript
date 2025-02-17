/*****************************************************************************
 * File: ArgParser.h
 * Description:
 * Author: Malcolm Hall
 * Date: 1/25/23
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
 *****************************************************************************/

#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class Argument {
public:
    Argument(const std::string& key, const std::string& description, bool has_value);
    void SetAlternateKeys(const std::vector<std::string>& keys);
    std::vector<std::string> GetAlternateKeys();
    [[nodiscard]] std::string GetKey() const;
    [[nodiscard]] std::string GetValue() const;
    void SetValue(const std::string& value);
    [[nodiscard]] std::string GetDescription() const;
    [[nodiscard]] bool IsSet() const;
    [[nodiscard]] bool HasValue() const;

private:
    std::string _key;
    std::string _value;
    bool _is_set = false;
    std::string _description;
    bool _has_value = false;
    std::vector<std::string> _alternates_keys{};
};

class ArgParser {
public:
    ArgParser();
    void SetMainDescription(const std::string& description);
    void AddArgument(const std::string& arg, const std::vector<std::string>& alternates,
                     const std::string& description, bool has_value = false,
                     const std::string& defaultValue = "");
    [[nodiscard]] bool IsSet(const std::string& arg);
    [[nodiscard]] std::string GetValue(const std::string& arg);
    void ShowHelp();
    void Parse(int argc, char* argv[]);
    [[nodiscard]] std::string GetInputFile() const;
    std::vector<std::string> GetProgramArguments();

private:
    std::map<std::string, Argument*> _arguments{};
    std::string _main_description;
    std::string _input_file;
    std::unordered_set<std::string> _help_exclusions{};
    std::vector<std::string> _ordered_args{};
    size_t _longest_key_length = 0;
    std::vector<std::string> _program_arguments;
};
