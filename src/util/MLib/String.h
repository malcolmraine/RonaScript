/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef MLIB_STRING_H
#define MLIB_STRING_H

#include <string>
#include <vector>

class String {
public:
    static std::string Join(const std::string& s, const std::vector<std::string>& v);
    static std::vector<std::string> Split(const std::string& subject,
                                          const std::string& search);
    static std::string Before(const std::string& subject, const std::string& search);
    static std::string After(const std::string& subject, const std::string& search);
    static std::string Trim(const std::string& subject);
    static std::string Replace(const std::string& subject, const std::string& search,
                               const std::string& replace);
    static bool Contains(const std::string& subject, const std::string& search);
    static bool IsWhiteSpace(const std::string& subject);
    static bool IsWhiteSpace(char c);
    static std::string Condense(const std::string& subject);
    static std::string Upper(const std::string& s);
    static std::string Lower(const std::string& s);
    static std::string SnakeCase(const std::string& s);
    static std::string CamelCase(const std::string& s);
    static std::string TitleCase(const std::string& s);
    static size_t Count(const std::string& s, const std::string& key);
    static std::string Wrap(const std::string& s, const std::string& lh,
                            const std::string& rh = "");
    static std::string Unwrap(const std::string& s, const std::string& lh,
                              const std::string& rh = "");
    static bool StartsWith(const std::string& s, const std::string& key);
    static bool EndsWith(const std::string& s, const std::string& key);
    static std::string Pad(const std::string& s, size_t len, char pad_char = ' ');

private:
    static bool _IsWhiteSpace(char c);
    String() = default;
    ~String() = default;
};

#endif  //MLIB_STRING_H