// -*- coding: utf-8-unix -*-

#pragma codeseg BANK0

#include "map/map.h"

extern const uint8_t map_bg_overworld_fine_day[];
extern const uint8_t map_bg_overworld_athletic[];
extern const uint8_t map_bg_over_sky[];
extern const uint8_t map_bg_under_water[];
extern const uint8_t map_bg_underworld[];
extern const uint8_t map_bg_castle[];

const uint8_t * const map_bg_layers[] = {
  [MAP_BG_OVERWORLD_FINE_DAY] = map_bg_overworld_fine_day,
  [MAP_BG_OVERWORLD_ATHLETIC] = map_bg_overworld_athletic,
  [MAP_BG_OVER_SKY]           = map_bg_over_sky,
  [MAP_BG_UNDER_WATER]        = map_bg_under_water,
  [MAP_BG_UNDERWORLD]         = map_bg_underworld,
  [MAP_BG_CASTLE]             = map_bg_castle,
};

const uint8_t map_bg_overworld_fine_day[] = {
  XY( 0, 8), MOUNTAIN(3),
  XY( 8, 1), CLOUD(3),
  XY(11,10), GRASS(5),

  XY( 0, 9), MOUNTAIN(2) | NEWPAGE,
  XY( 3, 0), CLOUD(3),
  XY( 7,10), GRASS(3),
  XY(11, 1), CLOUD(5),

  XY( 4, 0), CLOUD(4)    | NEWPAGE,
  XY( 9,10), GRASS(4),

  EOS,
};

const uint8_t map_bg_overworld_athletic[] = {
  EOS,
};

const uint8_t map_bg_over_sky[] = {
  EOS,
};

const uint8_t map_bg_under_water[] = {
  EOS,
};

const uint8_t map_bg_underworld[] = {
  EOS,
};

const uint8_t map_bg_castle[] = {
  EOS,
};
