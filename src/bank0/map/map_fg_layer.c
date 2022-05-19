// -*- coding: utf-8-unix -*-

#pragma codeseg BANK0

#include "map/map.h"

extern const uint8_t map_fg_layer_1_1_1[];

const uint8_t * const map_fg_layers[] = {
  map_fg_layer_1_1_1,
};

const uint8_t map_fg_layer_1_1_1[] = {
  // page #0

  // page #1
  XY( 0, 7), Q_BLOCK_COIN | NEWPAGE,
  XY( 4, 7), H_BRICKS(5),
  XY( 5, 7), Q_BLOCK_POWERUP,
  XY( 6, 3), Q_BLOCK_COIN,
  XY( 7, 7), Q_BLOCK_COIN,
  XY(12, 9), V_PIPE(2),

  // page #2
  XY( 6, 8), V_PIPE(3) | NEWPAGE,
  XY(14, 7), V_PIPE(4),

  // page #3
  XY( 9, 7), V_PIPE_ENTER(4) | NEWPAGE,

  // page #4
  XY( 0, 6), HIDDEN_COIN | NEWPAGE,
  XY( 5,14), VALLEY(2),         // E-command
  XY(13, 7), H_BRICKS(3),
  XY(14, 7), Q_BLOCK_COIN,

  // page #5
  XY( 0, 3), H_BRICKS(8) | NEWPAGE,
  XY( 6,14), VALLEY(3),         // E-command
  XY(11, 3), H_BRICKS(3),
  XY(14, 3), Q_BLOCK_COIN,
  XY(14, 7), H_BRICKS(1),

  // page #6
  XY( 4, 7), H_BRICKS(2) | NEWPAGE,
  XY( 5, 7), BRICK_STARMAN,
  XY(10, 7), Q_BLOCK_COIN,
  XY(13, 3), Q_BLOCK_COIN,
  XY(13, 7), Q_BLOCK_COIN,

  // page #7
  XY( 0, 7), Q_BLOCK_COIN | NEWPAGE,
  XY( 6, 7), H_BRICKS(1),
  XY( 9, 3), H_BRICKS(3),

  // page #8
  XY( 0, 3), H_BRICKS(4) | NEWPAGE,
  XY( 1, 3), Q_BLOCK_COIN,
  XY( 1, 7), H_BRICKS(2),
  XY( 2, 3), Q_BLOCK_COIN,
  XY( 6,13), UP_STAIRS(4),      // D-command
  XY(12,13), DOWN_STAIRS(4),    // D-command

  // page #9
  XY( 4,13), UP_STAIRS(4) | NEWPAGE, // D-command
  XY( 8, 7), V_BLOCKS(4),
  XY( 9,14), VALLEY(2),         // E-command
  XY(11,13), DOWN_STAIRS(4),    // D-command

  // page #10
  XY( 3, 9), V_PIPE(2) | NEWPAGE,
  XY( 8, 7), H_BRICKS(4),
  XY(10, 7), Q_BLOCK_COIN,

  // page #11
  XY( 3, 9), V_PIPE(2) | NEWPAGE,
  XY( 5,13), UP_STAIRS(8),      // D-command
  XY(13, 3), V_BLOCKS(8),

  // page #12
  XY( 6, 0), FLAG_POLE | NEWPAGE,
  XY(10,13), CASTLE(5),         // D-command

  EOS,
};
