// -*- coding: utf-8-unix -*-

#include "smb1.h"

static void mario_update_input_state_autopilot(void) {
  mario_state.input &= (VK_FIRE_0 | VK_FIRE_1);
  mario_state.input |= (mario_state.input << 2);
  if (mario_get_x() < 200) {
    mario_state.input |= VK_RIGHT;
  }
  else if (mario_get_x() < 256+76 && mario_state.speed < f10q6(1.0)) {
    mario_state.input |= VK_RIGHT;
  }
  else if (tick < 210 && 0 < mario_state.speed) {
    mario_state.input &= ~VK_RIGHT;
  }
  else if (tick < 210 && 0 == mario_state.speed) {
    mario_state.input |= VK_FIRE_0;
  }
  else if (mario_get_x() < 256+92 && mario_state.speed < f10q6(1.0)) {
    mario_state.input |= VK_RIGHT;
  } else {
    mario_state.input &= ~VK_RIGHT;
  }
}

static void game_core_task_autopilot(void) {
  if (!(tick & 1)) return;
  // update mario's input
  mario_update_input_state_autopilot();
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

static void set_visible(bool visible) {
  vdp_set_visible(visible);
  vdp_set_sprite_visible(visible);
}

static void clear_screen(void) {
  await_interrupt();
  vdp_set_hscroll(0);
  vdp_cmd_execute_HMMV(0, 0, 256, 212, 0x00);
  anime_clear_sprites();
}

static void get_ready(void) {
  event_init();
  stage_init();
  camera_init();
  mario_init();
  stage_setup_map();
}

static const char title_logo[] =
  "efccdfeadf\n"
  "ijccchcbck\n"
  "ghghc gacc\n"
  "\n"
  "elfefdfcef dfdfefef\n"
  "cccccccccc cccccccc\n"
  "cccccckccc ckckccij\n"
  "cccdcccccc cccccccc\n"
  "ccccccccgh chccghghc";

#define BX (44)
#define BY (15)
#define BW (176)
#define BH (88)

static void draw_title_logo(void) {
  // ---- draw title
  // top border
  vmem_memset(IMAGE+BY*128+BX/2, 0xaa, BW/2);
  for (int i = 1; i < BH-1; ++i) {
    vmemptr_t p = IMAGE+(BY+i)*128+BX/2;
    vmem_memset(p       , 0xab, 1);      // left border
    vmem_memset(p+1     , 0xbb, BW/2-2); // background
    vmem_memset(p+BW/2-1, 0xb9, 1);      // right border
  }
  // bottom border
  vmem_memset(IMAGE+(BY+BH-1)*128+BX/2, 0x99, BW/2);

  // vdp_cmd_execute_LMMV(0,512,256,212,0xbb, VDP_CMD_IMP);
  set_text_color(9,11);
  locate(0,512);
  puts(title_logo);
  vdp_cmd_execute_LMMM(0,512,20*8,9*8,6*8+5,3*8+4, VDP_CMD_TIMP);
  set_text_color(10,0);
  locate(0,512);
  puts(title_logo);
  vdp_cmd_execute_LMMM(0,512,20*8,9*8,6*8+4,3*8, VDP_CMD_TIMP);

  set_text_color(10,12);
  locate(13*8+4,13*8);
  puts("@1985 NINTENDO");
  set_text_color(14,12);
  locate(11*8+4,16*8);
  puts("1 PLAYER GAME\n"
       "\n"
       "2 PLAYER GAME");
}

static void show_title_demo(void) {
  for (;;) {
    set_visible(false);

    clear_screen();
    get_ready();
    draw_title_logo();

    vdp_cmd_await();

    set_visible(true);

    // ---- Title screen ----
    timer_reset();
    while (tick < 300) {
      timer_update();
      // wait for VSYNC interrupt and interrupt handler finished
      await_interrupt();
      // ---- sound / visual output task ----
      // vdp_set_hscroll(camera_get_x() & (2 * PIXELS_PER_LINE - 1));
      anime_update();
      // ----
      if (joypad_get_state(1) & VK_FIRE_0) {
        return;                 // start the game!
      }
    }

    // ---- auto pilot demo ----
    timer_reset();
    while (tick < 600) {
      timer_update();
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
      if (joypad_get_state(1) & VK_FIRE_0) {
        break;                  // return to title
      }
    }
  }
}

static void show_level_intro(void) {
  set_visible(false);

  clear_screen();

  set_text_color(14,0);
  locate(11*8+4, 8*8); puts("WORLD 1-1");
  locate(15*8+4,12*8); puts("x ");
  putc('0' + mario_get_life() / 10 % 10);
  putc('0' + mario_get_life() % 10);
  {
    struct sprite s = {0};
    sprite_set_xy(&s, 124-24, 104-16-1);
    s.pat = 0 * 4;
    vmem_set_sprite(SPRITES, 0, &s);
    s.pat = 1 * 4;
    vmem_set_sprite(SPRITES, 1, &s);
    vmem_write(SPRITE_PATTERNS+0x000,
               smb1spt + 64 * (STANDING + FACING_RIGHT),
               64);
  }
  vdp_cmd_await();

  set_visible(true);

  sleep_ticks(180);
}

static void play_game(void) {
  while (!mario_is_over()) {
    show_level_intro();

    set_visible(false);

    get_ready();

    // anime_show_sprites();

    set_visible(true);

    bool restart = false;
    timer_reset();
    while (!restart) {
      timer_update();
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
        mario_died();
        restart = true;
        sleep_ticks(60);
        break;
      }
      // ----
    }
  }
  // game over
}

static void game_over(void) {
  set_visible(false);

  clear_screen();
  set_text_color(14, 0);
  locate(96,96);
  puts("GAME OVER");

  set_visible(true);
  sleep_ticks(180);
}

void main(void) {
  graphics_init_vdp();
  graphics_clear_vram();
  assets_setup();
  timer_set_fps_visible(true);
  for (;;) {
    mario_set_life(3);
    show_title_demo();
    play_game();
    game_over();
  }
}
