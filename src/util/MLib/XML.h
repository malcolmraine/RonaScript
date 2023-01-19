/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef MLIB_XML_H
#define MLIB_XML_H

#include <string>
#include <map>
#include <vector>

class XMLDocument;
class XMLElement;

class XMLContent
{
 public:
	enum Type
	{
		ELEMENT, STRING
	};

	XMLContent();
	XMLContent(XMLElement* element);
	XMLContent(const std::string& s);
	~XMLContent();
	[[nodiscard]] XMLElement* GetElement() const;
	[[nodiscard]] std::string GetString() const;
	void SetElement(XMLElement* element);
	void SetString(const std::string& s);
	[[nodiscard]] Type GetType() const;
	[[nodiscard]] std::string ToString() const;

 private:
	Type _type = Type::STRING;
	XMLElement* _element = nullptr;
	std::string _str;

};

class XMLElement
{
 public:
	XMLElement(const std::string& tag);
	~XMLElement();
	XMLElement* AddChild(const std::string& tag,
		const std::map<std::string, std::string>& attributes = {});
	void AddData(const std::string& s);
	[[nodiscard]] XMLElement* GetChild(const std::string& path);
	void SetAttribute(const std::string& key, const std::string& value);
	void SetAttribute(const std::string& key, long long value);
	void SetAttribute(const std::string& key, double value);
	[[nodiscard]] std::string ToString() const;
	[[nodiscard]] XMLElement* GetParent() const;
	[[nodiscard]] XMLDocument* GetDocument() const;
	void SetParent(XMLElement* parent);
	void SetDocument(XMLDocument* document);

 private:
	std::string _tag;
	std::map<std::string, std::string> _attributes;
	std::vector<XMLContent*> _children;
	XMLElement* _parent = nullptr;
	XMLDocument* _document = nullptr;
};

class XMLDocument
{
 public:
	XMLDocument();
	~XMLDocument();
	[[nodiscard]] XMLElement* GetRoot() const;

 private:
	XMLElement* _root;

};

#endif //MLIB_XML_H