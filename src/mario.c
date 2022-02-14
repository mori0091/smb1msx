// -*- coding: utf-8-unix -*-

#include "smb1.h"

struct mario_state mario_state;

const vec2i_t W16H32D2[] = {
  {0,0},                        // layer #1 (lower body #1)
  {0,0},                        // layer #2 (lower body #2)
  {0,-16},                      // layer #3 (upper body #1)
  {0,-16},                      // layer #4 (upper body #2)
};

const uint8_t mario_pats[] = {
  0,                            // layer #1 (lower body #1)
  4,                            // layer #2 (lower body #2)
  8,                            // layer #3 (upper body #1)
  12,                           // layer #4 (upper body #2)
};

const metasprite_t mario_metasprite = {
  .n = 4,
  .anchor = {0,0},
  .layouts = W16H32D2,
  .pats = mario_pats,
};

static void mario_post_step(entity_t * e);

void mario_init(void) {
  // mario_state.life = 3;
  mario_set_pose(STANDING);
  mario_reset_physical_status();
  mario_reset_ability();

  player->input = 0;
  entity_set_controller(player, joystick1);
  entity_set_post_step(player, mario_post_step);
  entity_set_facing(player, FACING_RIGHT);
  entity_set_metasprite(player, &mario_metasprite);
  // entity_set_sprite_palette(player, MARIO_PALETTE);
  assets_set_sprite_palette(SPRITES_0, 2, MARIO_PALETTE);
  player->plane = 2;

  /* SDCC does not support ISO C99 compound literal */
  player->pos.x.i = 40;
  player->pos.x.d = 0;
  player->pos.y.i = 176;
  player->pos.y.d = 0;
  player->vel.x = 0;
  player->vel.y = 0;
  player->acc.x = 0;
  player->acc.y = 0;

  player->collision = COLLISION_FLOOR;
}

#define MARIO_PAT1_DX  (X_FROM_VMEMPTR(SPRITE_PATTERNS))
#define MARIO_PAT1_DY  (Y_FROM_VMEMPTR(SPRITE_PATTERNS))
#define MARIO_PAT2_DX  (MARIO_PAT1_DX+128)
#define MARIO_PAT2_DY  (MARIO_PAT1_DY)

#define BLANK_PAT_SX   (X_FROM_VMEMPTR(REF_SPRITE_PATTERNS)+128)
#define BLANK_PAT_SY   (Y_FROM_VMEMPTR(REF_SPRITE_PATTERNS)+255)

#define SPRITE_PAT_SX(idx)                                  \
  (X_FROM_VMEMPTR(REF_SPRITE_PATTERNS) + ((idx) & 1) * 128)

#define SPRITE_PAT_SY(idx)                                  \
  (Y_FROM_VMEMPTR(REF_SPRITE_PATTERNS) + ((idx) / 2))

static void mario_set_sprite_pat(uint8_t idx) {
  if (mario_has_super_ability()) {
    // layer #0, #1 (lower body)
    vdp_cmd_execute_HMMM(SPRITE_PAT_SX(idx),
                         SPRITE_PAT_SY(idx),
                         128, 1,
                         MARIO_PAT1_DX,
                         MARIO_PAT1_DY);
    // layer #2, #3 (upper body)
    vdp_cmd_execute_HMMM(SPRITE_PAT_SX(idx),
                         SPRITE_PAT_SY(idx),
                         128, 1,
                         MARIO_PAT2_DX,
                         MARIO_PAT2_DY);
  }
  else {
    // layer #0, #1 (lower body)
    vdp_cmd_execute_HMMM(SPRITE_PAT_SX(idx),
                         SPRITE_PAT_SY(idx),
                         128, 1,
                         MARIO_PAT1_DX,
                         MARIO_PAT1_DY);
    // layer #2, #3 (upper body ; blank)
    vdp_cmd_execute_HMMM(BLANK_PAT_SX,
                         BLANK_PAT_SY,
                         128, 1,
                         MARIO_PAT2_DX,
                         MARIO_PAT2_DY);
  }
}

void mario_show(int x, int y) {
  vmem_set_metasprite_a(SPRITES_0, 2, x, y, &mario_metasprite);
}

static uint8_t anim_tick;

void mario_animate(void) {
  uint8_t idx = mario_state.pose + player->facing;
  if (mario_state.pose != WALKING) {
    mario_set_sprite_pat(idx);
    return;
  }
  /* pose == WALKING */
  anim_tick++;
  if (speed_lo < player_get_speed()) {
    anim_tick++;
  }
  if (6 <= anim_tick) {
    anim_tick = 0;
  }
  const uint8_t t = anim_tick & ~1;
  mario_set_sprite_pat(t + idx);
}

void mario_animate_die(void) {
  mario_set_pose(DEAD);
  entity_set_facing(player, 0);
  player->input = A_BUTTON;
  player->vel.x = 0;
  if (212 - 16 < player->pos.y.i) {
    // falling down
    player->vel.y = f8q8(0.0);
    player->acc.y = gravity_lo;
  }
  else {
    // ouch!
    player->vel.y = f8q8(-20.0);
    player->acc.y = gravity_hi;
  }
  player->vel.x = 0;
  player->acc.x = 0;

  entity_update_sprites();
  await_vsync();
  entity_apply_sprites();
  sleep_millis(500);
  do {
    tick++;
    await_vsync();
    entity_apply_sprites();
    if (tick & 1) continue;
    entity_update_dynamics(player);
    entity_update_sprites();
  } while (player->pos.y.i < 240);
  sleep_millis(100);
}

static void mario_post_step(entity_t * e) {
  // assert(e == player);
  (void)e;
  // estimate mario's pose
  if (player->collision & COLLISION_FLOOR) {
    if (player->vel.y < 0) {
      mario_set_pose(JUMPING);
      sound_effect(&se_jump);
    }
    else if (player_get_speed() == 0) {
      mario_set_pose(STANDING);
    }
    else {
      mario_set_pose(WALKING);
    }
  }
  else {
    if (player->collision & COLLISION_CEIL) {
      uint8_t obj;
      uint8_t col = player->pos.x.i / TILE_WIDTH;
      uint8_t row = player->pos.y.i / TILE_HEIGHT - 1;
      if ((player->c1 < 0x80) ||
          ((player->c2 > 0x7f) && (8 <= (player->pos.x.i & 15)))) {
        obj = player->c2;
        col++;
      }
      else {
        obj = player->c1;
      }
      sound_effect(&se_block);
      switch (obj) {
      case 0xb0:                // '?' block
        entity_add_block(row, col, ITEM_MUSHROOM);
        sound_effect(&se_item);
        break;
      case 0xd1:                // brick #1
      case 0xd2:                // brick #2
        entity_add_brick(row, col, ITEM_NONE);
        break;
      case 0xff:                // hidden
        entity_add_block(row, col, ITEM_1UP_MUSHROOM);
        sound_effect(&se_item);
        break;
      }
    }
  }
  // Mario fell into the valley?
  if (211 < player->pos.y.i) {
    event_set(EV_PLAYER_DIES);
  }
}
