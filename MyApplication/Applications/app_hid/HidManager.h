#ifndef HIDMANAGER_H
#define HIDMANAGER_H

#include "mbed.h"
#include "MAFilter.h"


class HidManager {
 public:
  HidManager() {
    pot_filter = new MAFilter<int8_t, int16_t>(4);
    fsr_filter = new MAFilter<int8_t, int16_t>(4);

    for (int i=0; i<2; i++) {
      joy_filter[i] = new MAFilter<int8_t, int16_t>(8);
      acc_filter[i] = new MAFilter<int8_t, int16_t>(8);
    }
  }

  /* Keys */

  /* Potentiometer */
  MAFilter<int8_t, int16_t> *pot_filter;

  /* Force sensing resistor */
  MAFilter<int8_t, int16_t> *fsr_filter;

  /* Joystick */
  MAFilter<int8_t, int16_t> *joy_filter[2];

  /* Accelerometer */
  MAFilter<int8_t, int16_t> *acc_filter[2];

  /* Wheel */
  int8_t whe_value[2] = {0};
};

#endif