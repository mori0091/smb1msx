// -*- coding: utf-8-unix -*-

#include <sound.h>

#include "sound_macro.h"

// "T225 L32 C<G>CEG>C<G <A->CE-A-E-A->CE-A- <<B->DFB-FB->DFB-F<"
static const uint8_t intro_ch1[] = {
  // T225 L32 V15
  PACK( 2, V(15), O4C),         // O4 C
  PACK( 2, O3G),                // O3 G
  PACK( 2, O4C),                // O4 C
  PACK( 2, O4E),                //    E
  PACK( 2, O4G),                //    G
  PACK( 2, O5C),                // O5 C
  PACK( 2, O4G),                // O4 G

  PACK( 2, O3Gs),               // O3 A- (G+)
  PACK( 2, O4C),                // O4 C
  PACK( 2, O4Ds),               //    E- (D+)
  PACK( 2, O4Gs),               //    A- (G+)
  PACK( 2, O4Ds),               //    E- (D+)
  PACK( 2, O4Gs),               //    A- (G+)
  PACK( 2, O5C),                // O5 C
  PACK( 2, O5Ds),               //    E- (D+)
  PACK( 2, O5Gs),               //    A- (G+)

  PACK( 2, O3As),               // O3 B- (A+)
  PACK( 2, O4D),                // O4 D
  PACK( 2, O4F),                //    F
  PACK( 2, O4As),               //    B- (A+)
  PACK( 2, O4F),                //    F
  PACK( 2, O4As),               //    B- (A+)
  PACK( 2, O5D),                // O5 D
  PACK( 2, O5F),                //    F
  PACK( 2, O5As),               //    B- (A+)
  PACK( 2, O5F),                //    F

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

const struct sound_clip se_powup = {
  .priority = 2,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
