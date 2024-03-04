// -*- coding: utf-8-unix -*-

#pragma codeseg BANK2

#include <stdint.h>
#include <msx.h>

#include "boot.h"
#include "macros.h"

#include "assets/tileset.h"     // To construct BG tileset.
#include "assets/tilemap.h"     // To construct sprite patterns for Mario.

/* Pre-defined sprite pattern table */
extern const size_t smb1spt_size;
extern const char smb1spt[];

/*
 * Two tilesets (2 layers x 256 pattern x 8x8 pixel x 1bpp) and some tilemaps to
 * generate two-layer sprite patterns for Mario.
 */
extern const Tileset_256x8x8x1bpp tileset_mario_0;
extern const Tileset_256x8x8x1bpp tileset_mario_1;
const Tileset_256x8x8x1bpp * sprite_tilsets[] = {
  &tileset_mario_1,
  &tileset_mario_0,
};

extern const TilemapElement tilemap_mario[64];         // Tilemap for Mario.
extern const TilemapElement tilemap_super_mario_u[64]; // Tilemap for upper body of Super Mario.
extern const TilemapElement tilemap_super_mario_b[64]; // Tilemap for lower body of Super Mario.
extern const TilemapElement tilemap_fire_mario_u[64];  // Tilemap for upper body of Fire Mario. (Fireball, etc.)
extern const TilemapElement tilemap_fire_mario_b[64];  // Tilemap for lower body of Fire Mario. (Fireball, etc.)

#define VRAM_PAGE_FILL(pp, byte)                              \
  vdp_cmd_execute_HMMV(0,                                     \
                       (pp)*LINES_PER_VRAM_PAGE,              \
                       PIXELS_PER_LINE,                       \
                       LINES_PER_VRAM_PAGE,                   \
                       (byte))

void boot_init_vdp(void);
void boot_init_vmem(void);

void boot_main(void) __banked {
  CLIKSW = 0;
  boot_init_vdp();
  boot_init_vmem();
}

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

  /* Copy pre-defined sprite patterns (mushroom, fire-flower, etc.) to VRAM */
  vmem_write(SPRITE_PATTERNS, (void *)smb1spt, smb1spt_size);

  /* Construct Mario's sprite patterns from 2 tilesets and various tilemaps. */
  vmem_set_write_address(REF_SPRITE_PATTERNS);
  /* facing to left */
  tilemap_copy_to_vmem_4_hflip(64, tilemap_mario        , 2, sprite_tilsets); // + 0..+15
  tilemap_copy_to_vmem_4_hflip(64, tilemap_super_mario_u, 2, sprite_tilsets); // +16..+31
  tilemap_copy_to_vmem_4_hflip(64, tilemap_super_mario_b, 2, sprite_tilsets); // +32..+47
  tilemap_copy_to_vmem_4_hflip(64, tilemap_fire_mario_u , 2, sprite_tilsets); // +48..+63
  tilemap_copy_to_vmem_4_hflip(64, tilemap_fire_mario_b , 2, sprite_tilsets); // +64..+79
  /* facing to right */
  tilemap_copy_to_vmem_4      (64, tilemap_mario        , 2, sprite_tilsets);
  tilemap_copy_to_vmem_4      (64, tilemap_super_mario_u, 2, sprite_tilsets);
  tilemap_copy_to_vmem_4      (64, tilemap_super_mario_b, 2, sprite_tilsets);
  tilemap_copy_to_vmem_4      (64, tilemap_fire_mario_u , 2, sprite_tilsets);
  tilemap_copy_to_vmem_4      (64, tilemap_fire_mario_b , 2, sprite_tilsets);

  /* Construct BG tileset image to VRAM page #3 */
  tileset_decompress_into_vram();

  // /* Draw color palette at bottom of VRAM page #3 */
  // /* (debug purpose only) */
  // for (int i = 0; i < 16; ++i) {
  //   vdp_cmd_execute_HMMV(16 * i, 196 + 3 * LINES_PER_VRAM_PAGE, 16, 16, i*17);
  // }
}
