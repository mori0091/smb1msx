// -*- coding: utf-8-unix -*-

#include <sound.h>

#include "sound_macro.h"

// 1 "T180 O4 V15 L64 >CCGA- DDA-A E-E-AB- EEB-B R"
// 2 "T180 O4 V13 L64 R >CCGA- DDA-A E-E-AB- EEB-B"

// "T180 L64 = 1.25 tick"

static const uint8_t intro_ch1[] = {
  // T180 V15 L64
  PACK( 2, V(15), O4C),         // O4 C
  PACK( 1, O4C),                //    C
  PACK( 1, O4G),                //    G
  PACK( 1, O4Gs),               //    G+

  PACK( 2, O4D),                //    D
  PACK( 1, O4D),                //    D
  PACK( 1, O4Gs),               //    G+
  PACK( 1, O4A),                //    A

  PACK( 2, O4Ds),               //    D+
  PACK( 1, O4Ds),               //    D+
  PACK( 1, O4A),                //    A
  PACK( 1, O4As),               //    A+

  PACK( 2, O4E),                //    E
  PACK( 1, O4E),                //    E
  PACK( 1, O4As),               //    A+
  PACK( 1, O4B),                //    B

  //PACK( 1, R),                  //    R

  EOM,
};

static const uint8_t intro_ch2[] = {
  // T180 V13 L64
  PACK( 1, V(13), R),           //    R (delay)

  PACK( 2, O4C),                // O4 C
  PACK( 1, O4C),                //    C
  PACK( 1, O4G),                //    G
  PACK( 1, O4Gs),               //    G+

  PACK( 2, O4D),                //    D
  PACK( 1, O4D),                //    D
  PACK( 1, O4Gs),               //    G+
  PACK( 1, O4A),                //    A

  PACK( 2, O4Ds),               //    D+
  PACK( 1, O4Ds),               //    D+
  PACK( 1, O4A),                //    A
  PACK( 1, O4As),               //    A+

  PACK( 2, O4E),                //    E
  PACK( 1, O4E),                //    E
  PACK( 1, O4As),               //    A+
  PACK( 1, O4B),                //    B

  EOM,
};

static const struct sound_fragment intro = {
  .streams = {
    [0] = 0,
    [1] = intro_ch1,
    [2] = intro_ch2,
  },
};

static const struct sound_fragment * const music[] = {
  &intro,
};

const struct sound_clip se_item = {
  .priority = 2,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
