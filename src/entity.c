// -*- coding: utf-8-unix -*-

#include "smb1.h"

// | entity | entity # | plane # |
// | ------ | -------- | ------- |
// | player | 0        | #0..#3  |
// | blocks | 1        | #4..#5  |
// | items  | 2        | #6..#7  |

#define TILE_EMPTY     (0x7f)
#define TILE_BLOCK     (0xb4)   // #14 (#112, #116)
#define TILE_BRICK     (0xd1)   // #15 (#120, #124)

#define PLANE_BLOCKS   (6)
#define PLANE_ITEMS    (8)

static void put_tile(uint8_t row, uint8_t col, uint8_t tile) {
  const uint16_t pp = (TILE_WIDTH * col) & 256; // page #0 (0) or page #1 (256)
  const uint16_t ix = (TILE_WIDTH * col) & 255;
  const uint16_t iy = TILE_HEIGHT * row + pp;
  const uint16_t x = (tile & 0x0f) << 4;
  const uint16_t y = (tile & 0x70) + 3 * LINES_PER_VRAM_PAGE;
  vdp_cmd_execute_HMMM(x, y, TILE_WIDTH, TILE_HEIGHT, ix, iy);
  *mapld_get_buffer_ptr_at(row, col) = tile;
}

const vec2i_t W16H16D2[] = { {0,0}, {0,0}, };

const uint8_t block_pats[] = { 112, 116, };
const uint8_t brick_pats[] = { 120, 124, };
const uint8_t debris_pats[] = { 128, 132, };

static metasprite_t block_metasprite = {
  .n = 2,
  .anchor = {0,0},
  .layouts = W16H16D2,
  // .pats = block_pats,
};

static entity_t block_entity;
static uint8_t row0, col0;
static uint8_t tile0;

#define BLOCK_VY   f10q6(5.0)
#define BLOCK_AY   f10q6(2.0)

static void block_post_step(entity_t * e) {
  if (e->vel.y < +BLOCK_VY) {
    return;
  }
  put_tile(row0, col0, tile0);
  entity_remove(e);
}

static void block_entity_new(uint8_t row, uint8_t col, uint8_t tile) {
  row0 = row;
  col0 = col;
  tile0 = tile;
  block_metasprite.pats
    = (uint8_t *)((tile == TILE_BLOCK)
                  ? block_pats
                  : brick_pats);

  entity_set_controller(&block_entity, no_controller);
  entity_set_post_step(&block_entity, block_post_step);
  entity_set_metasprite(&block_entity, &block_metasprite);
  // entity_set_sprite_palette(&block_entity, BLOCK_PALETTE);
  assets_set_sprite_palette(SPRITES_0, PLANE_BLOCKS, BLOCK_PALETTE);
  block_entity.plane = PLANE_BLOCKS;

  block_entity.input = 0;
  block_entity.pos.x.i = col * TILE_WIDTH;
  block_entity.pos.x.d = 0;
  block_entity.pos.y.i = row * TILE_HEIGHT;
  block_entity.pos.y.d = 0;
  block_entity.vel.x = 0;
  block_entity.vel.y = -BLOCK_VY;
  block_entity.acc.x = 0;
  block_entity.acc.y = BLOCK_AY;

  entity_add(&block_entity);
  put_tile(row, col, TILE_EMPTY);
}

void entity_add_block(uint8_t row, uint8_t col) {
  block_entity_new(row, col, TILE_BLOCK);
}

void entity_add_brick(uint8_t row, uint8_t col) {
  block_entity_new(row, col, TILE_BRICK);
}
