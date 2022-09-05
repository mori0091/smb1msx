// -*- coding: utf-8-unix -*-

#include <sound.h>

#include "sound_macro.h"

// tick = 60Hz * 60s * 4 / (Tempo * L)
//      = 14400 / (Tempo * L)

// T288 L4 = 12.5 ticks @ 60Hz

// T288
// 1 O4 V15 @0 L4 [4 >cccr8c8 r8cr8cc    <bbbr8b8  r8br8bb]
// 2 O4 V15 @0 L4 [4 fffd8f8  r8fd8f8d8f eeec8e8   r8ec8e8c8e]
// 3 O4 V15 @0 L4 [4 drar8>d8 r2<a>d     <crgr8>c8 r2<g>c<]

static const uint8_t intro_ch1[] = {
  // Txxx L4 V15 @1
  PACK(13, V(15), E(1), O5C),   // O5 C4
  PACK(12, O5C),                //    C4
  PACK(13, O5C),                //    C4
  PACK( 6, R),                  //    R8
  PACK( 6, O5C),                //    C8

  PACK( 7, R),                  //    R8
  PACK(12, O5C),                //    C4
  PACK( 6, R),                  //    R8
  PACK(13, O5C),                //    C4
  PACK(12, O5C),                //    C4

  PACK(13, O4B),                // O4 B4
  PACK(12, O4B),                //    B4
  PACK(13, O4B),                //    B4
  PACK( 6, R),                  //    R8
  PACK( 6, O4B),                //    B8

  PACK( 7, R),                  //    R8
  PACK(12, O4B),                //    B4
  PACK( 6, R),                  //    R8
  PACK(13, O4B),                //    B4
  PACK(12, O4B),                //    B4

  EOM,
};

static const uint8_t intro_ch2[] = {
  // Txxx L4 V15 @1
  PACK(13, V(15), E(1), O4F),   // O4 F4
  PACK(12, O4F),                //    F4
  PACK(13, O4F),                //    F4
  PACK( 6, O4D),                //    D8
  PACK( 6, O4F),                //    F8

  PACK( 7, R),                  //    R8
  PACK(12, O4F),                //    F4
  PACK( 6, O4D),                //    D8
  PACK( 7, O4F),                //    F8
  PACK( 6, O4D),                //    D8
  PACK(12, O4F),                //    F4

  PACK(13, O4E),                //    E4
  PACK(12, O4E),                //    E4
  PACK(13, O4E),                //    E4
  PACK( 6, O4C),                //    C8
  PACK( 6, O4E),                //    E8

  PACK( 7, R),                  //    R8
  PACK(12, O4E),                //    E4
  PACK( 6, O4C),                //    C8
  PACK( 7, O4E),                //    E8
  PACK( 6, O4C),                //    C8
  PACK(12, O4E),                //    E4

  EOM,
};

// static const uint8_t intro_ch3[] = {
//   // Txxx L4 V14 @1
//   PACK(13, V(14), E(1), O4D),   // O4 D4
//   PACK(12, R),                  //    R4
//   PACK(13, O4A),                // O4 A4
//   PACK( 6, R),                  //    R8
//   PACK( 6, O5D),                // O5 D8

//   PACK(25, R),                  //    R2
//   PACK(13, O4A),                // O4 A4
//   PACK(12, O5D),                // O5 D4

//   PACK(13, O4C),                // O4 C4
//   PACK(12, R),                  //    R4
//   PACK(13, O4G),                // O4 G4
//   PACK( 6, R),                  //    R8
//   PACK( 6, O5C),                // O5 C8

//   PACK(25, R),                  //    R2
//   PACK(13, O4G),                // O4 G4
//   PACK(12, O5C),                // O5 C4

//   EOM,
// };
static const uint8_t intro_ch3[] = {
  // Txxx L4 V14 @1
  PACK( 4, V(14), E(1), N(0),
           MIX(0x18), O4D), PACK( 9, MIX(0x38), O4D), // O4 D4
  PACK(12, R),                                        //    R4
  PACK( 4, MIX(0x18), O4A), PACK( 9, MIX(0x38), O4A), //    O4 A4
  PACK( 6, R),                                        //    R8
  PACK( 4, MIX(0x18), O5D), PACK( 2, MIX(0x38), O5D), //    O5 D4

  PACK(25, R),                                        //    R2
  PACK( 4, MIX(0x18), O4A), PACK( 9, MIX(0x38), O4A), //    O4 A4
  PACK( 4, MIX(0x18), O5D), PACK( 8, MIX(0x38), O5D), //    O5 D4

  PACK( 4, MIX(0x18), O4C), PACK( 9, MIX(0x38), O4C), //    O4 C4
  PACK(12, R),                                        //    R4
  PACK( 4, MIX(0x18), O4G), PACK( 9, MIX(0x38), O4G), // O4 G4
  PACK( 6, R),                                        //    R8
  PACK( 4, MIX(0x18), O5C), PACK( 2, MIX(0x38), O5C), // O5 C8

  PACK(25, R),                                        //    R2
  PACK( 4, MIX(0x18), O4G), PACK( 9, MIX(0x38), O4G), // O4 G4
  PACK( 4, MIX(0x18), O5C), PACK( 8, MIX(0x38), O5C), // O5 C4

  EOM,
};

static const struct sound_fragment intro = {
  .streams = {
    [0] = intro_ch1,
    [1] = intro_ch2,
    [2] = intro_ch3,
  }
};

static const struct sound_fragment * const music[] = {
  &intro,
};

const struct sound_clip bgm_starman = {
  .priority = 10,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
