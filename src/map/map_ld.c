// -*- coding: utf-8-unix -*-

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

void map_ld_set_bg_layer(const uint8_t * bg_data, uint16_t block_layout) {
  ctx.bg.head = ctx.bg.ptr = bg_data;
  map_ld_fetch(&ctx.bg);
  for (uint8_t i = 0; i < 13; ++i) {
    ctx.block_layout[i] = (block_layout & 1) ? (0x70 | 0x80) : 0x7f;
    block_layout >>= 1;
  }
}

void map_ld_set_fg_layer(const uint8_t * fg_data) {
  ctx.fg.head = ctx.fg.ptr = fg_data;
  map_ld_fetch(&ctx.fg);
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
    // rewind
    ctx.bg.ptr = ctx.bg.head;
    map_ld_fetch(&ctx.bg);
    // -- optional (loop / sync) --
    if (ctx.fg.cmd[0] == 0xff) {
      // rewind
      ctx.fg.ptr = ctx.fg.head;
      map_ld_fetch(&ctx.fg);
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
