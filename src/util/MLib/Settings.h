/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef MLIB_SETTINGS_H
#define MLIB_SETTINGS_H

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

#endif  //MLIB_SETTINGS_H