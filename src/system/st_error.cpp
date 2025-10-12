#include "st_error.h"
#include <iomanip>

namespace Storytime {
    Error::Error(const std::string& message, const std::string& tag, const std::shared_ptr<Error>& previous_error)
        : message(message), tag(tag), previous_error(previous_error) {
    }

    const char* Error::what() const noexcept {
        return message.c_str();
    }

    std::vector<StacktraceEntry> Error::get_stacktrace() const {
        std::vector<StacktraceEntry> stack;
        auto current_error = std::make_shared<Error>(*this);
        while (current_error != nullptr) {
            StacktraceEntry entry{};
            entry.message = current_error->message;
            entry.tag = current_error->tag;
            stack.emplace(stack.begin(), entry);
            current_error = current_error->previous_error;
        }
        return stack;
    }

    void Error::print_stacktrace() const {
        print_stacktrace(*this);
    }

    void Error::print_stacktrace(const Error& error) {
        std::vector<StacktraceEntry> stack = error.get_stacktrace();
        std::string& error_message = stack[0].message;

        fprintf(stderr, "--------------------------------------------------------------------------------------------------------------\n");
        fprintf(stderr, "[Storytime::Error] %s\n", error_message.c_str());
        fprintf(stderr, "--------------------------------------------------------------------------------------------------------------\n");

        uint32_t longest_tag_length = 0;
        for (uint32_t i = 0; i < stack.size(); i++) {
            StacktraceEntry& stack_entry = stack[i];
            uint32_t tag_length = stack_entry.tag.size();
            if (tag_length > longest_tag_length) {
                longest_tag_length = tag_length;
            }
        }

        static constexpr uint8_t segment_spacing = 4;
        for (uint32_t i = 0; i < stack.size(); i++) {
            StacktraceEntry& stack_entry = stack[i];
            uint32_t tag_length = stack_entry.tag.size();
            uint32_t tag_right_spacing = segment_spacing + tag_length + (longest_tag_length - tag_length);
            fprintf(stderr, "[%d]", i);
            fprintf(stderr, " ");
            fprintf(stderr, "%*s", -tag_right_spacing, stack_entry.tag.c_str());
            fprintf(stderr, "%s", stack_entry.message.c_str());
            fprintf(stderr, "\n");
        }

        fprintf(stderr, "\n");
    }

    std::ostream& operator<<(std::ostream& os, const Error& error) {
        std::vector<StacktraceEntry> stack = error.get_stacktrace();
        std::string& error_message = stack[0].message;

        os << "--------------------------------------------------------------------------------------------------------------\n";
        os << "[Storytime::Error] " << error_message.c_str() << "\n";
        os << "--------------------------------------------------------------------------------------------------------------\n";

        uint32_t longest_tag_length = 0;
        for (const auto& stack_entry : stack) {
            uint32_t tag_length = stack_entry.tag.size();
            longest_tag_length = std::max(longest_tag_length, tag_length);
        }

        static constexpr uint8_t segment_spacing = 4;
        for (size_t i = 0; i < stack.size(); i++) {
            const auto& stack_entry = stack[i];
            os << "[" << i << "]";
            os << " ";
            os << std::setw(segment_spacing + longest_tag_length) << std::left << stack_entry.tag;
            os << stack_entry.message;
            os << "\n";
        }

        os << "\n";
        return os;
    }
}
