// -*- coding: utf-8-unix -*-

#include "smb1.h"

static const __at (0x002b) uint8_t INTERNATIONAL_ID_1;
static const __at (0x002c) uint8_t INTERNATIONAL_ID_2;

static uint8_t VSYNC_FREQ;

/**
 * The main-timer tick counter.
 */
uint16_t tick;

/**
 * The user-timer tick counter.
 */
uint16_t user_tick;

/**
 * Difference of the `user_tick` between before and after `timer_update()`
 * calls.
 */
uint8_t user_tick_delta;

#define COUNT_PER_SECOND    (1200)

static uint16_t prev_sys_tick2;
static uint16_t user_accum;
static uint8_t user_freq;

static uint8_t main_fps;
static uint8_t user_fps;

#define FPS_SPRITE_PAT     FPS_SPRITE_PAT_1
#define FPS_SPRITE_PAT_1   (252) /* top-left 8x8 pix */
#define FPS_SPRITE_PAT_2   (253) /* bottom-left 8x8 pix */
#define FPS_SPRITE_PLANE   FPS_SPRITE_PLANE_1
#define FPS_SPRITE_PLANE_1 (30)
#define FPS_SPRITE_PLANE_2 (31)

static uint16_t prev_sys_tick;
static uint16_t prev_main_tick;
static uint16_t prev_user_tick;

static bool fps_visible;

static const struct sprite fps_sprite[2] = {
  {.y = 195, .x = 240, .pat = FPS_SPRITE_PAT, },
  {.y = 196, .x = 241, .pat = FPS_SPRITE_PAT, },
};

void timer_set_fps_visible(bool visible) {
  vdp_cmd_await();
  if (visible) {
    graphics_set_sprite(FPS_SPRITE_PLANE  , &fps_sprite[0]);
    graphics_set_sprite(FPS_SPRITE_PLANE+1, &fps_sprite[1]);
  } else {
    graphics_hide_sprite(FPS_SPRITE_PLANE);
    graphics_hide_sprite(FPS_SPRITE_PLANE+1);
  }
  graphics_clear_sprite_pat(FPS_SPRITE_PAT, 32);
  fps_visible = visible;
}

void timer_init(void) {
  VSYNC_FREQ = msx_get_vsync_frequency();
  main_fps = VSYNC_FREQ;
  user_fps = VSYNC_FREQ;
  timer_set_user_freq(30);
}

void timer_set_user_freq(uint8_t Hz) {
  user_freq = Hz;
}

void timer_reset(void) {
  timer_set_fps_visible(fps_visible);
  // -- reset tick counters --
  prev_sys_tick = prev_main_tick = prev_user_tick = 0;
  JIFFY = tick = user_tick = 0;
  user_tick_delta = 0;
  user_accum = 0;
  prev_sys_tick2 = 0;
}

static void timer__update_fps_sprite(uint8_t fps, uint8_t pat) {
  graphics_set_sprite_pat(pat  , font_get_8x8_gryph('0'+ fps / 10 % 10), 8);
  graphics_set_sprite_pat(pat+2, font_get_8x8_gryph('0'+ fps % 10), 8);
}

static void timer_update_fps_sprite(void) {
  vdp_cmd_await();
  timer__update_fps_sprite(main_fps, FPS_SPRITE_PAT_1);
  timer__update_fps_sprite(user_fps, FPS_SPRITE_PAT_2);
}

static void timer_update_fps(void) {
  const uint16_t t0 = JIFFY;
  const uint16_t dt = t0 - prev_sys_tick;
  /* if (dt < VSYNC_FREQ) return; */
  if (dt < 10) return;
  prev_sys_tick = t0;
  main_fps += (tick      - prev_main_tick) * VSYNC_FREQ / dt;
  main_fps ++;
  main_fps >>= 1;
  user_fps += (user_tick - prev_user_tick) * VSYNC_FREQ / dt;
  user_fps ++;
  user_fps >>= 1;
  prev_main_tick = tick;
  prev_user_tick = user_tick;
}

static void timer_update_user_tick(void) {
  const uint16_t t0 = JIFFY;
  const uint16_t dt = t0 - prev_sys_tick2;
  prev_sys_tick2 = t0;
  const uint16_t count_per_user_tick = COUNT_PER_SECOND / user_freq;
  user_tick_delta = 0;
  while (count_per_user_tick <= user_accum) {
    user_accum -= count_per_user_tick;
    user_tick_delta++;
    user_tick++;
  }
  user_accum += (COUNT_PER_SECOND / VSYNC_FREQ * dt);
}

#define timer_update_main_tick()   (++tick)

void timer_update(void) {
  timer_update_fps();
  timer_update_fps_sprite();
  timer_update_main_tick();
  timer_update_user_tick();
}
