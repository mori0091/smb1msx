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

#define FPS_SPRITE_PAT     (63*4)
#define FPS_SPRITE_PLANE   FPS_SPRITE_PLANE_1
#define FPS_SPRITE_PLANE_1 (18)
#define FPS_SPRITE_PLANE_2 (19)

static uint16_t prev_sys_tick;
static uint16_t prev_main_tick;
static uint16_t prev_user_tick;

static bool fps_visible;

void timer_set_fps_visible(bool visible) {
  static struct sprite s;
  s.pat = FPS_SPRITE_PAT;
  s.x = 240;
  s.y = (visible ? 195 : 217);
  vdp_cmd_await();
  vmem_set_sprite(SPRITES, FPS_SPRITE_PLANE, &s);
  /* s.x++; */
  /* s.y++; */
  /* vmem_set_sprite(SPRITES, FPS_SPRITE_PLANE+1, &s); */

  vmem_memset(SPRITE_PATTERNS + 8 * FPS_SPRITE_PAT, 0, 32);
  fps_visible = visible;
}

void timer_init(void) {
  VSYNC_FREQ = ((INTERNATIONAL_ID_1 & 0x80) ? 50 : 60);
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

static void timer_update_fps_sprite(uint8_t fps, uint8_t offset) {
  vdp_cmd_await();
  vmem_write(SPRITE_PATTERNS + 8 * FPS_SPRITE_PAT + offset,
             font_get_8x8_gryph('0'+ fps / 10 % 10), 8);
  vmem_write(SPRITE_PATTERNS + 8 * FPS_SPRITE_PAT + offset + 16,
             font_get_8x8_gryph('0'+ fps % 10), 8);
}

static void timer_update_fps(void) {
  const uint16_t t0 = JIFFY;
  const uint16_t dt = t0 - prev_sys_tick;
  if (dt < VSYNC_FREQ) return;
  prev_sys_tick = t0;
  main_fps = (tick      - prev_main_tick) * VSYNC_FREQ / dt;
  user_fps = (user_tick - prev_user_tick) * VSYNC_FREQ / dt;
  prev_main_tick = tick;
  prev_user_tick = user_tick;
  // -- display fps --
  timer_update_fps_sprite(main_fps, 0);
  timer_update_fps_sprite(user_fps, 8);
}

static void timer_update_user_tick(void) {
  const uint16_t t0 = JIFFY;
  const uint16_t count_per_user_tick = COUNT_PER_SECOND / user_freq;
  user_tick_delta = 0;
  while (count_per_user_tick <= user_accum) {
    user_accum -= count_per_user_tick;
    user_tick_delta++;
    user_tick++;
  }
  user_accum += (COUNT_PER_SECOND / VSYNC_FREQ * (t0 - prev_sys_tick2));
  prev_sys_tick2 = t0;
}

#define timer_update_main_tick()   (++tick)

void timer_update(void) {
  timer_update_fps();
  timer_update_main_tick();
  timer_update_user_tick();
}

void sleep_ticks(uint16_t ticks) {
  while (ticks--) {
    await_interrupt();
  }
}

void sleep_millis(uint16_t ms) {
  // sleep_millis(VSYNC_FREQ * ms / 1000); // bad!
  // when (VSYNC * ms) > 65535, the computation overflows.
  // the below avoids that situation.
  sleep_ticks(VSYNC_FREQ * (ms / 1000) +
              VSYNC_FREQ * (ms % 1000) / 1000);
}
