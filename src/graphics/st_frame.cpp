#include "st_frame.h"

namespace Storytime {
    bool Batch::is_full() const {
        return quad_index >= quads.size() || texture_index >= textures.size();
    }

    i32 Batch::add_quad(const QuadInstanceData& quad) {
        i32 index = quad_index;
        ST_ASSERT_IN_BOUNDS(index, quads);
        quads.at(index) = quad;
        quad_index++;
        return index;
    }

    i32 Batch::add_texture(const Shared<VulkanImage>& texture) {
        i32 index = texture_index;
        ST_ASSERT_IN_BOUNDS(index, textures);
        textures.at(index) = texture;
        texture_index++;
        return index;
    }

    i32 Batch::find_texture(const Shared<VulkanImage>& texture) const {
        for (i32 i = 0; i < textures.size(); i++) {
            if (texture == textures.at(i)) {
                return i;
            }
        }
        return -1;
    }

    bool Batch::has_texture(const Shared<VulkanImage>& texture) const {
        return find_texture(texture) > -1;
    }
}

namespace Storytime {
    bool Frame::has_used_all_batches() const {
        return batch_index >= batches.size();
    }
}
