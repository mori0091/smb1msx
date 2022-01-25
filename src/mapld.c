// -*- coding: utf-8-unix -*-

#include "smb1.h"

#include <string.h>

#include "map/map_ld.h"

#define PAGES     STAGEMAP_BUFFER_PAGES
#define PAGE_ROWS STAGEMAP_PAGE_ROWS
#define PAGE_COLS STAGEMAP_PAGE_COLS

/* ring buffer of loaded map pages */
uint8_t map_buffer[PAGES][PAGE_COLS][PAGE_ROWS];

/* next page of buffer to be updated */
static uint8_t next_buffer_page;

void mapld_init(void) {
  map_ld_init();
  map_ld_set_bg_theme(0);       // \todo
  map_ld_set_bg_layer(map_bg_layers[0], 0x1800);
  map_ld_set_fg_layer(map_fg_layers[0]);
  //
  next_buffer_page = 0;
  mapld_load_next_page();
  mapld_load_next_page();
}

void mapld_load_next_page(void) {
  // map_ld_load_page(map_buffer[next_buffer_page]);
  map_ld_load_page(&(map_buffer[next_buffer_page][0][1]));
  next_buffer_page++;
  if (PAGES <= next_buffer_page) {
    next_buffer_page = 0;
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
