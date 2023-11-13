/*****************************************************************************
* File: ArgParser.cpp
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

#include "ArgParser.h"

#include <iomanip>
#include <iostream>
#include "String.h"

#define HELP_STRING_COLUMN_PAD (10)

/*****************************************************************************/
Argument::Argument(const std::string& key, const std::string& description,
                   bool has_value)
    : _key(key), _description(description), _has_value(has_value) {}

/*****************************************************************************/
void Argument::SetAlternateKeys(const std::vector<std::string>& keys) {
    _alternates_keys = keys;
}

/*****************************************************************************/
std::vector<std::string> Argument::GetAlternateKeys() {
    return _alternates_keys;
}

/*****************************************************************************/
std::string Argument::GetKey() const {
    return _key;
}

/*****************************************************************************/
std::string Argument::GetValue() const {
    return _value;
}

/*****************************************************************************/
void Argument::SetValue(const std::string& value) {
    _value = value;
    _is_set = true;
}

/*****************************************************************************/
std::string Argument::GetDescription() const {
    return _description;
}

/*****************************************************************************/
bool Argument::IsSet() const {
    return _is_set;
}

/*****************************************************************************/
bool Argument::HasValue() const {
    return _has_value;
}

/*****************************************************************************/
/*****************************************************************************/
ArgParser::ArgParser() = default;

/*****************************************************************************/
void ArgParser::SetMainDescription(const std::string& description) {
    _main_description = description;
}

/*****************************************************************************/
void ArgParser::AddArgument(const std::string& arg,
                            const std::vector<std::string>& alternates,
                            const std::string& description, bool has_value,
                            const std::string& defaultValue) {
    auto argument = new Argument(arg, description, has_value);
    _arguments[arg] = argument;
    _longest_key_length =
        _longest_key_length > arg.size() ? _longest_key_length : arg.size();
    _ordered_args.push_back(arg);

    if (!defaultValue.empty()) {
        argument->SetValue(defaultValue);
    }

    argument->SetAlternateKeys(alternates);
    for (const auto& key : alternates) {
        _arguments[key] = argument;
        _help_exclusions.insert(key);
    }
}

/*****************************************************************************/
bool ArgParser::IsSet(const std::string& arg) {
    if (_arguments.contains(arg)) {
        return _arguments[arg]->IsSet();
    }
    return false;
}

/*****************************************************************************/
std::string ArgParser::GetValue(const std::string& arg) {
    return _arguments[arg]->GetValue();
}

/*****************************************************************************/
void ArgParser::ShowHelp() {
    std::string help;
    help.append(_main_description + "\n");
    for (const auto& key : _ordered_args) {
        if (_help_exclusions.find(key) != _help_exclusions.end()) {
            continue;
        }

        auto argument = _arguments[key];
        std::string key_str = key;
        for (const auto& alternate_key : argument->GetAlternateKeys()) {
            key_str.append(", " + alternate_key);
        }
        help.append(
            "  " +
            String::Pad(key_str, _longest_key_length + HELP_STRING_COLUMN_PAD, ' ') +
            argument->GetDescription() + "\n");
    }

    std::cout << help;
}

/*****************************************************************************/
void ArgParser::Parse(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        auto arg_str = std::string(argv[i]);
        if (_input_file.empty()) {
            if (_arguments.contains(arg_str)) {
                auto argument = _arguments[arg_str];
                if (argument->HasValue()) {
                    i++;
                    argument->SetValue(std::string(argv[i]));
                } else {
                    // Need to do this to toggle the set flag
                    argument->SetValue("");
                }
            } else if (arg_str.ends_with(".rn") || arg_str.ends_with(".rnc")) {
                _input_file = arg_str;
            }
        } else {
            _program_arguments.push_back(arg_str);
        }
    }
}

/*****************************************************************************/
std::string ArgParser::GetInputFile() const {
    return _input_file;
}

/*****************************************************************************/
std::vector<std::string> ArgParser::GetProgramArguments() {
    return _program_arguments;
}