#include "MidiParam.h"


/****************************************************************/
/* Default Keyboard's parameters                                */
/****************************************************************/
const uint8_t def_kbd_key_hold_count = 2;

const MidiAssign_t def_kbd_key_assign[20] = {
 #if 0  // Patch
  MSG_PC|CH1,   1,            MODE_OS,      0,   // Key1, Acoustic Grand Piano
  MSG_PC|CH1,  28,            MODE_OS,      0,   // Key2, Electric Guitar(clean)
  MSG_PC|CH1,  33,            MODE_OS,      0,   // Key3, Acoustic Bass
  MSG_PC|CH1,  41,            MODE_OS,      0,   // Key4, Violin
 #endif
 #if 0  // MMC
  MSG_MC,      MMC_STOP,      MODE_OS,      0,   // Key1, MMC stop
  MSG_MC,      MMC_PLAY,      MODE_OS,      0,   // Key2, MMC play
  MSG_MC,      MMC_REC,       MODE_OS,      0,   // Key3, MMC record strobe
  MSG_MC,      MMC_FWD,       MODE_OS,      0,   // Key4, MMC fast forward
 #endif
 #if 0  // Custom
  MSG_UD|0,    1,             MODE_OS,      0,   // Key1, Custom Index0 / 8bytes / stop
  MSG_UD|2,    1,             MODE_OS,      0,   // Key2, Custom Index2 / 8bytes / play
  MSG_UD|4,    1,             MODE_OS,      0,   // Key3, Custom Index4 / 8bytes / Fast Forward
  MSG_UD|6,    1,             MODE_OS,      0,   // Key4, Custom Index6 / 8bytes / Rewind
 #endif
 #if 1  // Drum
  MSG_NT|CH10, NT_FIX|NT_B0,  MODE_SG,  CV_LH,   // Key1, Bass Drum 2(35)
  MSG_NT|CH10, NT_FIX|NT_D1,  MODE_SG,  CV_LH,   // Key2, Snare Drum 1(38)
  MSG_NT|CH10, NT_FIX|NT_FS1, MODE_SG,  CV_LH,   // Key3, Closed Hi-hat(42)
  MSG_NT|CH10, NT_FIX|NT_GS2, MODE_SG,  CV_LH,   // Key4, Cowbell(56)
 #endif

  MSG_NT|CH1,  NT_CS3,        MODE_SG,  CV_LH,   // Key5
  MSG_NT|CH1,  NT_DS3,        MODE_SG,  CV_LH,   // Key6
  MSG_CC|CH1,  CC64_SUS,      MODE_TG,  CV_LH,   // Key7, CC64_Sustain
  MSG_NT|CH1,  NT_FS3,        MODE_SG,  CV_LH,   // Key8

  MSG_NT|CH1,  NT_GS3,        MODE_SG,  CV_LH,   // Key9
  MSG_NT|CH1,  NT_AS3,        MODE_SG,  CV_LH,   // Key10
  MSG_PS,      PS_DEC12,      MODE_OS,      0,   // Key11, OctaveShift-
  MSG_PS,      PS_INC12,      MODE_OS,      0,   // Key12, OctaveShift+

  MSG_NT|CH1,  NT_C3,         MODE_SG,  CV_LH,   // Key13
  MSG_NT|CH1,  NT_D3,         MODE_SG,  CV_LH,   // Key14
  MSG_NT|CH1,  NT_E3,         MODE_SG,  CV_LH,   // Key15
  MSG_NT|CH1,  NT_F3,         MODE_SG,  CV_LH,   // Key16

  MSG_NT|CH1,  NT_G3,         MODE_SG,  CV_LH,   // Key17
  MSG_NT|CH1,  NT_A3,         MODE_SG,  CV_LH,   // Key18
  MSG_NT|CH1,  NT_B3,         MODE_SG,  CV_LH,   // Key19
  MSG_NT|CH1,  NT_C4,         MODE_SG,  CV_LH,   // Key20
};

const uint8_t def_kbd_key_userdefined_message[8][4] = {
 #if 1  // empty
  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,
 #endif
 #if 0  // MMC
  /* MMC AllDeivce Stop - F0 7F 7F 06 01 F7, 8 bytes */
  0x04,0xF0,0x7F,0x7F,  0x07,0x06,0x01,0xF7,
  /* MMC AllDeivce Play - F0 7F 7F 06 02 F7, 8 bytes */
  0x04,0xF0,0x7F,0x7F,  0x07,0x06,0x02,0xF7,
  /* MMC AllDeivce Fast Forward - F0 7F 7F 06 04 F7, 8 bytes */
  0x04,0xF0,0x7F,0x7F,  0x07,0x06,0x04,0xF7,
  /* MMC AllDeivce Rewind - F0 7F 7F 06 05 F7, 8 bytes */
  0x04,0xF0,0x7F,0x7F,  0x07,0x06,0x05,0xF7,
 #endif
 #if 0  // Mixed
  /* MMC AllDeivce Record Strobe - F0 7F 7F 06 06 F7, 8 bytes */
  0x04,0xF0,0x7F,0x7F,  0x07,0x06,0x06,0xF7,
  /* CC65(127) -> CC5(50) -> CC84(100), Portamento, 12 bytes */
  0x0B,0xB0,0x41,0x7F,  0x0B,0xB0,0x05,0x32,  0x0B,0xB0,0x54,0x64,
  /* CC100(0) -> CC101(0) -> CC6(12), PitchBend range, 12 bytes */
  0x0B,0xB0,0x64,0x00,  0x0B,0xB0,0x65,0x00,  0x0B,0xB0,0x06,0x0C,
 #endif
};

const uint8_t def_kbd_pot_hold_count = 2;

const MidiAssign_t def_kbd_pot_assign = {
  MSG_CC|CH1,  CC1_MOD,   0,  CV_LH,   // CC1_MOD
};

const uint8_t def_kbd_fsr_hold_count = 2;

const MidiAssign_t def_kbd_fsr_assign = {
  MSG_CP|CH1,        0,   0,  CV_LH,   // ChannelPressure(Aftertouch)
};

const uint8_t def_kbd_joy_hold_count = 2;

const uint8_t def_kbd_joybtn_hold_count = 2;

const MidiAssign_t def_kbd_joy_assign[4] = {
  MSG_NO,       0,    0,    0,          // X-axis-left,   None
  MSG_NO,       0,    0,    0,          // X-axis-right,  None
  MSG_PB|CH1,   0,    0,    CV_ML,      // Y-axis-down,   PitchBend
  MSG_PB|CH1,   0,    0,    CV_MH,      // Y-axis-up,     PitchBend
};

const uint8_t def_kbd_acc_hold_count = 2;

const MidiAssign_t def_kbd_acc_assign[4] = {
  MSG_CC|CH1,  CC10_PAN,  0,   CV_ML,   // X-axis-left,   CC10_PAN
  MSG_CC|CH1,  CC10_PAN,  0,   CV_MH,   // X-axis-right,  CC10_PAN
  MSG_NO,             0,  0,       0,   // Y-axis-down,   None
  MSG_NO,             0,  0,       0,   // Y-axis-up,     None
};

const uint8_t def_kbd_whe_hold_count = 2;

const MidiAssign_t def_kbd_whe_assign[2] = {
  MSG_AB|CH1,  CC7_VOL,   VMUL2,  32,   // wheel left,  Abs, CC7,  x2, initial_value 32
  MSG_RE|CH1,       65,   VMUL1,   0,   // wheel right, Rel, CC65, x1, Portamento On/Off
};

const uint8_t def_kbd_rgbled_mode = MIDI_RGBLED_MODE1_TimingClock;

const uint32_t def_kbd_rgbled_color[2] = {
  COLOR_ORANGE, 0
};

const uint8_t def_kbd_monoled_mode = MIDI_MONOLED_MODE1_Switch;

const uint32_t def_kbd_monoled_switches = 0xFF3BF;



/****************************************************************/
/* Default Pad's parameters                                     */
/****************************************************************/
const uint8_t def_pad_key_hold_count = 2;

const MidiAssign_t def_pad_key_assign[20] = {
  MSG_PS,      PS_C1,     MODE_OS,      0,   // Key1, Move the note scale to C1
  MSG_PS,      PS_E2,     MODE_OS,      0,   // Key2, Move the note scale to E2
  MSG_PS,      PS_GS3,    MODE_OS,      0,   // Key3, Move the note scale to G#3
  MSG_PS,      PS_C5,     MODE_OS,      0,   // Key4, Move the note scale to C5

  MSG_NT|CH1,  NT_GS1,    MODE_SG,  CV_LH,   // Key5,  pad9
  MSG_NT|CH1,  NT_A1,     MODE_SG,  CV_LH,   // Key6,  pad10
  MSG_NT|CH1,  NT_AS1,    MODE_SG,  CV_LH,   // Key7,  pad11
  MSG_NT|CH1,  NT_B1,     MODE_SG,  CV_LH,   // Key8,  pad12

  MSG_NT|CH1,  NT_C2,     MODE_SG,  CV_LH,   // Key9,  pad13
  MSG_NT|CH1,  NT_CS2,    MODE_SG,  CV_LH,   // Key10, pad14
  MSG_NT|CH1,  NT_D2,     MODE_SG,  CV_LH,   // Key11, pad15
  MSG_NT|CH1,  NT_DS2,    MODE_SG,  CV_LH,   // Key12, pad16

  MSG_NT|CH1,  NT_C1,     MODE_SG,  CV_LH,   // Key13, pad1
  MSG_NT|CH1,  NT_CS1,    MODE_SG,  CV_LH,   // Key14, pad2
  MSG_NT|CH1,  NT_D1,     MODE_SG,  CV_LH,   // Key15, pad3
  MSG_NT|CH1,  NT_DS1,    MODE_SG,  CV_LH,   // Key16, pad4

  MSG_NT|CH1,  NT_E1,     MODE_SG,  CV_LH,   // Key17, pad5
  MSG_NT|CH1,  NT_F1,     MODE_SG,  CV_LH,   // Key18, pad6
  MSG_NT|CH1,  NT_FS1,    MODE_SG,  CV_LH,   // Key19, pad7
  MSG_NT|CH1,  NT_G1,     MODE_SG,  CV_LH,   // Key20, pad8
};

const uint8_t def_pad_key_userdefined_message[8][4] = {
  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,
};

const uint8_t def_pad_pot_hold_count = 2;

const MidiAssign_t def_pad_pot_assign = {
  MSG_CC|CH1,  CC1_MOD,   0,  CV_LH,  // CC1_MOD
};

const uint8_t def_pad_fsr_hold_count = 2;

const MidiAssign_t def_pad_fsr_assign = {
  MSG_CP|CH1,         0,  0,  CV_LH,  // ChannelPressure(Aftertouch)
};

const uint8_t def_pad_joy_hold_count = 2;

const uint8_t def_pad_joybtn_hold_count = 2;

const MidiAssign_t def_pad_joy_assign[4] = {
  MSG_CC|CH1,     16,     0,    CV_LH,    // X-axis-left,   CC16 (Usually map to effect)
  MSG_CC|CH1,     17,     0,    CV_LH,    // X-axis-right,  CC17 (Usually map to effect)
  MSG_CC|CH1,     18,     0,    CV_LH,    // Y-axis-down,   CC18 (Usually map to effect)
  MSG_CC|CH1,     19,     0,    CV_LH,    // Y-axis-up,     CC19 (Usually map to effect)
};

const uint8_t def_pad_acc_hold_count = 2;

const MidiAssign_t def_pad_acc_assign[4] = {
  MSG_CC|CH1,  CC10_PAN,  0,    CV_ML,    // X-axis-left,   CC10_PAN
  MSG_CC|CH1,  CC10_PAN,  0,    CV_MH,    // X-axis-right,  CC10_PAN
  MSG_NO,             0,  0,        0,    // Y-axis-down,   None
  MSG_NO,             0,  0,        0,    // Y-axis-up,     None
};

const uint8_t def_pad_whe_hold_count = 2;

const MidiAssign_t def_pad_whe_assign[2] = {
  MSG_AB|CH1,  CC12_EF1,  VMUL2,  16,   // wheel left,  Abs, CC12_EF1, x2, initial_value 16
  MSG_AB|CH1,  CC13_EF2,  VMUL2,  16,   // wheel right, Abs, CC13_EF2, x2, initial_value 16
};

const uint8_t def_pad_rgbled_mode = MIDI_RGBLED_MODE1_TimingClock;

const uint32_t def_pad_rgbled_color[2] = {
  COLOR_BLUE, 0
};

const uint8_t def_pad_monoled_mode = MIDI_MONOLED_MODE2_SwitchHover;

const uint32_t def_pad_monoled_switches = 0x0FF0F;



/****************************************************************/
/* Default DJ's parameters                                     */
/****************************************************************/
const uint8_t def_dj_key_hold_count = 2;

const MidiAssign_t def_dj_key_assign[20] = {
  MSG_PS,      PS_C1,     MODE_OS,      0,   // Key1,  Move the note scale to C1
  MSG_PS,      PS_E2,     MODE_OS,      0,   // Key2,  Move the note scale to E2
  MSG_PS,      PS_GS3,    MODE_OS,      0,   // Key3,  Move the note scale to G#3
  MSG_PS,      PS_C5,     MODE_OS,      0,   // Key4,  Move the note scale to C5

  MSG_NT|CH1,  NT_GS1,    MODE_SG,  CV_LH,   // Key5,  dj9
  MSG_NT|CH1,  NT_A1,     MODE_SG,  CV_LH,   // Key6,  dj10
  MSG_NT|CH1,  NT_AS1,    MODE_SG,  CV_LH,   // Key7,  dj11
  MSG_NT|CH1,  NT_B1,     MODE_SG,  CV_LH,   // Key8,  dj12

  MSG_NT|CH1,  NT_C2,     MODE_SG,  CV_LH,   // Key9,  dj13
  MSG_NT|CH1,  NT_CS2,    MODE_SG,  CV_LH,   // Key10, dj14
  MSG_NT|CH1,  NT_D2,     MODE_SG,  CV_LH,   // Key11, dj15
  MSG_NT|CH1,  NT_DS2,    MODE_SG,  CV_LH,   // Key12, dj16

  MSG_NT|CH1,  NT_C1,     MODE_SG,  CV_LH,   // Key13, dj1
  MSG_NT|CH1,  NT_CS1,    MODE_SG,  CV_LH,   // Key14, dj2
  MSG_NT|CH1,  NT_D1,     MODE_SG,  CV_LH,   // Key15, dj3
  MSG_NT|CH1,  NT_DS1,    MODE_SG,  CV_LH,   // Key16, dj4

  MSG_NT|CH1,  NT_E1,     MODE_SG,  CV_LH,   // Key17, dj5
  MSG_NT|CH1,  NT_F1,     MODE_SG,  CV_LH,   // Key18, dj6
  MSG_NT|CH1,  NT_FS1,    MODE_SG,  CV_LH,   // Key19, dj7
  MSG_NT|CH1,  NT_G1,     MODE_SG,  CV_LH,   // Key20, dj8
};

const uint8_t def_dj_key_userdefined_message[8][4] = {
  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,
};

const uint8_t def_dj_pot_hold_count = 2;

const MidiAssign_t def_dj_pot_assign = {
  MSG_CC|CH1,  1,  0,  CV_LH,   // CC1
};

const uint8_t def_dj_fsr_hold_count = 2;

const MidiAssign_t def_dj_fsr_assign = {
  MSG_CC|CH1,  2,  0,  CV_LH,   // CC2
};

const uint8_t def_dj_joy_hold_count = 2;

const uint8_t def_dj_joybtn_hold_count = 2;

const MidiAssign_t def_dj_joy_assign[4] = {
  MSG_CC|CH1,     16,     0,    CV_LH,    // X-axis-left,   CC16 (Usually map to effect)
  MSG_CC|CH1,     17,     0,    CV_LH,    // X-axis-right,  CC17 (Usually map to effect)
  MSG_CC|CH1,     18,     0,    CV_LH,    // Y-axis-down,   CC18 (Usually map to effect)
  MSG_CC|CH1,     19,     0,    CV_LH,    // Y-axis-up,     CC19 (Usually map to effect)
};

const uint8_t def_dj_acc_hold_count = 2;

const MidiAssign_t def_dj_acc_assign[4] = {
  MSG_CC|CH1,     20,     0,    CV_LH,    // X-axis-left,   CC20 (Usually map to effect)
  MSG_CC|CH1,     21,     0,    CV_LH,    // X-axis-right,  CC21 (Usually map to effect)
  MSG_CC|CH1,     22,     0,    CV_LH,    // Y-axis-down,   CC22 (Usually map to effect)
  MSG_CC|CH1,     23,     0,    CV_LH,    // Y-axis-up,     CC23 (Usually map to effect)
};

const uint8_t def_dj_whe_hold_count = 2;

const MidiAssign_t def_dj_whe_assign[2] = {
  MSG_RE|CH1,  7,   VMUL1,    0,  // wheel left,  Rel, x1, CC7
  MSG_RE|CH1,  8,   VMUL1,    0,  // wheel right, Rel, x1, CC8
};

const uint8_t def_dj_rgbled_mode = MIDI_RGBLED_MODE2_NoteOnOff_DualColor;

const uint32_t def_dj_rgbled_color[2] = {
  COLOR_INDIGO, COLOR_CYAN
};

const uint8_t def_dj_monoled_mode = MIDI_MONOLED_MODE2_SwitchHover;

const uint32_t def_dj_monoled_switches = 0x1111F;



/****************************************************************/
/* Constructor                                                  */
/****************************************************************/
MidiParam::MidiParam(int mode) {
  midi_mode = mode;
}



/****************************************************************/
/* Method                                                       */
/****************************************************************/
void MidiParam::load_default_parameters() {
  if        (midi_mode == APP_MIDI_KB) {
    /* KB */
    load_key_param(def_kbd_key_hold_count, &def_kbd_key_assign[0], &def_kbd_key_userdefined_message[0][0]);
    load_pot_param(def_kbd_pot_hold_count, &def_kbd_pot_assign);
    load_fsr_param(def_kbd_fsr_hold_count, &def_kbd_fsr_assign);
    load_joy_param(def_kbd_joy_hold_count, def_kbd_joybtn_hold_count, &def_kbd_joy_assign[0]);
    load_acc_param(def_kbd_acc_hold_count, &def_kbd_acc_assign[0]);
    load_whe_param(def_kbd_whe_hold_count, &def_kbd_whe_assign[0]);
    load_rgbled_param(def_kbd_rgbled_mode, &def_kbd_rgbled_color[0]);
    load_monoled_param(def_kbd_monoled_mode, def_kbd_monoled_switches);
  } else if (midi_mode == APP_MIDI_PAD) {
    /* Pad */
    load_key_param(def_pad_key_hold_count, &def_pad_key_assign[0], &def_pad_key_userdefined_message[0][0]);
    load_pot_param(def_pad_pot_hold_count, &def_pad_pot_assign);
    load_fsr_param(def_pad_fsr_hold_count, &def_pad_fsr_assign);
    load_joy_param(def_pad_joy_hold_count, def_pad_joybtn_hold_count, &def_pad_joy_assign[0]);
    load_acc_param(def_pad_acc_hold_count, &def_pad_acc_assign[0]);
    load_whe_param(def_pad_whe_hold_count, &def_pad_whe_assign[0]);
    load_rgbled_param(def_pad_rgbled_mode, &def_pad_rgbled_color[0]);
    load_monoled_param(def_pad_monoled_mode, def_pad_monoled_switches);
  } else {
    /* DJ */
    load_key_param(def_dj_key_hold_count, &def_dj_key_assign[0], &def_dj_key_userdefined_message[0][0]);
    load_pot_param(def_dj_pot_hold_count, &def_dj_pot_assign);
    load_fsr_param(def_dj_fsr_hold_count, &def_dj_fsr_assign);
    load_joy_param(def_dj_joy_hold_count, def_dj_joybtn_hold_count, &def_dj_joy_assign[0]);
    load_acc_param(def_dj_acc_hold_count, &def_dj_acc_assign[0]);
    load_whe_param(def_dj_whe_hold_count, &def_dj_whe_assign[0]);
    load_rgbled_param(def_dj_rgbled_mode, &def_dj_rgbled_color[0]);
    load_monoled_param(def_dj_monoled_mode, def_dj_monoled_switches);
  }
}

void MidiParam::load_key_param(const uint8_t cnt, const MidiAssign_t *ass, const uint8_t *ud) {
  key_hold_count = cnt;
  memcpy(key_assign, ass, sizeof(key_assign));
  memcpy(key_userdefined_message, ud, sizeof(key_userdefined_message));
}

void MidiParam::load_pot_param(const uint8_t cnt, const MidiAssign_t *ass) {
  pot_hold_count = cnt;
  memcpy(&pot_assign, ass, sizeof(pot_assign));
}

void MidiParam::load_fsr_param(const uint8_t cnt, const MidiAssign_t *ass) {
  fsr_hold_count = cnt;
  memcpy(&fsr_assign, ass, sizeof(fsr_assign));
}

void MidiParam::load_joy_param(const uint8_t cnt, const uint8_t cnt2, const MidiAssign_t *ass) {
  joy_hold_count = cnt;
  joybtn_hold_count = cnt2;
  memcpy(joy_assign, ass, sizeof(joy_assign));
}

void MidiParam::load_acc_param(const uint8_t cnt, const MidiAssign_t *ass) {
  acc_hold_count = cnt;
  memcpy(acc_assign, ass, sizeof(acc_assign));
}

void MidiParam::load_whe_param(const uint8_t cnt, const MidiAssign_t *ass) {
  whe_hold_count = cnt;
  memcpy(whe_assign, ass, sizeof(whe_assign));
}

void MidiParam::load_rgbled_param(const uint8_t mode, const uint32_t *color) {
  rgbled_mode = mode;
  memcpy((uint8_t *)rgbled_color, (uint8_t *)color, sizeof(rgbled_color));
}

void MidiParam::load_monoled_param(const uint8_t mode, const uint32_t switches) {
  monoled_mode = mode;
  monoled_switches = switches;
}

uint8_t MidiParam::curve_bool(uint8_t idx, uint8_t in) {
  uint8_t out = 0;
  switch (idx) {
    case CV_LH:  out = (in) ? 127 : 0;   break; // 0/1 -> 0/127
    case CV_HL:  out = (in) ? 0 : 127;   break; // 0/1 -> 127/0
    case CV_ML:  out = (in) ? 0 : 64;    break; // 0/1 -> 64/0
    case CV_MH:  out = (in) ? 127 : 64;  break; // 0/1 -> 64/127
  }
  return out;
}

uint8_t MidiParam::curve_linear7bit(uint8_t idx, uint8_t in) {
  uint8_t out = 0;
  switch (idx) {
    case CV_LH:  out = in;                                break;  // 0~127 -> 0~127
    case CV_HL:  out = 127 - in;                          break;  // 0~127 -> 127~0
    case CV_ML:  out = roundf(map(in, 0, 127, 64, 0));    break;  // 0~127 -> 64~0
    case CV_MH:  out = roundf(map(in, 0, 127, 64, 127));  break;  // 0~127 -> 64~127
  }
  return out;
}

uint16_t MidiParam::curve_linear14bit(uint8_t idx, uint16_t in) {
  uint16_t out = 0;
  switch (idx) {
    case CV_LH:  out = in;                                      break;  // 0~16383 -> 0~16383
    case CV_HL:  out = 16383 - in;                              break;  // 0~16383 -> 16383~0
    case CV_ML:  out = roundf(map(in, 0, 16383, 8192, 0));      break;  // 0~16383 -> 8192~0
    case CV_MH:  out = roundf(map(in, 0, 16383, 8192, 16383));  break;  // 0~16383 -> 8192~16383
  }
  return out;
}
