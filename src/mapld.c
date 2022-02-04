// -*- coding: utf-8-unix -*-

#include "smb1.h"

#include <string.h>

#include "map/map_ld.h"

#define PAGES     STAGEMAP_BUFFER_PAGES
#define PAGE_ROWS STAGEMAP_PAGE_ROWS
#define PAGE_COLS STAGEMAP_PAGE_COLS

/* ring buffer of loaded map pages */
uint8_t map_buffer[PAGES][PAGE_COLS][PAGE_ROWS];

/* temporal page buffer */
uint8_t page[PAGE_COLS][PAGE_ROWS];

/* next page of buffer to be updated */
static uint8_t next_buffer_page;
static bool odd_half;

void mapld_init(void) {
  map_ld_init();
  map_ld_set_bg_theme(0);       // \todo
  map_ld_set_bg_layer(map_bg_layers[0], 0x1800);
  map_ld_set_fg_layer(map_fg_layers[0]);
  //
  next_buffer_page = 0;
  odd_half = false;
  memset(map_buffer, 0, sizeof(map_buffer));
  mapld_load_next_half_page();
  mapld_load_next_half_page();
  mapld_load_next_half_page();
}

#define HALF_PAGE_COLS  (PAGE_COLS / 2)
#define HALF_PAGE_SIZE  (PAGE_ROWS * PAGE_COLS / 2)

void mapld_load_next_half_page(void) {
  if (odd_half) {
    memcpy(map_buffer[next_buffer_page][HALF_PAGE_COLS],
           page[HALF_PAGE_COLS], HALF_PAGE_SIZE);
    odd_half = false;
    next_buffer_page ^= 1;      // \note PAGES == 2
  }
  else {
    map_ld_load_page(&(page[0][1]));
    memcpy(map_buffer[next_buffer_page],
           page, HALF_PAGE_SIZE);
    odd_half = true;
  }
}

uint8_t* mapld_get_buffer_ptr_at(uint8_t row, uint16_t col) {
  return &(map_buffer[mapld_get_buffer_page_at(col)][col & 15][row]);
}

uint8_t mapld_get_object_at(int x, int y) {
  x /= TILE_WIDTH;
  y /= TILE_HEIGHT;
  if (y < 0) y = 0;
  if (y > 13) y = 13;
  return *mapld_get_buffer_ptr_at(y, x);
}
