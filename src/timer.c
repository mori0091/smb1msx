// -*- coding: utf-8-unix -*-

#include "smb1.h"

/* timer tick counter */
uint16_t tick;

#define FPS_SPRITE_PAT     (63*4)
#define FPS_SPRITE_PLANE   FPS_SPRITE_PLANE_1
#define FPS_SPRITE_PLANE_1 (18)
#define FPS_SPRITE_PLANE_2 (19)

static uint16_t prev_sys_tick;
static uint16_t prev_usr_tick;
static bool fps_visible;

static const uint8_t font_Hz[] = {
  // 'H'
  0x033, // 00110011
  0x033, // 00110011
  0x033, // 00110011
  0x03f, // 00111111
  0x033, // 00110011
  0x033, // 00110011
  0x033, // 00110011
  0x000, // 00000000
  // 'z'
  0x000, // 00000000
  0x000, // 00000000
  0x07e, // 01111110
  0x00c, // 00001100
  0x018, // 00011000
  0x030, // 00110000
  0x07e, // 01111110
  0x000, // 00000000
};

static uint16_t diff_tick(uint16_t time, uint16_t prev) {
  if (prev <= time) {
    return time - prev;
  }
  return time + (65535 - prev) + 1;
}

void timer_set_fps_visible(bool visible) {
  struct sprite s = {.pat = FPS_SPRITE_PAT};
  vdp_cmd_await();
  if (visible) {
    sprite_set_xy(&s, 240, 195);
    vmem_set_sprite(SPRITES, FPS_SPRITE_PLANE, &s);
    sprite_set_xy(&s, 241, 196);
    vmem_set_sprite(SPRITES, FPS_SPRITE_PLANE+1, &s);
    const uint8_t* dash = font_get_8x8_gryph('-');
    vmem_write(SPRITE_PATTERNS + 8 * FPS_SPRITE_PAT     , dash, 8);
    vmem_write(SPRITE_PATTERNS + 8 * FPS_SPRITE_PAT + 16, dash, 8);
    vmem_write(SPRITE_PATTERNS + 8 * FPS_SPRITE_PAT + 8 , font_Hz    , 8);
    vmem_write(SPRITE_PATTERNS + 8 * FPS_SPRITE_PAT + 24, font_Hz + 8, 8);
  }
  else {
    sprite_set_y(&s, 127);
    vmem_set_sprite(SPRITES, FPS_SPRITE_PLANE  , &s);
    vmem_set_sprite(SPRITES, FPS_SPRITE_PLANE+1, &s);
  }
  fps_visible = visible;
}

void timer_reset(void) {
  timer_set_fps_visible(fps_visible);
  // -- reset tick counters --
  prev_sys_tick = 0;
  prev_usr_tick = 0;
  JIFFY = tick = 0;
}

void timer_update_fps(void) {
  const uint16_t jiffy = JIFFY;
  const uint16_t dt = diff_tick(jiffy, prev_sys_tick);
  if (dt < 24) return;
  const uint16_t fps = diff_tick(tick, prev_usr_tick) * 60 / dt;
  prev_sys_tick = jiffy;
  prev_usr_tick = tick;
  // -- display fps --
  vmem_write(SPRITE_PATTERNS + 8 * FPS_SPRITE_PAT,
             font_get_8x8_gryph('0'+ fps / 10 % 10), 8);
  vmem_write(SPRITE_PATTERNS + 8 * FPS_SPRITE_PAT + 16,
             font_get_8x8_gryph('0'+ fps % 10), 8);
}

void sleep_ticks(const uint16_t ticks) {
  for (uint16_t i = 0; i < ticks; ++i) {
    await_interrupt();
  }
}
