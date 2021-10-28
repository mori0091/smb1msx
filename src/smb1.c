// -*- coding: utf-8-unix -*-

#include "smb1.h"

static void game_core_task_autopilot(void) {
  if (!(tick & 1)) return;
  // update mario's input
  if (mario_get_x() < 216) {
    mario_state.input = VK_RIGHT;
  }
  else if (mario_get_x() < 256+92 && mario_state.speed < f10q6(1.0)) {
    mario_state.input = VK_RIGHT;
  }
  else {
    mario_state.input = 0;
  }
  // update mario's state
  mario_move();
  // update camera position and speed
  camera_move();
}

static void game_core_task(void) {
  if (!(tick & 1)) return;
  // update mario's input
  mario_update_input_state();
  // update mario's state
  mario_move();
  // update camera position and speed
  camera_move();
}

static void show_fps(void) {
  const uint16_t jiffy = JIFFY;
  if (jiffy < 60) return;
  const uint16_t fps = tick * 60 / jiffy;
  JIFFY = tick = 0;
  struct sprite s = {0};
  sprite_set_xy(&s, 124, 59 - fps);
  s.pat = 8*4;
  vmem_set_sprite(SPRITES, 8, &s);
  s.pat = 9*4;
  vmem_set_sprite(SPRITES, 9, &s);
}

void sleep_ticks(const uint16_t ticks) {
  for (uint16_t i = 0; i < ticks; ++i) {
    await_interrupt();
  }
}

static void show_title_demo(void) {
  for (;;) {
    vdp_set_visible(false);
    vdp_set_sprite_visible(false);

    await_interrupt();
    vdp_set_hscroll(0);
    vdp_cmd_execute_HMMV(0, 0, 256, 212, 0x00);
    {
      anime_clear_sprites();

      event_init();
      stage_init();
      camera_init();
      mario_init();

      stage_setup_map();
      // draw title
      const int x = 45;
      const int y = 17;
      const int w = 174;
      const int h = 86;
      vdp_cmd_execute_LMMV(x  , y  , w, h, 0xbb, VDP_CMD_IMP); // background
      vdp_cmd_execute_LMMV(x  , y-1, w, 1, 0xaa, VDP_CMD_IMP); // top border
      vdp_cmd_execute_LMMV(x  , y+h, w, 1, 0x99, VDP_CMD_IMP); // bottom border
      vdp_cmd_execute_LMMV(x-1, y  , 1, h, 0xaa, VDP_CMD_IMP); // left border
      vdp_cmd_execute_LMMV(x+w, y  , 1, h, 0x99, VDP_CMD_IMP); // right border
    }
    vdp_cmd_await();

    vdp_set_sprite_visible(true);
    vdp_set_visible(true);

    // ---- Title screen ----
    JIFFY = tick = 0;
    while (tick < 300) {
      ++tick;
      // wait for VSYNC interrupt and interrupt handler finished
      await_interrupt();
      // ---- sound / visual output task ----
      // vdp_set_hscroll(camera_get_x() & (2 * PIXELS_PER_LINE - 1));
      anime_update();
      // ----
      uint8_t joy = joypad_get_state(1);
      if (joy & VK_FIRE_0) {
        return;                 // start the game!
      }
    }

    // ---- auto pilot demo ----
    JIFFY = tick = 0;
    while (tick < 600) {
      ++tick;
      // wait for VSYNC interrupt and interrupt handler finished
      await_interrupt();
      // ---- sound / visual output task ----
      vdp_set_hscroll(camera_get_x() & (2 * PIXELS_PER_LINE - 1));
      anime_update();
      // ---- stage map rendering task ----
      stage_update_map();
      // ---- game core task ----
      game_core_task_autopilot();     // autopilot
      // ----
      uint8_t joy = joypad_get_state(1);
      if (joy & VK_FIRE_0) {
        break;                  // return to title
      }
    }
  }
}

static void show_level_intro(void) {
  vdp_set_visible(false);
  vdp_set_sprite_visible(false);

  await_interrupt();
  vdp_set_hscroll(0);
  vdp_cmd_execute_HMMV(0, 0, 256, 212, 0x00);

  vdp_cmd_execute_LMMV(124-36, 104-40, 40, 8, 0xee, VDP_CMD_IMP); // WORLD
  vdp_cmd_execute_LMMV(124+12, 104-40, 8, 8, 0x55, VDP_CMD_IMP); // (major)
  vdp_cmd_execute_LMMV(124+20, 104-40, 8, 8, 0xee, VDP_CMD_IMP); // -
  vdp_cmd_execute_LMMV(124+28, 104-40, 8, 8, 0xdd, VDP_CMD_IMP); // (minor)
  vdp_cmd_execute_LMMV(124-4, 104-8, 8, 8, 0xee, VDP_CMD_IMP); // x
  vdp_cmd_execute_LMMV(124+12, 104-8, 8, 8, 0x55, VDP_CMD_IMP); // 10
  vdp_cmd_execute_LMMV(124+20, 104-8, 8, 8, 0xdd, VDP_CMD_IMP); // 1
  {
    anime_clear_sprites();
    struct sprite s = {0};
    sprite_set_xy(&s, 124-28, 104-16-1);
    s.pat = 0 * 4;
    vmem_set_sprite(SPRITES, 0, &s);
    s.pat = 1 * 4;
    vmem_set_sprite(SPRITES, 1, &s);
    vmem_write(SPRITE_PATTERNS+0x000,
               smb1spt + 64 * (STANDING + FACING_RIGHT),
               64);
  }
  vdp_cmd_await();

  vdp_set_sprite_visible(true);
  vdp_set_visible(true);

  sleep_ticks(180);
}

static void play_game(void) {
  bool restart = true;
  for (;;) {
    if (restart) {
      show_level_intro();
    }
    vdp_set_visible(false);
    vdp_set_sprite_visible(false);

    event_init();
    stage_init();
    camera_init();
    mario_init();

    stage_setup_map();
    // anime_show_sprites();

    vdp_set_sprite_visible(true);
    vdp_set_visible(true);

    restart = false;
    JIFFY = tick = 0;
    while (!restart) {
      ++tick;
      // wait for VSYNC interrupt and interrupt handler finished
      await_interrupt();
      // ---- sound / visual output task ----
      vdp_set_hscroll(camera_get_x() & (2 * PIXELS_PER_LINE - 1));
      anime_update();
      // ---- event dispatch ----
      switch (event_get()) {
      default:
        // ---- stage map rendering task ----
        stage_update_map();
        // ---- game core task ----
        game_core_task();
        break;
      case EV_PLAYER_DIES:
        // CUT IN ANIMATION: MARIO DIES
        mario_animate_die();
        restart = true;
        sleep_ticks(60);
        break;
      }
      // ----
      // show_fps();
    }
  }
}

void main(void) {
  graphics_init_vdp();
  graphics_clear_vram();
  assets_setup();
  for (;;) {
    show_title_demo();
    play_game();
  }
}
