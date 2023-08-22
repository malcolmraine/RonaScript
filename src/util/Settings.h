/*****************************************************************************
* File: Settings.h
* Description:
* Author: Malcolm Hall
* Date: 6/23/22
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

#pragma once

#include <map>
#include <string>
#include <variant>

typedef std::variant<long, std::string, double, bool> SettingsVariant;

/*****************************************************************************/
class Settings {
public:
    static Settings* Instance();
    void Store(const std::string& key, const std::string& value);
    void Store(const std::string& key, const char* value);
    void Store(const std::string& key, long value);
    void Store(const std::string& key, double value);
    void Store(const std::string& key, bool value);
    std::string GetString(const std::string& key,
                          const std::string& default_value = "");
    long GetInteger(const std::string& key, long default_value = -1);
    double GetDouble(const std::string& key, double default_value = -1);
    bool GetBool(const std::string& key, bool default_value = false);
    SettingsVariant* GetValue(const std::string& key);
    bool Delete(const std::string& key);

private:
    static Settings* _instance;
    Settings();
    ~Settings();
    std::map<std::string, SettingsVariant*> _settings;
};
