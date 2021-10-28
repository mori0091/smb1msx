// -*- coding: utf-8-unix -*-

#ifndef ASSETS_H
#define ASSETS_H

#pragma once

#include <stdlib.h>
#include <msx.h>

/* defined sprite pattern table */
extern const size_t smb1spt_size;
extern const char smb1spt[];

/* defined tileset */
extern const size_t smb1tile_size;
extern const char smb1tile[];

/* stage map */
extern const size_t smb1map_size;
extern const char smb1map[];

/**
 * Default color palette.
 */
extern const palette_t color_palette[16];

/**
 * Enumerations of color combination for layered sprites.
 */
enum sprite_palette {
  SPRITE_PALETTE_1 = 0,
  SPRITE_PALETTE_2 = 1,
  SPRITE_PALETTE_3 = 2,
  SPRITE_PALETTE_4 = 3,
  SPRITE_PALETTE_5 = 4,
  // aliases (Mario color)
  MARIO_PALETTE          = SPRITE_PALETTE_1,
  // aliases (Fire color)
  FIRE_PALETTE           = SPRITE_PALETTE_2,
  FIRE_MARIO_PALETTE     = SPRITE_PALETTE_2,
  FIRE_LUIGI_PALETTE     = SPRITE_PALETTE_2,
  MUSHROOM_PALETTE       = SPRITE_PALETTE_2,
  STARMAN_PALETTE        = SPRITE_PALETTE_2,
  STARMAN_1_PALETTE      = SPRITE_PALETTE_2,
  // aliases (Luigi color)
  LUIGI_PALETTE          = SPRITE_PALETTE_3,
  GREEN_MUSHROOM_PALETTE = SPRITE_PALETTE_3,
  GREEN_KOOPA_PALETTE    = SPRITE_PALETTE_3,
  // aliases (Block color)
  BLOCK_PALETTE          = SPRITE_PALETTE_4,
  BRICK_PALETTE          = SPRITE_PALETTE_4,
  GOOMBA_PALETTE         = SPRITE_PALETTE_4,
  // aliases (Fireflower color)
  FIREFLOWER_PALETTE     = SPRITE_PALETTE_5,
  FIREFLOWER_1_PALETTE   = SPRITE_PALETTE_5,
};

void assets_setup(void);

void assets_set_sprite_palette(vmemptr_t base, uint8_t plane,
                               enum sprite_palette x);

#endif
