// -*- coding: utf-8-unix -*-

#include "smb1.h"

/* next map column to be written */
static uint16_t map_next;

/* state of renderer coroutine (indicates next row of tile to be rendered) */
static uint8_t renderer_state;

void stage_init(void) {
  map_next = STAGEMAP_VISIBLE_COLS;
  renderer_state = 0;
}

void stage_setup(void) {
  /* Render the 1st page of the stage map */
  for (int j = 0; j < STAGEMAP_PAGE_COLS; ++j) {
    const uint8_t * p = mapld_get_buffer_ptr_at(0, j);
    for (int i = 0; i < STAGEMAP_VISIBLE_ROWS; ++i) {
      const uint8_t c = *p++;
      uint16_t x1 = (c & 0x0f) << 4;
      uint16_t y1 = (c & 0x70) + 3 * LINES_PER_VRAM_PAGE;
      uint16_t x2 = 16 * j;
      uint16_t y2 = 16 * i;
      vdp_cmd_execute_HMMM(x1, y1, 16, 16, x2, y2);
    }
  }
  vdp_cmd_execute_HMMV(0,   0, 256, 16, 0xcc);
  vdp_cmd_execute_HMMV(0, 256, 256, 16, 0xcc);
}

#define MAX_PAGES_TO_WRAPAROUND    (32)
#define MAX_COLUMNS_TO_WRAPAROUND  (MAX_PAGES_TO_WRAPAROUND * STAGEMAP_PAGE_COLS)
#define MAX_PIXELS_TO_WRAPAROUND   (MAX_COLUMNS_TO_WRAPAROUND * TILE_WIDTH)

static void stage_test_and_fix_wraparound(void) {
  if (camera_get_x() >= MAX_PIXELS_TO_WRAPAROUND) {
    player_set_x(player_get_x() - MAX_PIXELS_TO_WRAPAROUND);
    camera_set_x(camera_get_x() - MAX_PIXELS_TO_WRAPAROUND);
    map_next -= MAX_COLUMNS_TO_WRAPAROUND;
  }
}

static const uint8_t * p;
static uint16_t pp;
static uint16_t ix;
static uint16_t iy;

inline void map_renderer_task_begin(void) {
  p = mapld_get_buffer_ptr_at(1, map_next);
  pp = (TILE_WIDTH * map_next) & 256; // page #0 (0) or page #1 (256)
  ix = (TILE_WIDTH * map_next) & 255;
  iy = TILE_HEIGHT + pp;
  // --
  renderer_state = 1;
}

inline void map_renderer_task_do(void) {
  // tile := c|y|y|y|x|x|x|x ... 'c': collision bit
  const uint8_t tile = *p++;
  // tile position on VRAM page #3
  const uint16_t x = (tile & 0x0f) << 4;
  const uint16_t y = (tile & 0x70) + 3 * LINES_PER_VRAM_PAGE;
  vdp_cmd_execute_HMMM(x, y, TILE_WIDTH, TILE_HEIGHT, ix, iy);
  // --
  renderer_state++;
  iy += TILE_HEIGHT;
}

inline void map_renderer_task_end(void) {
  renderer_state = 0;
  map_next++;
  if (!(map_next % STAGEMAP_PAGE_COLS)) {
    mapld_load_next_page();
    stage_test_and_fix_wraparound();
  }
}

inline bool map_renderer_task_is_buffer_full(void) {
  // return ((camera_get_x() + 2 * PIXELS_PER_LINE) / TILE_WIDTH <= map_next);
  return (camera_get_x() / TILE_WIDTH + 2 * STAGEMAP_PAGE_COLS <= map_next);
}

inline void map_renderer_task(void) {
  if (!renderer_state) {
    if (map_renderer_task_is_buffer_full()) {
      return;
    }
    map_renderer_task_begin();
  }
  {
    map_renderer_task_do();
  }
  if (renderer_state >= STAGEMAP_VISIBLE_ROWS) {
    map_renderer_task_end();
  }
}

// Sim.freq = 30Hz @ 60fps
// Sim.freq = 25Hz @ 50fps
#define PIXELS_PER_FRAME  (camera_get_speed() >> 7)
// Sim.freq = 30Hz @ 50fps
// #define PIXELS_PER_FRAME  ((camera_get_speed() * 30 / 50) >> 6)
// Sim.freq = 25Hz @ 60fps
// #define PIXELS_PER_FRAME  ((camera_get_speed() * 25 / 60) >> 6)

#define ROWS              (STAGEMAP_VISIBLE_ROWS - 1)

void stage_update(void) {
  static int16_t timeslot_counter = 0;
  {
    const uint16_t x1 = camera_get_x() + PIXELS_PER_LINE;
    const uint16_t x2 = map_next * TILE_WIDTH;
    if (x2 < x1) {
      timeslot_counter += (x1 - x2) * ROWS;
    }
  }
  {
    timeslot_counter += PIXELS_PER_FRAME * ROWS;
  }
  while (0 < timeslot_counter) {
    timeslot_counter -= TILE_WIDTH;
    map_renderer_task();
  }
}
