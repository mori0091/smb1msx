// -*- coding: utf-8-unix -*-

#include "smb1.h"

/* default color palette */
const palette_t color_palette[16] = {
  RGB(0,0,0), RGB(6,1,0), RGB(3,3,1), RGB(7,5,4),
  RGB(4,6,0), RGB(0,5,0), RGB(1,5,7), RGB(7,4,1),
  RGB(6,2,0), RGB(1,1,1), RGB(7,5,5), RGB(6,2,0),
  RGB(2,4,7), RGB(6,1,0), RGB(7,7,7), RGB(7,5,0),
};

const enum sprite_palette sp[] = {
  [0] = MARIO_PALETTE,
  [1] = MARIO_PALETTE,
  [2] = GOOMBA_PALETTE,
  [3] = GOOMBA_PALETTE,
  [4] = FIRE_MARIO_PALETTE,
  [5] = FIRE_MARIO_PALETTE,
  [6] = LUIGI_PALETTE,
  [7] = LUIGI_PALETTE,
  [8] = SPRITE_PALETTE_1,
  [9] = SPRITE_PALETTE_2,
  [10] = SPRITE_PALETTE_3,
  [11] = MUSHROOM_PALETTE,
  [12] = FIREFLOWER_PALETTE,
  [13] = STARMAN_PALETTE,
  [14] = BLOCK_PALETTE,
  [15] = BRICK_PALETTE,
};

void assets_setup(void) {
  /* Apply palette table to VDP */
  vdp_write_palette(color_palette);

  /* Copy sprite patterns and sprite colors to VRAM */
  vmem_write(SPRITE_PATTERNS, smb1spt, smb1spt_size);
  for (int i = 0; i < 16; ++i) {
    assets_set_sprite_palette(SPRITES, i*2, sp[i]);
  }

  /* Copy tileset image to VRAM page #3 */
  vmem_write(TILESET, smb1tile, smb1tile_size);

  /* Draw color palette at bottom of VRAM page #3 */
  /* (debug purpose only) */
  for (int i = 0; i < 16; ++i) {
    vdp_cmd_execute_HMMV(16 * i, 196 + 3 * LINES_PER_VRAM_PAGE, 16, 16, i*17);
  }
}

static const tagged_color_t sp_palettes[][2] = {
  [SPRITE_PALETTE_1] = {{0x01}, {0x02 | SPRITE_TAG_CC}},
  [SPRITE_PALETTE_2] = {{0x0e}, {0x0d | SPRITE_TAG_CC}},
  [SPRITE_PALETTE_3] = {{0x0e}, {0x05 | SPRITE_TAG_CC}},
  [SPRITE_PALETTE_4] = {{0x0a}, {0x09 | SPRITE_TAG_CC}},
};

void assets_set_sprite_palette(vmemptr_t base, uint8_t plane,
                               enum sprite_palette x) {
  switch (x) {
  case SPRITE_PALETTE_1:
  case SPRITE_PALETTE_2:
  case SPRITE_PALETTE_3:
  case SPRITE_PALETTE_4:
    vmem_set_sprite_color_m(base, plane, 2, &(sp_palettes[x][0]));
    break;
  case SPRITE_PALETTE_5:
    vmem_memset(base - 0x0200 + (plane+0) * 16  , 0x0e, 8);
    vmem_memset(base - 0x0200 + (plane+0) * 16+8, 0x05, 8);
    vmem_memset(base - 0x0200 + (plane+1) * 16  , 0x0d | SPRITE_TAG_CC, 8);
    vmem_memset(base - 0x0200 + (plane+1) * 16+8, 0x05 | SPRITE_TAG_CC, 8);
    break;
  default:
    break;
  }
}
