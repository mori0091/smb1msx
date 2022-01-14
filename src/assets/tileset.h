// -*- coding: utf-8-unix -*-

#ifndef TILESET_H
#define TILESET_H

#pragma once

#include <stdint.h>
#include <stdlib.h>

/**
 * Decompress tileset data and write it to VRAM.
 */
void tileset_decompress_into_vram(void);

/**
 * Constructs 1 byte tile-color code.
 *
 * \param a    4-color set (in 4 hex-digits) for top 8 lines
 * \param b    4-color set (in 4 hex-digits) for bottom 8 lines
 */
#define TILE_COLOR(a, b)                        \
  TILE_COLOR0(TILE_PALETTE_ID(a),               \
              TILE_PALETTE_ID(b))
#define TILE_COLOR0(id_a, id_b)                 \
  (((id_a) & 15) << 4 | ((id_b) & 15))

/**
 * Constructs an index of the four-color set table.
 *
 * \param x    4-color set (in 4 hex-digits)
 */
#define TILE_PALETTE_ID(x)   TILE_PALETTE_ID0(x)
#define TILE_PALETTE_ID0(x)  TILE_PALETTE_ID_ ## x

#define TILE_PALETTE_ID_C540  (0)  // C540 (C5*0 C*40 *5*0 *540)
#define TILE_PALETTE_ID_C970  (1)  // C970
#define TILE_PALETTE_ID_C97B  (2)  // C97B (C9*B)
#define TILE_PALETTE_ID_C980  (3)  // C98*
#define TILE_PALETTE_ID_C98B  (4)  // C98B (C9*B)
#define TILE_PALETTE_ID_C9AB  (5)  // C9AB (C9*B)
#define TILE_PALETTE_ID_C9FB  (6)  // C9FB (C9*B)
#define TILE_PALETTE_ID_C9FD  (7)  // C9FD
#define TILE_PALETTE_ID_CBA0  (8)  // CBA0 (CB*0 *BA0 *B*0 C*A*)
#define TILE_PALETTE_ID_CBD0  (9)  // CBD0 (*BD0)
#define TILE_PALETTE_ID_CE50  (10) // CE50 (CE5*)
#define TILE_PALETTE_ID_CE5F  (11) // CE5F
#define TILE_PALETTE_ID_CE60  (12) // CE60 (CE*0 CE6*)
#define TILE_PALETTE_ID_CEDF  (13) // CEDF

/**
 * Tile-palette table (List of four color combinations).
 */
extern const uint16_t tile_palette_table[16];

/**
 * List of (tile-id, tile-color) 2 byte tuples.
 *
 * The 1st byte is `tile-id`. (0x00..0x7f)
 * The 2nd byte consists of two 4-color set id:
 * - Most significant 4 bits are a 4-color set id for top 8 lines.
 * - Least significant 4 bits are a 4-color set id for bottom 8 lines.
 */
extern const uint8_t tileset_table[];

/**
 * Size of the `tileset_table` in bytes.
 */
extern const size_t tileset_table_size;

/**
 * Tile pattern table (64 bytes per tile).
 */
extern const uint8_t tile_patterns[];

#endif
