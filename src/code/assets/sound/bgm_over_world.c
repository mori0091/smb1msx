// -*- coding: utf-8-unix -*-

#include <sound.h>

#include "sound_macro.h"

static const uint8_t intro_ch1[] = {
  // T180 V15
  PACK(10, V(15), O5E),         // L8 O5 E
  PACK(10, O5E),                // L8 O5 E
  PACK(10, R),                  // L8    R
  PACK(10, O5E),                // L8 O5 E
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C
  PACK(20, O5E),                // L4 O5 E

  PACK(20, O5G),                // L4 O5 G
  PACK(20, R),                  // L4    R
  PACK(20, O4G),                // L4 O4 G
  PACK(20, R),                  // L4    R

  EOM,
};

static const uint8_t intro_ch2[] = {
  // T180 V15
  PACK(10, V(15), O4Fs),        // L8 O4 F+
  PACK(10, O4F),                // L8 O4 F
  PACK(10, R),                  // L8    R
  PACK(10, O4F),                // L8 O4 F
  PACK(10, R),                  // L8    R
  PACK(10, O4F),                // L8 O4 F
  PACK(20, O4F),                // L4 O4 F

  PACK(20, O4B),                // L4 O4 B
  PACK(20, R),                  // L4    R
  PACK(40, R),                  // L2    R

  EOM,
};

static const uint8_t intro_ch3[] = {
  // T180 V15
  PACK(10, V(15), O4D),         // L8 O4 D
  PACK(10, O4D),                // L8 O4 D
  PACK(10, R),                  // L8    R
  PACK(10, O4D),                // L8 O4 D
  PACK(10, R),                  // L8    R
  PACK(10, O4D),                // L8 O4 D
  PACK(20, O4D),                // L4 O4 D

  PACK(20, O5G),                // L4 O5 G
  PACK(20, R),                  // L4    R
  PACK(20, O4G),                // L4 O4 G
  PACK(20, R),                  // L4    R

  EOM,
};

static const struct sound_fragment intro = {
  .streams = {
    [0] = intro_ch1,
    [1] = intro_ch2,
    [2] = intro_ch3,
  }
};

static const uint8_t a_part_ch1[] = {
  // T180
  PACK(20, O5C),                // L4 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O4G),                // L8 O4 G
  PACK(20, R),                  // L4    R
  PACK(20, O4E),                // L4 O4 E

  PACK(10, R),                  // L8    R
  PACK(10, O4A),                // L8 O4 A
  PACK(10, R),                  // L8    R
  PACK(10, O4B),                // L8 O4 B
  PACK(10, R),                  // L8    R
  PACK(10, O4As),               // L8 O4 A+
  PACK(20, O4A),                // L4 O4 A

  PACK(13, O4G),                // L6 O4 G
  PACK(13, O5E),                // L6 O5 E
  PACK(14, O5G),                // L6 O5 G
  PACK(20, O5A),                // L4 O5 A
  PACK(10, O5F),                // L8 O5 F
  PACK(10, O5G),                // L8 O5 G

  PACK(10, R),                  // L8    R
  PACK(10, O5E),                // L8 O5 E
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C
  PACK(10, O5D),                // L8 O5 D
  PACK(10, O4B),                // L8 O4 B
  PACK(20, R),                  // L4    R

  EOM,
};

static const uint8_t a_part_ch2[] = {
  // T180
  PACK(20, O4E),                // L4 O4 E
  PACK(10, R),                  // L8    R
  PACK(10, O4C),                // L8 O4 C
  PACK(20, R),                  // L4    R
  PACK(20, O3G),                // L4 O3 G

  PACK(10, R),                  // L8    R
  PACK(10, O4C),                // L8 O4 C
  PACK(10, R),                  // L8    R
  PACK(10, O4D),                // L8 O4 D
  PACK(10, R),                  // L8    R
  PACK(10, O4Cs),               // L8 O4 C+
  PACK(20, O4C),                // L4 O4 C

  PACK(13, O4C),                // L6 O4 C
  PACK(13, O4G),                // L6 O4 G
  PACK(14, O4B),                // L6 O4 B
  PACK(20, O5C),                // L4 O5 C
  PACK(10, O4A),                // L8 O4 A
  PACK(10, O4B),                // L8 O4 B

  PACK(10, R),                  // L8    R
  PACK(10, O4A),                // L8 O4 A
  PACK(10, R),                  // L8    R
  PACK(10, O4E),                // L8 O4 E
  PACK(10, O4F),                // L8 O4 F
  PACK(10, O4D),                // L8 O4 D
  PACK(20, R),                  // L4    R

  EOM,
};

static const uint8_t a_part_ch3[] = {
  // T180
  PACK(20, O4G),                // L4 O4 G
  PACK(10, R),                  // L8    R
  PACK(10, O4E),                // L8 O4 E
  PACK(20, R),                  // L4    R
  PACK(20, O4C),                // L4 O4 C

  PACK(10, R),                  // L8    R
  PACK(10, O4F),                // L8 O4 F
  PACK(10, R),                  // L8    R
  PACK(10, O4G),                // L8 O4 G
  PACK(10, R),                  // L8    R
  PACK(10, O4Fs),               // L8 O4 F+
  PACK(20, O4F),                // L4 O4 F

  PACK(13, O4E),                // L6 O4 E
  PACK(13, O5C),                // L6 O5 C
  PACK(14, O5E),                // L6 O5 E
  PACK(20, O5F),                // L4 O5 F
  PACK(10, O5D),                // L8 O5 D
  PACK(10, O5E),                // L8 O5 E

  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O4A),                // L8 O4 A
  PACK(10, O4B),                // L8 O4 B
  PACK(10, O4G),                // L8 O4 G
  PACK(20, R),                  // L4    R

  EOM,
};

static const struct sound_fragment a_part = {
  .streams = {
    [0] = a_part_ch1,
    [1] = a_part_ch2,
    [2] = a_part_ch3,
  }
};

static const uint8_t b_part_ch1[] = {
  PACK(20, R),                  // L4    R
  PACK(10, O5G),                // L8 O5 G
  PACK(10, O5Fs),               // L8 O5 F+
  PACK(10, O5E),                // L8 O5 F- (E)
  PACK(10, O5Ds),               // L8 O5 D+
  PACK(10, R),                  // L8    R
  PACK(10, O5E),                // L8 O5 E

  PACK(10, R),                  // L8    R
  PACK(10, O4Fs),               // L8 O4 G- (F+)
  PACK(10, O4A),                // L8 O4 A
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O4A),                // L8 O4 A
  PACK(10, O5C),                // L8 O5 C
  PACK(10, O5D),                // L8 O5 D

  PACK(20, R),                  // L4    R
  PACK(10, O5G),                // L8 O5 G
  PACK(10, O5Fs),               // L8 O5 F+
  PACK(10, O5E),                // L8 O5 F- (E)
  PACK(10, O5Ds),               // L8 O5 D+
  PACK(10, R),                  // L8    R
  PACK(10, O5E),                // L8 O5 E

  PACK(10, R),                  // L8    R
  PACK(10, O6C),                // L8 O6 C
  PACK(10, R),                  // L8    R
  PACK(10, O6C),                // L8 O6 C
  PACK(20, O6C),                // L4 O6 C
  PACK(20, R),                  // L4    R

  PACK(20, R),                  // L4    R
  PACK(10, O5G),                // L8 O5 G
  PACK(10, O5Fs),               // L8 O5 F+
  PACK(10, O5E),                // L8 O5 F- (E)
  PACK(10, O5Ds),               // L8 O5 D+
  PACK(10, R),                  // L8    R
  PACK(10, O5E),                // L8 O5 E

  PACK(10, R),                  // L8    R
  PACK(10, O4Gs),               // L8 O4 G+
  PACK(10, O4A),                // L8 O4 A
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O4A),                // L8 O4 A
  PACK(10, O5C),                // L8 O5 C
  PACK(10, O5D),                // L8 O5 D

  PACK(20, R),                  // L4    R
  PACK(20, O5Ds),               // L4 O5 E- (D+)
  PACK(10, R),                  // L8    R
  PACK(10, O5D),                // L8 O5 D
  PACK(20, R),                  // L4    R

  PACK(20, O5C),                // L4 O5 C
  PACK(20, R),                  // L4    R
  PACK(40, R),                  // L2    R

  EOM,
};

static const uint8_t b_part_ch2[] = {
  // T180
  PACK(20, R),                  // L4    R
  PACK(10, O5E),                // L8 O5 E
  PACK(10, O5Ds),               // L8 O5 D+
  PACK(10, O5Cs),               // L8 O5 D- (C+)
  PACK(10, O4B),                // L8 O4 B
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C

  PACK(10, R),                  // L8    R
  PACK(10, O4E),                // L8 O4 E
  PACK(10, O4F),                // L8 O4 F
  PACK(10, O4G),                // L8 O4 G
  PACK(10, R),                  // L8    R
  PACK(10, O4C),                // L8 O4 C
  PACK(10, O4E),                // L8 O4 E
  PACK(10, O4F),                // L8 O4 F

  PACK(20, R),                  // L4    R
  PACK(10, O5E),                // L8 O5 E
  PACK(10, O5Ds),               // L8 O5 D+
  PACK(10, O5Cs),               // L8 O5 D- (C+)
  PACK(10, O4B),                // L8 O4 B
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C

  PACK(10, R),                  // L8    R
  PACK(10, O5F),                // L8 O5 F
  PACK(10, R),                  // L8    R
  PACK(10, O5F),                // L8 O5 F
  PACK(20, O5F),                // L4 O5 F
  PACK(20, R),                  // L4    R

  PACK(20, R),                  // L4    R
  PACK(10, O5E),                // L8 O5 E
  PACK(10, O5Ds),               // L8 O5 D+
  PACK(10, O5Cs),               // L8 O5 D- (C+)
  PACK(10, O4B),                // L8 O4 B
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C

  PACK(10, R),                  // L8    R
  PACK(10, O4E),                // L8 O4 E
  PACK(10, O4F),                // L8 O4 F
  PACK(10, O4G),                // L8 O4 G
  PACK(10, R),                  // L8    R
  PACK(10, O4C),                // L8 O4 C
  PACK(10, O4E),                // L8 O4 E
  PACK(10, O4F),                // L8 O4 F

  PACK(20, R),                  // L4    R
  PACK(20, O4Gs),               // L4 O4 A- (G+)
  PACK(10, R),                  // L8    R
  PACK(10, O4F),                // L8 O4 F
  PACK(20, R),                  // L4    R

  PACK(10, O4E),                // L8 O4 E
  PACK(20, R),                  // L4    R
  PACK(40, R),                  // L2    R

  EOM,
};

static const uint8_t b_part_ch3[] = {
  // T180
  PACK(20, O4C),                // L4 O4 C
  PACK(10, R),                  // L8    R
  PACK(10, O4G),                // L8 O4 G
  PACK(20, R),                  // L4    R
  PACK(20, O5C),                // L4 O5 C

  PACK(20, O4F),                // L4 O4 F
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C
  PACK(20, O5C),                // L4 O5 C
  PACK(20, O4F),                // L4 O4 F

  PACK(20, O4C),                // L4 O4 C
  PACK(10, R),                  // L8    R
  PACK(10, O4E),                // L8 O4 E
  PACK(20, R),                  // L4    R
  PACK(10, O4G),                // L8 O4 G
  PACK(10, O5C),                // L8 O5 C

  PACK(10, R),                  // L8    R
  PACK(10, O6G),                // L8 O6 G
  PACK(10, R),                  // L8    R
  PACK(10, O6G),                // L8 O6 G
  PACK(20, O6G),                // L4 O6 G
  PACK(20, O4G),                // L4 O4 G

  PACK(20, O4C),                // L4 O4 C
  PACK(10, R),                  // L8    R
  PACK(10, O4G),                // L8 O4 G
  PACK(20, R),                  // L4    R
  PACK(20, O5C),                // L4 O5 C

  PACK(20, O4F),                // L4 O4 F
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C
  PACK(20, O5C),                // L4 O5 C
  PACK(20, O4F),                // L4 O4 F

  PACK(20, O4C),                // L4 O4 C
  PACK(20, O4Gs),               // L4 O4 A- (G+)
  PACK(10, R),                  // L8    R
  PACK(10, O4As),               // L8 O4 B- (A+)
  PACK(20, R),                  // L4    R

  PACK(20, O5C),                // L4 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O4G),                // L8 O4 G
  PACK(20, O4G),                // L4 O4 G
  PACK(20, O4C),                // L4 O4 C

  EOM,
};

static const struct sound_fragment b_part = {
  .streams = {
    [0] = b_part_ch1,
    [1] = b_part_ch2,
    [2] = b_part_ch3,
  }
};

static const uint8_t c_part_ch1[] = {
  // T180
  PACK(10, O5C),                // L8 O5 C
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C
  PACK(20, O5D),                // L4 O5 D

  PACK(10, O5E),                // L8 O5 E
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O4A),                // L8 O4 A
  PACK(20, O4G),                // L4 O4 G
  PACK(20, R),                  // L4    R

  PACK(10, O5C),                // L8 O5 C
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C
  PACK(10, O5D),                // L8 O5 D
  PACK(10, O5E),                // L8 O5 E

  PACK(80, R),                  // L1    R

  PACK(10, O5C),                // L8 O5 C
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C
  PACK(20, O5D),                // L4 O5 D

  PACK(10, O5E),                // L8 O5 E
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O4A),                // L8 O4 A
  PACK(20, O4G),                // L4 O4 G
  PACK(20, R),                  // L4    R

  EOM,
};

static const uint8_t c_part_ch2[] = {
  // T180
  PACK(10, O4Gs),               // L8 O4 A- (G+)
  PACK(10, O4A),                // L8 O4 A
  PACK(10, R),                  // L8    R
  PACK(10, O4A),                // L8 O4 A
  PACK(10, R),                  // L8    R
  PACK(10, O4A),                // L8 O4 A
  PACK(20, O4As),               // L4 O4 B- (A+)

  PACK(10, O4G),                // L8 O4 G
  PACK(10, O4E),                // L8 O4 E
  PACK(10, R),                  // L8    R
  PACK(10, O4E),                // L8 O4 E
  PACK(20, O4C),                // L4 O4 C
  PACK(20, R),                  // L4    R

  PACK(10, O4Gs),               // L8 O4 A- (G+)
  PACK(10, O4A),                // L8 O4 A
  PACK(10, R),                  // L8    R
  PACK(10, O4A),                // L8 O4 A
  PACK(10, R),                  // L8    R
  PACK(10, O4A),                // L8 O4 A
  PACK(10, O4As),               // L8 O4 B- (A+)
  PACK(10, O4G),                // L8 O4 G

  PACK(80, R),                  // L1    R

  PACK(10, O4Gs),               // L8 O4 A- (G+)
  PACK(10, O4A),                // L8 O4 A
  PACK(10, R),                  // L8    R
  PACK(10, O4A),                // L8 O4 A
  PACK(10, R),                  // L8    R
  PACK(10, O4A),                // L8 O4 A
  PACK(20, O4As),               // L4 O4 B- (A+)

  PACK(10, O4G),                // L8 O4 G
  PACK(10, O4E),                // L8 O4 E
  PACK(10, R),                  // L8    R
  PACK(10, O4E),                // L8 O4 E
  PACK(20, O4C),                // L4 O4 C
  PACK(20, R),                  // L4    R

  EOM,
};

static const uint8_t c_part_ch3[] = {
  // T180
  PACK(20, O3Gs),               // L4 O3 A- (G+)
  PACK(10, R),                  // L8    R
  PACK(10, O4Ds),               // L8 O4 E- (D+)
  PACK(20, R),                  // L8    R
  PACK(20, O4Gs),               // L4 O4 A- (G+)

  PACK(20, O4G),                // L4 O4 G
  PACK(10, R),                  // L8    R
  PACK(10, O4C),                // L8 O4 C
  PACK(20, R),                  // L4    R
  PACK(20, O3G),                // L4 O3 G

  PACK(20, O3Gs),               // L4 O3 A- (G+)
  PACK(10, R),                  // L8    R
  PACK(10, O4Ds),               // L8 O4 E- (D+)
  PACK(20, R),                  // L8    R
  PACK(20, O4Gs),               // L4 O4 A- (G+)

  PACK(20, O4G),                // L4 O4 G
  PACK(10, R),                  // L8    R
  PACK(10, O4C),                // L8 O4 C
  PACK(20, R),                  // L4    R
  PACK(20, O3G),                // L4 O3 G

  PACK(20, O3Gs),               // L4 O3 A- (G+)
  PACK(10, R),                  // L8    R
  PACK(10, O4Ds),               // L8 O4 E- (D+)
  PACK(20, R),                  // L8    R
  PACK(20, O4Gs),               // L4 O4 A- (G+)

  PACK(20, O4G),                // L4 O4 G
  PACK(10, R),                  // L8    R
  PACK(10, O4C),                // L8 O4 C
  PACK(20, R),                  // L4    R
  PACK(20, O3G),                // L4 O3 G

  EOM,
};

static const struct sound_fragment c_part = {
  .streams = {
    [0] = c_part_ch1,
    [1] = c_part_ch2,
    [2] = c_part_ch3,
  }
};

static const uint8_t d_part_ch1[] = {
  // T180
  PACK(10, O5E),                // L8 O5 E
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O4G),                // L8 O4 G
  PACK(20, R),                  // L4    R
  PACK(20, O4Gs),               // L4 O4 G+

  PACK(10, O4A),                // L8 O4 A
  PACK(10, O5F),                // L8 O5 F
  PACK(10, R),                  // L8    R
  PACK(10, O5F),                // L8 O5 F
  PACK(20, O4A),                // L4 O4 A
  PACK(20, R),                  // L4    R

  PACK(14, O4B),                // L6 O4 B
  PACK(13, O5A),                // L6 O5 A
  PACK(13, O5A),                // L6 O5 A
  PACK(14, O5A),                // L6 O5 A
  PACK(13, O5G),                // L6 O5 G
  PACK(13, O5F),                // L6 O5 F

  PACK(10, O5E),                // L8 O5 E
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O4A),                // L8 O4 A
  PACK(20, O4G),                // L4 O4 G
  PACK(20, R),                  // L4    R

  PACK(10, O5E),                // L8 O5 E
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O4G),                // L8 O4 G
  PACK(20, R),                  // L4    R
  PACK(20, O4Gs),               // L4 O4 G+

  PACK(10, O4A),                // L8 O4 A
  PACK(10, O5F),                // L8 O5 F
  PACK(10, R),                  // L8    R
  PACK(10, O5F),                // L8 O5 F
  PACK(20, O4A),                // L4 O4 A
  PACK(20, R),                  // L4    R

  PACK(10, O4B),                // L8 O4 B
  PACK(10, O5F),                // L8 O5 F
  PACK(10, R),                  // L8    R
  PACK(10, O5F),                // L8 O5 F
  PACK(14, O5F),                // L6 O5 F
  PACK(13, O5E),                // L6 O5 E
  PACK(13, O5D),                // L6 O5 D

  PACK(20, O5C),                // L4 O5 C
  PACK(20, R),                  // L4    R
  PACK(40, R),                  // L2    R

  EOM,
};

static const uint8_t d_part_ch2[] = {
  // T180
  PACK(10, O5C),                // L8 O5 C
  PACK(10, O4A),                // L8 O4 A
  PACK(10, R),                  // L8    R
  PACK(10, O4E),                // L8 O4 E
  PACK(20, R),                  // L4    R
  PACK(20, O4E),                // L4 O4 E

  PACK(10, O4F),                // L8 O4 F
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C
  PACK(20, O4F),                // L4 O4 F
  PACK(20, R),                  // L4    R

  PACK(14, O4G),                // L6 O4 G
  PACK(13, O5F),                // L6 O5 F
  PACK(13, O5F),                // L6 O5 F
  PACK(14, O5F),                // L6 O5 F
  PACK(13, O5E),                // L6 O5 E
  PACK(13, O5D),                // L6 O5 D

  PACK(10, O5C),                // L8 O5 C
  PACK(10, O4A),                // L8 O4 A
  PACK(10, R),                  // L8    R
  PACK(10, O4F),                // L8 O4 F
  PACK(20, O4E),                // L4 O4 E
  PACK(20, R),                  // L4    R

  PACK(10, O5C),                // L8 O5 C
  PACK(10, O4A),                // L8 O4 A
  PACK(10, R),                  // L8    R
  PACK(10, O4E),                // L8 O4 E
  PACK(20, R),                  // L4    R
  PACK(20, O4E),                // L4 O4 E

  PACK(10, O4F),                // L8 O4 F
  PACK(10, O5C),                // L8 O5 C
  PACK(10, R),                  // L8    R
  PACK(10, O5C),                // L8 O5 C
  PACK(20, O4F),                // L4 O4 F
  PACK(20, R),                  // L4    R

  PACK(10, O4G),                // L8 O4 G
  PACK(10, O5D),                // L8 O5 D
  PACK(10, R),                  // L8    R
  PACK(10, O5D),                // L8 O5 D
  PACK(14, O5D),                // L6 O5 D
  PACK(13, O5C),                // L6 O5 C
  PACK(13, O4B),                // L6 O4 B

  PACK(10, O4G),                // L8 O4 G
  PACK(10, O4E),                // L8 O4 E
  PACK(10, R),                  // L8    R
  PACK(10, O4E),                // L8 O4 E
  PACK(20, O4C),                // L4 O4 C
  PACK(20, R),                  // L4    R

  EOM,
};

static const uint8_t d_part_ch3[] = {
  // T180
  PACK(20, O4C),                // L4 O4 C
  PACK(10, R),                  // L8    R
  PACK(10, O4E),                // L8 O4 F- (E)
  PACK(20, O4G),                // L4 O4 G
  PACK(20, O5C),                // L4 O5 C

  PACK(20, O4F),                // L4 O4 F
  PACK(20, O4F),                // L4 O4 F
  PACK(10, O5C),                // L8 O5 C
  PACK(10, O5C),                // L8 O5 C
  PACK(20, O4F),                // L4 O4 F

  PACK(20, O4D),                // L4 O4 D
  PACK(10, R),                  // L8    R
  PACK(10, O4F),                // L8 O4 F
  PACK(20, O4G),                // L4 O4 G
  PACK(20, O4B),                // L4 O4 B

  PACK(20, O4G),                // L4 O4 G
  PACK(20, O4G),                // L4 O4 G
  PACK(10, O5C),                // L8 O5 C
  PACK(10, O5C),                // L8 O5 C
  PACK(20, O4G),                // L4 O4 G

  PACK(20, O4C),                // L4 O4 C
  PACK(10, R),                  // L8    R
  PACK(10, O4E),                // L8 O4 F- (E)
  PACK(20, O4G),                // L4 O4 G
  PACK(20, O5C),                // L4 O5 C

  PACK(20, O4F),                // L4 O4 F
  PACK(20, O4F),                // L4 O4 F
  PACK(10, O5C),                // L8 O5 C
  PACK(10, O5C),                // L8 O5 C
  PACK(20, O4F),                // L4 O4 F

  PACK(20, O4G),                // L4 O4 G
  PACK(10, R),                  // L8    R
  PACK(10, O4G),                // L8 O4 G
  PACK(14, O4G),                // L6 O4 G
  PACK(13, O4A),                // L6 O4 A
  PACK(13, O4B),                // L6 O4 B

  PACK(20, O5C),                // L4 O5 C
  PACK(20, O4G),                // L4 O4 G
  PACK(20, O4C),                // L4 O4 C
  PACK(20, R),                  // L4    R

  EOM,
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
