// -*- coding: utf-8-unix -*-

#pragma once

#ifndef MAP_LD_H
#define MAP_LD_H

#include "map.h"

struct map_ptr {
  const uint8_t * head;         // pointer to the head of map command stream
  const uint8_t * ptr;          // pointer to the next map command
  uint8_t cmd[2];               // pre-fetched map command
};

struct map_ctx {
  uint8_t bg_theme;             // BG color theme / BG tile theme.
  uint8_t block_layout[13];     // Reference wall block placement pattern.
  struct map_ptr bg;            // BG map data pointer.
  struct map_ptr fg;            // FG map data pointer.
  uint8_t page;                 // Current page number.
};

void map_ld_init(void);
void map_ld_set_bg_theme(uint8_t theme);
void map_ld_set_bg_layer(const uint8_t * bg_data, uint16_t block_layout);
void map_ld_set_fg_layer(const uint8_t * fg_data);
void map_ld_load_page(uint8_t * canvas);

#endif
