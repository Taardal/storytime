#pragma once

namespace Storytime {

    /// An object ID is a unique identifier assigned to each object within a single map.
    typedef u32 TiledObjectID;

    /// A layer ID is a unique identifier assigned to each layer within a single map.
    /// Every layer type (tile layer, object layer, image layer, group layer) gets a layer ID
    typedef u32 TiledLayerID;

    /// A local ID is a unique identifier assigned to each tile within a single tileset.
    /// A local ID is the same as the index of a tile in the tileset, starting from 0
    /// and incremented by 1 per tile.
    typedef u32 TiledLocalTileID;

    /// A Global ID (GID) is a unique identifier assigned to each tile across all tilesets used in a map.
    ///
    /// When a tileset is added to a map, Tiled Editor assigns it a "first global ID" (firstgid).
    /// This is the GID of the first tile in that tileset.
    ///
    /// To calculate the GID of any tile in a tileset: GID = firstgid + local ID
    ///
    typedef u32 TiledGlobalTileID;
}
