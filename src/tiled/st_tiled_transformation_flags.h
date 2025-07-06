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

#pragma once

namespace Storytime {
    enum TiledTransformationFlags {
        TiledTransformationFlags_None = 0,

        TiledTransformationFlags_FlippedHorizontally = 0x80000000,  // Bit 32 -> 1000 0000 0000 0000 0000 0000 0000 0000
        TiledTransformationFlags_FlippedVertically = 0x40000000,    // Bit 31 -> 0100 0000 0000 0000 0000 0000 0000 0000
        TiledTransformationFlags_FlippedDiagonally = 0x20000000,    // Bit 30 -> 0010 0000 0000 0000 0000 0000 0000 0000
        TiledTransformationFlags_Rotated120_Hexagonal = 0x10000000, // Bit 29 -> 0001 0000 0000 0000 0000 0000 0000 0000

        TiledTransformationFlags_Rotated60_Hexagonal = TiledTransformationFlags_FlippedDiagonally,
        TiledTransformationFlags_FlippedDiagonally_Isometric = TiledTransformationFlags_FlippedDiagonally,
        TiledTransformationFlags_FlippedDiagonally_Orthographic = TiledTransformationFlags_FlippedDiagonally,
    };
}