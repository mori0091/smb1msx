// -*- coding: utf-8-unix -*-

#pragma codeseg BANK0

#include <stdint.h>

#include "font.h"

extern const uint8_t font_title[];
extern const uint8_t font_punct[];
extern const uint8_t font_digit[];
extern const uint8_t font_alpha[];

const uint8_t* font_get_8x8_gryph(int c) {
  if ('0' <= c && c <= '9') {
    return font_digit + (c - '0') * 8;
  }
  if ('A' <= c && c <= 'Z') {
    return font_alpha + (c - 'A') * 8;
  }
  if ('a' <= c && c <= 'l') {
    return font_title + (c - 'a') * 8;
  }
  if (c == ' ') {
    return font_punct + 0;
  }
  if (c == '-') {
    return font_punct + 8;
  }
  if (c == 'x') {
    return font_punct + 16;
  }
  if (c == '!') {
    return font_punct + 24;
  }
  if (c == '@') {
    return font_punct + 32;
  }
  if (c == '.') {
    return font_punct + 40;
  }
  return font_punct + 0;
}
