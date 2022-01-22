// -*- coding: utf-8-unix -*-

#include "smb1.h"

#include <string.h>

/* number of columns of stage map */
static uint16_t map_cols;

/* width of stage map in pixels */
static uint16_t map_width;

/* ring buffer of loaded map pages */
uint8_t map_buffer[STAGEMAP_BUFFER_PAGES][STAGEMAP_PAGE_COLS][STAGEMAP_PAGE_ROWS];

/* next page of buffer to be updated */
static uint8_t next_buffer_page;

/* the 1st column of the next page to be loaded */
static uint16_t next_page_col;

void mapld_init(void) {
  //
  map_cols = smb1map_size / STAGEMAP_PAGE_ROWS;
  map_width = map_cols * TILE_WIDTH;
  //
  next_buffer_page = 0;
  next_page_col = 0;
  mapld_load_next_page();
  mapld_load_next_page();
}

void mapld_load_next_page(void) {
  const uint8_t * src = &(smb1map[STAGEMAP_PAGE_ROWS * next_page_col]);
  uint8_t * dst = &(map_buffer[next_buffer_page][0][0]);
  memcpy(dst, src, STAGEMAP_PAGE_COLS * STAGEMAP_PAGE_ROWS);
  next_buffer_page++;
  if (STAGEMAP_BUFFER_PAGES <= next_buffer_page) {
    next_buffer_page = 0;
  }
  next_page_col += STAGEMAP_PAGE_COLS;
  if (map_cols <= next_page_col) {
    next_page_col = 0;
  }
}

uint8_t* mapld_get_buffer_ptr_at(uint8_t row, uint16_t col) {
  return &(map_buffer[mapld_get_buffer_page_at(col)][col & 15][row]);
}

uint16_t mapld_get_columns(void) {
  return map_cols;
}

uint16_t mapld_get_width(void) {
  return map_width;
}

uint8_t mapld_get_object_at(int x, int y) {
  const uint16_t map_width = mapld_get_width();
  while (map_width <= x) x -= map_width;
  x /= TILE_WIDTH;
  y /= TILE_HEIGHT;
  if (y < 0) y = 0;
  if (y > 13) y = 13;
  return *mapld_get_buffer_ptr_at(y, x);
}
