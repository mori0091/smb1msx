// -*- coding: utf-8-unix -*-

#pragma once

#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <stdbool.h>
#include <stdint.h>

typedef uint16_t uBCD16_t;

void countdown_timer_set_visible(bool visible);

void countdown_timer_print(void);

void countdown_timer_set(uBCD16_t t);

void countdown_timer_update(void);

bool countdown_is_time_up(void);

#endif
