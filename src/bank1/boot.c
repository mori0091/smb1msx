// -*- coding: utf-8-unix -*-

#pragma codeseg BANK1

#include <msx.h>

#include "boot.h"
#include "macros.h"

void boot_init_vdp(void);
void boot_init_vmem(void);

void boot_main(void) __banked {
  CLIKSW = 0;
  boot_init_vdp();
  boot_init_vmem();
}

/* defined tileset */
#include "assets/tileset.h"

/* defined sprite pattern table */
extern const size_t smb1spt_size;
extern const char smb1spt[];

void boot_init_vdp(void) {
  /* Set backdrop color (border color of the screen) */
  vdp_set_color(0x00);

  vdp_set_visible(false);
  vdp_set_screen_mode(SCREEN_MODE);
  vdp_set_screen_lines(SCREEN_LINES);

  // vdp_set_pattern_table(PATTERNS);
  // vdp_set_color_table(COLORS);

  vdp_set_sprite_visible(false);
  vdp_set_sprite_attribute_table(SPRITES);
  vdp_set_sprite_pattern_table(SPRITE_PATTERNS);
  vdp_set_sprite_size(VDP_SPRITE_SIZE_16x16);

  /* Dual page horizontal scroll mode */
  vdp_set_image_table(IMAGE + SIZE_OF_VRAM_PAGE); /* SET PAGE 1 */
  vdp_set_hscroll_dual_page(true); /* enable 2 pages horizontal scroll */
  vdp_set_hscroll_mask(true);      /* hide ugly left border of 8 pixels */
  vdp_set_adjust(-4, 0);           /* centering visible display area */
}

#define VRAM_PAGE_FILL(pp, byte)                              \
  vdp_cmd_execute_HMMV(0,                                     \
                       (pp)*LINES_PER_VRAM_PAGE,              \
                       PIXELS_PER_LINE,                       \
                       LINES_PER_VRAM_PAGE,                   \
                       (byte))

void boot_init_vmem(void) {
  /* clear VRAM page #0 and #1 */
  VRAM_PAGE_FILL(0, 0x00);
  VRAM_PAGE_FILL(1, 0x00);

  // In rare cases, access to VRAM may fail or may be omitted depending on the
  // timing of the access. This occurs during a VDP command is executing.
  // Therefore, waits for VDP command finished before accsess to VRAM.
  vdp_cmd_await();
  /* Clear sprites */
  vmem_memset(SPRITES, 217, sizeof(struct sprite) * 32);

  /* Copy sprite patterns to VRAM */
  vmem_write(REF_SPRITE_PATTERNS, (void *)smb1spt, smb1spt_size);
  vmem_write(SPRITE_PATTERNS, (void *)smb1spt, smb1spt_size);

  /* Copy tileset image to VRAM page #3 */
  tileset_decompress_into_vram();

  // /* Draw color palette at bottom of VRAM page #3 */
  // /* (debug purpose only) */
  // for (int i = 0; i < 16; ++i) {
  //   vdp_cmd_execute_HMMV(16 * i, 196 + 3 * LINES_PER_VRAM_PAGE, 16, 16, i*17);
  // }
}
