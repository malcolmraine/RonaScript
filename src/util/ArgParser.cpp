/*****************************************************************************
 * File: ArgParser.cpp
 * Description:
 * Author: Malcolm Hall
 * Date: 1/25/23
 * Version: 1
 *
 *****************************************************************************/

#include "ArgParser.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "MLib/String.h"

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
ArgParser::ArgParser() {}

/*****************************************************************************/
void ArgParser::SetMainDescription(const std::string& description) {
    _main_description = description;
}

/*****************************************************************************/
void ArgParser::AddArgument(const std::string& arg,
                            const std::vector<std::string>& alternates,
                            const std::string& description, bool has_value,
                            const std::string& defaultValue) {
    auto argument = std::make_shared<Argument>(arg, description, has_value);
    _arguments[arg] = argument;
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
    std::stringstream help;
    help << _main_description + "\n";
    for (const auto& key : _ordered_args) {
        if (_help_exclusions.contains(key)) {
            continue;
        }
        //../examples/Test.rn -past -ptok -pcode

        auto argument = _arguments[key];
        std::string key_str = key;
        for (const auto& alternate_key : argument->GetAlternateKeys()) {
            key_str += ", " + alternate_key;
        }
        help << "  " << String::Pad(key_str, 20, ' ') << argument->GetDescription()
             << "\n";
        std::cout << "Creating help" << std::endl;
    }

    std::cout << help.str();
}

/*****************************************************************************/
void ArgParser::Parse(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        auto arg_str = std::string(argv[i]);
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
    }
}

/*****************************************************************************/
std::string ArgParser::GetInputFile() const {
    return _input_file;
}