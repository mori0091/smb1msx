// -*- coding: utf-8-unix -*-

#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#pragma once

#include <stdbool.h>

void countdown_timer_print(void);

void countdown_timer_set(int t);

void countdown_timer_update(void);

bool countdown_is_time_up(void);

#endif
