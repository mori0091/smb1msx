// -*- coding: utf-8-unix -*-

#include <sound.h>

#include "sound_macro.h"

// tick = 60Hz * 60s * 4 / (Tempo * L)
//      = 14400 / (Tempo * L)

// "T300 o4 v15 L48 >[3 [2 AD<G> r16] r12]"
static const uint8_t intro_ch1[] = {
  // T300 V15
  PACK( 1, V(15), O5A),         // V15 O5 A48
  PACK( 1, O5D),                //     O5 D48
  PACK( 1, O4G),                //     O4 G48
  PACK( 3, R),                  //        R16

  PACK( 1, O5A),                //     O5 A48
  PACK( 1, O5D),                //     O5 D48
  PACK( 1, O4G),                //     O4 G48
  PACK( 3, R),                  //        R16

  PACK( 4, R),                  //        R12

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
  &intro,
  &intro,
};

const struct sound_clip se_pipe = {
  .priority = 2,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
