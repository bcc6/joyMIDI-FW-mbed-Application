#include "HidParam.h"


/****************************************************************/
/* Default parameters                                           */
/****************************************************************/
const HidAssign_t def_p2l_assign = {
  CTRL_JOY_XY,   // X/Y
  CTRL_JOG_LR,   // T/R

  CTRL_KEY_1,   CTRL_KEY_2,   CTRL_KEY_3,   CTRL_KEY_4,   // Button 1  ~ 4
  CTRL_KEY_5,   CTRL_KEY_6,   CTRL_KEY_7,   CTRL_KEY_8,   // Button 5  ~ 8
  CTRL_KEY_9,   CTRL_KEY_10,  CTRL_KEY_11,  CTRL_KEY_12,  // Button 9  ~ 12
  CTRL_KEY_13,  CTRL_KEY_14,  CTRL_KEY_15,  CTRL_KEY_16,  // Button 13 ~ 16
  CTRL_KEY_17,  CTRL_KEY_18,  CTRL_KEY_19,  CTRL_NONE,    // Button 17 ~ 20 (Not available in PPSSPP)
  CTRL_KEY_21,                                            // Button 21      (Not available in PPSSPP)

  CTRL_KEY_20_JOY_XY, // POV-hat
};

const uint8_t def_p2l_jog_gain = 9; // 10 = gain(9+1) 

const uint8_t def_monoled_mode = HID_MONOLED_MODE1_Switch;

const uint32_t def_monoled_switches = 0xFFFFF;



/****************************************************************/
/* Constructor                                                  */
/****************************************************************/
HidParam::HidParam() {}



/****************************************************************/
/* Method                                                       */
/****************************************************************/
void HidParam::load_default_parameters() {
    load_p2l_param(&def_p2l_assign, def_p2l_jog_gain);
    load_monoled_param(def_monoled_mode, def_monoled_switches);
}

void HidParam::load_p2l_param(const HidAssign_t *ass, const uint8_t jog_gain) {
  memcpy(&p2l_assign, ass, sizeof(p2l_assign));
  p2l_jog_gain = jog_gain;
}

void HidParam::load_monoled_param(const uint8_t mode, const uint32_t switches) {
  monoled_mode = mode;
  monoled_switches = switches;
}