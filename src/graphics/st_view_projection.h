#pragma once

namespace Storytime {
    struct ViewProjection {
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        operator glm::mat4() const;
    };
}
