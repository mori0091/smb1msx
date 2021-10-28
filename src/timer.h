// -*- coding: utf-8-unix -*-

#ifndef TIEMR_H
#define TIMER_H

#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * Tick counter (TIME) of MSX-BASIC
 */
static volatile __at (0xfc9e) uint16_t JIFFY;

/**
 * timer tick counter
 */
extern uint16_t tick;

void timer_set_fps_visible(bool visible);
void timer_update_fps(void);
void timer_reset(void);

inline uint16_t timer_get_tick(void) {
  return tick;
}

inline uint16_t timer_update(void) {
  timer_update_fps();
  return ++tick;
}

void sleep_ticks(const uint16_t ticks);

#endif
