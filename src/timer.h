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
 * The main-timer tick counter.
 */
extern uint16_t tick;

/**
 * The user-timer tick counter.
 */
extern uint16_t user_tick;

/**
 * Difference of the `user_tick` between before and after `timer_update()`
 * calls.
 */
extern uint8_t user_tick_delta;

void timer_init(void);
void timer_set_user_freq(uint8_t Hz);
void timer_set_fps_visible(bool visible);
void timer_reset(void);
void timer_update(void);

#endif
