// -*- coding: utf-8-unix -*-

#include "smb1.h"

#define HSYNC_LINE (13)

static __at(0xfd9f) uint8_t H_TIMI[5];

volatile uint16_t scroll_x;

static void vsync_handler(void) {
  // vdp_set_hscroll(0);
  {
    VDP_SET_CONTROL_REGISTER(26, 0);
    VDP_SET_CONTROL_REGISTER(27, 0);
  }
}

inline void hsync_handler(void) {
  // ---- HSYNC ----
  // vdp_set_hscroll(...);
  {
    VDP_SET_CONTROL_REGISTER(26, RG26SA);
    VDP_SET_CONTROL_REGISTER(27, RG27SA);
  }
  {
    volatile uint16_t x = scroll_x;
    RG26SA = (x >> 3) & 0x3f;
    RG27SA = (~x + 1) & 0x07;
    if (RG27SA) {
      RG26SA++;
    }
  }
  sound_player();
}

static void interrupt_handler(void) {
  VDP_SET_STATUS_REGISTER_POINTER(1);
  if (VDP_GET_STATUS_REGISTER_VALUE() & 1) {
    VDP_SET_STATUS_REGISTER_POINTER(0);
    hsync_handler();
    __asm__("ei");
    return;
  }
  VDP_SET_STATUS_REGISTER_POINTER(0);
}

void setup_interrupt(void) {
  set_hscroll(0);
  vdp_set_control(19, (RG19SA = HSYNC_LINE));
  vdp_set_control(0, (RG0SAV |= 0x10));
  set_interrupt_handler(interrupt_handler);
  set_vsync_handler(vsync_handler);
}
