// -*- coding: utf-8-unix -*-

#pragma once

#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>

extern uint16_t text_x;
extern uint16_t text_y;

void set_foreground_color(uint8_t fg);

void set_background_color(uint8_t bg);

void set_text_color(uint8_t fg, uint8_t bg);

inline void locate(uint16_t x, uint16_t y) {
  text_x = x;
  text_y = y;
}

void text_putc(uint8_t c);

void text_puts(const uint8_t* s);

#endif
