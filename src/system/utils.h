#pragma once

#define ST_COUTLN(message)\
    std::cout << message << std::endl

#define ST_CERRLN(message)\
    std::cerr << message << std::endl

#define ST_TO_STRING(value)\
    #value

#define ST_STRING_CONCAT(string_variable, string_content)\
    std::stringstream ss;\
    ss << string_content;\
    string_variable = ss.str()

bool streq(const char* a, const char* b);

namespace Storytime {
    bool string_ends_with(const std::string& str, const std::string& suffix);

    std::vector<std::string> string_split(const std::string& str, char delimiter);

    void string_replace_first(std::string& str, const std::string& target, const std::string& replacement);

    void string_replace_all(std::string& str, const std::string& target, const std::string& replacement);

    void string_to_upper(std::string& str);

    void string_to_lower(std::string& str);
}

