bool streq(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

namespace Storytime {
    bool string_ends_with(const std::string& str, const std::string& suffix) {
        // Check if the string is at least as long as the suffix
        if (str.size() < suffix.size()) {
            return false;
        }
        // Compare the end of the string with the suffix
        return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
    }

    std::vector<std::string> string_split(const std::string& str, char delimiter) {
        // If the delimiter is not found, return a vector containing the original string
        if (str.find(delimiter) == std::string::npos) {
            return {str};
        }
        // Split the string by the delimiter and return the tokens/segments in a vector
        std::string token;
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    void string_replace_first(std::string& str, const std::string& target, const std::string& replacement) {
        size_t start = str.find(target);
        if (start == std::string::npos) {
            return;
        }
        str.replace(start, target.length(), replacement);
    }

    void string_replace_all(std::string& str, const std::string& target, const std::string& replacement) {
        size_t start = 0;
        while ((start = str.find(target)) != std::string::npos) {
            str.replace(start, target.length(), replacement);
        }
    }

    void string_to_upper(std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    }

    void string_to_lower(std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    }

    f64 smooth_average(f64 value, f64 current_average, f64 smoothing_factor) {
        return smoothing_factor * value + (1.0 - smoothing_factor) * current_average;
    }
}
