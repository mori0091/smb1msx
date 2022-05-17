// -*- coding: utf-8-unix -*-

#include "../smb1.h"
#include <stdint.h>

#define MUSHROOM_VX    f8q8(2.0)
#define MUSHROOM_AY    gravity_hi;

static entity_t item_entity;
static entity_state_t item_state;

const uint8_t mushroom_pats[] = { 88, 92, };
const uint8_t fireflower_pats[] = { 96, 100, };
const uint8_t starman_pats[] = { 104, 108, };

#pragma codeseg BANK0

const metasprite_t mushroom_metasprite = {
  .n = 2,
  .anchor = {0,0},
  .layouts = W16H16D2,
  .pats = mushroom_pats,
};

const metasprite_t fireflower_metasprite = {
  .n = 2,
  .anchor = {0,0},
  .layouts = W16H16D2,
  .pats = fireflower_pats,
};

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
    physics_remove_entity(e);
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
  if (item_state.item == ITEM_1UP_MUSHROOM) {
    mario_1up();
    sound_effect(&se_1up);
  }
  else {
    mario_power_up();
    sound_effect(&se_powup);
  }
  physics_remove_entity(e);
  return true;
}

void mushroom_post_step2(entity_t * e) {
  if (item_collision_handler(e)) return;
  default_post_step(e);
}

void mushroom_post_step(entity_t * e) {
  if (item_state.tick--) {
    e->pos.x.i = item_state.x0;
    e->pos.y.i = item_state.y0 + item_state.tick;
  }
  else {
    e->pos.x.i = item_state.x0;
    e->pos.x.d = 0;
    e->pos.y.i = item_state.y0;
    e->pos.y.d = 0;
    if (item_state.item == ITEM_FIREFLOWER) {
        e->vel.x = 0;
        e->vel.y = 0;
        e->acc.x = 0;
        e->acc.y = 0;
        entity_set_controller(e, no_controller);
    }
    else {
        e->vel.x = MUSHROOM_VX;
        e->vel.y = 0;
        e->acc.x = 0;
        e->acc.y = MUSHROOM_AY;
        entity_set_controller(e, mushroom_controller);
    }
    entity_set_post_step(e, mushroom_post_step2);
    e->collision = 0;
  }
}

void mushroom_entity_new(uint8_t row, uint8_t col, uint8_t item) {
  item_state.x0 = col * TILE_WIDTH;
  item_state.y0 = row * TILE_HEIGHT;
  item_state.item = item;
  item_state.tick = 12;

  physics_remove_entity(&item_entity);
  entity_set_controller(&item_entity, no_controller);
  entity_set_post_step(&item_entity, mushroom_post_step);
  switch (item) {
      case ITEM_MUSHROOM:
          entity_set_metasprite(&item_entity, &mushroom_metasprite);
          assets_set_sprite_palette(SPRITES, PLANE_ITEMS, MUSHROOM_PALETTE);
          break;
      case ITEM_1UP_MUSHROOM:
          entity_set_metasprite(&item_entity, &mushroom_metasprite);
          assets_set_sprite_palette(SPRITES, PLANE_ITEMS, GREEN_MUSHROOM_PALETTE);
          break;
      case ITEM_FIREFLOWER:
      default:
          entity_set_metasprite(&item_entity, &fireflower_metasprite);
          assets_set_sprite_palette(SPRITES, PLANE_ITEMS, FIREFLOWER_PALETTE);
          break;
  }
  item_entity.plane = PLANE_ITEMS;
  item_entity.input = 0;
  item_entity.pos.x.i = item_state.x0;
  item_entity.pos.x.d = 0;
  item_entity.pos.y.i = item_state.y0 + item_state.tick;
  item_entity.pos.y.d = 0;
  physics_add_entity(&item_entity);
}
