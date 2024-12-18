#pragma once

#include <memory>

namespace Storytime {
    template<typename T>
    using Shared = std::shared_ptr<T>;

    template<typename T, typename... Args>
    Shared<T> shared(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Unique = std::unique_ptr<T>;

    template<typename T, typename... Args>
    Unique<T> unique(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    Unique<T> unique(size_t size) {
        return std::make_unique<T>(size);
    }

    template<typename T>
    using Weak = std::weak_ptr<T>;
}
