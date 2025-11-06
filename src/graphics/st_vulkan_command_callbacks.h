#pragma once

namespace Storytime {
    using RecordCommandsFn = std::function<void(VkCommandBuffer)>;
    using OnRecordCommandsFn = std::function<void(const RecordCommandsFn&)>;
    using RecordAndSubmitCommandsFn = std::function<void(const OnRecordCommandsFn&)>;
}