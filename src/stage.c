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

#define BUFFER_COLS   (STAGEMAP_BUFFER_PAGES * STAGEMAP_PAGE_COLS)
#define BUFFER_WIDTH  (BUFFER_COLS * TILE_WIDTH)

void stage_test_and_fix_wraparound(void) {
  if (camera_get_x() >= BUFFER_WIDTH) {
    player_set_x(player_get_x() - BUFFER_WIDTH);
    camera_set_x(camera_get_x() - BUFFER_WIDTH);
    map_next -= BUFFER_COLS;
  }
}

static const uint8_t * p;
static uint16_t ix;
static uint16_t pp;

inline void map_renderer_task_begin(void) {
  p = mapld_get_buffer_ptr_at(1, map_next);
  ix = (TILE_WIDTH * map_next) & 255;
  pp = (TILE_WIDTH * map_next) & 256; // page #0 (0) or page #1 (256)
  // --
  renderer_state = 1;
}

inline void map_renderer_task_do(void) {
  const uint16_t iy = TILE_HEIGHT * renderer_state + pp;
  // tile := c|y|y|y|x|x|x|x ... 'c': collision bit
  const uint8_t tile = *p++;
  // tile position on VRAM page #3
  const uint16_t x = (tile & 0x0f) << 4;
  const uint16_t y = (tile & 0x70) + 3 * LINES_PER_VRAM_PAGE;
  vdp_cmd_execute_HMMM(x, y, TILE_WIDTH, TILE_HEIGHT, ix, iy);
  // --
  renderer_state++;
}

inline void map_renderer_task_end(void) {
  renderer_state = 0;
  map_next++;
  if (!(map_next & 15)) {
    mapld_load_next_page();
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
    stage_test_and_fix_wraparound();
    map_renderer_task_begin();
  }
  {
    map_renderer_task_do();
  }
  if (renderer_state >= STAGEMAP_VISIBLE_ROWS) {
    map_renderer_task_end();
  }
}

// #define MAX_PIXELS_PER_FRAME  (3) // highest value of scroll speed
// #define TIMESLOT_1 (MAX_PIXELS_PER_FRAME * (STAGEMAP_VISIBLE_ROWS - 1))
// #define TIMESLOT_2 (TILE_WIDTH)
// #define MIN_TILES_PER_FRAME (TIMESLOT_1 / TIMESLOT_2)     // = 39/16 (@60fps)
// #define MAX_TILES_PER_FRAME (2 * TIMESLOT_1 / TIMESLOT_2) // = 39/8 (@30fps)

void stage_update(void) {
  static int16_t timeslot_counter = 0;
  {
    const uint16_t x1 = camera_get_x() + PIXELS_PER_LINE;
    const uint16_t x2 = map_next * TILE_WIDTH;
    if (x2 < x1) {
      timeslot_counter += (x1 - x2) * (STAGEMAP_VISIBLE_ROWS - 1);
    }
  }
  {
#   define VX  (camera.speed >> 7) // @60fps ; Sim.freq = 30Hz
    timeslot_counter += VX * (STAGEMAP_VISIBLE_ROWS - 1);
  }
  while (0 < timeslot_counter) {
    timeslot_counter -= TILE_WIDTH;
    map_renderer_task();
  }
}
