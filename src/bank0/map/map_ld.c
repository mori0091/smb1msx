// -*- coding: utf-8-unix -*-

#pragma codeseg BANK0

#include "map_ld.h"
#include "map_img.h"

static struct map_ctx ctx;

void map_ld_init(void) {
  ctx.page = 0;
}

void map_ld_set_bg_theme(uint8_t theme) {
  ctx.bg_theme = theme;
}

static void map_ld_fetch(struct map_ptr * mp) {
  mp->cmd[0] = *mp->ptr++;
  mp->cmd[1] = *mp->ptr++;
}

static void map_ld_rewind(struct map_ptr * mp) {
  mp->ptr = mp->head;
  map_ld_fetch(mp);
}

void map_ld_set_bg_layer(const uint8_t * bg_data, uint16_t block_layout) {
  ctx.bg.head = bg_data;
  map_ld_rewind(&ctx.bg);
  for (uint8_t i = 0; i < 13; ++i) {
    ctx.block_layout[i] = (block_layout & 1) ? (0x70 | 0x80) : 0x7f;
    block_layout >>= 1;
  }
}

void map_ld_set_fg_layer(const uint8_t * fg_data) {
  ctx.fg.head = fg_data;
  map_ld_rewind(&ctx.fg);
}

inline
void map_ld_load_bg_page(uint8_t * canvas) {
  for (;;) {
    if (ctx.bg.cmd[0] == 0xff) {
      return;
    }
    if (ctx.bg.cmd[1] & NEWPAGE) {
      ctx.bg.cmd[1] &= ~NEWPAGE;
      return;
    }
    map_bg(ctx.bg.cmd[0], ctx.bg.cmd[1], canvas);
    map_ld_fetch(&ctx.bg);
  }
}

inline
void map_ld_load_fg_page(uint8_t * canvas) {
  for (;;) {
    if (ctx.fg.cmd[0] == 0xff) {
      return;
    }
    if (ctx.fg.cmd[1] & NEWPAGE) {
      ctx.fg.cmd[1] &= ~NEWPAGE;
      return;
    }
    map_fg(ctx.fg.cmd[0], ctx.fg.cmd[1], canvas);
    map_ld_fetch(&ctx.fg);
  }
}

void map_ld_load_page(uint8_t * canvas) {
  if (ctx.bg.cmd[0] == 0xff) {
    map_ld_rewind(&ctx.bg);
    // -- optional (loop / sync) --
    if (32 <= ctx.page && ctx.fg.cmd[0] == 0xff) {
      map_ld_rewind(&ctx.fg);
      ctx.page = 0;
    }
  }
  uint8_t * p = ((uint8_t *)canvas);
  for (uint8_t j = 0; j < 16; ++j) {
    memcpy(p, ctx.block_layout, 13);
    p += ROWS;
  }
  map_ld_load_bg_page(canvas);
  map_ld_load_fg_page(canvas);
  ctx.page++;
}
