// -*- coding: utf-8-unix -*-

#include "map.h"
#include "map_img.h"

#pragma codeseg BANK0

inline
void map_fg_q_block(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  (void)obj;
  canvas[xy] = 0x30 | 0x80;
}

inline
void map_fg_hidden(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  (void)obj;
  canvas[xy] = 0x7f | 0x80;
}

inline
void map_fg_brick(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  (void)obj;
  canvas[xy] = 0x51 | 0x80;
}

inline
void map_fg_h_pipe_head(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  (void)obj;
  uint8_t * p = &(canvas[xy]);
  *p++ = 0x06 | 0x80;
  *p   = 0x16 | 0x80;
}

inline
void map_fg_fixed_block(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  (void)obj;
  canvas[xy] = 0x34 | 0x80;
}

inline
void map_fg_spring_board(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  (void)obj;
  uint8_t * p = &(canvas[xy]);
  *p++ = 0x6a | 0x80;
  *p   = 0x7a | 0x80;
}

// note: transposed
static const uint8_t inv_L_shaped_pipe[] = {
  0x7f, 0x7f, 0x44, 0x54,
  0x7f, 0x7f, 0x45, 0x55,
  0x26, 0x36, 0x46, 0x56,
  0x27, 0x37, 0x37, 0x37,
};

inline
void map_fg_inv_L_shaped_pipe(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  (void)obj;
  copy_rect(&(canvas[xy]), inv_L_shaped_pipe, 4, 4);
}

inline
void map_fg_flag_pole(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  (void)obj;
  uint8_t * p = &(canvas[xy & 0xf0]);
  p[0] = 0x15 /* | 0x80 */ ;
  memset(&p[1], 0x25 /* | 0x80 */, 9);
  p[10] = 0x35 | 0x80;
}

inline
void map_fg_h_coins(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  uint8_t w = SIZED_OBJECT_LEN(obj);
  h_line(&(canvas[xy]), 0x20, w);
}

inline
void map_fg_h_blocks(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  uint8_t w = SIZED_OBJECT_LEN(obj);
  h_line(&(canvas[xy]), (0x35 | 0x80), w);
}

inline
void map_fg_v_blocks(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  uint8_t h = SIZED_OBJECT_LEN(obj);
  v_line(&(canvas[xy]), (0x35 | 0x80), h);
}

inline
void map_fg_h_bricks(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  uint8_t w = SIZED_OBJECT_LEN(obj);
  h_line(&(canvas[xy]), (0x51 | 0x80), w);
}

inline
void map_fg_v_bricks(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  uint8_t h = SIZED_OBJECT_LEN(obj);
  uint8_t * p = &(canvas[xy]);
  p[0] = 0x51 | 0x80;
  v_line(&p[1], (0x52 | 0x80), h);
}

inline
void map_fg_v_pipe(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  uint8_t h = SIZED_OBJECT_LEN(obj);
  if (8 < h) {
    h -= 8;
  }
  uint8_t * p = &(canvas[xy]);
  p[0] = 0x26 | 0x80;
  v_line(&p[1], (0x36 | 0x80), h-1);
  p += ROWS;
  p[0] = 0x27 | 0x80;
  v_line(&p[1], (0x37 | 0x80), h-1);
}

inline
void map_fg_up_stairs(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  uint8_t h = SIZED_OBJECT_LEN(obj);
  uint8_t * p = &(canvas[(xy & 0xf0) + 11]);
  for (uint8_t i = 1; i <= h; ++i) {
    v_line(p - i, (0x35 | 0x80), i);
    p += ROWS;
  }
}

inline
void map_fg_down_stairs(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  uint8_t h = SIZED_OBJECT_LEN(obj);
  uint8_t * p = &(canvas[(xy & 0xf0) + 11]);
  for (uint8_t i = h; i >= 1; --i) {
    v_line(p - i, (0x35 | 0x80), i);
    p += ROWS;
  }
}

// note: transposed (i.e. small_castle[x][y])
static const uint8_t small_castle[] = {
  0x7f, 0x7f, 0x64, 0x52, 0x52,
  0x64, 0x74, 0x66, 0x52, 0x52,
  0x64, 0x52, 0x66, 0x65, 0x75,
  0x64, 0x76, 0x66, 0x52, 0x52,
  0x7f, 0x7f, 0x64, 0x52, 0x52,
};

inline
void map_fg_castle(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  (void)obj;
  copy_rect(&(canvas[(xy & 0xf0) + 6]), small_castle, 5, 5);
}

inline
void map_fg_valley(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  uint8_t w = SIZED_OBJECT_LEN(obj);
  uint8_t * p = &(canvas[(xy & 0xf0) + 8]);
  while (w--) {
    v_line(p, 0x7f, 5);
    p += ROWS;
  }
}

void map_fg(uint8_t xy, uint8_t obj, uint8_t * canvas) {
  // obj &= ~NEWPAGE;
  uint8_t y = ROW(xy);
  const uint8_t id = SIZED_OBJECT_ID(obj);
  if (y < 13) {
    switch (id) {
    case 0:
      switch (obj) {
      case Q_BLOCK_POWERUP:
      case Q_BLOCK_COIN:
        map_fg_q_block(xy, obj, canvas);
        return;
      case HIDDEN_COIN:
      case HIDDEN_1UP:
        map_fg_hidden(xy, obj, canvas);
        return;
      case BRICK_POWERUP:
      case BRICK_VINE_PLANT:
      case BRICK_STARMAN:
      case BRICK_10_COINS:
      case BRICK_1UP:
        map_fg_brick(xy, obj, canvas);
        return;
      case H_PIPE_HEAD:
        map_fg_h_pipe_head(xy, obj, canvas);
        return;
      case FIXED_BLOCK:
        map_fg_fixed_block(xy, obj, canvas);
        return;
      case SPRING_BOARD:
        map_fg_spring_board(xy, obj, canvas);
        return;
      case INV_L_SHAPED_PIPE:
        map_fg_inv_L_shaped_pipe(xy, obj, canvas);
        return;
      case FLAG_POLE:
        map_fg_flag_pole(xy, obj, canvas);
        return;
      default:
        return;
      }
    case 1:
      return;
    case 2:
      map_fg_h_coins(xy, obj, canvas);
      return;
    case 3:
      map_fg_h_blocks(xy, obj, canvas);
      return;
    case 4:
      map_fg_v_blocks(xy, obj, canvas);
      return;
    case 5:
      map_fg_h_bricks(xy, obj, canvas);
      return;
    case 6:
      map_fg_v_bricks(xy, obj, canvas);
      return;
    case 7:
      map_fg_v_pipe(xy, obj, canvas);
      return;
    default:
      return;
    }
  }

  if (y < 14) {
    switch (id) {
    case 1:
      map_fg_up_stairs(xy, obj, canvas);
      return;
    case 2:
      map_fg_down_stairs(xy, obj, canvas);
      return;
    case 3:
      map_fg_castle(xy, obj, canvas);
      return;
    default:
      return;
    }
  }

  if (y < 15) {
    switch (id) {
    case 1:
      map_fg_valley(xy, obj, canvas);
      return;
    default:
      return;
    }
  }
}
