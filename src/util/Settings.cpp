/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "Settings.h"

Settings* Settings::_instance = nullptr;

/*****************************************************************************/
Settings::Settings() {}

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
