#pragma once

#include <variant>

namespace Storytime {
    template<typename... Types>
    class Variant {
    protected:
        std::variant<Types...> variant;

    private:
        template<typename T>
        using MustBeVariantType = std::enable_if_t<std::is_same_v<std::decay_t<T>, std::variant<Types...>>>;

    public:
        template<typename T, MustBeVariantType<T>>
        Variant(T&& value) : variant(std::forward<T>(value)) {}

        Variant(const std::variant<Types...>& v) : variant(v) {}

        Variant(std::variant<Types...>&& v) : variant(std::move(v)) {}

        size_t type() const {
            return variant.index();
        }

        template<typename Visitor>
        auto visit(Visitor&& visitor) {
            return std::visit(std::forward<Visitor>(visitor), variant);
        }

        template<typename T>
        bool is() const {
            return std::holds_alternative<T>(variant);
        }

        template<typename T>
        T& as() {
            ST_ASSERT(std::holds_alternative<T>(variant), "Variant must be of type [" << type_name<T>() << "]");
            return std::get<T>(variant);
        }

        template<typename T>
        const T& as() const {
            ST_ASSERT(std::holds_alternative<T>(variant), "Variant must be of type [" << type_name<T>() << "]");
            return std::get<T>(variant);
        }

        template<typename T>
        T* try_as() {
            if (!std::holds_alternative<T>(variant)) {
                return nullptr;
            }
            return &std::get<T>(variant);
        }

        template<typename T>
        const T* try_as() const {
            if (!std::holds_alternative<T>(variant)) {
                return nullptr;
            }
            return &std::get<T>(variant);
        }

        template<typename T>
        operator T&() {
            return as<T>();
        }

        template<typename T>
        operator T*() {
            return try_as<T>();
        }
    };
}