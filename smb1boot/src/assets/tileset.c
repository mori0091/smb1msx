// -*- coding: utf-8-unix -*-

/* #include "../smb1.h" */
#include <msx.h>
#include "../macros.h"
#include "tileset.h"

/**
 * Tile-palette table (List of four color combinations).
 */
const uint16_t tile_palette_table[16] = {
  [TILE_PALETTE_ID(C540)] = 0xC540,
  [TILE_PALETTE_ID(C970)] = 0xC970,
  [TILE_PALETTE_ID(C97B)] = 0xC97B,
  [TILE_PALETTE_ID(C980)] = 0xC980,
  [TILE_PALETTE_ID(C98B)] = 0xC98B,
  [TILE_PALETTE_ID(C9AB)] = 0xC9AB,
  [TILE_PALETTE_ID(C9FB)] = 0xC9FB,
  [TILE_PALETTE_ID(C9FD)] = 0xC9FD,
  [TILE_PALETTE_ID(CBA0)] = 0xCBA0,
  [TILE_PALETTE_ID(CBD0)] = 0xCBD0,
  [TILE_PALETTE_ID(CE50)] = 0xCE50,
  [TILE_PALETTE_ID(CE5F)] = 0xCE5F,
  [TILE_PALETTE_ID(CE60)] = 0xCE60,
  [TILE_PALETTE_ID(CEDF)] = 0xCEDF,
  [14] = 0x0000,
  [15] = 0x0000,
};

static uint8_t color_lut[2][16];

static void make_color_lut(const uint8_t tile_color) {
  // 4-colors tile-palette for top half
  const uint16_t c4_a = tile_palette_table[(tile_color >> 4) & 15];
  // 4-colors tile-palette for bottom half
  const uint16_t c4_b = tile_palette_table[(tile_color >> 0) & 15];
  // 2 bpp x 2 pixels -> 4 bpp x 2 pixels
  for (uint8_t i = 0; i < 16; ++i) {
    const uint8_t s0 = (i & 0x0c);
    const uint8_t s1 = (i & 0x03) * 4;
    // color lookup table for top half
    color_lut[0][i] = (((c4_a >> s0) & 15) << 4) | ((c4_a >> s1) & 15);
    // color lookup table for bottom half
    color_lut[1][i] = (((c4_b >> s0) & 15) << 4) | ((c4_b >> s1) & 15);
  }
}

void tileset_decompress_into_vram(void) {
  const uint8_t n = tileset_table_size / 2;
  const uint8_t * ts = tileset_table;
  const uint8_t * p = tile_patterns;
  for (uint8_t i = 0; i < n; ++i, ts += 2) {
    make_color_lut(ts[1]);
    const uint8_t tile_id = ts[0];
    vmemptr_t q = TILESET + (tile_id & 0xf0) * 128 + (tile_id & 0x0f) * 8;
    for (uint8_t j = 0; j < 16; ++j) {
      const uint8_t* ctbl = color_lut[j/8];
      __critical {
        vmem_set_write_address(q);
        for (uint8_t k = 0; k < 4; ++k) {
          vmem_set(ctbl[(*p >> 4) & 15]);
          vmem_set(ctbl[(*p >> 0) & 15]);
          p++;
        }
      }
      q += 128;
    }
  }
}
