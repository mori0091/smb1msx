// -*- coding: utf-8-unix -*-

#include "../sound.h"

#define CH1_INTRO                                                       \
  0xa0, 0x02, 0xd0, 0x10, 0x00, 0x00, 0xd6, /* T180 O5 C32 S0 M4096 */  \
  0x60, 0x03, 0x90, 0x00, 0xca,             /* T180    C+32 */          \
  0x60, 0x05, 0x90, 0x00, 0xbe,             /* T180    D16 */           \
  0x20, 0x0a, 0x10,                         /* T180    R8 */            \
  0x20, 0x14, 0x10,                         /* T180    R4 */            \

#define CH2_INTRO                                                       \
  CH1_INTRO

#define CH3_INTRO                                                       \
  0xa0, 0x02, 0xd0, 0x10, 0x00, 0x01, 0xac, /* T180 O4 C32 S0 M4096 */  \
  0x60, 0x03, 0x90, 0x01, 0x94,             /* T180    C+32 */          \
  0x60, 0x05, 0x90, 0x01, 0x7d,             /* T180    D16 */           \
  0x20, 0x0a, 0x10,                         /* T180    R8 */            \
  0x20, 0x14, 0x10,                         /* T180    R4 */            \

#define CH1_A_PART                                                      \
  0xa0, 0x0a, 0xd0, 0x10, 0x00, 0x00, 0xe3, /* T180 O4 B8 S0 M4096 */   \
  0x60, 0x0a, 0x90, 0x00, 0xa0,             /* T180 O5 F8 */            \
  0x20, 0x0a, 0x10,                         /* T180    R8 */            \
  0x60, 0x0a, 0x90, 0x00, 0xa0,             /* T180    F8 */            \
  0x60, 0x0e, 0x90, 0x00, 0xa0,             /* T180    F6 */            \
  0x60, 0x0d, 0x90, 0x00, 0xaa,             /* T180    E6 */            \
  0x60, 0x0d, 0x90, 0x00, 0xbe,             /* T180    D6 */            \
  0x60, 0x14, 0x90, 0x00, 0xd6,             /* T180    C4 */            \
  0x20, 0x14, 0x10,                         /* T180    R4 */            \
  0x20, 0x28, 0x10,                         /* T180    R2 */            \

#define CH2_A_PART                                                      \
  0xa0, 0x14, 0xd0, 0x10, 0x00, 0x01, 0x1d, /* T180 O4 G4 S0 M4096 */   \
  0x20, 0x0a, 0x10,                         /* T180    R8 */            \
  0x60, 0x0a, 0x90, 0x01, 0x1d,             /* T180    G8 */            \
  0x60, 0x0e, 0x90, 0x01, 0x1d,             /* T180    G6 */            \
  0x60, 0x0d, 0x90, 0x00, 0xfe,             /* T180    A6 */            \
  0x60, 0x0d, 0x90, 0x00, 0xe3,             /* T180    B6 */            \
  0x60, 0x14, 0x90, 0x00, 0xd6,             /* T180 O5 C4 */            \
  0x60, 0x14, 0x90, 0x01, 0x1d,             /* T180 O4 G4 */            \
  0x60, 0x14, 0x90, 0x01, 0xac,             /* T180    C4 */            \
  0x20, 0x14, 0x10,                         /* T180    R4 */            \

#define CH3_A_PART                                                      \
  0xa0, 0x14, 0xd0, 0x10, 0x00, 0x02, 0x3b, /* T180 O3 G4 S0 M4096 */   \
  0x20, 0x0a, 0x10,                         /* T180    R8 */            \
  0x60, 0x0a, 0x90, 0x02, 0x3b,             /* T180    G8 */            \
  0x60, 0x0e, 0x90, 0x02, 0x3b,             /* T180    G6 */            \
  0x60, 0x0d, 0x90, 0x01, 0xfd,             /* T180    A6 */            \
  0x60, 0x0d, 0x90, 0x01, 0xc5,             /* T180    B6 */            \
  0x60, 0x14, 0x90, 0x01, 0xac,             /* T180 O4 C4 */            \
  0x60, 0x14, 0x90, 0x02, 0x3b,             /* T180 O3 G4 */            \
  0x60, 0x14, 0x90, 0x03, 0x57,             /* T180    C4 */            \
  0x20, 0x14, 0x10,                         /* T180    R4 */            \

static const uint8_t channel_1[] ={
  CH1_INTRO
  CH1_A_PART
  //
  0xff,                         /* end of stream */
};
static const uint8_t channel_2[] = {
  CH2_INTRO
  CH2_A_PART
  //
  0xff,                         /* end of stream */
};
static const uint8_t channel_3[] = {
  CH3_INTRO
  CH3_A_PART
  //
  0xff,                         /* end of stream */
};

const struct sound_clip bgm_player_down = {
  .streams = {
    [0] = channel_1,
    [1] = channel_2,
    [2] = channel_3,
  }
};
