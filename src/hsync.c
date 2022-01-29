// -*- coding: utf-8-unix -*-

#include "smb1.h"

#define HSYNC_LINE (13)

inline uint8_t is_vsync(void) {
  return (VDP_GET_STATUS_REGISTER_VALUE() & 0x80);
}

inline uint8_t is_hsync(void) {
  VDP_SET_STATUS_REGISTER_POINTER(1);
  uint8_t hsync = (VDP_GET_STATUS_REGISTER_VALUE() & 1);
  VDP_SET_STATUS_REGISTER_POINTER(0);
  return hsync;
}

/**
 * Timing-critical VSYNC interrupt handler.
 */
inline void tc_vsync_handler(void) {
  // vdp_set_hscroll(0);
  {
    VDP_SET_CONTROL_REGISTER(26, 0);
    VDP_SET_CONTROL_REGISTER(27, 0);
  }
}

/**
 * VSYNC interrupt handler.
 */
static void vsync_handler(void) {
  vsync_busy = false;
  JIFFY++;
}

/**
 * Non-critical VSYNC handler (called with interrupts enabled).
 */
static void on_vsync(void) {
  anime_on_vsync();
}

/**
 * Timing-critical HSYNC interrupt handler.
 */
inline void tc_hsync_handler(void) {
  // ---- HSYNC ----
  // vdp_set_hscroll(...);
  {
    VDP_SET_CONTROL_REGISTER(26, RG26SA);
    VDP_SET_CONTROL_REGISTER(27, RG27SA);
  }
}

/**
 * Non-critical HSYNC handler (called with interrupts enabled).
 */
static void on_hsync(void) {
  sound_player();
}

static void interrupt_handler(void) {
  if (is_hsync()) {
    tc_hsync_handler();
    // ---- override BIOS VSYNC routine
    if (is_vsync()) {
      // tc_vsync_handler();    // Don't call `vsync_handler()` here. (too late)
      vsync_handler();          // Catching up on delayed VSYNC.
      __asm__("ei");
      on_vsync();
      on_hsync();
      return;
    }
    __asm__("ei");
    on_hsync();
    return;
  }
  // ---- override BIOS VSYNC routine
  if (is_vsync()) {
    tc_vsync_handler();
    vsync_handler();
    __asm__("ei");
    on_vsync();
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
