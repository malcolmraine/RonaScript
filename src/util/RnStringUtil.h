/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNSTRINGUTIL_H
#define RONASCRIPT_RNSTRINGUTIL_H

#include <string>
#include <vector>

namespace RnStringUtil
{
/*************************************************************************/
	static std::string Pad(const std::string& s, unsigned long len,
		const std::string& pad_char = " ")
	{
		auto out = s;
		unsigned long pad_len = len - s.length();

		pad_len = pad_len >= 0 ? pad_len : 0;

		for (unsigned long i = 0; i < pad_len; i++)
		{
			out += pad_char;
		}

		return out;
	}

/*************************************************************************/
	template<typename T = std::string>
	static std::string Join(std::vector<T> v, const std::string& join_char = "")
	{
		std::string s;

		for (auto& item : v)
		{
			s += std::to_string(item) + join_char;
		}

		return s;
	}

}
#endif //RONASCRIPT_RNSTRINGUTIL_H
