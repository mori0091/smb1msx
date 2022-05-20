// -*- coding: utf-8-unix -*-

#pragma once

#ifndef MAP_DATA_H_
#define MAP_DATA_H_

#include <stdint.h>

enum map_bg {
  MAP_BG_OVERWORLD_FINE_DAY = 0,
  MAP_BG_OVERWORLD_ATHLETIC,
  MAP_BG_OVER_SKY,
  MAP_BG_UNDER_WATER,
  MAP_BG_UNDERWORLD,
  MAP_BG_CASTLE,
};

/**
 * BG layer table.
 *
 * The list of pointers to map command streams for BG layer.
 * The list is indexed by `enum map_bg`.
 */
extern const uint8_t * const map_bg_layers[];

/**
 * FG layer table.
 *
 * The list of pointers to map command streams for FG layer.
 */
extern const uint8_t * const map_fg_layers[];

#endif // MAP_DATA_H_
