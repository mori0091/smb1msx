// -*- coding: utf-8-unix -*-

#ifndef MACROS_H
#define MACROS_H

#pragma once

#define max(a, b)               ((a) >= (b) ? (a) : (b))

// ---- configurations of GRAPHIC 4 (SCREEN 5)
#define SCREEN_MODE             VDP_SCREEN_MODE_GRAPHIC_4
#define SCREEN_LINES            VDP_SCREEN_LINES_212
#define IMAGE                   (0x00000)
#define SPRITE_PATTERNS         (0x07800)
#define SPRITES                 (0x07600)
#define SPRITE_COLORS           (SPRITES - 0x0200)

#define SPRITES_0               (0x07600)
#define SPRITES_COLORS_0        (SPRITES_0 - 0x0200)
#define SPRITES_1               (0x07200)
#define SPRITES_COLORS_1        (SPRITES_1 - 0x0200)

#define PIXELS_PER_BYTE         (2)
#define PIXELS_PER_LINE         (256)
#define LINES_PER_VRAM_PAGE     (256)

#define SIZE_OF_VRAM_PAGE       ((uint32_t)0x08000)
#define SIZE_OF_IMAGE           (0x06A00)
#define SIZE_OF_SPRITE_PATTERNS (256 * 8)
#define SIZE_OF_SPRITE_COLORS   (32 * sizeof(struct sprite_color))
#define SIZE_OF_SPRITES         (32 * sizeof(struct sprite))

#define X_FROM_VMEMPTR(vptr)    (((vptr) & 0x7f) << 1)
#define Y_FROM_VMEMPTR(vptr)    ((vptr) >> 7)

// ---- for SMB1
#define REF_SPRITE_PATTERNS          (2 * SIZE_OF_VRAM_PAGE)
#define SIZE_OF_REF_SPRITE_PATTERNS  (SIZE_OF_VRAM_PAGE)

#define TILESET                 (3 * SIZE_OF_VRAM_PAGE)
#define SIZE_OF_TILESET         (SIZE_OF_VRAM_PAGE / 2)

#define TILESET_COLS            (16)
#define TILESET_ROWS            (8)

#define TILE_WIDTH              (16)
#define TILE_HEIGHT             (16)

#define STAGEMAP_PAGE_COLS      (16)
#define STAGEMAP_PAGE_ROWS      (16)
#define STAGEMAP_VISIBLE_COLS   (16)
#define STAGEMAP_VISIBLE_ROWS   (14)

#define STAGEMAP_BUFFER_PAGES   (2)

#define VRAM_FREE_AREA          (TILESET + SIZE_OF_TILESET)
#define VRAM_FREE_AREA_X        X_FROM_VMEMPTR(VRAM_FREE_AREA)
#define VRAM_FREE_AREA_Y        Y_FROM_VMEMPTR(VRAM_FREE_AREA)
#define SIZE_OF_VRAM_FREE_AREA  (SIZE_OF_VRAM_PAGE / 2)

#define SCROLL_SPEED_MAX (f10q6i(112))



#endif
