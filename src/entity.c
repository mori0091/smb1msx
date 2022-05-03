// -*- coding: utf-8-unix -*-

#include "smb1.h"

// | entity | entity # | plane # |
// | ------ | -------- | ------- |
// | player | 0        | #0..#3  |
// | blocks | 1        | #4..#5  |
// | items  | 2        | #6..#7  |

#define PLANE_BLOCKS   (6)
#define PLANE_ITEMS    (8)

#define MUSHROOM_VX    f8q8(2.0)
#define MUSHROOM_AY    gravity_hi;

#define BLOCK_VY       f8q8(5.0)
#define BLOCK_AY       f8q8(2.0)

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

const uint8_t mushroom_pats[] = { 88, 92, };
const uint8_t fireflower_pats[] = { 96, 100, };
const uint8_t starman_pats[] = { 104, 108, };

const uint8_t block_pats[] = { 112, 116, };
const uint8_t brick_pats[] = { 120, 124, };
const uint8_t debris_pats[] = { 128, 132, };

const metasprite_t mushroom_metasprite = {
  .n = 2,
  .anchor = {0,0},
  .layouts = W16H16D2,
  .pats = mushroom_pats,
};

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

static entity_t item_entity;
static uint8_t item0;
static uint8_t item_tick;
static uint16_t item_x0, item_y0;

static entity_t block_entity;
static uint8_t tile0;
static uint8_t block_tick;
static uint8_t row0, col0;

static uint8_t mushroom_controller(void) {
  if (item_entity.collision & COLLISION_RIGHT) {
    item_entity.vel.x = -MUSHROOM_VX;
  }
  if (item_entity.collision & COLLISION_LEFT) {
    item_entity.vel.x = MUSHROOM_VX;
  }
  return (item_entity.vel.x < 0 ? VK_LEFT : VK_RIGHT);
}

void default_post_step(entity_t * e) {
  uint16_t x0 = camera_get_x();
  if ((e->pos.x.i <= x0 - 64)        ||
      (x0 + 256+64-16 <= e->pos.x.i) ||
      (212 <= e->pos.y.i)) {
    entity_remove(e);
    return;
  }
}

bool item_collision_handler(entity_t * e) {
  static rect_t a, b;
  entity_get_bounds(e, &a);
  entity_get_bounds(player, &b);
  if (!rect_intersects(&a, &b)) {
    return false;
  }
  if (item0 == ITEM_1UP_MUSHROOM) {
    mario_1up();
  }
  else {
    mario_power_up();
    sound_effect(&se_powup);
  }
  entity_remove(e);
  return true;
}

void mushroom_post_step2(entity_t * e) {
  if (item_collision_handler(e)) return;
  default_post_step(e);
}

void mushroom_post_step(entity_t * e) {
  if (item_tick--) {
    e->pos.x.i = item_x0;
    e->pos.y.i = item_y0 + item_tick;
  }
  else {
    e->pos.x.i = item_x0;
    e->pos.x.d = 0;
    e->pos.y.i = item_y0;
    e->pos.y.d = 0;
    e->vel.x = MUSHROOM_VX;
    e->vel.y = 0;
    e->acc.x = 0;
    e->acc.y = MUSHROOM_AY;
    e->collision = 0;
    entity_set_controller(e, mushroom_controller);
    entity_set_post_step(e, mushroom_post_step2);
  }
}

static void mushroom_entity_new(uint8_t row, uint8_t col) {
  entity_remove(&item_entity);

  entity_set_controller(&item_entity, no_controller);
  entity_set_post_step(&item_entity, mushroom_post_step);
  entity_set_metasprite(&item_entity, &mushroom_metasprite);
  // entity_set_sprite_palette(&item_entity, MUSHROOM_PALETTE);
  assets_set_sprite_palette(SPRITES_0, PLANE_ITEMS,
                            (item0 == ITEM_MUSHROOM)
                            ? MUSHROOM_PALETTE
                            : GREEN_MUSHROOM_PALETTE);
  item_entity.plane = PLANE_ITEMS;
  item_entity.input = 0;
  item_x0 = col * TILE_WIDTH;
  item_y0 = row * TILE_HEIGHT;
  item_tick = 12;
  item_entity.pos.x.i = item_x0;
  item_entity.pos.x.d = 0;
  item_entity.pos.y.i = item_y0 + item_tick;
  item_entity.pos.y.d = 0;
  entity_add(&item_entity);
}

static void block_post_step2(entity_t * e) {
  if (block_tick--) {
    e->pos.x.i = col0 * TILE_WIDTH;
    e->pos.y.i = row0 * TILE_HEIGHT;
  }
  else {
    entity_remove(e);
  }
}

static void block_post_step(entity_t * e) {
  if (e->vel.y < +BLOCK_VY) {
    return;
  }
  put_tile(row0, col0, tile0);
  switch (item0) {
  case ITEM_NONE:
    break;
  case ITEM_COIN:
    break;
  case ITEM_MUSHROOM:
  case ITEM_1UP_MUSHROOM:
    mushroom_entity_new(row0 - 1, col0);
    block_tick = 12;
    entity_set_post_step(e, block_post_step2);
    return;
  case ITEM_FIREFLOWER:
  case ITEM_STARMAN:
  default:
    break;
  }
  entity_remove(e);
}

void entity_get_bounds(const entity_t * e, rect_t * rect) {
  if (e == player && mario_has_super_ability()) {
    rect->left   = player->pos.x.i;
    rect->top    = player->pos.y.i - 16;
    rect->right  = rect->left + 15;
    rect->bottom = rect->top  + 31;
  }
  else {
    rect->left   = e->pos.x.i;
    rect->top    = e->pos.y.i;
    rect->right  = rect->left + 15;
    rect->bottom = rect->top  + 15;
  }
}

void entity_add_block(uint8_t row, uint8_t col, uint8_t tile, uint8_t item) {
  row0 = row;
  col0 = col;
  tile0 = tile;
  item0 = item;

  entity_set_controller(&block_entity, no_controller);
  entity_set_post_step(&block_entity, block_post_step);
  entity_set_metasprite(&block_entity, (tile == TILE_BLOCK) ? &block_metasprite : &brick_metasprite);
  // entity_set_sprite_palette(&block_entity, BLOCK_PALETTE);
  // assets_set_sprite_palette(SPRITES_0, PLANE_BLOCKS, BLOCK_PALETTE);
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
