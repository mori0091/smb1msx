// -*- coding: utf-8-unix -*-

#include "../smb1.h"
#include <stdint.h>

#define BLOCK_VY       f8q8(5.0)
#define BLOCK_AY       f8q8(2.0)

const uint8_t block_pats[] = { 112, 116, };
const uint8_t brick_pats[] = { 120, 124, };
const uint8_t debris_pats[] = { 128, 132, };

const metasprite_t block_metasprite = {
  .n = 2,
  .anchor = {0,0},
  .layouts = W16H16D2,
  .pats = block_pats,
};

const metasprite_t brick_metasprite = {
  .n = 2,
  .anchor = {0,0},
  .layouts = W16H16D2,
  .pats = brick_pats,
};

static void block_post_step2(entity_t * e) {
  if (block_state.tick--) {
    e->pos.x.i = block_state.x0;
    e->pos.y.i = block_state.y0;
  }
  else {
    physics_remove_entity(e);
  }
}

static void block_post_step(entity_t * e) {
  if (e->vel.y < +BLOCK_VY) {
    return;
  }
  stage_put_tile(block_state.row0, block_state.col0, block_state.kind);
  switch (item_state.kind) {
  case ITEM_NONE:
    break;
  case ITEM_COIN:
    break;
  case ITEM_MUSHROOM:
  case ITEM_1UP_MUSHROOM:
    mushroom_entity_new(block_state.row0 - 1, block_state.col0);
    block_state.tick = 12;
    entity_set_post_step(e, block_post_step2);
    return;
  case ITEM_FIREFLOWER:
  case ITEM_STARMAN:
  default:
    break;
  }
  physics_remove_entity(e);
}

void block_entity_new(uint8_t row, uint8_t col, uint8_t tile, uint8_t item) {
  block_state.row0 = row;
  block_state.col0 = col;
  block_state.x0 = col * TILE_WIDTH;
  block_state.y0 = row * TILE_HEIGHT;
  block_state.kind = tile;
  item_state.kind = item;

  entity_set_controller(&block_entity, no_controller);
  entity_set_post_step(&block_entity, block_post_step);
  entity_set_metasprite(&block_entity, (tile == TILE_BLOCK) ? &block_metasprite : &brick_metasprite);
  // entity_set_sprite_palette(&block_entity, BLOCK_PALETTE);
  // assets_set_sprite_palette(SPRITES_0, PLANE_BLOCKS, BLOCK_PALETTE);
  block_entity.plane = PLANE_BLOCKS;

  block_entity.input = 0;
  block_entity.pos.x.i = block_state.x0;
  block_entity.pos.x.d = 0;
  block_entity.pos.y.i = block_state.y0;
  block_entity.pos.y.d = 0;
  block_entity.vel.x = 0;
  block_entity.vel.y = -BLOCK_VY;
  block_entity.acc.x = 0;
  block_entity.acc.y = BLOCK_AY;

  physics_add_entity(&block_entity);
  stage_put_tile(row, col, TILE_EMPTY);
}
