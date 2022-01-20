// -*- coding: utf-8-unix -*-

#include "smb1.h"

#define SIM_FREQ         (30)
#define TITLE_DURATION   (5 * SIM_FREQ)
#define DEMO_DURATION_1  (8 * SIM_FREQ)
#define DEMO_DURATION_2  (17 * SIM_FREQ)

// short version demo
static uint8_t auto_pilot_1(void) {
  uint8_t ret = 0;
  uint16_t x = mario_get_x();
  ret |= VK_RIGHT;
  if (x < 256+10) return ret;
  if (x < 256+76 && mario_state.speed < f10q6i(1)) return ret;
  if (user_tick < 133) {
    if (0 < mario_state.speed) {
      return 0;
    }
    if (x == 256+80) {
      return VK_FIRE_0;
    }
  }
  if (x < 256+94 && mario_state.speed < f10q6i(1)) return ret;
  ret &= ~VK_RIGHT;
  return ret;
}

// long version demo
static uint8_t auto_pilot_2(void) {
  uint8_t ret = VK_FIRE_1 | VK_RIGHT;
  if (mario_state.collision_state & COLLISION_RIGHT) {
    return ret | VK_FIRE_0;
  }
  if (mario_state.dynamics_state.vel.y < 0) {
    return ret | VK_FIRE_0;
  }
  return ret;
}

typedef bool (* callback_t)(void * arg);

struct scene_conf {
  controller_t controller;      // input controller
  uint16_t duration;            // duration of scene (tick count in 30Hz)
  bool countdown;               // visibility of countdown timer
  callback_t on_after;          // callback called on every after frame
};

static bool demo_canceler(void * arg) {
  (void)arg;
  if (joypad_get_state(1) & VK_FIRE_0) {
    return true;                // break
  }
  return false;                 // continue
}

const struct scene_conf title_scene = {
  .controller = no_controller,
  .duration   = TITLE_DURATION,
  .countdown  = false,
  .on_after   = demo_canceler,
};

const struct scene_conf demo_scenes[] = {
  {
    .controller = auto_pilot_1,
    .duration   = DEMO_DURATION_1,
    .countdown  = false,
    .on_after   = demo_canceler,
  },
  {
    .controller = auto_pilot_2,
    .duration   = DEMO_DURATION_2,
    .countdown  = false,
    .on_after   = demo_canceler,
  },
};

const struct scene_conf game_scene = {
  .controller = joystick1,
  .duration   = 0,              // infinite
  .countdown  = true,
  .on_after   = NULL,
};

static void set_visible(bool visible) {
  vdp_set_visible(visible);
  vdp_set_sprite_visible(visible);
}

static void clear_screen(void) {
  anime_set_enable_on_vsync(false);
  set_hscroll(0);
  await_vsync();
  vdp_cmd_execute_HMMV(0, 0, 256, 212, 0x00);
  graphics_hide_all_sprites();
}

static void show_hud(void) {
  set_foreground_color(14);
  locate(28,0);
  text_puts("MARIO\n"
            "000000");
  locate(92,8);
  text_puts(" x00");
  locate(148,0);
  text_puts("WORLD\n"
            " 1-1");
  locate(204,0);
  text_puts("TIME\n"
            "    ");
  locate(92,8);
  // mini-coin
  set_foreground_color(7);
  text_puts("!");
}

static void get_ready(void) {
  event_init();
  stage_init();
  camera_init();
  mario_init();
  stage_setup_map();
  countdown_timer_set(0x400);
  // ---- hud ----
  set_text_color(14,12);
  show_hud();
  anime_set_enable_on_vsync(true);
}

static const char title_logo[] =
  "efccdfeadf\n"
  "ijccchcbck\n"
  "ghghc gacc\n"
  "\n"
  "elfefdfcef dfdfefef\n"
  "cccccccccc cccccccc\n"
  "cccccckccc ckckccij\n"
  "cccdaccccc cccccccc\n"
  "ccccccccgh chccghghc";

static const uint8_t title_dots[] = {
  0x0ba, 0x0ab,
  0x0ba, 0x0a9,
  0x0bb, 0x099,
};

#define BX (44)
#define BY (16)
#define BW (176)
#define BH (88)

static void draw_title_dot(uint8_t x, uint8_t y) {
  vmemptr_t p = IMAGE+y*128+x/2;
  vmem_write(p, title_dots + 0, 2); p += 128;
  vmem_write(p, title_dots + 2, 2); p += 128;
  vmem_write(p, title_dots + 4, 2);
}

static void draw_title_board_line(uint8_t y, uint8_t cl, uint8_t cc, uint8_t cr) {
  vmemptr_t p = IMAGE+y*128+BX/2;
  vmem_memset(p, cl, 1);      p++;
  vmem_memset(p, cc, BW/2-2); p+=BW/2-2;
  vmem_memset(p, cr, 1);
}

static void draw_title_logo(void) {
  // ---- draw title
  // board
  draw_title_board_line(BY, 0xca, 0xaa, 0xac);
  for (int i = 1; i < BH-1; ++i) {
    draw_title_board_line(BY+i, 0xab, 0xbb, 0xb9);
  }
  draw_title_board_line(BY+BH-1, 0xc9, 0x99, 0x9c);
  // corner dots
  draw_title_dot(BX   +2, BY   +3);
  draw_title_dot(BX+BW-6, BY   +3);
  draw_title_dot(BX   +2, BY+BH-5);
  draw_title_dot(BX+BW-6, BY+BH-5);

  {
    const uint16_t x = VRAM_FREE_AREA_X;
    const uint16_t y = VRAM_FREE_AREA_Y;
    set_text_color(9,11);
    locate(x, y);
    text_puts(title_logo);
    vdp_cmd_execute_LMMM(x,y,20*8,9*8,6*8+5,3*8+4, VDP_CMD_TIMP);
    set_text_color(10,0);
    locate(x,y);
    text_puts(title_logo);
    vdp_cmd_execute_LMMM(x,y,20*8,9*8,6*8+4,3*8, VDP_CMD_TIMP);
  }

  set_text_color(10,12);
  locate(13*8+4,13*8);
  text_puts("@1985 NINTENDO");
  set_text_color(14,12);
  locate(11*8+4,16*8);
  text_puts("1 PLAYER GAME\n"
       "\n"
       "2 PLAYER GAME");
}

static void play_music(void) {
  sound_set_repeat(true);
  sound_set_speed(SOUND_SPEED_1X); // 1.0x
  sound_set_bgm(&bgm_over_world);
  sound_start();                // start BGM
}

bool game_main(void) {
  // wait for VSYNC interrupt and interrupt handler finished
  await_vsync();
  set_hscroll(camera_get_x() & (2 * PIXELS_PER_LINE - 1));
  timer_update();
  // ---- sound / visual output task ----
  anime_update();
  // ---- stage map rendering task ----
  stage_update_map();
  // ---- event dispatch ----
  switch (event_get()) {
  default:;
    // ---- game core task ----
    uint8_t n = user_tick_delta;
    while (n--) {
      // update mario's input
      mario_update_input_state();
      // update mario's state
      mario_move();
      // update camera position and speed
      camera_move();
      // To avoid overflow and to keep invariant,
      // correct camera position and next column to be rendered.
      stage_test_and_fix_wraparound();
      // time
      countdown_timer_update();
    }
    break;
  case EV_PLAYER_DIES:
    sound_set_repeat(false);         // turn off the auto-repeat of the BGM.
    sound_set_speed(SOUND_SPEED_1X); // 1.0x
    sound_set_bgm(&bgm_player_down); // stop the BGM and then replace it.
    sound_start();                   // start the BGM.
    // CUT IN ANIMATION: MARIO DIES
    mario_animate_die();
    mario_died();
    sleep_millis(2000);
    return false;
  }
  // ---- (optional) frame rate / sim.frequency display ----
  if (!(tick & 31)) {
    fps_display_update();
  }
  return true;
}

bool game_main_loop(const struct scene_conf * scene) {
  msx_set_cpu_mode(0x82);     // R800 DRAM mode (if MSXturboR)
  mario_set_controller(scene->controller);
  countdown_timer_set_visible(scene->countdown);
  fps_display_reset();
  timer_reset();
  bool canceled = false;
  for (;;) {
    if (!game_main()) {
      break;                  // (mario died) return to title
    }
    // ----
    if (scene->on_after && scene->on_after(scene)) {
      canceled = true;
      break;
    }
    if (scene->duration && scene->duration <= user_tick) {
      break;
    }
  }
  msx_set_cpu_mode(0x80);     // Z80 mode (if MSXturboR)
  return canceled;
}

void title_demo(void) {
  uint8_t demo_version = 0;
  mario_set_life(3);
  for (;;) {
    // ---- Title screen ----
    set_visible(false);
    clear_screen();
    get_ready();
    draw_title_logo();
    vdp_cmd_await();
    set_visible(true);
    if (game_main_loop(&title_scene)) {
      return;                   // start the game!
    }
    // ---- auto pilot demo ----
    play_music();
    game_main_loop(&demo_scenes[demo_version]);
    sound_stop();
    demo_version = (demo_version + 1) % 2;
  }
}

void level_intro(void) {
  set_visible(false);

  clear_screen();
  vdp_cmd_await();
  set_text_color(14, 0);
  show_hud();
  set_text_color(14, 0);
  locate(11*8+4, 8*8);
  text_puts("WORLD 1-1\n"
       "\n"
       "\n"
       "\n"
       "    x ");
  {
    const uint8_t x = mario_get_life();
    text_putc(x < 10 ? ' ' : ('0' + x / 10 % 10));
    text_putc('0' + mario_get_life() % 10);
  }
  {
    mario_set_pose(STANDING);
    mario_set_facing(FACING_RIGHT);
    mario_animate();
    mario_show(100, 87);
  }

  set_visible(true);
  sleep_millis(3000);
}

static void show_message(const char* msg) {
  set_visible(false);

  clear_screen();
  set_text_color(14, 0);
  show_hud();
  set_text_color(14, 0);
  locate(96,96);
  text_puts(msg);

  set_visible(true);
  sleep_millis(3000);
}

void play_game(void) {
  mario_set_life(3);
  while (!mario_is_over()) {
    level_intro();
    set_visible(false);
    clear_screen();
    get_ready();
    set_visible(true);

    play_music();
    game_main_loop(&game_scene);
    sound_stop();

    if (countdown_is_time_up()) {
      show_message(" TIME UP ");
    }
  }
  show_message("GAME OVER");
}

/** key click beep swith (0:off, non-zero:on) */
static volatile __at (0xf3db) uint8_t CLIKSW;

void main(void) {
  CLIKSW = 0;
  graphics_init_vdp();
  graphics_clear_vram();
  assets_setup();
  timer_init();
  fps_display_set_visible(true);

  sound_init();
  /* Register software envelope patterns */
  sound_set_eg_table(envelope_table);
  /* sound_set_volume(12); */
  setup_interrupt();

  for (;;) {
    title_demo();
    play_game();
  }
}
