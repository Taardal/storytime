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

    template<typename T>
    struct Size {
        T width;
        T height;
    };

    //
    // Calculate a running average, that is "smoothed out" to provide a more stable value,
    // using the _Exponential Moving Average_ formula. Useful for FPS counters.
    //
    // The smoothing factor (alpha) determines how quickly the average reacts to changes (0 < alpha ≤ 1):
    // - A higher value makes it more reactive.
    // - A lower value makes it more stable.
    //
    f64 smooth_average(f64 value, f64 current_average, f64 smoothing_factor = 0.1);
}

