// -*- coding: utf-8-unix -*-

#include "../smb1.h"
#include "tileset.h"

/**
 * List of four color combinations.
 */
const uint16_t four_color_set[16] = {
  [FOUR_COLOR_SET_ID(C540)] = 0xC540,
  [FOUR_COLOR_SET_ID(C970)] = 0xC970,
  [FOUR_COLOR_SET_ID(C97B)] = 0xC97B,
  [FOUR_COLOR_SET_ID(C980)] = 0xC980,
  [FOUR_COLOR_SET_ID(C98B)] = 0xC98B,
  [FOUR_COLOR_SET_ID(C9AB)] = 0xC9AB,
  [FOUR_COLOR_SET_ID(C9FB)] = 0xC9FB,
  [FOUR_COLOR_SET_ID(C9FD)] = 0xC9FD,
  [FOUR_COLOR_SET_ID(CBA0)] = 0xCBA0,
  [FOUR_COLOR_SET_ID(CBD0)] = 0xCBD0,
  [FOUR_COLOR_SET_ID(CE50)] = 0xCE50,
  [FOUR_COLOR_SET_ID(CE5F)] = 0xCE5F,
  [FOUR_COLOR_SET_ID(CE60)] = 0xCE60,
  [FOUR_COLOR_SET_ID(CEDF)] = 0xCEDF,
  [14] = 0x0000,
  [15] = 0x0000,
};

static uint8_t color_table[2][16];

static void make_color_table(const uint8_t color_id) {
  const uint16_t c4_a = four_color_set[(color_id >> 4) & 15];
  const uint16_t c4_b = four_color_set[(color_id >> 0) & 15];
  for (int i = 0; i < 16; ++i) {
    const uint8_t s0 = 4 * ((i >> 2) & 3);
    const uint8_t s1 = 4 * ((i >> 0) & 3);
    color_table[0][i] = (((c4_a >> s0) & 15) << 4) | ((c4_a >> s1) & 15);
    color_table[1][i] = (((c4_b >> s0) & 15) << 4) | ((c4_b >> s1) & 15);
  }
}

void tileset_decompress_into_vram(void) {
  for (int i = 0; i < tileset_table_size; i+=2) {
    make_color_table(tileset_table[i+1]);
    const uint8_t tile = tileset_table[i+0];
    vmemptr_t q = TILESET + (tile & 0xf0) * 128 + (tile & 0x0f) * 8;
    const uint8_t* p = tile_patterns + 4 * 16 * i/2;
    for (int j = 0; j < 16; ++j) {
      const uint8_t* ctbl = color_table[j/8];
      __critical {
        vmem_set_write_address(q);
        for (int k = 0; k < 4; ++k) {
          vmem_set(ctbl[(*p >> 4) & 15]);
          vmem_set(ctbl[(*p >> 0) & 15]);
          p++;
        }
      }
      q += 128;
    }
  }
}
