// -*- coding: utf-8-unix -*-

#include <msx.h>

// tick = 60Hz * 60s * 4 / (Tempo * L)
//      = 14400 / (Tempo * L)

// "T140 L16 >>EG>ECDG"
static const uint8_t intro_ch1[] = { // T140 L16 V15
  0x60, 0x06, 0x8f, 0x00, 0x55,      // O6 E
  0x40, 0x06, 0x00, 0x47,            //    G
  0x40, 0x06, 0x00, 0x47,            // O7 E
  0x40, 0x06, 0x00, 0x35,            //    C
  0x40, 0x06, 0x00, 0x30,            //    D
  0x40, 0x06, 0x00, 0x24,            //    G

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

const struct sound_clip se_1up = {
  .priority = 2,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
