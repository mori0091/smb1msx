// -*- coding: utf-8-unix -*-

#ifndef HSYNC_H
#define HSYNC_H

#pragma once

#include <stdint.h>

extern volatile uint16_t scroll_x;

inline void set_hscroll(uint16_t x) {
  scroll_x = x;
}

void setup_interrupt(void);

void await_hsync(void);

#endif
