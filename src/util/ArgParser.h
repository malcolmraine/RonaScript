/*****************************************************************************
* File: ArgParser.h
* Description:
* Author: Malcolm Hall
* Date: 1/25/23
* Version: 1
*
*****************************************************************************/

#ifndef RONASCRIPT_SRC_UTIL_ARGPARSER_H_
#define RONASCRIPT_SRC_UTIL_ARGPARSER_H_

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

class Argument
{
 public:
	Argument(const std::string& key, const std::string& description, bool has_value);
	void SetAlternateKeys(const std::vector<std::string>& keys);
	std::vector<std::string> GetAlternateKeys();
	[[nodiscard]] std::string GetKey() const;
	[[nodiscard]] std::string GetValue() const;
	void SetValue(const std::string& value);
	[[nodiscard]] std::string GetDescription() const;
	[[nodiscard]] bool IsSet() const;
	[[nodiscard]] bool HasValue() const;

 private:
	std::string _key;
	std::string _value;
	bool _is_set = false;
	std::string _description;
	bool _has_value = false;
	std::vector<std::string> _alternates_keys;
};

class ArgParser
{
 public:
	ArgParser();
	void SetMainDescription(const std::string& description);
	void AddArgument(const std::string& arg, const std::string& description,
		bool has_value = false, const std::string& defaultValue = "",
		const std::vector<std::string>& alternates = {});
	[[nodiscard]] bool IsSet(const std::string& arg);
	[[nodiscard]] std::string GetValue(const std::string& arg);
	void ShowHelp();
	void Parse(int argc, char* argv[]);
	[[nodiscard]] std::string GetInputFile() const;

 private:
	std::map<std::string, std::shared_ptr<Argument>> _arguments;
	std::string _main_description;
	std::string _input_file;
	std::unordered_set<std::string> _help_exclusions;
};

#endif //RONASCRIPT_SRC_UTIL_ARGPARSER_H_