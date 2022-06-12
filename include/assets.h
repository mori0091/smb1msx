// -*- coding: utf-8-unix -*-

#pragma once

#ifndef ASSETS_H
#define ASSETS_H

#include <stdlib.h>
#include <msx.h>

/* Software envelope patterns */
extern const struct sound_eg_attribute envelope_table[16];

/* BGMs */
extern const struct sound_clip bgm_player_down;
extern const struct sound_clip bgm_over_world;
extern const struct sound_clip bgm_starman;

/* SEs */
extern const struct sound_clip se_jump;
extern const struct sound_clip se_block;
extern const struct sound_clip se_hurryup;
extern const struct sound_clip se_item;
extern const struct sound_clip se_powup;
extern const struct sound_clip se_1up;
extern const struct sound_clip se_destruct;
extern const struct sound_clip se_pipe;
extern const struct sound_clip se_coin;

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
};

extern const tagged_color_t sp_palettes[][2];

// aliases (Mario color)
#define MARIO_PALETTE           SPRITE_PALETTE_1
// aliases (Fire color)
#define FIRE_PALETTE            SPRITE_PALETTE_2
#define FIRE_MARIO_PALETTE      SPRITE_PALETTE_2
#define FIRE_LUIGI_PALETTE      SPRITE_PALETTE_2
#define MUSHROOM_PALETTE        SPRITE_PALETTE_2
#define STARMAN_PALETTE         SPRITE_PALETTE_2
#define STARMAN_1_PALETTE       SPRITE_PALETTE_2
// aliases (Luigi color)
#define LUIGI_PALETTE           SPRITE_PALETTE_3
#define GREEN_MUSHROOM_PALETTE  SPRITE_PALETTE_3
#define GREEN_KOOPA_PALETTE     SPRITE_PALETTE_3
// aliases (Block color)
#define BLOCK_PALETTE           SPRITE_PALETTE_4
#define BRICK_PALETTE           SPRITE_PALETTE_4
#define GOOMBA_PALETTE          SPRITE_PALETTE_4
// aliases (Fireflower color)
#define FIREFLOWER_PALETTE      SPRITE_PALETTE_5
#define FIREFLOWER_1_PALETTE    SPRITE_PALETTE_5
// aliases (text w/ drop-shadow)
#define TEXT_1_PALETTE          SPRITE_PALETTE_2
#define TEXT_2_PALETTE          SPRITE_PALETTE_3

void assets_setup(void);

void assets_set_sprite_palette(vmemptr_t base, uint8_t plane,
                               enum sprite_palette x);

#endif
