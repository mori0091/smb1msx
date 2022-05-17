// -*- coding: utf-8-unix -*-

#include "smb1.h"

#pragma codeseg BANK0

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

/**
 * Estimated frame rate of the main-timer.
 */
uint8_t main_fps;

/**
 * Estimated frame rate of the user-timer.
 */
uint8_t user_fps;

static uint8_t VSYNC_FREQ;

#define COUNT_PER_SECOND    (1200)

static uint8_t count_per_vsync;
static uint8_t count_per_user_tick;

static uint16_t prev_sys_tick2;
static uint16_t user_accum;
static uint8_t user_freq;

static uint16_t prev_sys_tick;
static uint16_t prev_main_tick;
static uint16_t prev_user_tick;

void timer_init(void) {
  VSYNC_FREQ = msx_get_vsync_frequency();
  count_per_vsync = COUNT_PER_SECOND / VSYNC_FREQ;
  main_fps = VSYNC_FREQ;
  user_fps = VSYNC_FREQ;
  timer_set_user_freq(30);
}

void timer_set_user_freq(uint8_t Hz) {
  user_freq = Hz;
  count_per_user_tick = COUNT_PER_SECOND / user_freq;
}

void timer_reset(void) {
  // -- reset tick counters --
  prev_sys_tick = prev_main_tick = prev_user_tick = 0;
  JIFFY = tick = user_tick = 0;
  user_tick_delta = 0;
  user_accum = 0;
  prev_sys_tick2 = 0;
}

inline void timer_update_fps(void) {
  const uint16_t t0 = JIFFY;
  const uint16_t dt = t0 - prev_sys_tick;
  /* if (dt < VSYNC_FREQ) return; */
  if (dt < 31) return;
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

inline void timer_update_user_tick(void) {
  {
    const uint16_t t0 = JIFFY;
    user_accum += count_per_vsync * (t0 - prev_sys_tick2);
    prev_sys_tick2 = t0;
  }
  user_tick_delta = 0;
  while (count_per_user_tick <= user_accum) {
    user_accum -= count_per_user_tick;
    user_tick_delta++;
    user_tick++;
  }
}

#define timer_update_main_tick()   (++tick)

void timer_update(void) {
  timer_update_fps();
  timer_update_main_tick();
  timer_update_user_tick();
}
