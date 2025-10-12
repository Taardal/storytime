#pragma once

#define ST_STRINGIFY(value) #value

#define ST_STRING(message)\
    ([&]() -> std::string {\
        std::stringstream ss;\
        ss << message;\
        return ss.str();\
    }())

bool streq(const char* a, const char* b);

namespace Storytime {
    bool string_ends_with(const std::string& str, const std::string& suffix);

    std::vector<std::string> string_split(const std::string& str, char delimiter);

    void string_replace_first(std::string& str, const std::string& target, const std::string& replacement);

    void string_replace_all(std::string& str, const std::string& target, const std::string& replacement);

    void string_to_upper(std::string& str);

    void string_to_lower(std::string& str);
}