/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "String.h"
#include <algorithm>

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
            if (_IsWhiteSpace(subject[i])) {
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
    return _IsWhiteSpace(subject.c_str()[0]);
}

/*****************************************************************************/
bool String::IsWhiteSpace(char c) {
    return _IsWhiteSpace(c);
}

/*****************************************************************************/
bool String::_IsWhiteSpace(char c) {
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
        if (_IsWhiteSpace(c)) {
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
std::string String::TitleCase(const std::string& s) {
    // TODO: Implement TitleCase
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

    pad_len = pad_len >= 0 ? pad_len : 0;

    for (size_t i = 0; i < pad_len; i++) {
        out += std::string(1, pad_char);
    }

    return out;
}