/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "Json.h"
#include <cctype>
#include <numeric>
#include <utility>

#define TAB ("\t")

/*****************************************************************************/
std::string MakeTabString(size_t n) {
    std::string result;
    for (size_t i = 0; i < n; i++) {
        result += TAB;
    }
    return result;
}

/*****************************************************************************/
/*****************************************************************************/
JSONObject::JSONObject() = default;

/*****************************************************************************/
JSONObject::~JSONObject() {
    for (const auto& pair : _data) {
        delete pair.second;
    }
}

/*****************************************************************************/
std::string JSONObject::ToString() const {
    std::string result = "{\n";
    for (const auto& pair : _data) {
        if (pair.second) {
            result += MakeTabString(_level) + "\"" + pair.first +
                      "\": " + pair.second->ToString() + ",\n";
        } else {
            result += MakeTabString(_level) + "\"" + pair.first + "\": null,\n";
        }
    }
    result = result.substr(0, result.length() - 1);

    if (_parent) {
        result += "\n" + MakeTabString(_parent->GetLevel()) + "}";
    } else {
        result += "\n}";
    }

    return result;
}

/*****************************************************************************/
JSONObject* JSONObject::SetKeyValue(const std::string& key, JSONNode* value) {
    // TODO: Handle existing key-value pairs with identical keys
    _data[key] = value;
    value->SetParent(this);
    return this;
}

/*****************************************************************************/
JSONObject* JSONObject::SetKeyValue(const std::string& key, const std::string& value) {
    _data[key] = new JSONString(value);
    return this;
}

/*****************************************************************************/
JSONObject* JSONObject::SetKeyValue(const std::string& key, double value) {
    _data[key] = new JSONFloat(value);
    return this;
}

/*****************************************************************************/
JSONObject* JSONObject::SetKeyValue(const std::string& key, long value) {
    _data[key] = new JSONInt(value);
    return this;
}

/*****************************************************************************/
JSONObject* JSONObject::SetKeyValue(const std::string& key, int value) {
    _data[key] = new JSONInt(value);
    return this;
}

/*****************************************************************************/
void JSONObject::RemoveKeyValue(const std::string& key, JSONNode* value) {
    auto iter = _data.find(key);
    if (iter == _data.end()) {
        _data.erase(iter);
    }
}

/*****************************************************************************/
/*****************************************************************************/
JSONList::JSONList() = default;

/*****************************************************************************/
JSONList::JSONList(const std::vector<JSONNode*>& data) {}

/*****************************************************************************/
JSONList::~JSONList() {
    for (auto element : GetData()) {
        delete element;
    }
}

/*****************************************************************************/
std::string JSONList::ToString() const {
    return JSONValue::ToString();
}

/*****************************************************************************/
/*****************************************************************************/
JSONFloat::JSONFloat() {
    _data = 0.0;
}

/*****************************************************************************/
JSONFloat::JSONFloat(double data) : JSONValue(data) {}

/*****************************************************************************/
std::string JSONFloat::ToString() const {
    return std::to_string(_data);
}

/*****************************************************************************/
/*****************************************************************************/
JSONInt::JSONInt() {
    _data = 0;
}

/*****************************************************************************/
JSONInt::JSONInt(long data) : JSONValue(data) {}

/*****************************************************************************/
std::string JSONInt::ToString() const {
    return std::to_string(_data);
}

/*****************************************************************************/
/*****************************************************************************/
JSONString::JSONString() {
    SetData("");
}

/*****************************************************************************/
JSONString::JSONString(std::string data) : JSONValue(std::move(data)) {}

/*****************************************************************************/
std::string JSONString::ToString() const {
    return "\"" + _data + "\"";
}

/*****************************************************************************/
/*****************************************************************************/
JSONParser::JSONParser() {}

/*****************************************************************************/
JSONParser::~JSONParser() {}

/*****************************************************************************/
JSONNode* JSONParser::Parse(const std::string& filename) {
    return nullptr;
}

/*****************************************************************************/
void JSONParser::Expect(char c) {
    if (_contents[_idx] != c) {
        throw std::runtime_error("Invalid character '" + std::string(1, c) +
                                 "' at position " + std::to_string(_idx));
    }
}

/*****************************************************************************/
void JSONParser::ConsumeWhiteSpace() {
    while (_idx < _contents.length() &&
           (_contents[_idx] == '\t' || _contents[_idx] == '\r' ||
            _contents[_idx] == '\n' || _contents[_idx] == ' ')) {
        _idx++;
    }
}

/*****************************************************************************/
JSONList* JSONParser::ParseList() {
    return nullptr;
}

/*****************************************************************************/
JSONObject* JSONParser::ParseObject() {
    ConsumeWhiteSpace();
    Expect('{');
    _idx++;
    ConsumeWhiteSpace();
    auto obj = new JSONObject();

    while (_idx < _contents.length() && _contents[_idx] != '}') {
        std::string key;
        while (_idx < _contents.length() && _contents[_idx] != ':') {
            key += std::string(1, _contents[_idx]);
        }
        Expect(':');
        _idx++;

        obj->SetKeyValue(key, ParseValue());
    }
    return obj;
}

/*****************************************************************************/
JSONString* JSONParser::ParseString() {
    return nullptr;
}

/*****************************************************************************/
JSONNode* JSONParser::ParseNumeric() {
    bool decimal_found = false;
    std::string result;

    while (_contents[_idx] != ',' && _contents[_idx] != '\n') {
        if (std::isdigit(_contents[_idx]) ||
            (!decimal_found && _contents[_idx] == '.')) {
            result += std::string(1, _contents[_idx]);
            _idx++;
        }
    }

    if (decimal_found) {
        return new JSONFloat(std::atof(result.c_str()));
    }
    return new JSONInt(std::atol(result.c_str()));
}

/*****************************************************************************/
JSONNode* JSONParser::ParseValue() {
    return nullptr;
}
