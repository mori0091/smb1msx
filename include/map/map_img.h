// -*- coding: utf-8-unix -*-

#pragma once

#ifndef MAP_IMG_H
#define MAP_IMG_H

#include <stdint.h>
#include <string.h>

#include "../macros.h"

#define ROWS  STAGEMAP_PAGE_ROWS
#define COLS  STAGEMAP_PAGE_COLS

void copy_rect(uint8_t * dst, const uint8_t * src, uint8_t w, uint8_t h);

void h_line(uint8_t * dst, uint8_t val, uint8_t w);

inline void v_line(uint8_t * dst, uint8_t val, uint8_t h) {
  memset(dst, val, h);
}

#endif
