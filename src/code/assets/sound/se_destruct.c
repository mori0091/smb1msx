// -*- coding: utf-8-unix -*-

#include <sound.h>

#include "sound_macro.h"

// tick = 60Hz * 60s * 4 / (Tempo * L)
//      = 14400 / (Tempo * L)

// "T180 mixer(101 010) noise(16) @3 V15 C4 V12 C8"
static const uint8_t intro_ch1[] = {
  // T180 mixer(101 010) noise(16) @3 V15
  PACK(20, MIX(0x2a), N(16), E(3), V(15), O4C),
  PACK(10, V(12), O4C),

  EOM,
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
