// -*- coding: utf-8-unix -*-

#include "smb1.h"

#define SIM_FREQ         (30)
#define TITLE_DURATION   (5 * SIM_FREQ)

#define MANUAL_PILOT     (mario_update_input_state)
#define NONE_PILOT       (0)

#define AUTO_PILOT_1     (mario_update_input_state_autopilot_1)
#define DEMO_DURATION_1  (8 * SIM_FREQ)

#define AUTO_PILOT_2     (mario_update_input_state_autopilot_2)
#define DEMO_DURATION_2  (17 * SIM_FREQ)

typedef void (*pilot_func)(void);

static pilot_func pilot;

static void set_pilot(pilot_func f) {
  pilot = f;
}

static void run_pilot(void) {
  if (pilot) pilot();
  else {
    mario_state.input = 0;
  }
}

// short version demo
static void mario_update_input_state_autopilot_1(void) {
  mario_backup_input_state();
  uint16_t x = mario_get_x();
  mario_state.input |= VK_RIGHT;
  if (x < 256+10) return;
  if (x < 256+76 && mario_state.speed < f10q6i(1)) return;
  if (user_tick < 133) {
    if (0 < mario_state.speed) {
      mario_state.input = 0;
      return;
    }
    if (x == 256+80) {
      mario_state.input = VK_FIRE_0;
      return;
    }
  }
  if (x < 256+94 && mario_state.speed < f10q6i(1)) return;
  mario_state.input &= ~VK_RIGHT;
}

// long version demo
static void mario_update_input_state_autopilot_2(void) {
  mario_backup_input_state();
  mario_state.input |= VK_FIRE_1 | VK_RIGHT;
  if (mario_state.collision_state & COLLISION_RIGHT) {
    mario_state.input |= VK_FIRE_0;
    return;
  }
  if (mario_state.dynamics_state.vel.y < 0) {
    mario_state.input |= VK_FIRE_0;
    return;
  }
}

const pilot_func AUTO_PILOT[] = {
  AUTO_PILOT_1,
  AUTO_PILOT_2,
};
const uint16_t DEMO_DURATION[] = {
  DEMO_DURATION_1,
  DEMO_DURATION_2,
};

static void set_visible(bool visible) {
  vdp_set_visible(visible);
  vdp_set_sprite_visible(visible);
}

static void clear_screen(void) {
  set_hscroll(0);
  await_hsync();
  /* vdp_set_hscroll(0); */
  vdp_cmd_execute_HMMV(0, 0, 256, 212, 0x00);
  graphics_hide_all_sprites();
  anime_reset_palette();
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
  // countdown_timer_print();
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
  countdown_timer_set(300);
  // ---- hud ----
  set_text_color(14,12);
  show_hud();
}

static bool game_main(void) {
  // wait for VSYNC interrupt and interrupt handler finished
  await_hsync();
  timer_update();
  // ---- (optional) frame rate / sim.frequency display ----
  if (!(tick & 31)) {
    fps_display_update();
  }
  // ---- sound / visual output task ----
  /* vdp_set_hscroll(camera_get_x() & (2 * PIXELS_PER_LINE - 1)); */
  set_hscroll(camera_get_x() & (2 * PIXELS_PER_LINE - 1));
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
      run_pilot();
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
  return true;
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

  set_text_color(9,11);
  locate(0,512);
  text_puts(title_logo);
  vdp_cmd_execute_LMMM(0,512,20*8,9*8,6*8+5,3*8+4, VDP_CMD_TIMP);
  set_text_color(10,0);
  locate(0,512);
  text_puts(title_logo);
  vdp_cmd_execute_LMMM(0,512,20*8,9*8,6*8+4,3*8, VDP_CMD_TIMP);

  set_text_color(10,12);
  locate(13*8+4,13*8);
  text_puts("@1985 NINTENDO");
  set_text_color(14,12);
  locate(11*8+4,16*8);
  text_puts("1 PLAYER GAME\n"
       "\n"
       "2 PLAYER GAME");
}

static void show_title_demo(void) {
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
    // ---- auto pilot demo ----
    msx_set_cpu_mode(0x82);     // R800 DRAM mode (if MSXturboR)
    set_pilot(NONE_PILOT);
    countdown_timer_print();
    fps_display_reset();
    timer_reset();
    while (user_tick < TITLE_DURATION) {
      if (!game_main()) {
        break;                  // (mario died) return to title
      }
      // ----
      if (joypad_get_state(1) & VK_FIRE_0) {
        return;                 // start the game!
      }
    }
    msx_set_cpu_mode(0x80);     // Z80 mode (if MSXturboR)
    // ----
    sound_set_repeat(true);
    sound_set_speed(SOUND_SPEED_1X); // 1.0x
    sound_set_bgm(&bgm_over_world);
    sound_start();              // start BGM

    msx_set_cpu_mode(0x82);     // R800 DRAM mode (if MSXturboR)
    set_pilot(AUTO_PILOT[demo_version]);
    countdown_timer_print();
    fps_display_reset();
    timer_reset();
    while (user_tick < DEMO_DURATION[demo_version]) {
      if (!game_main()) {
        break;                  // (mario died) return to title
      }
      // ----
      if (joypad_get_state(1) & VK_FIRE_0) {
        break;                  // return to title
      }
    }
    msx_set_cpu_mode(0x80);     // Z80 mode (if MSXturboR)
    demo_version ^= 1;

    sound_stop();               // stop BGM
  }
}

static void show_level_intro(void) {
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
    struct sprite s;
    s.x = 124-24;
    s.y = 104-16-1;
    s.pat = 0 * 4;
    graphics_set_sprite(0, &s);
    s.pat = 1 * 4;
    graphics_set_sprite(1, &s);
    graphics_set_sprite_pat(0, smb1spt + 64 * (STANDING + FACING_RIGHT), 64);
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

static void time_up(void) {
  show_message(" TIME UP ");
}

static void game_over(void) {
  show_message("GAME OVER");
}

static void play_game(void) {
  mario_set_life(3);
  while (!mario_is_over()) {
    show_level_intro();
    set_visible(false);
    get_ready();
    set_visible(true);
    set_pilot(MANUAL_PILOT);

    sound_set_repeat(true);
    sound_set_speed(SOUND_SPEED_1X); // 1.0x
    sound_set_bgm(&bgm_over_world);
    sound_start();              // start BGM

    msx_set_cpu_mode(0x82);     // R800 DRAM mode (if MSXturboR)
    countdown_timer_print();
    fps_display_reset();
    timer_reset();
    while (game_main());        // main-loop (until mario die)
    msx_set_cpu_mode(0x80);     // Z80 mode (if MSXturboR)

    sound_stop();
    if (countdown_is_time_up()) {
      time_up();
    }
  }
  // game over
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
    show_title_demo();
    play_game();
    game_over();
  }
}
