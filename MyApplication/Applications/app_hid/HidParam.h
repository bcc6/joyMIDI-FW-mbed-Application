#ifndef HIDPARAM_H
#define HIDPARAM_H

#include "mbed.h"
#include "my_utility.h"
#include "app_common.h"


// ======== Button / Key ========
// Button (btn) : Mean logical
// Key    (key) : Mean physical


enum HidAssignPhysicalControlType {
  CTRL_NONE = 0,

  /* Joystick */
  CTRL_JOY_XY,

  /* Accelerometer */
  CTRL_ACC_XY,

  /* Jogs */
  CTRL_JOG_LR,

  /* Keys */
  CTRL_KEY_1,   CTRL_KEY_2,   CTRL_KEY_3,   CTRL_KEY_4,
  CTRL_KEY_5,   CTRL_KEY_6,   CTRL_KEY_7,   CTRL_KEY_8,
  CTRL_KEY_9,   CTRL_KEY_10,  CTRL_KEY_11,  CTRL_KEY_12,
  CTRL_KEY_13,  CTRL_KEY_14,  CTRL_KEY_15,  CTRL_KEY_16,
  CTRL_KEY_17,  CTRL_KEY_18,  CTRL_KEY_19,  CTRL_KEY_20,
  CTRL_KEY_21,  // Joystick's center-key

  /* Key_N + Joystick (Compound key) */
  CTRL_KEY_1_JOY_XY,   CTRL_KEY_2_JOY_XY,   CTRL_KEY_3_JOY_XY,   CTRL_KEY_4_JOY_XY,
  CTRL_KEY_5_JOY_XY,   CTRL_KEY_6_JOY_XY,   CTRL_KEY_7_JOY_XY,   CTRL_KEY_8_JOY_XY,
  CTRL_KEY_9_JOY_XY,   CTRL_KEY_10_JOY_XY,  CTRL_KEY_11_JOY_XY,  CTRL_KEY_12_JOY_XY,
  CTRL_KEY_13_JOY_XY,  CTRL_KEY_14_JOY_XY,  CTRL_KEY_15_JOY_XY,  CTRL_KEY_16_JOY_XY,
  CTRL_KEY_17_JOY_XY,  CTRL_KEY_18_JOY_XY,  CTRL_KEY_19_JOY_XY,  CTRL_KEY_20_JOY_XY,
  CTRL_KEY_21_JOY_XY,  // Joystick's switch

  /* Key {5, 6, 14, 13} mapping to POV-hat{North, East, South, West} */
  CTRL_KEY_5_6_14_13_NESW,
};

typedef struct HidAssignType {
  uint8_t xy;         // Logical X/Y axis
  uint8_t tr;         // Logical Throttle/rudder
  uint8_t btn[20+1];  // Logical Button1 ~ 21
  uint8_t hat;        // Logical POV-hat
} HidAssign_t;

/**** Control Assign ****/
//  Function | Component
//  ---------|----------------------
//  X/Y      | NONE/JOY/ACC
//  T/R      | NONE/JOY/ACC/JOG/KEY+JOY
//  BTN      | NONE/KEY
//  HAT      | NONE/KEY/KEY+JOY

enum HID_MONOLED_ModeType {
  HID_MONOLED_MODE0_TurnOff=0,  // Turn off
  HID_MONOLED_MODE1_Switch,     // Switch the LED1~20 On/Off
};



class HidParam {
 public:
  HidParam();

  void load_default_parameters();


  /****************************************************************/
  /* Controler physical to logical mapping                        */
  /****************************************************************/
  /**
   * Assign the physical to logical mappping
   */
  HidAssign_t p2l_assign;

  /**
   * Jog's gain, Value = Interval * (gain + 1)
   */
  uint8_t p2l_jog_gain;

  /**
   * Load P2L's parameters from memory
   * @param ass p2l_assign
   * @param gain p2l_jog_gain
   */
  void load_p2l_param(const HidAssign_t *ass, const uint8_t jog_gain);


  /****************************************************************/
  /* MONOLED                                                      */
  /****************************************************************/
  /**
   * MONOLED configuration (HID_MONOLED_ModeType)
   */
  uint8_t monoled_mode;

  /**
   * MONOLED On/Off switches
   *  Bit0~19 map to Key1~20
   */
  uint32_t monoled_switches;

  /**
   * Load MONOLED's parameters from memory
   * @param mode monoled_mode
   * @param color monoled_switches
   */
  void load_monoled_param(const uint8_t mode, const uint32_t switches);
};

#endif