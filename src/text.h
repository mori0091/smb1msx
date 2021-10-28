// -*- coding: utf-8-unix -*-

#ifndef TEXT_H
#define TEXT_H

#pragma once

#include <stdint.h>

extern uint16_t text_x;
extern uint16_t text_y;
extern uint8_t text_fg_color;
extern uint8_t text_bg_color;

inline void set_text_color(uint8_t fg, uint8_t bg) {
  text_fg_color = fg;
  text_bg_color = bg;
}

inline void locate(uint16_t x, uint16_t y) {
  text_x = x;
  text_y = y;
}

void putc(uint8_t c);

void puts(const uint8_t* s);

#endif