// -*- coding: utf-8-unix -*-

#pragma codeseg BANK2

#include <stdint.h>
#include <string.h>

#include <vdp.h>
#include <vmem.h>

#include "macros.h"

#include "assets/tilemap.h"

static uint8_t h_flip_8bit(uint8_t x) {
  x = ((x << 1) & 0xaa) | ((x >> 1) & 0x55);
  x = ((x << 2) & 0xcc) | ((x >> 2) & 0x33);
  x = ((x << 4) & 0xf0) | ((x >> 4) & 0x0f);
  return x;
}

static void h_flip_8x8_1bpp(const uint8_t src[8], uint8_t dst[8]) {
  uint8_t n = 8;
  while (n--) {
    *dst++ = h_flip_8bit(*src++);
  }
}

static uint8_t tmp[8];

static void v_flip_8x8_1bpp(const uint8_t src[8], uint8_t dst[8]) {
  uint8_t * q = tmp + 7;
  uint8_t n = 8;
  while (n--) {
    *q-- = *src++;
  }
  memcpy(dst, tmp, 8);
}

static void transpose_8x8_1bpp(const uint8_t src[8], uint8_t dst[8]) {
  for (uint8_t i = 0; i < 8; ++i) {
    uint8_t x = src[i];
    uint8_t * q = tmp;
    for (uint8_t j = 0; j < 8; ++j) {
      *q <<= 1;
      if (x & 0x80) {
        *q |= 1;
      }
      q++;
      x <<= 1;
    }
  }
  memcpy(dst, tmp, 8);
}

/**
 * Copy 1 bpp tile pattern of 8x8 pixels to VRAM with or without matrix transformation.
 *
 * `attr` shall be 0 or OR-ed set of the following matrix transformation:
 * - FLIP_HORIZONTAL
 * - FLIP_VERTICAL
 * - TRANSPOSE
 *
 * When two or more matrix transformation is applied, they are applied as following order:
 * ~~~
 * TRANSPOSE >> FLIP_VERTICAL >> FLIP_HORIZONTAL
 * ~~~
 *
 * \param attr  attribute set of matrix transformation
 * \param tile  1bpp tile pattern of 8x8 pixels
 *
 * \note vmem_set_write_address() is required before call to this function.
 */
static void copy_tile_to_vmem(uint8_t attr, const uint8_t tile[8]) {
  static uint8_t buf[8];
  if (attr & TRANSPOSE) {
    transpose_8x8_1bpp(tile, buf);
  }
  else {
    memcpy(buf, tile, 8);
  }
  if (attr & FLIP_VERTICAL) {
    v_flip_8x8_1bpp(buf, buf);
  }
  if (attr & FLIP_HORIZONTAL) {
    h_flip_8x8_1bpp(buf, buf);
  }
  uint8_t * p = buf;
  uint8_t n = 8;
  while (n--) {
    vmem_set(*p++);
  }
}

void tilemap_copy_to_vmem(uint8_t n, const TilemapElement * tilemap,
                          uint8_t m, const Tileset_256x8x8x1bpp ** tileset) {
  const uint8_t * p = (const uint8_t *)tilemap;
  while (n--) {
    const uint8_t idx = *p++;
    const uint8_t attr = *p++;
    for (uint8_t i = 0; i < m; ++i) {
      copy_tile_to_vmem(attr, (*tileset[i])[idx]);
    }
  }
}

void tilemap_copy_to_vmem_hflip(uint8_t n, const TilemapElement * tilemap,
                                uint8_t m, const Tileset_256x8x8x1bpp ** tileset) {
  const uint8_t * p = (const uint8_t *)tilemap;
  while (n--) {
    const uint8_t idx = *p++;
    const uint8_t attr = *p++ ^ FLIP_HORIZONTAL;
    for (uint8_t i = 0; i < m; ++i) {
      copy_tile_to_vmem(attr, (*tileset[i])[idx]);
    }
  }
}

void tilemap_copy_to_vmem_vflip(uint8_t n, const TilemapElement * tilemap,
                                uint8_t m, const Tileset_256x8x8x1bpp ** tileset) {
  const uint8_t * p = (const uint8_t *)tilemap;
  while (n--) {
    const uint8_t idx = *p++;
    const uint8_t attr = *p++ ^ FLIP_VERTICAL;
    for (uint8_t i = 0; i < m; ++i) {
      copy_tile_to_vmem(attr, (*tileset[i])[idx]);
    }
  }
}

void tilemap_copy_to_vmem_4(uint8_t n, const TilemapElement * tilemap,
                            uint8_t m, const Tileset_256x8x8x1bpp ** tileset) {
  const uint8_t * p = (const uint8_t *)tilemap;
  for (uint8_t i = 0; i < n; i += 4) {
    const uint8_t idx0 = *p++;
    const uint8_t attr0 = *p++;
    const uint8_t idx1 = *p++;
    const uint8_t attr1 = *p++;
    const uint8_t idx2 = *p++;
    const uint8_t attr2 = *p++;
    const uint8_t idx3 = *p++;
    const uint8_t attr3 = *p++;
    for (uint8_t j = 0; j < m; ++j) {
      const Tileset_256x8x8x1bpp * ts = tileset[j];
      copy_tile_to_vmem(attr0, (*ts)[idx0]);
      copy_tile_to_vmem(attr1, (*ts)[idx1]);
      copy_tile_to_vmem(attr2, (*ts)[idx2]);
      copy_tile_to_vmem(attr3, (*ts)[idx3]);
    }
  }
}

void tilemap_copy_to_vmem_4_hflip(uint8_t n, const TilemapElement * tilemap,
                                  uint8_t m, const Tileset_256x8x8x1bpp ** tileset) {
  const uint8_t * p = (const uint8_t *)tilemap;
  for (uint8_t i = 0; i < n; i += 4) {
    const uint8_t idx0 = *p++;
    const uint8_t attr0 = *p++ ^ FLIP_HORIZONTAL;
    const uint8_t idx1 = *p++;
    const uint8_t attr1 = *p++ ^ FLIP_HORIZONTAL;
    const uint8_t idx2 = *p++;
    const uint8_t attr2 = *p++ ^ FLIP_HORIZONTAL;
    const uint8_t idx3 = *p++;
    const uint8_t attr3 = *p++ ^ FLIP_HORIZONTAL;
    for (uint8_t j = 0; j < m; ++j) {
      const Tileset_256x8x8x1bpp * ts = tileset[j];
      copy_tile_to_vmem(attr2, (*ts)[idx2]);
      copy_tile_to_vmem(attr3, (*ts)[idx3]);
      copy_tile_to_vmem(attr0, (*ts)[idx0]);
      copy_tile_to_vmem(attr1, (*ts)[idx1]);
    }
  }
}

void tilemap_copy_to_vmem_4_vflip(uint8_t n, const TilemapElement * tilemap,
                                  uint8_t m, const Tileset_256x8x8x1bpp ** tileset) {
  const uint8_t * p = (const uint8_t *)tilemap;
  for (uint8_t i = 0; i < n; i += 4) {
    const uint8_t idx0 = *p++;
    const uint8_t attr0 = *p++ ^ FLIP_VERTICAL;
    const uint8_t idx1 = *p++;
    const uint8_t attr1 = *p++ ^ FLIP_VERTICAL;
    const uint8_t idx2 = *p++;
    const uint8_t attr2 = *p++ ^ FLIP_VERTICAL;
    const uint8_t idx3 = *p++;
    const uint8_t attr3 = *p++ ^ FLIP_VERTICAL;
    for (uint8_t j = 0; j < m; ++j) {
      const Tileset_256x8x8x1bpp * ts = tileset[j];
      copy_tile_to_vmem(attr1, (*ts)[idx1]);
      copy_tile_to_vmem(attr0, (*ts)[idx0]);
      copy_tile_to_vmem(attr3, (*ts)[idx3]);
      copy_tile_to_vmem(attr2, (*ts)[idx2]);
    }
  }
}
