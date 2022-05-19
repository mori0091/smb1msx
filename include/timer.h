// -*- coding: utf-8-unix -*-

#pragma once

#ifndef TIEMR_H
#define TIMER_H

#include <stdint.h>
#include <config.h>

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

/**
 * Estimated frame rate of the main-timer.
 */
extern uint8_t main_fps;

/**
 * Estimated frame rate of the user-timer.
 */
extern uint8_t user_fps;

void timer_init(void);
void timer_set_user_freq(uint8_t Hz);
void timer_reset(void);
void timer_update(void);

#endif
