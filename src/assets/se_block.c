// -*- coding: utf-8-unix -*-

#include <msx.h>

static const uint8_t intro_ch1[] = { // T225 L64
  0x60, 0x01, 0x8f, 0x01, 0xfd,      // V15 O3 A
  0x60, 0x01, 0x8f, 0x02, 0x81,      //     O3 F
  0x60, 0x01, 0x8f, 0x02, 0xfa,      //     O3 D
  0x60, 0x01, 0x8f, 0x03, 0x8a,      //     O2 B
  0x60, 0x01, 0x8f, 0x03, 0xf9,      //     O2 A
  0x60, 0x01, 0x8f, 0x03, 0xc0,      //     O2 A+
  0x60, 0x01, 0x8f, 0x03, 0x8a,      //     O2 B
  0x60, 0x01, 0x8f, 0x03, 0x8a,      //     O2 B

  0xff,
};

static const struct sound_fragment intro = {
  .streams = {
    [0] = 0,
    [1] = 0,
    [2] = intro_ch1,
  },
};

static const struct sound_fragment * const music[] = {
  &intro,
};

const struct sound_clip se_block = {
  .priority = 1,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
