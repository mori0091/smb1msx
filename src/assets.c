// -*- coding: utf-8-unix -*-

#include "smb1.h"

void assets_setup(void) {
  /* Apply palette table to VDP */
  vdp_write_palette(color_palette);

  /* Copy sprite patterns and sprite colors to VRAM */
  vmem_write(SPRITE_PATTERNS, smb1spt, smb1spt_size);
  vmem_write(SPRITE_COLORS, smb1sct, smb1sct_size);

  /* Copy tileset image to VRAM page #3 */
  vmem_write(TILESET, smb1tile, smb1tile_size);

  /* Draw color palette at bottom of VRAM page #3 */
  /* (debug purpose only) */
  for (int i = 0; i < 16; ++i) {
    vdp_cmd_execute_HMMV(16 * i, 196 + 3 * LINES_PER_VRAM_PAGE, 16, 16, i*17);
  }
}
