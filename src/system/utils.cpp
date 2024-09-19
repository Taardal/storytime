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

    void string_replace(std::string& haystack, const char* needle) {
        size_t start = haystack.find(needle);
        if (start == std::string::npos) {
            return;
        }
        size_t end = start + strlen(needle);
        haystack.replace(start, end, needle);
    }

    void string_replace(std::string& haystack, std::string_view needle) {
        size_t start = haystack.find(needle);
        if (start == std::string::npos) {
            return;
        }
        size_t end = start + needle.length();
        haystack.replace(start, end, needle);
    }

    void string_replace(std::string& haystack, const std::string& needle) {
        size_t start = haystack.find(needle);
        if (start == std::string::npos) {
            return;
        }
        size_t end = start + needle.length();
        haystack.replace(start, end, needle);
    }
}
