// -*- coding: utf-8-unix -*-

#include <stdlib.h>
#include <msx.h>

#define max(a, b)   ((a) >= (b) ? (a) : (b))

/**
 * Type of 16 bit signed Q6 fixed point number.
 * - integer part : 10 bits
 * - decimal part : 6 bits
 */
typedef int16_t f10q6_t;

/**
 * Constructs a f10q6_t value from integer.
 * \param i   an integer, that shall be 10 bits signed integer.
 */
#define f10q6i(i)    ((f10q6_t)((i) << 6))

/**
 * Type of 22 bit signed fixed point number.
 * - integer part : 16 bits
 * - decimal part : 6 bits
 */
typedef struct f16q6_t {
  int16_t i;                    /* integer part */
  f10q6_t d;                    /* decimal part */
} f16q6_t;

/* defined sprite pattern table */
extern const size_t smb1spt_size;
extern const char smb1spt[];

/* defined sprite color table */
extern const size_t smb1sct_size;
extern const char smb1sct[];

/* defined tileset */
extern const size_t smb1tile_size;
extern const char smb1tile[];

/* stage map */
extern const size_t smb1map_size;
extern const char smb1map[];

#define SCREEN_MODE             VDP_SCREEN_MODE_GRAPHIC_4
#define SCREEN_LINES            VDP_SCREEN_LINES_212
#define IMAGE                   (0x00000)
#define SPRITES                 (0x07600)
#define SPRITE_PATTERNS         (0x07800)
#define SPRITE_COLORS           (SPRITES - 0x0200)

#define PIXELS_PER_BYTE         (2)
#define PIXELS_PER_LINE         (256)
#define LINES_PER_VRAM_PAGE     (256)

#define SIZE_OF_VRAM_PAGE       ((uint32_t)0x08000)
#define SIZE_OF_IMAGE           (0x06A00)
#define SIZE_OF_SPRITE_PATTERNS (256 * 8)
#define SIZE_OF_SPRITE_COLORS   (32 * sizeof(struct sprite_color))
#define SIZE_OF_SPRITES         (32 * sizeof(struct sprite))

#define TILESET                 (IMAGE + 3 * SIZE_OF_VRAM_PAGE)
#define TILESET_COLS            (16)
#define TILESET_ROWS            (8)

#define TILE_WIDTH              (16)
#define TILE_HEIGHT             (16)

#define STAGEMAP_PAGE_COLS      (16)
#define STAGEMAP_PAGE_ROWS      (16)
#define STAGEMAP_VISIBLE_COLS   (16)
#define STAGEMAP_VISIBLE_ROWS   (14)

#define SCROLL_SPEED_MAX (f10q6i(32))

static const palette_t color_palette[16] = {
  RGB(0,0,0), RGB(6,1,0), RGB(3,3,1), RGB(7,5,4),
  RGB(4,6,0), RGB(0,5,0), RGB(1,5,7), RGB(7,4,1),
  RGB(6,2,0), RGB(1,1,1), RGB(7,5,5), RGB(6,2,0),
  RGB(4,5,7), RGB(6,1,0), RGB(7,7,7), RGB(7,5,0),
};

static const palette_t coin_colors[] = {
  RGB(7,4,1), RGB(6,2,0), RGB(3,0,0), RGB(6,2,0),
  RGB(7,4,1), RGB(7,4,1), RGB(7,4,1), RGB(7,4,1),
};

static void init_vdp(void) {
  /* Set backdrop color (border color of the screen) */
  vdp_set_color(0x00);

  vdp_set_visible(false);
  vdp_set_screen_mode(SCREEN_MODE);
  vdp_set_screen_lines(SCREEN_LINES);

  // vdp_set_pattern_table(PATTERNS);
  // vdp_set_color_table(COLORS);

  vdp_set_sprite_visible(false);
  vdp_set_sprite_attribute_table(SPRITES);
  vdp_set_sprite_pattern_table(SPRITE_PATTERNS);
  vdp_set_sprite_size(VDP_SPRITE_SIZE_16x16);

  /* Dual page horizontal scroll mode */
  vdp_set_image_table(IMAGE + SIZE_OF_VRAM_PAGE); /* SET PAGE 1 */
  vdp_set_hscroll_dual_page(true); /* enable 2 pages horizontal scroll */
  vdp_set_hscroll_mask(true);      /* hide ugly left border of 8 pixels */
  vdp_set_adjust(-4, 0);           /* centering visible display area */
}

static void clear_vram(void) {
  /* Fill the pattern name table */
  vdp_cmd_execute_HMMV(0, 0, PIXELS_PER_LINE, 2 * LINES_PER_VRAM_PAGE, 0x00);

  /* Clear sprite pattern table and sprite color table */
  vmem_memset(SPRITE_PATTERNS, 0, SIZE_OF_SPRITE_PATTERNS);
  vmem_memset(SPRITE_COLORS, 0, SIZE_OF_SPRITE_COLORS);

  /* Clear sprites */
  struct sprite s = { .y = 217 };
  for (int i = 0; i < 32; ++i) {
    vmem_write(SPRITES + 4 * i, &s, 4);
  }
}

static void setup_graphics_assets(void) {
  /* Apply palette table to VDP */
  vdp_write_palette(color_palette);

  /* Copy sprite ptterns and sprite colors to VRAM */
  vmem_write(SPRITE_PATTERNS, smb1spt, smb1spt_size);
  vmem_write(SPRITE_COLORS, smb1sct, smb1sct_size);

  /* Copy tileset image to VRAM page #3 */
  vmem_write(TILESET, smb1tile, smb1tile_size);

  /* Draw color palette at bottom of VRAM page #3 */
  /* (debug purpose only) */
  for (int i = 0; i < 16; ++i) {
    vdp_cmd_execute_HMMV(16 * i, 196 + 3 * LINES_PER_VRAM_PAGE, 16, 16, i*17);
  }
}

static void setup_stage_map(void) {
  /* Render the 1st page of the stage map */
  for (int j = 0; j < STAGEMAP_PAGE_COLS; ++j) {
    for (int i = 0; i < STAGEMAP_VISIBLE_ROWS; ++i) {
      uint8_t c = smb1map[STAGEMAP_PAGE_ROWS*j+i];
      uint16_t x1 = (c & 0x0f) << 4;
      uint16_t y1 = (c & 0x70) + 3 * LINES_PER_VRAM_PAGE;
      uint16_t x2 = 16 * j;
      uint16_t y2 = 16 * i;
      vdp_cmd_execute_HMMM(x1, y1, 16, 16, x2, y2);
    }
  }
}

static void show_sprites(void) {
  // Show 16 sprite characters for demo
  struct sprite s = {0};
  for (int i = 0; i < 32; ++i) {
    int x =  8 + 16 * (i / 2 % 4);
    int y = -1 + 16 * (i / 8);
    sprite_set_xy(&s, x, y);
    s.pat = 4 * i;
    vmem_write(SPRITES + 4 * i, &s, sizeof(struct sprite));
  }
  // ---- Mario (player) ---- (#0,#1)
  sprite_set_xy(&s, 124, 175);
  s.pat = 0;
  vmem_write(SPRITES + 0, &s, sizeof(struct sprite));
  s.pat = 4;
  vmem_write(SPRITES + 4, &s, sizeof(struct sprite));
}

/* timer tick counter */
static uint16_t tick;

/* number of columns of stage map */
static uint16_t map_cols;

/* next map column to be written */
static uint16_t map_next;

/* map position (camera position) */
static struct {
  f16q6_t x;
  f10q6_t speed;
} camera;

/* accumulator for cooperative task interleaving */
static uint8_t timeslots_counter;

struct mario_state {
  uint8_t input;
  f10q6_t speed;
  enum facing {
    FACING_LEFT  = 0,
    FACING_RIGHT = 1,
  } facing;
};

static struct mario_state  mario_state = {
  .input = 0,
  .speed = 0,
  .facing = FACING_RIGHT,
};

enum mario_pose {
  STANDING    = 0,
  STANDING_L  = 0,
  STANDING_R  = 1,

  JUMPING     = 2,
  JUMPING_L   = 2,
  JUMPING_R   = 3,

  WALKING_1   = 4,
  WALKING_1_L = 4,
  WALKING_1_R = 5,

  WALKING_2   = 6,
  WALKING_2_L = 6,
  WALKING_2_R = 7,

  WALKING_3   = 8,
  WALKING_3_L = 8,
  WALKING_3_R = 9,

  DEAD        = 10,
};

static void animation_task(void) {
  uint8_t j;

  if (mario_state.speed == 0) {
    vmem_write(SPRITE_PATTERNS+0x000,
               smb1spt + 64 * (STANDING + mario_state.facing),
               64);
  } else {
    const uint8_t b = (mario_state.input & VK_FIRE_1) ? 1 : 2;
    if (!(tick & ((1 << b) - 1))) {
      j = (tick >> b) % 3;
      vmem_write(SPRITE_PATTERNS+0x000,
                 smb1spt + 64 * (2*j + (WALKING_1 + mario_state.facing)),
                 64);
    }
  }

  /* palette animation (coin / ?-block) */
  if (!(tick & 7)) {
    j = (tick >> 3) & 7;
    vdp_set_palette(7, coin_colors[j]);
  }

  // ---------------------------------------
  // sprite animation #1
  {
    j = tick % 3;
    vmem_write(SPRITE_PATTERNS+0x100, smb1spt + 64 * (2*j + WALKING_1_R), 64);
  }
  // sprite animation #2
  if (!(tick & 1)) {
    j = (tick >> 1) % 3;
    vmem_write(SPRITE_PATTERNS+0x140, smb1spt + 64 * (2*j + WALKING_1_R), 64);
  }
  // sprite animation #3
  if (!(tick & 3)) {
    j = (tick >> 2) % 3;
    vmem_write(SPRITE_PATTERNS+0x180, smb1spt + 64 * (2*j + WALKING_1_R), 64);
  }
  // sprite animation #4
  if (!(tick & 7)) {
    j = (tick >> 3) % 3;
    vmem_write(SPRITE_PATTERNS+0x1C0, smb1spt + 64 * (2*j + WALKING_1_R), 64);
  }
}

static void map_renderer_task(void) {
  // if (map_cols <= map_next) return;         /* no more map page */

  // To avoid overflow and to keep invariant,
  // correct camera position and next column to be rendered
  if (camera.x.i >= map_cols * TILE_WIDTH) {
    camera.x.i -= map_cols * TILE_WIDTH;
    map_next -= map_cols;
  }

  if (camera.x.i + 2 * PIXELS_PER_LINE <= TILE_WIDTH * map_next) {
    /* buffer full */
    return;
  }

  const uint16_t col = map_next % map_cols; // wrap around (loop the stage map)
  const uint8_t* p = smb1map + STAGEMAP_PAGE_ROWS * col;
  const uint16_t ix = (TILE_WIDTH * col) & 255;
  const uint16_t pp = (map_next & 0x10) << 4; // page #0 (0) or page #1 (256)

  for (int i = 0; i < STAGEMAP_VISIBLE_ROWS; ++i) {
    const uint16_t iy = TILE_HEIGHT * i + pp;
    // tile := c|y|y|y|x|x|x|x ... 'c': collision bit
    const uint8_t tile = *p++;
    // tile position on VRAM page #3
    const uint16_t x = (tile & 0x0f) << 4;
    const uint16_t y = (tile & 0x70) + 3 * LINES_PER_VRAM_PAGE;
    vdp_cmd_execute_HMMM(x, y, TILE_WIDTH, TILE_HEIGHT, ix, iy);
  }
  map_next++;
}

static void game_core_task(void) {
  // ....
  {
    // TODO
  }
  // update camera position
  {
    camera.x.d += camera.speed;
    camera.x.i += (camera.x.d >> 6);
    camera.x.d &= ((1 << 6) - 1);
  }
  // update mario's state
  {
    const f10q6_t speed_hi = f10q6i(10);
    const f10q6_t speed_lo = f10q6i(6);
    const uint8_t accel = 16;
    const uint8_t brake = 24;

    uint8_t FORWARD_KEY;
    uint8_t BACKWARD_KEY;
    if (mario_state.facing == FACING_LEFT) {
      FORWARD_KEY = VK_LEFT;
      BACKWARD_KEY = VK_RIGHT;
    } else {
      FORWARD_KEY = VK_RIGHT;
      BACKWARD_KEY = VK_LEFT;
    }

    uint8_t joy = joypad_get_state(1);
    if (joy & FORWARD_KEY) {
      if (joy & VK_FIRE_1) {
        mario_state.speed += 2 * accel;
        if (speed_hi <= mario_state.speed) {
          mario_state.speed = speed_hi;
        }
      } else if (mario_state.speed < speed_lo) {
        mario_state.speed += accel;
      } else {
        mario_state.speed -= accel;
      }
    }
    if (joy & BACKWARD_KEY) {
      if (brake < mario_state.speed) {
        mario_state.speed -= brake;
      } else {
        mario_state.speed = 0;
      }
    }
    if (0 < mario_state.speed && !(joy & (VK_LEFT | VK_RIGHT))) {
      mario_state.speed -= accel;
    }

    mario_state.input &= 0x30;
    mario_state.input <<= 2;
    mario_state.input |= joy & 0x3f;
  }
  // update scroll speed
  {
    camera.speed = mario_state.speed;

    if (SCROLL_SPEED_MAX < camera.speed) {
      camera.speed = SCROLL_SPEED_MAX;
    }
  }
}

static void interleave_tasks(void) {
  // Timeslots ratio of two tasks is
  // core task : renderer = TILE_WIDTH : CAMERA.SPEED
  const uint8_t PIX_PER_FRAME = (camera.speed >> 6);
  const uint8_t TIMESLOTS_1 = TILE_WIDTH;
  const uint8_t TIMESLOTS_2 = max(PIX_PER_FRAME, 1);
  const uint8_t margin = 2 * max(TILE_WIDTH, PIX_PER_FRAME);
  while (TIMESLOTS_1 <= timeslots_counter) {
    timeslots_counter -= TIMESLOTS_1;
    while (camera.x.i + PIXELS_PER_LINE + margin > map_next * TILE_WIDTH) {
      map_renderer_task();
    }
  }
  timeslots_counter += TIMESLOTS_2;
  // game_core_task();
}

static volatile __at (0xfc9e) uint16_t JIFFY; // TIME (tick count)

static void play_game(void) {
  setup_stage_map();
  show_sprites();
  vdp_set_sprite_visible(true);
  vdp_set_visible(true);

  map_cols = smb1map_size / STAGEMAP_PAGE_ROWS; /* number of stage map columns */
  map_next = STAGEMAP_VISIBLE_COLS;   /* next stage map column to be rendered */
  camera.x.i = 0;               /* intefer part of camera position */
  camera.x.d = 0;               /* decimal part of camera position */
  camera.speed = 0;             /* scroll speed ; 10.6 fixedpoint [pix] */
  timeslots_counter = 0;     /* accumulator for cooperative task interleaving */

  mario_state.input = 0;
  mario_state.speed = 0;
  mario_state.facing = FACING_RIGHT;

  for (JIFFY = tick = 0; ;) {
    ++tick;
    // wait for VSYNC interrupt and interrupt handler finished
    await_interrupt();
    // ---- sound / visual output task ----
    vdp_set_hscroll(camera.x.i & (2 * PIXELS_PER_LINE - 1));
    animation_task();
    // ---- cooperative task interleaving ----
    interleave_tasks();
    if (tick & 1) {
      game_core_task();
    }
  }
}

void main(void) {
  init_vdp();
  clear_vram();
  setup_graphics_assets();
  play_game();
}
