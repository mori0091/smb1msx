// -*- coding: utf-8-unix -*-

#include "smb1.h"

/* number of columns of stage map */
static uint16_t map_cols;

/* next map column to be written */
static uint16_t map_next;

/* state of renderer coroutine (indicates next row of tile to be rendered) */
static uint8_t renderer_state;

void stage_init(void) {
  map_cols = smb1map_size / STAGEMAP_PAGE_ROWS;
  map_next = STAGEMAP_VISIBLE_COLS;
  renderer_state = 0;
}

uint16_t stage_get_columns(void) {
  return map_cols;
}

uint16_t stage_get_width(void) {
  return map_cols * TILE_WIDTH;
}

uint8_t stage_get_object_at(int x, int y) {
  const uint16_t w = stage_get_width();
  while (w <= x) x -= w;
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

static void map_renderer_task(void) {
  // if (map_cols <= map_next) return;         /* no more map page */

  if (!renderer_state) {
    // To avoid overflow and to keep invariant,
    // correct camera position and next column to be rendered
    if (camera_get_x() >= map_cols * TILE_WIDTH) {
      mario_set_x(mario_get_x() - map_cols * TILE_WIDTH);
      camera_set_x(camera_get_x() - map_cols * TILE_WIDTH);
      map_next -= map_cols;
    }

    if ((camera_get_x() + 2 * PIXELS_PER_LINE) / TILE_WIDTH <= map_next) {
      /* buffer full */
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

#define MARGIN      TILE_WIDTH
void stage_update_map(void) {
  while (map_next * TILE_WIDTH < camera_get_x() + PIXELS_PER_LINE + MARGIN) {
    map_renderer_task();
    map_renderer_task();
  }
}
