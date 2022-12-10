// -*- coding: utf-8-unix -*-

#pragma codeseg BANK1

#include <string.h>

#include "smb1.h"
#include "map_data.h"

#include "map/map_ld.h"

#define PAGES     STAGEMAP_BUFFER_PAGES
#define PAGE_ROWS STAGEMAP_PAGE_ROWS
#define PAGE_COLS STAGEMAP_PAGE_COLS

/* ring buffer of loaded map pages */
uint8_t map_buffer[PAGES][PAGE_COLS][PAGE_ROWS];
/* ring buffer of loaded map pages (object attributes) */
uint8_t obj_buffer[PAGES][PAGE_COLS][PAGE_ROWS];

/* temporal page buffer */
uint8_t page[PAGE_COLS][PAGE_ROWS];
/* temporal page buffer (object attributes) */
uint8_t obj_page[PAGE_COLS][PAGE_ROWS];

/* next page of buffer to be updated */
static uint8_t next_buffer_page;
static bool odd_half;

void map_init(void) {
  map_ld_init();
  map_ld_set_bg_theme(0);       // \todo
  map_ld_set_bg_layer(map_bg_layers[0], 0x1800);
  map_ld_set_fg_layer(map_fg_layers[0]);
  //
  next_buffer_page = 0;
  odd_half = false;
  memset(map_buffer, 0, sizeof(map_buffer));
  memset(obj_buffer, 0, sizeof(obj_buffer));
  map_load_next_half_page();
}

#define HALF_PAGE_COLS  (PAGE_COLS / 2)
#define HALF_PAGE_SIZE  (PAGE_ROWS * PAGE_COLS / 2)

void map_load_next_half_page(void) {
  if (odd_half) {
    memcpy(map_buffer[next_buffer_page][HALF_PAGE_COLS],
           page[HALF_PAGE_COLS], HALF_PAGE_SIZE);
    memcpy(obj_buffer[next_buffer_page][HALF_PAGE_COLS],
           obj_page[HALF_PAGE_COLS], HALF_PAGE_SIZE);
    odd_half = false;
    next_buffer_page ^= 1;      // \note PAGES == 2
  }
  else {
    memset(obj_page, 0, sizeof(obj_page));
    map_ld_load_page(&(page[0][1]), &(obj_page[0][1]));
    memcpy(map_buffer[next_buffer_page],
           page, HALF_PAGE_SIZE);
    memcpy(obj_buffer[next_buffer_page],
           obj_page, HALF_PAGE_SIZE);
    odd_half = true;
  }
}

uint8_t* map_get_buffer_ptr_at(uint8_t row, uint16_t col) {
  const uint16_t idx = ((col & 0x1f) << 4) + (row & 0x0f);
  return (uint8_t *)map_buffer + idx;
}

void map_put_tile(uint8_t row, uint8_t col, uint8_t tile) {
  const uint16_t idx = ((col & 0x1f) << 4) + (row & 0x0f);
  *((uint8_t *)map_buffer + idx) = tile;
}

void map_consume_item(uint8_t row, uint8_t col) {
  const uint16_t idx = ((col & 0x1f) << 4) + (row & 0x0f);
  uint8_t * p = (uint8_t *)obj_buffer + idx;
  if (*p == OBJ_10_COINS) {
    if (!(--coin_10x_counter)) {
      *p = OBJ_COIN;
    }
  }
  else {
    *p = OBJ_EMPTY;
  }
}

uint8_t map_get_object_at(int x, int y) {
  if (y < 0 || 211 < y) {
    return 0;
  }
  const uint16_t idx = (x & 0x1f0) + ((y & 0xf0) >> 4);
  return *((uint8_t *)map_buffer + idx);
}

uint8_t map_get_attribute_at(int x, int y) {
  if (y < 0 || 211 < y) {
    return OBJ_EMPTY;
  }
  const uint16_t idx = (x & 0x1f0) + ((y & 0xf0) >> 4);
  return *((uint8_t *)obj_buffer + idx);
}
