/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef MLIBTEST_JSON_H
#define MLIBTEST_JSON_H

#include <string>
#include <map>
#include <vector>

/*****************************************************************************/
/*****************************************************************************/
class JSONNode
{
 public:
	enum TYPE
	{
		FLOAT, INTEGER, STRING, OBJECT, LIST, NULL_VALUE, DEFAULT,
	};

	[[nodiscard]] virtual std::string ToString() const = 0;
	virtual ~JSONNode() = default;
	virtual void SetParent(JSONNode* parent) = 0;
	[[nodiscard]] virtual JSONNode* GetParent() const = 0;
	[[nodiscard]] virtual size_t GetLevel() const = 0;
	virtual void SetLevel(size_t level) = 0;
	[[nodiscard]] virtual TYPE GetType() const = 0;
};

/*****************************************************************************/
/*****************************************************************************/
template<typename T>
class JSONValue : public JSONNode
{
 public:
	/*************************************************************************/
	JSONValue() = default;

	/*************************************************************************/
	explicit JSONValue(T data) : _data(data)
	{

	}

	/*************************************************************************/
	~JSONValue() override = default;

	/*************************************************************************/
	[[nodiscard]] T GetData() const
	{
		return _data;
	}

	/*************************************************************************/
	void SetData(T data)
	{
		_data = data;
	}

	/*************************************************************************/
	[[nodiscard]] std::string ToString() const override
	{
		throw std::runtime_error("ToString not implemented");
	}

	/*************************************************************************/
	void SetParent(JSONNode* parent) override
	{
		_parent = parent;
		if (_parent)
		{
			_level = _parent->GetLevel() + 1;
		}
	}

	/*************************************************************************/
	[[nodiscard]] JSONNode* GetParent() const override
	{
		return _parent;
	}

	/*************************************************************************/
	[[nodiscard]] size_t GetLevel() const override
	{
		return _level;
	}

	/*************************************************************************/
	void SetLevel(size_t level) override
	{
		_level = level;
	}

	/*************************************************************************/
	[[nodiscard]] TYPE GetType() const override
	{
		return DEFAULT;
	}

 protected:
	T _data;
	size_t _level = 1;
	JSONNode* _parent = nullptr;
};


/*****************************************************************************/
/*****************************************************************************/
class JSONObject : public JSONValue<std::map<std::string, JSONNode*>>
{
 public:
	JSONObject();
	~JSONObject() override;
	[[nodiscard]] std::string ToString() const override;
	JSONObject* SetKeyValue(const std::string& key, JSONNode* value);
	JSONObject* SetKeyValue(const std::string& key, const std::string& value);
	JSONObject* SetKeyValue(const std::string& key, double value);
	JSONObject* SetKeyValue(const std::string& key, long value);
	JSONObject* SetKeyValue(const std::string& key, int value);
	void RemoveKeyValue(const std::string& key, JSONNode* value);

	/*************************************************************************/
	[[nodiscard]] TYPE GetType() const override
	{
		return OBJECT;
	}
};

/*****************************************************************************/
/*****************************************************************************/
class JSONList : public JSONValue<std::vector<JSONNode*>>
{
 public:
	JSONList();
	JSONList(const std::vector<JSONNode*>& data);
	~JSONList() override;
	[[nodiscard]] std::string ToString() const override;

	/*************************************************************************/
	[[nodiscard]] TYPE GetType() const override
	{
		return LIST;
	}
};

/*****************************************************************************/
/*****************************************************************************/
class JSONFloat : public JSONValue<double>
{
 public:
	JSONFloat();
	JSONFloat(double data);
	[[nodiscard]] std::string ToString() const override;

	/*************************************************************************/
	[[nodiscard]] TYPE GetType() const override
	{
		return FLOAT;
	}
};

/*****************************************************************************/
/*****************************************************************************/
class JSONInt : public JSONValue<long>
{
 public:
	JSONInt();
	JSONInt(long data);
	[[nodiscard]] std::string ToString() const override;

	/*************************************************************************/
	[[nodiscard]] TYPE GetType() const override
	{
		return INTEGER;
	}
};

/*****************************************************************************/
/*****************************************************************************/
class JSONString : public JSONValue<std::string>
{
 public:
	JSONString();
	JSONString(std::string data);
	[[nodiscard]] std::string ToString() const override;

	/*************************************************************************/
	[[nodiscard]] TYPE GetType() const override
	{
		return STRING;
	}
};

/*****************************************************************************/
/*****************************************************************************/
class JSONParser
{
 public:
	JSONParser();
	~JSONParser();
	JSONNode* Parse(const std::string& filename);

 private:
	void Expect(char c);
	void ConsumeWhiteSpace();
	JSONList* ParseList();
	JSONObject* ParseObject();
	JSONString* ParseString();
	JSONNode* ParseNumeric();
	JSONNode* ParseValue();

 private:
	std::string _filename;
	std::string _contents;
	size_t _idx = 0;

};

#endif //MLIBTEST_JSON_H