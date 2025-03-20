#pragma once

#include <memory>

namespace Storytime {
    template<typename T>
    using Shared = std::shared_ptr<T>;

    template<typename T>
    using Unique = std::unique_ptr<T>;

    template<typename T>
    using Weak = std::weak_ptr<T>;
}
