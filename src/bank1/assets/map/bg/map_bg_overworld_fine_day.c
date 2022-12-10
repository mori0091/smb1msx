// -*- coding: utf-8-unix -*-

#pragma codeseg BANK1

#include "map_cmd.h"

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
