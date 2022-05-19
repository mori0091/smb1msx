// -*- coding: utf-8-unix -*-

#include <sound.h>

static const uint8_t intro_ch1[] = {
  // 0xa0, 0x0a, 0xd0, 0x0c, 0x00, 0x00, 0xaa, // T180 L8 O5 E S0 M3072
  0x60, 0x0a, 0x8f, 0x00, 0xaa,             // T180 L8 O5 E V15
  0x40, 0x0a, 0x00, 0xaa,                   // T180 L8    E
  0x20, 0x0a, 0x10,                         // T180 L8    R
  0x40, 0x0a, 0x00, 0xaa,                   // T180 L8    E
  0x20, 0x0a, 0x10,                         // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,                   // T180 L8    C
  0x40, 0x14, 0x00, 0xaa,                   // T180 L4    E

  0x40, 0x14, 0x00, 0x8f,                   // T180 L4    G
  0x20, 0x14, 0x10,                         // T180 L4    R
  0x40, 0x14, 0x01, 0x1d,                   // T180 L4 O4 G
  0x20, 0x14, 0x10,                         // T180 L4    R

  0xff,                                     // end of stream
};

static const uint8_t intro_ch2[] = {
  // 0xa0, 0x0a, 0xd0, 0x0c, 0x00, 0x01, 0x2e, // T180 L8 O4 F+ S0 M3072
  0x60, 0x0a, 0x8f, 0x01, 0x2e,             // T180 L8 O4 F+ V15
  0x40, 0x0a, 0x01, 0x40,                   // T180 L8    F
  0x20, 0x0a, 0x10,                         // T180 L8    R
  0x40, 0x0a, 0x01, 0x40,                   // T180 L8    F
  0x20, 0x0a, 0x10,                         // T180 L8    R
  0x40, 0x0a, 0x01, 0x40,                   // T180 L8    F
  0x40, 0x14, 0x01, 0x40,                   // T180 L4    F

  0x40, 0x14, 0x00, 0xe3,                   // T180 L4    B
  0x20, 0x14, 0x10,                         // T180 L4    R
  0x20, 0x28, 0x10,                         // T180 L2    R

  0xff,                                     // end of stream
};

static const uint8_t intro_ch3[] = {
  // 0xa0, 0x0a, 0xd0, 0x0c, 0x00, 0x01, 0x7d, // T180 L8 O4 D S0 M3072
  0x60, 0x0a, 0x8f, 0x01, 0x7d,             // T180 L8 O4 D V15
  0x40, 0x0a, 0x01, 0x7d,                   // T180 L8    D
  0x20, 0x0a, 0x10,                         // T180 L8    R
  0x40, 0x0a, 0x01, 0x7d,                   // T180 L8    D
  0x20, 0x0a, 0x10,                         // T180 L8    R
  0x40, 0x0a, 0x01, 0x7d,                   // T180 L8    D
  0x40, 0x14, 0x01, 0x7d,                   // T180 L4    D

  0x40, 0x14, 0x00, 0x8f,                   // T180 L4 O5 G
  0x20, 0x14, 0x10,                         // T180 L4    R
  0x40, 0x14, 0x01, 0x1d,                   // T180 L4 O4 G
  0x20, 0x14, 0x10,                         // T180 L4    R

  0xff,                                     // end of stream
};

static const struct sound_fragment intro = {
  .streams = {
    [0] = intro_ch1,
    [1] = intro_ch2,
    [2] = intro_ch3,
  }
};

static const uint8_t a_part_ch1[] = {
  0x40, 0x14, 0x00, 0xd6,             // T180 L4 O5 C S0
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x14, 0x01, 0x53,             // T180 L4    E

  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8    A
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xe3,             // T180 L8    B
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xf0,             // T180 L8    A+
  0x40, 0x14, 0x00, 0xfe,             // T180 L4    A

  0x40, 0x0d, 0x01, 0x1d,             // T180 L6    G
  0x40, 0x0d, 0x00, 0xaa,             // T180 L6 O5 E
  0x40, 0x0e, 0x00, 0x8f,             // T180 L6    G
  0x40, 0x14, 0x00, 0x7f,             // T180 L4    A
  0x40, 0x0a, 0x00, 0xa0,             // T180 L8    F
  0x40, 0x0a, 0x00, 0x8f,             // T180 L8    G

  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xaa,             // T180 L8    E
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8    C
  0x40, 0x0a, 0x00, 0xbe,             // T180 L8    D
  0x40, 0x0a, 0x00, 0xe3,             // T180 L8 O4 B
  0x20, 0x14, 0x10,                   // T180 L4    R

  0xff,                               // end of stream
};

static const uint8_t a_part_ch2[] = {
  0x40, 0x14, 0x01, 0x53,             // T180 L4 O4 E S0
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0xac,             // T180 L8    C
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x14, 0x02, 0x3b,             // T180 L4 O3 G

  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0xac,             // T180 L8 O4 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x7d,             // T180 L8    D
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x94,             // T180 L8    C+
  0x40, 0x14, 0x01, 0xac,             // T180 L4    C

  0x40, 0x0d, 0x01, 0xac,             // T180 L6    C
  0x40, 0x0d, 0x01, 0x1d,             // T180 L6    G
  0x40, 0x0e, 0x00, 0xe3,             // T180 L6    B
  0x40, 0x14, 0x00, 0xd6,             // T180 L4 O5 C
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x40, 0x0a, 0x00, 0xe3,             // T180 L8    B

  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8    A
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x53,             // T180 L8    E
  0x40, 0x0a, 0x01, 0x40,             // T180 L8    F
  0x40, 0x0a, 0x01, 0x7d,             // T180 L8    D
  0x20, 0x14, 0x10,                   // T180 L4    R

  0xff,                               // end of stream
};

static const uint8_t a_part_ch3[] = {
  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G S0
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x53,             // T180 L8    E
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x14, 0x01, 0xac,             // T180 L4    C

  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x40,             // T180 L8    F
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x1d,             // T180 L8    G
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x2e,             // T180 L8    F+
  0x40, 0x14, 0x01, 0x40,             // T180 L4    F

  0x40, 0x0d, 0x01, 0x53,             // T180 L6    E
  0x40, 0x0d, 0x00, 0xd6,             // T180 L6 O5 C
  0x40, 0x0e, 0x00, 0xaa,             // T180 L6    E
  0x40, 0x14, 0x00, 0xa0,             // T180 L4    F
  0x40, 0x0a, 0x00, 0xbe,             // T180 L8    D
  0x40, 0x0a, 0x00, 0xaa,             // T180 L8    E

  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8    C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x40, 0x0a, 0x00, 0xe3,             // T180 L8    B
  0x40, 0x0a, 0x01, 0x1d,             // T180 L8    G
  0x20, 0x14, 0x10,                   // T180 L4    R

  0xff,                               // end of stream
};

static const struct sound_fragment a_part = {
  .streams = {
    [0] = a_part_ch1,
    [1] = a_part_ch2,
    [2] = a_part_ch3,
  }
};

static const uint8_t b_part_ch1[] = {
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x0a, 0x00, 0x8f,             // T180 L8 O5 G
  0x40, 0x0a, 0x00, 0x97,             // T180 L8 O5 F+
  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 F- (E)
  0x40, 0x0a, 0x00, 0xb4,             // T180 L8 O5 D+
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 E

  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x2e,             // T180 L8 O4 G- (F+)
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x0a, 0x00, 0xbe,             // T180 L8 O5 D

  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x0a, 0x00, 0x8f,             // T180 L8 O5 G
  0x40, 0x0a, 0x00, 0x97,             // T180 L8 O5 F+
  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 F- (E)
  0x40, 0x0a, 0x00, 0xb4,             // T180 L8 O5 D+
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 E

  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0x6b,             // T180 L8 O6 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0x6b,             // T180 L8 O6 C
  0x40, 0x14, 0x00, 0x6b,             // T180 L4 O6 C
  0x20, 0x14, 0x10,                   // T180 L4    R

  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x0a, 0x00, 0x8f,             // T180 L8 O5 G
  0x40, 0x0a, 0x00, 0x97,             // T180 L8 O5 F+
  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 F- (E)
  0x40, 0x0a, 0x00, 0xb4,             // T180 L8 O5 D+
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 E

  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x0d,             // T180 L8 O4 G+
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x0a, 0x00, 0xbe,             // T180 L8 O5 D

  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x14, 0x00, 0xb4,             // T180 L4 O5 E- (D+)
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xbe,             // T180 L8 O5 D
  0x20, 0x14, 0x10,                   // T180 L4    R

  0x40, 0x14, 0x00, 0xd6,             // T180 L4 O5 C
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x20, 0x28, 0x10,                   // T180 L2    R

  0xff,                               // end of stream
};

static const uint8_t b_part_ch2[] = {
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 E
  0x40, 0x0a, 0x00, 0xb4,             // T180 L8 O5 D+
  0x40, 0x0a, 0x00, 0xca,             // T180 L8 O5 D- (C+)
  0x40, 0x0a, 0x00, 0xe3,             // T180 L8 O4 B
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C

  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 E
  0x40, 0x0a, 0x01, 0x40,             // T180 L8 O4 F
  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0xac,             // T180 L8 O4 C
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 E
  0x40, 0x0a, 0x01, 0x40,             // T180 L8 O4 F

  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 E
  0x40, 0x0a, 0x00, 0xb4,             // T180 L8 O5 D+
  0x40, 0x0a, 0x00, 0xca,             // T180 L8 O5 D- (C+)
  0x40, 0x0a, 0x00, 0xe3,             // T180 L8 O4 B
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C

  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xa0,             // T180 L8 O5 F
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xa0,             // T180 L8 O5 F
  0x40, 0x14, 0x00, 0xa0,             // T180 L4 O5 F
  0x20, 0x14, 0x10,                   // T180 L4    R

  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 E
  0x40, 0x0a, 0x00, 0xb4,             // T180 L8 O5 D+
  0x40, 0x0a, 0x00, 0xca,             // T180 L8 O5 D- (C+)
  0x40, 0x0a, 0x00, 0xe3,             // T180 L8 O4 B
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C

  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 E
  0x40, 0x0a, 0x01, 0x40,             // T180 L8 O4 F
  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0xac,             // T180 L8 O4 C
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 E
  0x40, 0x0a, 0x01, 0x40,             // T180 L8 O4 F

  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x14, 0x01, 0x0d,             // T180 L4 O4 A- (G+)
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x40,             // T180 L8 O4 F
  0x20, 0x14, 0x10,                   // T180 L4    R

  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 E
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x20, 0x28, 0x10,                   // T180 L2    R

  0xff,                               // end of stream
};

static const uint8_t b_part_ch3[] = {
  0x40, 0x14, 0x01, 0xac,             // T180 L4 O4 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x14, 0x00, 0xd6,             // T180 L4 O5 C

  0x40, 0x14, 0x01, 0x40,             // T180 L4 O4 F
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x14, 0x00, 0xd6,             // T180 L4 O5 C
  0x40, 0x14, 0x01, 0x40,             // T180 L4 O4 F

  0x40, 0x14, 0x01, 0xac,             // T180 L4 O4 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 E
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C

  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0x47,             // T180 L8 O6 G
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0x47,             // T180 L8 O6 G
  0x40, 0x14, 0x00, 0x47,             // T180 L4 O6 G
  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G

  0x40, 0x14, 0x01, 0xac,             // T180 L4 O4 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x14, 0x00, 0xd6,             // T180 L4 O5 C

  0x40, 0x14, 0x01, 0x40,             // T180 L4 O4 F
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x14, 0x00, 0xd6,             // T180 L4 O5 C
  0x40, 0x14, 0x01, 0x40,             // T180 L4 O4 F

  0x40, 0x14, 0x01, 0xac,             // T180 L4 O4 C
  0x40, 0x14, 0x01, 0x0d,             // T180 L4 O4 A- (G+)
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xf0,             // T180 L8 O4 B- (A+)
  0x20, 0x14, 0x10,                   // T180 L4    R

  0x40, 0x14, 0x00, 0xd6,             // T180 L4 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G
  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G
  0x40, 0x14, 0x01, 0xac,             // T180 L4 O4 C

  0xff,                               // end of stream
};

static const struct sound_fragment b_part = {
  .streams = {
    [0] = b_part_ch1,
    [1] = b_part_ch2,
    [2] = b_part_ch3,
  }
};

static const uint8_t c_part_ch1[] = {
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x14, 0x00, 0xbe,             // T180 L4 O5 D

  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 E
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G
  0x20, 0x14, 0x10,                   // T180 L4    R

  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x0a, 0x00, 0xbe,             // T180 L8 O5 D
  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 E

  0x20, 0x50, 0x10,                   // T180 L1    R

  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x14, 0x00, 0xbe,             // T180 L4 O5 D

  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 E
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G
  0x20, 0x14, 0x10,                   // T180 L4    R

  0xff,                               // end of stream
};

static const uint8_t c_part_ch2[] = {
  0x40, 0x0a, 0x01, 0x0d,             // T180 L8 O4 A- (G+)
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x40, 0x14, 0x00, 0xf0,             // T180 L4 O4 B- (A+)

  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 E
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 E
  0x40, 0x14, 0x01, 0xac,             // T180 L4 O4 C
  0x20, 0x14, 0x10,                   // T180 L4    R

  0x40, 0x0a, 0x01, 0x0d,             // T180 L8 O4 A- (G+)
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x40, 0x0a, 0x00, 0xf0,             // T180 L8 O4 B- (A+)
  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G

  0x20, 0x50, 0x10,                   // T180 L1    R

  0x40, 0x0a, 0x01, 0x0d,             // T180 L8 O4 A- (G+)
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x40, 0x14, 0x00, 0xf0,             // T180 L4 O4 B- (A+)

  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 E
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 E
  0x40, 0x14, 0x01, 0xac,             // T180 L4 O4 C
  0x20, 0x14, 0x10,                   // T180 L4    R

  0xff,                               // end of stream
};

static const uint8_t c_part_ch3[] = {
  0x40, 0x14, 0x02, 0x1b,             // T180 L4 O3 A- (G+)
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x68,             // T180 L8 O4 E- (D+)
  0x20, 0x14, 0x10,                   // T180 L8    R
  0x40, 0x14, 0x01, 0x0d,             // T180 L4 O4 A- (G+)

  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0xac,             // T180 L8 O4 C
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x14, 0x02, 0x3b,             // T180 L4 O3 G

  0x40, 0x14, 0x02, 0x1b,             // T180 L4 O3 A- (G+)
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x68,             // T180 L8 O4 E- (D+)
  0x20, 0x14, 0x10,                   // T180 L8    R
  0x40, 0x14, 0x01, 0x0d,             // T180 L4 O4 A- (G+)

  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0xac,             // T180 L8 O4 C
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x14, 0x02, 0x3b,             // T180 L4 O3 G

  0x40, 0x14, 0x02, 0x1b,             // T180 L4 O3 A- (G+)
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x68,             // T180 L8 O4 E- (D+)
  0x20, 0x14, 0x10,                   // T180 L8    R
  0x40, 0x14, 0x01, 0x0d,             // T180 L4 O4 A- (G+)

  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0xac,             // T180 L8 O4 C
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x14, 0x02, 0x3b,             // T180 L4 O3 G

  0xff,                               // end of stream
};

static const struct sound_fragment c_part = {
  .streams = {
    [0] = c_part_ch1,
    [1] = c_part_ch2,
    [2] = c_part_ch3,
  }
};

static const uint8_t d_part_ch1[] = {
  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 E
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x14, 0x01, 0x0d,             // T180 L4 O4 G+

  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x40, 0x0a, 0x00, 0xa0,             // T180 L8 O5 F
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xa0,             // T180 L8 O5 F
  0x40, 0x14, 0x00, 0xfe,             // T180 L4 O4 A
  0x20, 0x14, 0x10,                   // T180 L4    R

  0x40, 0x0e, 0x00, 0xe3,             // T180 L6 O4 B
  0x40, 0x0d, 0x00, 0x7f,             // T180 L6 O5 A
  0x40, 0x0d, 0x00, 0x7f,             // T180 L6 O5 A
  0x40, 0x0e, 0x00, 0x7f,             // T180 L6 O5 A
  0x40, 0x0d, 0x00, 0x8f,             // T180 L6 O5 G
  0x40, 0x0d, 0x00, 0xa0,             // T180 L6 O5 F

  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 E
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G
  0x20, 0x14, 0x10,                   // T180 L4    R

  0x40, 0x0a, 0x00, 0xaa,             // T180 L8 O5 E
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x14, 0x01, 0x0d,             // T180 L4 O4 G+

  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x40, 0x0a, 0x00, 0xa0,             // T180 L8 O5 F
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xa0,             // T180 L8 O5 F
  0x40, 0x14, 0x00, 0xfe,             // T180 L4 O4 A
  0x20, 0x14, 0x10,                   // T180 L4    R

  0x40, 0x0a, 0x00, 0xe3,             // T180 L8 O4 B
  0x40, 0x0a, 0x00, 0xa0,             // T180 L8 O5 F
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xa0,             // T180 L8 O5 F
  0x40, 0x0e, 0x00, 0xa0,             // T180 L6 O5 F
  0x40, 0x0d, 0x00, 0xaa,             // T180 L6 O5 E
  0x40, 0x0d, 0x00, 0xbe,             // T180 L6 O5 D

  0x40, 0x14, 0x00, 0xd6,             // T180 L4 O5 C
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x20, 0x28, 0x10,                   // T180 L2    R

  0xff,                               // end of stream
};

static const uint8_t d_part_ch2[] = {
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 E
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x14, 0x01, 0x53,             // T180 L4 O4 E

  0x40, 0x0a, 0x01, 0x40,             // T180 L8 O4 F
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x14, 0x01, 0x40,             // T180 L4 O4 F
  0x20, 0x14, 0x10,                   // T180 L4    R

  0x40, 0x0e, 0x01, 0x1d,             // T180 L6 O4 G
  0x40, 0x0d, 0x00, 0xa0,             // T180 L6 O5 F
  0x40, 0x0d, 0x00, 0xa0,             // T180 L6 O5 F
  0x40, 0x0e, 0x00, 0xa0,             // T180 L6 O5 F
  0x40, 0x0d, 0x00, 0xaa,             // T180 L6 O5 E
  0x40, 0x0d, 0x00, 0xbe,             // T180 L6 O5 D

  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x40,             // T180 L8 O4 F
  0x40, 0x14, 0x01, 0x53,             // T180 L4 O4 E
  0x20, 0x14, 0x10,                   // T180 L4    R

  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x0a, 0x00, 0xfe,             // T180 L8 O4 A
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 E
  0x20, 0x14, 0x10,                   // T180 L4    R
  0x40, 0x14, 0x01, 0x53,             // T180 L4 O4 E

  0x40, 0x0a, 0x01, 0x40,             // T180 L8 O4 F
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x14, 0x01, 0x40,             // T180 L4 O4 F
  0x20, 0x14, 0x10,                   // T180 L4    R

  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G
  0x40, 0x0a, 0x00, 0xbe,             // T180 L8 O5 D
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x00, 0xbe,             // T180 L8 O5 D
  0x40, 0x0e, 0x00, 0xbe,             // T180 L6 O5 D
  0x40, 0x0d, 0x00, 0xd6,             // T180 L6 O5 C
  0x40, 0x0d, 0x00, 0xe3,             // T180 L6 O4 B

  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 E
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 E
  0x40, 0x14, 0x01, 0xac,             // T180 L4 O4 C
  0x20, 0x14, 0x10,                   // T180 L4    R

  0xff,                               // end of stream
};

static const uint8_t d_part_ch3[] = {
  0x40, 0x14, 0x01, 0xac,             // T180 L4 O4 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 F- (E)
  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G
  0x40, 0x14, 0x00, 0xd6,             // T180 L4 O5 C

  0x40, 0x14, 0x01, 0x40,             // T180 L4 O4 F
  0x40, 0x14, 0x01, 0x40,             // T180 L4 O4 F
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x14, 0x01, 0x40,             // T180 L4 O4 F

  0x40, 0x14, 0x01, 0x7d,             // T180 L4 O4 D
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x40,             // T180 L8 O4 F
  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G
  0x40, 0x14, 0x00, 0xe3,             // T180 L4 O4 B

  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G
  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G

  0x40, 0x14, 0x01, 0xac,             // T180 L4 O4 C
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x53,             // T180 L8 O4 F- (E)
  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G
  0x40, 0x14, 0x00, 0xd6,             // T180 L4 O5 C

  0x40, 0x14, 0x01, 0x40,             // T180 L4 O4 F
  0x40, 0x14, 0x01, 0x40,             // T180 L4 O4 F
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x0a, 0x00, 0xd6,             // T180 L8 O5 C
  0x40, 0x14, 0x01, 0x40,             // T180 L4 O4 F

  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G
  0x20, 0x0a, 0x10,                   // T180 L8    R
  0x40, 0x0a, 0x01, 0x1d,             // T180 L8 O4 G
  0x40, 0x0e, 0x01, 0x1d,             // T180 L6 O4 G
  0x40, 0x0d, 0x00, 0xfe,             // T180 L6 O4 A
  0x40, 0x0d, 0x00, 0xe3,             // T180 L6 O4 B

  0x40, 0x14, 0x00, 0xd6,             // T180 L4 O5 C
  0x40, 0x14, 0x01, 0x1d,             // T180 L4 O4 G
  0x40, 0x14, 0x01, 0xac,             // T180 L4 O4 C
  0x20, 0x14, 0x10,                   // T180 L4    R

  0xff,                               // end of stream
};

static const struct sound_fragment d_part = {
  .streams = {
    [0] = d_part_ch1,
    [1] = d_part_ch2,
    [2] = d_part_ch3,
  }
};

static const struct sound_fragment * const music[] = {
  &intro, &a_part, &a_part, &b_part, &b_part, &c_part,
  &intro, &a_part, &a_part, &d_part, &d_part, &c_part,
  &intro, &d_part, &d_part,
};

const struct sound_clip bgm_over_world = {
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};