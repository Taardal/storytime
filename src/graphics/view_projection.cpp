#include "view_projection.h"

namespace Storytime {
    ViewProjection::operator glm::mat4() const {
        return projection * view;
    }
}
