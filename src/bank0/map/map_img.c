// -*- coding: utf-8-unix -*-

#pragma codeseg BANK0

#include "map_img.h"

void copy_rect(uint8_t * dst, const uint8_t * src, uint8_t w, uint8_t h) {
  for (uint8_t j = 0; j < w; ++j) {
    memcpy(dst, src, h);
    dst += ROWS;
    src += h;
  }
}

void h_line(uint8_t * dst, uint8_t val, uint8_t w) {
  while (w--) {
    *dst = val;
    dst += ROWS;
  }
}
