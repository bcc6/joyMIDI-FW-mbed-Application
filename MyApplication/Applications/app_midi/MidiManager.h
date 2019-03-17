#ifndef MIDIMANAGER_H
#define MIDIMANAGER_H

#include "mbed.h"
#include "MAFilter.h"

typedef struct KeyManagerType {
  uint8_t step;    // step of state machine
  uint8_t count;   // elapsed time (count * timebase)
  uint8_t toggle;  // phase of toggle mode
  uint8_t channel; // store the channel-number to ensure completed execute cycle
  uint8_t number;  // store the note/cc-number to ensure completed execute cycle
} KeyManager_t;

typedef struct PotManagerType {
  uint8_t step;    // step of state machine
  uint8_t count;   // elapsed time (count * timebase)
  uint8_t value;   // store the input-value to be used on prevent repeat data
} PotManager_t;

typedef struct FsrManagerType {
  uint8_t step;    // step of state machine
  uint8_t count;   // elapsed time (count * timebase)
  uint8_t value;   // store the input-value to be used on prevent repeat data
} FsrManager_t;

typedef struct JoyManagerType {
  uint8_t step;    // step of state machine
  uint8_t count;   // elapsed time (count * timebase)
  uint16_t value;  // store the input-value to be used on prevent repeat data
} JoyManager_t;

typedef struct JoyBtnManagerType {
  uint8_t step;    // step of state machine
  uint8_t count;   // elapsed time (count * timebase)
} JoyBtnManager_t;

typedef struct AccManagerType {
  uint8_t step;    // step of state machine
  uint8_t count;   // elapsed time (count * timebase)
  uint8_t value;   // store the input-value to be used on prevent repeat data
} AccManager_t;

typedef struct WheManagerType {
  uint8_t step;    // step of state machine
  uint8_t count;   // elapsed time (count * timebase)
  uint8_t value;   // CC_Abs: Load the initial-value then modify it, CC_Rel: Temporarily buffer
} WheManager_t;



class MidiManager {
 public:
  MidiManager() {
    pot_filter = new MAFilter<int8_t, int16_t>(4);
    fsr_filter = new MAFilter<int8_t, int16_t>(4);

    for (int i=0; i<4; i++) {
      joy_filter[i] = new MAFilter<int16_t, int32_t>(8);
      acc_filter[i] = new MAFilter<int8_t, int16_t>(8);
    }
  }

  /* Keys */
  KeyManager_t key_manager[20] = {0};
  int8_t pitch_shift = 0;  // Range (+/-5)*12
                           // 0 means default C3(60) while keyboard mode
                           // 0 means default C1(36) while pad mode

  /* Potentiometer */
  PotManager_t pot_manager = {0};
  MAFilter<int8_t, int16_t> *pot_filter;

  /* Force sensing resistor */
  FsrManager_t fsr_manager = {0};
  MAFilter<int8_t, int16_t> *fsr_filter;

  /* Joystick */
  JoyManager_t joy_manager[4] = {0};
  MAFilter<int16_t, int32_t> *joy_filter[4];
  JoyBtnManager_t joybtn_manager = {0};     // Joystick's button

  /* Accelerometer */
  bool acc_enable = 0;
  AccManager_t acc_manager[4] = {0};
  MAFilter<int8_t, int16_t> *acc_filter[4];

  /* Wheel */
  WheManager_t whe_manager[2] = {0};
};

#endif