// -*- coding: utf-8-unix -*-

#pragma codeseg BANK1

#include "map_data.h"

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

extern const uint8_t map_fg_world_1_1_room_1[];

const uint8_t * const map_fg_layers[] = {
  map_fg_world_1_1_room_1,
};
