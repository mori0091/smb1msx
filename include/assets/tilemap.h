// -*- coding: utf-8-unix -*-

#pragma once

#ifndef TILEMAP_H_
#define TILEMAP_H_

#include <stdint.h>

/**
 * Enumerations of matrix transformation.
 *
 * \sa tilemap_copy_to_vmem()
 * \sa tilemap_copy_to_vmem_hflip()
 * \sa tilemap_copy_to_vmem_vflip()
 * \sa tilemap_copy_to_vmem_4()
 * \sa tilemap_copy_to_vmem_4_hflip()
 * \sa tilemap_copy_to_vmem_4_vflip()
 */
enum {
  IDENTITY        = 0,
  FLIP_HORIZONTAL = (1 << 7),
  FLIP_VERTICAL   = (1 << 6),
  TRANSPOSE       = (1 << 5),
  ROTATE_90       = (FLIP_HORIZONTAL | TRANSPOSE),
  ROTATE_180      = (FLIP_VERTICAL | FLIP_HORIZONTAL),
  ROTATE_270      = (FLIP_VERTICAL | TRANSPOSE),
  ROTATE_90_CCW   = ROTATE_270,
  ROTATE_180_CCW  = ROTATE_180,
  ROTATE_270_CCW  = ROTATE_90,
};

/**
 * The elements of the tilemap consist of the pattern index to the tileset and
 * the identifier of the matrix transformation.
 */
typedef uint8_t TilemapElement[2];

/**
 * 8x8 pixel 1bpp image.
 */
typedef uint8_t Img8x8x1[8];

/**
 * Set of 256 bitmap tiles.
 *
 * \note
 * Its layout is same as pattern generator talble and sprite pattern generator
 * table of VDP.
 */
typedef Img8x8x1 Tileset_256x8x8x1bpp[256];

/**
 * Copy 1 bpp tile pattern of 8x8 pixels to VRAM according to the given tilemap.
 *
 * This function, according to the given `tilemap`, reads 1bpp tile pattern of
 * 8x8 pixels from the given `tileset` and applies some matrix transformation,
 * and then write it to VRAM.
 *
 * A word `tileset` here means a series of 1 bpp tile pattern of 8x8 pixels,
 * that is represented as 256x8 matrix of uint8_t type. So its size shall be
 * 2KiB (2048 bytes = 256 tiles * 8 bytes/tile).
 *
 * This function can takes one or more `tileset`. If two or more tilesets are
 * passed (i.e. `m > 1`), they are treated as multi-layered tilesets.
 *
 * A word `tilemap` here means a meta-data to make a complete tiled image by
 * composing 8x8 pixels tiles picked up from `tileset`.
 *
 * `tilemap` is represeted as a list of `(idx, attr)` pairs.
 * - The 1st byte `idx` shall be the index number of a tile in `tileset`. (0..255)
 * - The 2nd byte `attr` shall be 0 or OR-ed set of the following matrix transformation:
 *   - FLIP_HORIZONTAL
 *   - FLIP_VERTICAL
 *   - TRANSPOSE
 *
 * When two or more matrix transformation are specified, they are applied as
 * following order:
 * ~~~
 * TRANSPOSE >> FLIP_VERTICAL >> FLIP_HORIZONTAL
 * ~~~
 *
 * For example, when `n = 4` and `m = 2`, the resulting VRAM memory map will be
 * as follows:
 * ~~~
 * +0x0000 +-------------------------------------+
 *         | pattern #0 / layer #0 / line #0..#7 | <- f0(tileset[0][idx0])
 * +0x0008 +-------------------------------------+
 *         | pattern #0 / layer #1 / line #0..#7 | <- f0(tileset[1][idx0])
 * +0x0010 +-------------------------------------+
 *         | pattern #1 / layer #0 / line #0..#7 | <- f1(tileset[0][idx1])
 * +0x0018 +-------------------------------------+
 *         | pattern #1 / layer #1 / line #0..#7 | <- f1(tileset[1][idx1])
 * +0x0020 +-------------------------------------+
 *         | pattern #2 / layer #0 / line #0..#7 | <- f2(tileset[0][idx2])
 * +0x0028 +-------------------------------------+
 *         | pattern #2 / layer #1 / line #0..#7 | <- f2(tileset[1][idx2])
 * +0x0030 +-------------------------------------+
 *         | pattern #3 / layer #0 / line #0..#7 | <- f3(tileset[0][idx3])
 * +0x0038 +-------------------------------------+
 *         | pattern #3 / layer #1 / line #0..#7 | <- f3(tileset[1][idx3])
 * +0x0040 +-------------------------------------+
 *
 * where
 * - idx<i> is an index number represented by tilemap[i][0]
 * - f<i>() is a matrix transformation represented by tilemap[i][1]
 * ~~~
 *
 * \param n        the number of elements of tilemap
 * \param tilemap  n-element list of (idx, attr) tupples
 * \param m        the number of leyars / the number of tilesets
 * \param tileset  m-element list of pointers to tileset. (one tileset per one layer)
 *
 * \note vmem_set_write_address() is required before call to this function.
 */
void tilemap_copy_to_vmem(uint8_t n, const TilemapElement * tilemap,
                          uint8_t m, const Tileset_256x8x8x1bpp ** tileset);

/**
 * Same as tilemap_copy_to_vmem() but resulting tiles are flipped horizonally.
 *
 * \param n        the number of elements of tilemap
 * \param tilemap  n-element list of (idx, attr) tupples
 * \param m        the number of leyars / the number of tilesets
 * \param tileset  m-element list of pointers to tileset. (one tileset per one layer)
 *
 * \note vmem_set_write_address() is required before call to this function.
 */
void tilemap_copy_to_vmem_hflip(uint8_t n, const TilemapElement * tilemap,
                                uint8_t m, const Tileset_256x8x8x1bpp ** tileset);

/**
 * Same as tilemap_copy_to_vmem() but resulting tiles are flipped vertically.
 *
 * \param n        the number of elements of tilemap
 * \param tilemap  n-element list of (idx, attr) tupples
 * \param m        the number of leyars / the number of tilesets
 * \param tileset  m-element list of pointers to tileset. (one tileset per one layer)
 *
 * \note vmem_set_write_address() is required before call to this function.
 */
void tilemap_copy_to_vmem_vflip(uint8_t n, const TilemapElement * tilemap,
                                uint8_t m, const Tileset_256x8x8x1bpp ** tileset);

/**
 * Same as tilemap_copy_to_vmem() but tilemap is processed for each 4 elements.
 *
 * This function is same as tilemap_copy_to_vmem() except for order of the
 * resulting VRAM memory map.
 *
 * In contrast to tilemap_copy_to_vmem(), this function processes `tilemap` for
 * each 4 elements. Therefore the resulting VRAM image of this function may be
 * used as "(multi-layer) sprite pattern generator table" consists of 16x16
 * pixels sprite patterns.
 *
 * For example, when `n = 4` and `m = 2`, the resulting VRAM memory map
 * will be as follows:
 * ~~~
 * +0x0000 +-------------------------------------+
 *         | pattern #0 / layer #0 / line #0..#7 | <- f0(tileset[0][idx0]) ; top-left
 * +0x0008 +-------------------------------------+
 *         | pattern #1 / layer #0 / line #0..#7 | <- f1(tileset[0][idx1]) ; bottom-left
 * +0x0010 +-------------------------------------+
 *         | pattern #2 / layer #0 / line #0..#7 | <- f2(tileset[0][idx2]) ; top-right
 * +0x0018 +-------------------------------------+
 *         | pattern #3 / layer #0 / line #0..#7 | <- f3(tileset[0][idx3]) ; bottom-right
 * +0x0020 +-------------------------------------+
 *         | pattern #0 / layer #1 / line #0..#7 | <- f0(tileset[1][idx0]) ; top-left
 * +0x0028 +-------------------------------------+
 *         | pattern #1 / layer #1 / line #0..#7 | <- f1(tileset[1][idx1]) ; bottom-left
 * +0x0030 +-------------------------------------+
 *         | pattern #2 / layer #1 / line #0..#7 | <- f2(tileset[1][idx2]) ; top-right
 * +0x0038 +-------------------------------------+
 *         | pattern #3 / layer #1 / line #0..#7 | <- f3(tileset[1][idx3]) ; bottom-right
 * +0x0040 +-------------------------------------+
 *
 * where
 * - idx<i> is an index number represented by tilemap[i][0]
 * - f<i>() is a matrix transformation represented by tilemap[i][1]
 * ~~~
 *
 * \param n        the number of elements of tilemap (multiple of 4)
 * \param tilemap  n-element list of (idx, attr) tupples
 * \param m        the number of leyars / the number of tilesets
 * \param tileset  m-element list of pointers to tileset. (one tileset per one layer)
 *
 * \note vmem_set_write_address() is required before call to this function.
 *
 * \note If `m` is 1, this function produces exactly the same result as
 * tilemap_copy_to_vmem().
 */
void tilemap_copy_to_vmem_4(uint8_t n, const TilemapElement * tilemap,
                            uint8_t m, const Tileset_256x8x8x1bpp ** tileset);

/**
 * Same as tilemap_copy_to_vmem_4() but resulting tiles are flipped horizonally.
 *
 * \param n        the number of elements of tilemap (multiple of 4)
 * \param tilemap  n-element list of (idx, attr) tupples
 * \param m        the number of leyars / the number of tilesets
 * \param tileset  m-element list of pointers to tileset. (one tileset per one layer)
 *
 * \note vmem_set_write_address() is required before call to this function.
 */
void tilemap_copy_to_vmem_4_hflip(uint8_t n, const TilemapElement * tilemap,
                                  uint8_t m, const Tileset_256x8x8x1bpp ** tileset);

/**
 * Same as tilemap_copy_to_vmem_4() but resulting tiles are flipped vertically.
 *
 * \param n        the number of elements of tilemap (multiple of 4)
 * \param tilemap  n-element list of (idx, attr) tupples
 * \param m        the number of leyars / the number of tilesets
 * \param tileset  m-element list of pointers to tileset. (one tileset per one layer)
 *
 * \note vmem_set_write_address() is required before call to this function.
 */
void tilemap_copy_to_vmem_4_vflip(uint8_t n, const TilemapElement * tilemap,
                                  uint8_t m, const Tileset_256x8x8x1bpp ** tileset);

#endif // TILEMAP_H_
