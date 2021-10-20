// -*- coding: utf-8-unix -*-

#include "smb1.h"

static void game_core_task(void) {
  // update mario's state
  mario_move();
  // update camera position
  camera_move();
  // update scroll speed
  camera_set_speed(mario_state.speed);
}

static void play_game(void) {
  stage_init();
  camera_init();
  mario_init();

  stage_setup_map();
  anime_show_sprites();

  vdp_set_sprite_visible(true);
  vdp_set_visible(true);

  JIFFY = tick = 0;
  for (;;) {
    ++tick;
    // wait for VSYNC interrupt and interrupt handler finished
    await_interrupt();
    // ---- sound / visual output task ----
    vdp_set_hscroll(camera_get_x() & (2 * PIXELS_PER_LINE - 1));
    anime_update();
    // ---- stage map rendering task ----
    stage_update_map();
    // ---- game core task ----
    if (tick & 1) continue;
    game_core_task();
  }
}

void main(void) {
  graphics_init_vdp();
  graphics_clear_vram();
  assets_setup();
  play_game();
}
