/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
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
******************************************************************************/

#include "Settings.h"

Settings* Settings::_instance = nullptr;

/*****************************************************************************/
Settings::Settings() = default;

/*****************************************************************************/
Settings::~Settings() {
    for (auto& setting : _settings) {
        delete setting.second;
    }
}

/*****************************************************************************/
Settings* Settings::Instance() {
    if (!_instance) {
        _instance = new Settings();
    }
    return _instance;
}

/*****************************************************************************/
void Settings::Store(const std::string& key, const std::string& value) {
    Delete(key);
    _settings[key] = new SettingsVariant(value);
}

/*****************************************************************************/
void Settings::Store(const std::string& key, const char* value) {
    Store(key, std::string(value));
}

/*****************************************************************************/
void Settings::Store(const std::string& key, long value) {
    Delete(key);
    _settings[key] = new SettingsVariant(value);
}

/*****************************************************************************/
void Settings::Store(const std::string& key, double value) {
    Delete(key);
    _settings[key] = new SettingsVariant(value);
}

/*****************************************************************************/
void Settings::Store(const std::string& key, bool value) {
    Delete(key);
    _settings[key] = new SettingsVariant(value);
}

/*****************************************************************************/
std::string Settings::GetString(const std::string& key,
                                const std::string& default_value) {
    auto value = GetValue(key);
    if (value) {
        return std::get<std::string>(*value);
    }
    return default_value;
}

/*****************************************************************************/
long Settings::GetInteger(const std::string& key, long default_value) {
    auto value = GetValue(key);
    if (value) {
        return std::get<long>(*value);
    }
    return default_value;
}

/*****************************************************************************/
double Settings::GetDouble(const std::string& key, double default_value) {
    auto value = GetValue(key);
    if (value) {
        return std::get<double>(*value);
    }
    return default_value;
}

/*****************************************************************************/
bool Settings::GetBool(const std::string& key, bool default_value) {
    auto value = GetValue(key);
    if (value) {
        return std::get<bool>(*value);
    }
    return default_value;
}

/*****************************************************************************/
SettingsVariant* Settings::GetValue(const std::string& key) {
    auto value = _settings.find(key);
    if (value == _settings.end()) {
        return nullptr;
    }
    return value->second;
}

/*****************************************************************************/
bool Settings::Delete(const std::string& key) {
    auto value = GetValue(key);
    delete value;
    return true;
}
