// -*- coding: utf-8-unix -*-

#include <sound.h>

#include "sound_macro.h"

// tick = 60Hz * 60s * 4 / (Tempo * L)
//      = 14400 / (Tempo * L)

static const uint8_t intro_ch1[] = {
  // T120 L16 V15
  PACK( 7, V(15), O6E),
  PACK( 7, O6G),
  PACK( 7, O7E),
  PACK( 7, O7C),
  PACK( 7, O7D ),
  PACK( 7, O7G ),

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

const struct sound_clip se_1up = {
  .priority = 2,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
