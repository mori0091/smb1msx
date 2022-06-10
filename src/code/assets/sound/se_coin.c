// -*- coding: utf-8-unix -*-

#include <sound.h>

// tick = 60Hz * 60s * 4 / (Tempo * L)
//      = 14400 / (Tempo * L)

// "T225 O4 V15 @1 >B32. >E2"
static const uint8_t intro_ch1[] = {  // T225 V15 @1
  0x80, 0x03, 0x8f, 0xc1, 0x00, 0x71, // O5 B32.
  0x40, 0x20, 0x00, 0x55,             // O6 E2

  0xff,
};

static const struct sound_fragment intro = {
  .streams = {
    [0] = 0,
    [1] = intro_ch1,
    [2] = 0,
  },
};

static const struct sound_fragment * const music[] = {
  &intro,
};

const struct sound_clip se_coin = {
  .priority = 2,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
