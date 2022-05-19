// -*- coding: utf-8-unix -*-

#pragma codeseg BANK0

#include <stdint.h>

#include "smb1.h"

#define FPS_SPRITE_PAT     FPS_SPRITE_PAT_1
#define FPS_SPRITE_PAT_1   (252) /* top-left 8x8 pix */
#define FPS_SPRITE_PAT_2   (253) /* bottom-left 8x8 pix */
#define FPS_SPRITE_PLANE   FPS_SPRITE_PLANE_1
/* #define FPS_SPRITE_PLANE_1 (30) */
/* #define FPS_SPRITE_PLANE_2 (31) */
#define FPS_SPRITE_PLANE_1 (0)
#define FPS_SPRITE_PLANE_2 (1)

static bool fps_visible;

static const struct sprite fps_sprite[2] = {
  {.y = 195, .x = 240, .pat = FPS_SPRITE_PAT, },
  {.y = 196, .x = 241, .pat = FPS_SPRITE_PAT, },
};

void fps_display_set_visible(bool visible) {
  vdp_cmd_await();
  if (visible) {
    // assets_set_sprite_palette(SPRITES, FPS_SPRITE_PLANE, TEXT_1_PALETTE);
    graphics_set_sprite(FPS_SPRITE_PLANE  , &fps_sprite[0]);
    graphics_set_sprite(FPS_SPRITE_PLANE+1, &fps_sprite[1]);
  } else {
    graphics_hide_sprite(FPS_SPRITE_PLANE);
    graphics_hide_sprite(FPS_SPRITE_PLANE+1);
  }
  graphics_clear_sprite_pat(FPS_SPRITE_PAT, 32);
  fps_visible = visible;
}

void fps_display_reset(void) {
  fps_display_set_visible(fps_visible);
}

static void fps_display__update_sprite(uint8_t fps, uint8_t pat) {
  graphics_set_sprite_pat(pat  , font_get_8x8_gryph('0'+ fps / 10 % 10), 8);
  graphics_set_sprite_pat(pat+2, font_get_8x8_gryph('0'+ fps % 10), 8);
}

void fps_display_update(void) {
  /* vdp_cmd_await(); */
  fps_display__update_sprite(main_fps, FPS_SPRITE_PAT_1);
  fps_display__update_sprite(user_fps, FPS_SPRITE_PAT_2);
}
