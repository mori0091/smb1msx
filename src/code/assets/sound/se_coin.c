// -*- coding: utf-8-unix -*-

#include <sound.h>

#include "sound_macro.h"

// tick = 60Hz * 60s * 4 / (Tempo * L)
//      = 14400 / (Tempo * L)

// "T225 O4 V15 @1 >B32. >E2"
static const uint8_t intro_ch1[] = {
  // T225 V15 @1
  PACK( 3, V(15), E(1), O5B),
  PACK(32, O6E),

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

const struct sound_clip se_coin = {
  .priority = 2,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
