// -*- coding: utf-8-unix -*-

#include "smb1.h"

#define HSYNC_LINE (13)

inline void vsync_handler(void) {
  // vdp_set_hscroll(0);
  {
    VDP_SET_CONTROL_REGISTER(26, 0);
    VDP_SET_CONTROL_REGISTER(27, 0);
  }
}

static void vsync_handler_epilogue(void) {
  vsync_busy = false;
  JIFFY++;
  __asm__("ei");
  anime_on_vsync();
}

inline void hsync_handler(void) {
  // ---- HSYNC ----
  // vdp_set_hscroll(...);
  {
    VDP_SET_CONTROL_REGISTER(26, RG26SA);
    VDP_SET_CONTROL_REGISTER(27, RG27SA);
  }
}

static void interrupt_handler(void) {
  // Checking if HSYNC is caught.
  VDP_SET_STATUS_REGISTER_POINTER(1);
  const uint8_t hsync = (VDP_GET_STATUS_REGISTER_VALUE() & 1);
  // Don't forget reset the status register pointer to 0
  VDP_SET_STATUS_REGISTER_POINTER(0);
  if (hsync) {
    hsync_handler();
    // ---- override BIOS VSYNC routine
    if (VDP_GET_STATUS_REGISTER_VALUE() & 0x80) {
      // Catching up on delayed VSYNC.
      // Note that we do not call `vsync_handler()` here.
      vsync_handler_epilogue();
      sound_player();
      return;
    }
    __asm__("ei");
    sound_player();
    return;
  }
  // ---- override BIOS VSYNC routine
  if (VDP_GET_STATUS_REGISTER_VALUE() & 0x80) {
    vsync_handler();
    vsync_handler_epilogue();
  }
}

void set_hscroll(uint16_t x) {
  const uint8_t r27 = (-x) & 0x07;
  const uint8_t r26 = ((x+7) >> 3) & 0x3f;
  // __asm__("di");
  {
    RG26SA = r26;
    RG27SA = r27;
  }
  // __asm__("ei");
}

void setup_interrupt(void) {
  set_hscroll(0);
  vdp_set_control(19, (RG19SA = HSYNC_LINE));
  vdp_set_control(0, (RG0SAV |= 0x10));
  set_interrupt_handler(interrupt_handler);
}
