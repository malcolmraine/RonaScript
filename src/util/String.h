/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#pragma once

#include <string>
#include <vector>

class String {
public:
    enum TITLECASE_MODE {
        SIMPLE_TITLECASE,
        ADVANCED_TITLECASE,
    };

    static std::string Join(const std::string& s,
                            const std::vector<std::string>& v);  // TODO: Unit test
    static std::vector<std::string> Split(
        const std::string& subject,
        const std::string& search);  // TODO: Unit test
    static std::string Before(const std::string& subject,
                              const std::string& search);  // TODO: Unit test
    static std::string After(const std::string& subject,
                             const std::string& search);  // TODO: Unit test
    static std::string Trim(const std::string& subject);  // TODO: Unit test
    static std::string Replace(const std::string& subject, const std::string& search,
                               const std::string& replace);  // TODO: Unit test
    static bool Contains(const std::string& subject,
                         const std::string& search);          // TODO: Unit test
    static bool IsWhiteSpace(const std::string& subject);     // TODO: Unit test
    static bool IsWhiteSpace(char c);                         // TODO: Unit test
    static std::string Condense(const std::string& subject);  // TODO: Unit test
    static std::string Upper(const std::string& s);           // TODO: Unit test
    static std::string Lower(const std::string& s);           // TODO: Unit test
    static std::string SnakeCase(const std::string& s);       // TODO: Unit test
    static std::string CamelCase(const std::string& s);       // TODO: Unit test
    static std::string TitleCase(const std::string& s, TITLECASE_MODE mode = SIMPLE_TITLECASE);       // TODO: Unit test
    static size_t Count(const std::string& s,
                        const std::string& key);  // TODO: Unit test
    static std::string Wrap(const std::string& s, const std::string& lh,
                            const std::string& rh = "");  // TODO: Unit test
    static std::string Unwrap(const std::string& s, const std::string& lh,
                              const std::string& rh = "");  // TODO: Unit test
    static bool StartsWith(const std::string& s,
                           const std::string& key);  // TODO: Unit test
    static bool EndsWith(const std::string& s,
                         const std::string& key);  // TODO: Unit test
    static std::string Pad(const std::string& s, size_t len,
                           char pad_char = ' ');  // TODO: Unit test

    template <typename T = std::string>
    static std::string Join(std::vector<T> v, const std::string& join_char = "") {
        std::string s;

        for (auto& item : v) {
            s += std::to_string(item) + join_char;
        }

        return s;
    }

private:
    static std::string SimpleTitleCase(const std::string& s);
    static std::string AdvancedTitleCase(const std::string& s, bool useExemptions = true);
    String() = default;
    ~String() = default;
};
