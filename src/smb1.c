// -*- coding: utf-8-unix -*-

#include <stdlib.h>
#include <msx.h>

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

/* #define SCREEN_MODE     VDP_SCREEN_MODE_GRAPHIC_1 */
/* #define SCREEN_LINES    VDP_SCREEN_LINES_192 */
/* #define PATTERNS        (0x00000) */
/* #define IMAGE           (0x01800) */
/* #define SPRITES         (0x01B00) */
/* #define COLORS          (0x02000) */
/* #define SPRITE_PATTERNS (0x03800) */

/* #define SIZE_OF_IMAGE    (0x0300) */
/* #define SIZE_OF_PATTERNS (0x0800) */
/* #define SIZE_OF_COLORS  (32) */

/* #define SCREEN_MODE     VDP_SCREEN_MODE_GRAPHIC_3 */
/* #define SCREEN_LINES    VDP_SCREEN_LINES_192 */
/* #define PATTERNS        (0x00000) */
/* #define IMAGE           (0x01800) */
/* #define SPRITES         (0x01E00) */
/* #define COLORS          (0x02000) */
/* #define SPRITE_PATTERNS (0x03800) */
/* #define SPRITE_COLORS   (SPRITES - 0x0200) */

/* #define SIZE_OF_IMAGE    (0x0300) */
/* #define SIZE_OF_PATTERNS (0x1800) */
/* #define SIZE_OF_COLORS   (0x1800) */

/* #define SCREEN_MODE     VDP_SCREEN_MODE_GRAPHIC_3 */
/* #define SCREEN_LINES    VDP_SCREEN_LINES_212 */
/* #define PATTERNS        (0x00000) */
/* #define COLORS          (0x02000) */
/* #define IMAGE           (0x04000) */
/* #define SPRITES         (0x04600) */
/* #define SPRITE_PATTERNS (0x04800) */
/* #define SPRITE_COLORS   (SPRITES - 0x0200) */

/* #define SIZE_OF_IMAGE    (0x0400) */
/* #define SIZE_OF_PATTERNS (0x2000) */
/* #define SIZE_OF_COLORS   (0x2000) */

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
  /* Show 16 sprite characters for demo */
  struct sprite s = {0};
  for (int i = 0; i < 32; ++i) {
    int x =  96 + 16 * (i / 8) + 4;
    int y = 127 + 16 * (i / 2 % 4);
    sprite_set_xy(&s, x, y);
    s.pat = 4 * i;
    vmem_write(SPRITES + 4 * i, &s, 4);
  }
}

/* timer tick counter */
static uint16_t tick;

/* number of columns of stage map */
static uint16_t WD;

/* next map column to be written */
static uint16_t WX;

/* map position (camera position) */
static uint16_t SX;             /* integer part of camera position */
static uint16_t SXd;            /* decimal part of camera position */
static uint16_t scroll_speed = 0; /* scroll speed ; 10.6 fixedpoint [pix] */
static const uint16_t SCROLL_SPEED_MAX = (10 << 6); /* 10.6 fixedpoint [pix] */

/* accumulator for cooperative task interleaving */
static uint8_t timeslots_counter;

// Time sharing scheduling ; core task : renderer = 16:10
// Because TILE_WIDTH = 16 and SCROLL_SPEED_MAX = 10
static const uint8_t TIMESLOTS_FOR_CORE_TASK = 16; /* regular task */
static const uint8_t TIMESLOTS_FOR_RENDERER  = 10; /* interleaved task */

static void animation_task(void) {
  uint8_t j;
  /* sprite animation #1 */
  {
    j = tick % 3;
    vmem_write(SPRITE_PATTERNS+0x000, smb1spt + 64 * (2 * j + 5), 64);
  }
  /* sprite animation #2 */
  if (!(tick & 1)) {
    j = (tick >> 1) % 3;
    vmem_write(SPRITE_PATTERNS+0x100, smb1spt + 64 * (2 * j + 5), 64);
  }
  /* sprite animation #3 */
  if (!(tick & 3)) {
    j = (tick >> 2) % 3;
    vmem_write(SPRITE_PATTERNS+0x200, smb1spt + 64 * (2 * j + 5), 64);
  }
  /* palette animation (coin / ?-block) */
  if (!(tick & 7)) {
    j = (tick >> 3) & 7;
    vdp_set_palette(7, coin_colors[j]);
  }
}

static void map_renderer_task(void) {
  // if (WD <= WX) return;         /* no more map page */

  // To avoid overflow and to keep invariant,
  // correct SX (camera position) and WX (next column to be rendered)
  if (SX >= WD * TILE_WIDTH) {
    SX -= WD * TILE_WIDTH;
    WX -= WD;
  }

  if (SX + 2 * PIXELS_PER_LINE <= TILE_WIDTH * WX) return; /* buffer full */

  const uint16_t next_column = WX % WD; /* wrap around (loop the stage map) */
  const uint8_t* p = smb1map + STAGEMAP_PAGE_ROWS * next_column;
  const uint16_t ix = (TILE_WIDTH * next_column) & 255;
  const uint16_t pp = (WX & 0x10) << 4; /* page #0 (0) or page #1 (256) */

  for (int i = 0; i < STAGEMAP_VISIBLE_ROWS; ++i) {
    const uint16_t iy = TILE_HEIGHT * i + pp;
    // tile := c|y|y|y|x|x|x|x ... 'c': collision bit
    const uint8_t tile = *p++;
    // tile position on VRAM page #3
    const uint16_t x = (tile & 0x0f) << 4;
    const uint16_t y = (tile & 0x70) + 3 * LINES_PER_VRAM_PAGE;
    vdp_cmd_execute_HMMM(x, y, TILE_WIDTH, TILE_HEIGHT, ix, iy);
  }
  WX++;
}

static void game_core_task(void) {
  // ....
  {
    // TODO
  }
  // update camera position
  {
    SXd += scroll_speed;
    SX  += (SXd >> 6);
    SXd &= ((1 << 6) - 1);
  }
  // update scroll speed
  {
    uint8_t joy = joypad_get_state(1);
    if (joy & VK_RIGHT) {
      scroll_speed++;
    }
    if (joy & VK_LEFT) {
      if (2 < scroll_speed) {
        scroll_speed -= 2;
      } else {
        scroll_speed = 0;
      }
    }
    if (0 < scroll_speed && !(joy & (VK_LEFT | VK_RIGHT))) {
      scroll_speed--;
    }
    if (scroll_speed > SCROLL_SPEED_MAX) {
      scroll_speed = SCROLL_SPEED_MAX;
    }
  }
}

static void interleave_tasks(void) {
  while (TIMESLOTS_FOR_CORE_TASK <= timeslots_counter) {
    timeslots_counter -= TIMESLOTS_FOR_CORE_TASK;
    if (SX + PIXELS_PER_LINE + 32 > WX * TILE_WIDTH) {
      map_renderer_task();
    }
  }
  timeslots_counter += TIMESLOTS_FOR_RENDERER;
  game_core_task();
}

static void play_game(void) {
  setup_stage_map();
  show_sprites();
  vdp_set_sprite_visible(true);
  vdp_set_visible(true);

  WD = smb1map_size / STAGEMAP_PAGE_ROWS; /* number of stage map columns */
  WX = STAGEMAP_VISIBLE_COLS;   /* next stage map column to be rendered */
  SX = 0;                       /* integer part of camera position */
  SXd = 0;                      /* decimal part of camera position */
  scroll_speed = 0;             /* scroll speed ; 10.6 fixedpoint [pix] */
  timeslots_counter = 0;     /* accumulator for cooperative task interleaving */

  for (tick = 0; ; ++tick) {
    // wait for VSYNC interrupt and interrupt handler finished
    await_interrupt();
    // ---- sound / visual output task ----
    vdp_set_hscroll(SX & (2 * PIXELS_PER_LINE - 1));
    animation_task();
    // ---- cooperative task interleaving ----
    interleave_tasks();
  }
}

void main(void) {
  init_vdp();
  clear_vram();
  setup_graphics_assets();
  play_game();
}
