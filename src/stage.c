// -*- coding: utf-8-unix -*-

#include "smb1.h"

/* number of columns of stage map */
static uint16_t map_cols;

/* next map column to be written */
static uint16_t map_next;

/* state of renderer coroutine (indicates next row of tile to be rendered) */
static uint8_t renderer_state;

/* width of stage map in pixels */
static uint16_t map_width;

void stage_init(void) {
  map_cols = smb1map_size / STAGEMAP_PAGE_ROWS;
  map_next = STAGEMAP_VISIBLE_COLS;
  renderer_state = 0;
  map_width = map_cols * TILE_WIDTH;
}

uint16_t stage_get_columns(void) {
  return map_cols;
}

uint16_t stage_get_width(void) {
  return map_width;
}

uint8_t stage_get_object_at(int x, int y) {
  while (map_width <= x) x -= map_width;
  y /= TILE_HEIGHT;
  if (y < 0) y = 0;
  if (y > 13) y = 13;
  return *(smb1map + (x & 0x0fff0) + y);
}

void stage_setup_map(void) {
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
  vdp_cmd_execute_HMMV(0, 256, 256, 16, 0xcc);
}

void stage_test_and_fix_wraparound(void) {
  if (camera_get_x() >= map_width) {
    mario_set_x(mario_get_x() - map_width);
    camera_set_x(camera_get_x() - map_width);
    map_next -= map_cols;
  }
}


static const uint8_t * p;
static uint16_t ix;
static uint16_t pp;

inline void map_renderer_task_begin(void) {
  const uint16_t col = map_next % map_cols; // wrap around (loop the stage map)
  /* p = smb1map + STAGEMAP_PAGE_ROWS * col; */
  p = smb1map + STAGEMAP_PAGE_ROWS * col + 1;
  ix = (TILE_WIDTH * col) & 255;
  pp = (map_next & 0x10) << 4; // page #0 (0) or page #1 (256)
}

static void map_renderer_task_do(const int row) {
  const uint16_t iy = TILE_HEIGHT * row + pp;
  // tile := c|y|y|y|x|x|x|x ... 'c': collision bit
  const uint8_t tile = *p++;
  // tile position on VRAM page #3
  const uint16_t x = (tile & 0x0f) << 4;
  const uint16_t y = (tile & 0x70) + 3 * LINES_PER_VRAM_PAGE;
  vdp_cmd_execute_HMMM(x, y, TILE_WIDTH, TILE_HEIGHT, ix, iy);
}

inline bool map_renderer_task_is_buffer_full(void) {
  // return ((camera_get_x() + 2 * PIXELS_PER_LINE) / TILE_WIDTH <= map_next);
  return (camera_get_x() / TILE_WIDTH + 2 * STAGEMAP_PAGE_COLS <= map_next);
}

static void map_renderer_task(void) {
  // if (map_cols <= map_next) return;         /* no more map page */

  if (!renderer_state) {
    if (map_renderer_task_is_buffer_full()) {
      return;
    }

    map_renderer_task_begin();
    renderer_state = 1;
  }
  map_renderer_task_do(renderer_state++);
  if (renderer_state >= STAGEMAP_VISIBLE_ROWS) {
    renderer_state = 0;
    map_next++;
  }
}

// #define MAX_PIXELS_PER_FRAME  (4) // highest value of scroll speed
// #define TIMESLOT_1 (MAX_PIXELS_PER_FRAME * (STAGEMAP_VISIBLE_ROWS - 1))
// #define TIMESLOT_2 (TILE_WIDTH)
// #define MIN_TILES_PER_FRAME (TIMESLOT_1 / TIMESLOT_2)     // = 3.25 (@60fps)
// #define MAX_TILES_PER_FRAME (2 * TIMESLOT_1 / TIMESLOT_2) // = 6.50 (@30fps)

void stage_update_map(void) {
  {
    int8_t n = 5;
    while (n--) {
      map_renderer_task();
    }
  }
  // {
  //   // const uint16_t col = (camera_get_x() + PIXELS_PER_LINE) / TILE_WIDTH;
  //   const uint16_t col = camera_get_x() / TILE_WIDTH + STAGEMAP_PAGE_COLS;
  //   while (map_next <= col) {
  //     map_renderer_task();
  //   }
  // }
}
