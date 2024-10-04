#pragma once

#define ST_COUT(message)\
    std::cout << message << std::endl

#define ST_CERR(message)\
    std::cerr << message << std::endl

#define ST_STRING(value)\
    #value

#define ST_STRING_CONCAT(string_variable, string_content)\
    std::stringstream ss;\
    ss << string_content;\
    string_variable = ss.str()

namespace Storytime {
    template<typename T>
    T as(void* pointer) {
        return reinterpret_cast<T>(pointer);
    }

    bool string_ends_with(const std::string& str, const std::string& suffix);

    std::vector<std::string> string_split(const std::string& str, char delimiter);

    void string_replace(std::string& haystack, const char* needle);

    void string_replace(std::string& haystack, std::string_view needle);

    void string_replace(std::string& haystack, const std::string& needle);
}
