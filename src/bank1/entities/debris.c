// -*- coding: utf-8-unix -*-

#pragma codeseg BANK1

#include <stdint.h>

#include "smb1.h"

#define DEBRIS_VX       f8q8(5.0)
#define DEBRIS_VY       f8q8(5.0)
#define DEBRIS_AY       f8q8(2.0)

static entity_t debris_entity[4];

const uint8_t debris_pats[] = { 128, 132, };

const metasprite_t debris_metasprite = {
  .n = 2,
  .anchor = {0,0},
  .layouts = W16H16D2,
  .pats = debris_pats,
};

static void debris_post_step(entity_t * e) {
  if (e->vel.y < DEBRIS_VY) {
    return;
  }
  physics_remove_entity(e);
}

void debris_entity_new(uint8_t row, uint8_t col) {
  stage_put_tile(row, col, TILE_EMPTY);

  debris_entity[0].vel.x = 2 * DEBRIS_VX;
  debris_entity[0].vel.y = -DEBRIS_VY;
  debris_entity[1].vel.x = 2 * -DEBRIS_VX;
  debris_entity[1].vel.y = -DEBRIS_VY;
  debris_entity[2].vel.x = DEBRIS_VX;
  debris_entity[2].vel.y = 2 * -DEBRIS_VY;
  debris_entity[3].vel.x = -DEBRIS_VX;
  debris_entity[3].vel.y = 2 * -DEBRIS_VY;
  for (int8_t i = 0; i < 4; ++i) {
      physics_remove_entity(&debris_entity[i]);
      entity_set_controller(&debris_entity[i], no_controller);
      entity_set_post_step(&debris_entity[i], debris_post_step);
      entity_set_metasprite(&debris_entity[i], &debris_metasprite);
      // entity_set_sprite_palette(&debris_entity[i], DEBRIS_PALETTE);
      assets_set_sprite_palette(SPRITES, PLANE_DEBRIS + 2 * i, BLOCK_PALETTE);
      debris_entity[i].plane = PLANE_DEBRIS + 2 * i;

      debris_entity[i].input = 0;
      debris_entity[i].pos.x.i = col * TILE_WIDTH;
      debris_entity[i].pos.x.d = 0;
      debris_entity[i].pos.y.i = (row-1) * TILE_HEIGHT;
      debris_entity[i].pos.y.d = 0;
      debris_entity[i].acc.x = 0;
      debris_entity[i].acc.y = DEBRIS_AY;

      physics_add_entity(&debris_entity[i]);
  }
}
