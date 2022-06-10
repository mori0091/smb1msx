// -*- coding: utf-8-unix -*-

#pragma codeseg BANK0

#include "smb1.h"

#define ENTITY_MAX  (8)

struct {
  uint8_t length;
  entity_t * list[ENTITY_MAX];
} entities;

entity_t player_entity;
entity_t * const player = &player_entity;

#define BUFFER_COLS   (STAGEMAP_BUFFER_PAGES * STAGEMAP_PAGE_COLS)
#define BUFFER_WIDTH  (BUFFER_COLS * TILE_WIDTH)

void physics_init(void) {
  entities.length = 1;
  entities.list[0] = player;
  sm2_clear_sprites();
}

void physics_add_entity(entity_t * const e) {
  if (ENTITY_MAX <= entities.length || !e) return;
  entities.list[entities.length++] = e;
}

void physics_remove_entity(entity_t * const e) {
  if (!entities.length || !e) return;
  if (e->metasprite) {
    sm2_hide_sprites(e->plane, e->metasprite);
  }
  entity_t ** p = &(entities.list[entities.length - 1]);
  entity_t * const last = *p;
  uint8_t n = entities.length;
  while (n--) {
    if (*p == e) {
      entities.length--;
      *p = last;
      return;
    }
    p--;
  }
}

inline void player_correct_position(void) {
  // Limits the state of the dynamics to between the upper and lower limits.
  uint16_t x0 = camera_get_x() + 8;
  if (player->pos.x.i < x0) {
    player->pos.x.i = x0;
    player->pos.x.d = 0;
    player->vel.x = 0;
  }
}

static void foreach_active_entity(void (* f)(entity_t *)) {
  entity_t ** es = entities.list;
  uint8_t n = entities.length;
  while (n--) {
    f(*es++);
  }
}

void physics_update(void) {
  // Action planning task
  foreach_active_entity(entity_update_input);
  // Dynamics state update task (position)
  foreach_active_entity(entity_update_dynamics);
  player_correct_position();
  // Collision test task
  foreach_active_entity(entity_update_collision);
  // Dynamics state update task (velocity / acceleration)
  foreach_active_entity(entity_update_speed);
  // update camera position and speed
  camera_update();
  // Pose update & Sound / Visual effects, etc.
  foreach_active_entity(entity_run_post_step);
}

static void entity_set_sprites(entity_t * e) {
  if (!e->metasprite) return;
  const int x = e->pos.x.i - camera_get_x();
  const int y = e->pos.y.i - 1;
  sm2_show_sprites(e->plane, e->metasprite, x, y);
}

void physics_update_sprites(void) {
  sm2_clear_sprites();
  foreach_active_entity(entity_set_sprites);
}

void physics_apply_sprites(void) {
  sm2_apply_sprites();
}
