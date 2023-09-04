/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
* MIT License
*
* Copyright (c) 2021 Malcolm Hall
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************/

#include "String.h"
#include <algorithm>
#include <unordered_set>

/*****************************************************************************/
std::string String::Join(const std::string& s, const std::vector<std::string>& v) {
    if (v.size() == 1) {
        return v[0];
    } else {
        std::string result;
        for (size_t i = 0; i < v.size() - 1; ++i) {
            result += v[i] + s;
        }

        if (v.size() > 1) {
            result += v.back();
        }
        return result;
    }
}

/*****************************************************************************/
std::vector<std::string> String::Split(const std::string& subject,
                                       const std::string& search) {
    unsigned long index = subject.find(search);
    unsigned long base_index = 0;

    if (index == std::string::npos) {
        return {subject};
    } else {
        std::vector<std::string> result;
        while (index != std::string::npos) {
            std::string sub = subject.substr(base_index, index - base_index);

            if (!sub.empty()) {
                result.emplace_back(sub);
            }

            base_index = index + search.length();
            index = subject.find(search, base_index);
        }

        std::string sub = subject.substr(base_index, subject.length());
        if (!sub.empty()) {
            result.emplace_back(sub);
        }

        return result;
    }
}

/*****************************************************************************/
std::string String::Before(const std::string& subject, const std::string& search) {
    unsigned long index = subject.find(search, 0);
    if (index == std::string::npos) {
        return subject;
    } else {
        return subject.substr(0, index);
    }
}

/*****************************************************************************/
std::string String::After(const std::string& subject, const std::string& search) {
    unsigned long index = subject.find(search, 0);
    if (index == std::string::npos) {
        return subject;
    } else {
        return subject.substr(index + search.length(), subject.length());
    }
}

/*****************************************************************************/
std::string String::Trim(const std::string& subject) {
    if (subject.empty()) {
        return "";
    } else {
        std::string result;
        unsigned long start_idx = 0;
        unsigned long end_idx = subject.length() - 1;
        bool start_found = false;

        for (size_t i = 0; i < subject.length(); ++i) {
            if (IsWhiteSpace(subject[i])) {
                if (!start_found) {
                    start_idx++;
                }
            } else {
                if (!start_found) {
                    start_found = true;
                }

                if (i >= 0) {
                    end_idx = i;
                }
            }
        }

        return subject.substr(start_idx, (end_idx - start_idx) + 1);
    }
}

/*****************************************************************************/
std::string String::Replace(const std::string& subject, const std::string& search,
                            const std::string& replace) {
    unsigned long index = subject.find(search);
    unsigned long base_index = 0;

    if (index == std::string::npos) {
        return subject;
    } else {
        std::string result;
        while (index != std::string::npos) {
            std::string sub = subject.substr(base_index, index - base_index);
            result += sub + replace;
            base_index = index + search.length();
            index = subject.find(search, base_index);
        }

        std::string sub = subject.substr(base_index, subject.length());
        result += sub;

        return result;
    }
}

/*****************************************************************************/
bool String::Contains(const std::string& subject, const std::string& search) {
    return subject.find(search, 0) != std::string::npos;
}

/*****************************************************************************/
bool String::IsWhiteSpace(const std::string& subject) {
    return IsWhiteSpace(subject.c_str()[0]);
}

/*****************************************************************************/
bool String::IsWhiteSpace(char c) {
    switch (c) {
        case ' ':
        case '\n':
        case '\t':
        case '\r':
        case '\0':
            return true;
        default:
            return false;
    }
}

/*****************************************************************************/
std::string String::Condense(const std::string& subject) {
    std::string result;
    bool ws = false;
    char last_ws;

    for (char c : subject) {
        if (IsWhiteSpace(c)) {
            if (!ws || c != last_ws) {
                result += c;
                last_ws = c;
                ws = true;
            }
        } else {
            ws = false;
            last_ws = '\0';
            result += c;
        }
    }

    return result;
}

/*****************************************************************************/
std::string String::Upper(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

/*****************************************************************************/
std::string String::Lower(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

/*****************************************************************************/
std::string String::SnakeCase(const std::string& s) {
    // TODO: Implement SnakeCase
    return s;
}

/*****************************************************************************/
std::string String::CamelCase(const std::string& s) {
    // TODO: Implement CamelCase
    return s;
}

/*****************************************************************************/
std::string String::TitleCase(const std::string& s, String::TITLECASE_MODE mode) {
    if (mode == SIMPLE_TITLECASE) {
        return String::SimpleTitleCase(s);
    } else {
        return String::AdvancedTitleCase(s);
    }
    return s;
}

/*****************************************************************************/
size_t String::Count(const std::string& s, const std::string& key) {
    size_t index = s.find(key, 0);
    size_t count = 0;

    while (index != std::string::npos) {
        count++;
        index = s.find(key, index + key.length());
    }
    return count;
}

/*****************************************************************************/
std::string String::Wrap(const std::string& s, const std::string& lh,
                         const std::string& rh) {
    if (rh.empty()) {
        return lh + s + lh;
    } else {
        return lh + s + rh;
    }
}

/*****************************************************************************/
std::string String::Unwrap(const std::string& s, const std::string& lh,
                           const std::string& rh) {
    std::string result;
    if (StartsWith(s, lh)) {
        result = s.substr(0, s.length() - lh.length());
    }

    if (rh.empty()) {
        if (EndsWith(s, lh)) {
            result = result.substr(0, result.length() - lh.length());
        }
    } else if (EndsWith(s, rh)) {
        result = result.substr(0, result.length() - rh.length());
    }

    return result;
}

/*****************************************************************************/
bool String::StartsWith(const std::string& s, const std::string& key) {
    return s.substr(0, s.length() - key.length()) == key;
}

/*****************************************************************************/
bool String::EndsWith(const std::string& s, const std::string& key) {
    return s.substr(s.length() - key.length(), s.length()) == key;
}

/*****************************************************************************/
std::string String::Pad(const std::string& s, size_t len, char pad_char) {
    auto out = s;
    size_t pad_len = len - s.length();
    if (pad_len > s.length() + len) {
        return s;
    }

    for (size_t i = 0; i < pad_len; i++) {
        out += std::string(1, pad_char);
    }

    return out;
}

///////////////////////////////////////////////////////////////////////////////
std::string String::SimpleTitleCase(const std::string& s)
{
    if (s.empty()) { return ""; }

    std::string result = s;
    bool needsUpper = true;
    for (size_t i = 0; i < s.length(); i++)
    {
        char c = s[i];
        if (!std::isalpha(c) && c != '\'') { needsUpper = true; }
        else if (needsUpper)
        {
            result[i] = static_cast<char>(std::toupper(c));
            needsUpper = false;
        }
        else { result[i] = static_cast<char>(std::tolower(c)); }
    }
    return result;
}

///////////////////////////////////////////////////////////////////////////////
std::string String::AdvancedTitleCase(const std::string& s, bool useExemptions = true)
{
    // More advanced title case conversion that takes short words and
    // hyphens into account

    if (s.empty()) { return ""; }

    // Prepositions and other short words that don't get capitalized
    static const std::unordered_set<std::string> exemptions = {
        "of", "at", "to",  "in",  "for", "on", "by",  "but", "per", "via", "off",
        "a",  "an", "the", "and", "as",  "if", "nor", "or",  "so",  "yet", "of"};

    std::string result = s;
    size_t currentWordStart = 0;
    bool needsUpper = true;
    for (size_t i = 0; i < s.length(); i++)
    {
        char c = s[i];
        if ((i == 0 || !std::isalpha(s[i - 1]))&& c != '\'')
        {
            result[i] = static_cast<char>(std::toupper(s[i]));
            needsUpper = false;
            continue;
        }

        if (!std::isalpha(c) && c != '\'' && i < s.length() - 1)
        {
            needsUpper = true;
            if (currentWordStart == i) { continue; }
            else if (!useExemptions
                     || exemptions.find(s.substr(currentWordStart, i - currentWordStart))
                            == exemptions.end())
            {
                result[currentWordStart] =
                    static_cast<char>(std::toupper(s[currentWordStart]));
            }
            currentWordStart = i + 1;
        }
        else
        {
            result[i] = static_cast<char>(std::tolower(c));
        }
        //		else { currentWord += static_cast<char>(std::tolower(c)); }
    }

    // Capitalize the last word
    //	if (!currentWord.empty())
    //	{
    //		currentWord[0] = static_cast<char>(std::toupper(currentWord[0]));
    //		result += currentWord;
    //	}
    if (currentWordStart < s.length() - 1)
    {
        result[currentWordStart] = static_cast<char>(std::toupper(s[currentWordStart]));
    }

    return result;
}