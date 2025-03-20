#pragma once

#include "st_tiled_object.h"
#include "st_tiled_property.h"

namespace Storytime {
    struct TiledAnimationFrame {
        int duration;
        int tileid;
    };

    struct TiledTile {
        std::vector<TiledAnimationFrame> animation;
        int id;
        TiledObjectGroup objectgroup;
        std::vector<TiledProperty> properties;

        static TiledTile create(const std::string& json);

        const TiledProperty& get_property(const std::string& name) const;

        const TiledProperty* try_get_property(const std::string& name) const;

        template<typename T>
        T get_property_value(const std::string& name) {
            return get_tiled_property_value<T>(properties, name);
        }

        template<typename T>
        std::optional<T> try_get_property_value(const std::string& name) {
            return try_get_tiled_property_value<T>(properties, name);
        }
    };

    ///
    /// Tiled Editor: Tile flipping
    ///
    /// Each tile has a global ID (GID) which is a 32-bit number. GIDs include both the
    /// tile’s actual ID and additional flag bits for transformations like flipping or rotating
    ///
    /// The highest four bits of the 32-bit GID are transformation flags:
    ///
    /// - Bit 32: Horizontally flipped
    /// - Bit 31: Vertically flipped
    ///
    /// In orthogonal and isometric maps:
    /// - Bit 30: Flipped diagonally
    /// - Bit 29: Can be ignored
    ///
    /// In hexagonal maps:
    /// - Bit 30: Rotated 60 degrees clockwise
    /// - Bit 29: Rotated 120 degrees clockwise
    ///
    /// Ref:
    /// - https://doc.mapeditor.org/en/stable/reference/global-tile-ids/#gid-tile-flipping
    ///
    enum TiledTileTransformationFlags {
        TiledTileTransformationFlags_None = 0,

        TiledTileTransformationFlags_FlippedHorizontally = 0x80000000,  // Bit 32 -> 1000 0000 0000 0000 0000 0000 0000 0000
        TiledTileTransformationFlags_FlippedVertically = 0x40000000,    // Bit 31 -> 0100 0000 0000 0000 0000 0000 0000 0000
        TiledTileTransformationFlags_FlippedDiagonally = 0x20000000,    // Bit 30 -> 0010 0000 0000 0000 0000 0000 0000 0000
        TiledTileTransformationFlags_Rotated120_Hexagonal = 0x10000000, // Bit 29 -> 0001 0000 0000 0000 0000 0000 0000 0000

        TiledTileTransformationFlags_Rotated60_Hexagonal = TiledTileTransformationFlags_FlippedDiagonally,
        TiledTileTransformationFlags_FlippedDiagonally_Isometric = TiledTileTransformationFlags_FlippedDiagonally,
        TiledTileTransformationFlags_FlippedDiagonally_Orthographic = TiledTileTransformationFlags_FlippedDiagonally,
    };
}
