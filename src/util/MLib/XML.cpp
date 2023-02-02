/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "XML.h"

/*****************************************************************************/
XMLElement::XMLElement(const std::string& tag) {
    _tag = tag;
}

/*****************************************************************************/
XMLElement::~XMLElement() {
    for (auto& child : _children) {
        delete child;
    }
}

/*****************************************************************************/
XMLElement* XMLElement::AddChild(const std::string& tag,
                                 const std::map<std::string, std::string>& attributes) {
    auto child = new XMLElement(tag);
    child->SetParent(this);
    child->SetDocument(_document);

    for (auto& attr : attributes) {
        SetAttribute(attr.first, attr.second);
    }
    _children.push_back(new XMLContent(child));

    return child;
}

/*****************************************************************************/
void XMLElement::AddData(const std::string& s) {
    _children.push_back(new XMLContent(s));
}

/*****************************************************************************/
XMLElement* XMLElement::GetChild(const std::string& path) {
    return nullptr;
}

/*****************************************************************************/
void XMLElement::SetAttribute(const std::string& key, const std::string& value) {
    _attributes[key] = value;
}

/*****************************************************************************/
void XMLElement::SetAttribute(const std::string& key, long long int value) {
    _attributes[key] = std::to_string(value);
}

/*****************************************************************************/
void XMLElement::SetAttribute(const std::string& key, double value) {
    _attributes[key] = std::to_string(value);
}

/*****************************************************************************/
std::string XMLElement::ToString() const {
    std::string result = "<" + _tag;

    for (auto& attr : _attributes) {
        result += " " + attr.first + "=" + "\"" + attr.second + "\"";
    }
    result += ">";

    for (auto child : _children) {
        result += child->ToString();
    }
    result += "</" + _tag + ">";

    return result;
}

/*****************************************************************************/
XMLElement* XMLElement::GetParent() const {
    return _parent;
}

/*****************************************************************************/
XMLDocument* XMLElement::GetDocument() const {
    return _document;
}

/*****************************************************************************/
void XMLElement::SetParent(XMLElement* parent) {
    _parent = parent;
}

/*****************************************************************************/
void XMLElement::SetDocument(XMLDocument* document) {
    _document = document;
}

/*****************************************************************************/
XMLContent::XMLContent() {}

/*****************************************************************************/
XMLContent::XMLContent(XMLElement* element) {
    _element = element;
    _type = Type::ELEMENT;
}

/*****************************************************************************/
XMLContent::XMLContent(const std::string& s) {
    _str = s;
    _type = Type::STRING;
}

/*****************************************************************************/
XMLContent::~XMLContent() {}

/*****************************************************************************/
XMLElement* XMLContent::GetElement() const {
    return _element;
}

/*****************************************************************************/
std::string XMLContent::GetString() const {
    return _str;
}

/*****************************************************************************/
void XMLContent::SetElement(XMLElement* element) {
    _element = element;
    _type = Type::ELEMENT;
}

/*****************************************************************************/
void XMLContent::SetString(const std::string& s) {
    _str = s;
    _type = Type::STRING;
}

/*****************************************************************************/
XMLContent::Type XMLContent::GetType() const {
    return _type;
}

/*****************************************************************************/
std::string XMLContent::ToString() const {
    if (_type == Type::STRING) {
        return _str;
    } else {
        return _element->ToString();
    }
}
