#ifndef APP_COMMON_H
#define APP_COMMON_H

#include "mbed.h"
#include "my_debug.h"
#include "DataFlash.h"
#include "USBMIDI.h"
#include "USBJoystick.h"
#include "MAFilter.h"
#include "SlideRotary.h"
#include "FSR.h"
#include "Joystick.h"
#include "RGBLed.h"
#include "LEDs.h"
#include "Keys.h"
#include "QEI.h"
#include "BMA253.h"
#include "SSD1306.h"
#include "SerialMIDI.h"

enum {
  APP_MIDI_KB  = 0,
  APP_MIDI_PAD = 1,
  APP_MIDI_DJ  = 2,
};

typedef struct {
  DataFlash *df;

  USBMIDI *usbmidi;
  USBJoystick *usbjoy;

  SerialMIDI  *serialmidi;

  SlideRotary *pot; // Fader
  FSR *fsr;         // Pressure
  Joystick *joy;    // Joystick
  RGBLed *rgb;      // Color LED
  LEDs *leds;       // Mono LEDs
  Keys *keys;       // Keys
  QEI *jw[2];       // Jogs

  BMA253 *acc;      // Accelerometer
  SSD1306 *oled;    // OLED display
} AppCtrl_t;

#endif