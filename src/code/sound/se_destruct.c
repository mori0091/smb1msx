// -*- coding: utf-8-unix -*-

#include <sound.h>

// tick = 60Hz * 60s * 4 / (Tempo * L)
//      = 14400 / (Tempo * L)

// "T180 mixer(101 010) noise(16) @3 V15 C4 V12 C8"
static const uint8_t intro_ch1[] = {
  0xe0, 0x14, 0x70, 0x2a, 0x30, 0xc3, 0x8f, 0x01, 0xac,
  0x60, 0x0a,                         0x8c, 0x01, 0xac,

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

const struct sound_clip se_destruct = {
  .priority = 2,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
