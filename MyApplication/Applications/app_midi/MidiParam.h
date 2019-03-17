#ifndef MIDIPARAM_H
#define MIDIPARAM_H

#include "mbed.h"
#include "my_utility.h"
#include "app_common.h"

enum MidiAssignMessageType {
  /* None, NoteOnOff, ChannelPressure(Aftertouch) */
  MSG_NO=0x00,  MSG_NT=0x10,  MSG_CP=0x20,
  /* ControlChange, ControlChange_Abs, ControlChange_Rel (Abs and Rel both are dedicated for endless rotary) */
  MSG_CC=0x30,  MSG_AB=0x40,  MSG_RE=0x50,
  /* ProgramChange, PitchBend, PitchShift */
  MSG_PC=0x60,  MSG_PB=0x70,  MSG_PS=0x80,
  /* MMC, UserDefined */
  MSG_MC=0x90,  MSG_UD=0xA0,
};

enum MidiAssignChannelType {
  CH1=0, CH2, CH3, CH4, CH5, CH6, CH7, CH8, CH9, CH10, CH11, CH12, CH13, CH14, CH15, CH16,

  /* UserDefined */
  // Index of CustomizedMessage, range of index 0-7
};

enum MidiAssignNumberType {
  /* Note number */
  /* C      C#      D       D#      E       F       F#      G       G#      A       A#      B */
  NT_C0=24, NT_CS0, NT_D0,  NT_DS0, NT_E0,  NT_F0,  NT_FS0, NT_G0,  NT_GS0, NT_A0,  NT_AS0, NT_B0,
  NT_C1=36, NT_CS1, NT_D1,  NT_DS1, NT_E1,  NT_F1,  NT_FS1, NT_G1,  NT_GS1, NT_A1,  NT_AS1, NT_B1,
  NT_C2=48, NT_CS2, NT_D2,  NT_DS2, NT_E2,  NT_F2,  NT_FS2, NT_G2,  NT_GS2, NT_A2,  NT_AS2, NT_B2,
  NT_C3=60, NT_CS3, NT_D3,  NT_DS3, NT_E3,  NT_F3,  NT_FS3, NT_G3,  NT_GS3, NT_A3,  NT_AS3, NT_B3,
  NT_C4=72, NT_CS4, NT_D4,  NT_DS4, NT_E4,  NT_F4,  NT_FS4, NT_G4,  NT_GS4, NT_A4,  NT_AS4, NT_B4,
  NT_C5=84, NT_CS5, NT_D5,  NT_DS5, NT_E5,  NT_F5,  NT_FS5, NT_G5,  NT_GS5, NT_A5,  NT_AS5, NT_B5,
  NT_C6=96, NT_CS6, NT_D6,  NT_DS6, NT_E6,  NT_F6,  NT_FS6, NT_G6,  NT_GS6, NT_A6,  NT_AS6, NT_B6,
  NT_FIX=0x80,  // This flag mean the note who use it won't affected by PitchShift

  /* Control number */
  CC1_MOD=1,    CC2_BRE=2,    CC4_FOOT=4,   CC5_PTM=5,    CC7_VOL=7,    CC8_BAL=8,
  CC10_PAN=10,  CC11_EXP=11,  CC12_EF1=12,  CC13_EF2=13,  CC64_SUS=64,  CC91_RVB=91,
  CC93_CHO=93,  CC123_ANO=123,

  /* Patch number  */
  // Use the patch number diretly

  /* Pitch shift */
  PS_C1=0, PS_E2, PS_GS3, PS_C5,  // Directly move the note scale which interval is every 16 notes (Used in PAD mode)
  PS_INC12, PS_DEC12,             // Pitch +/- 12 (aka. Octave Up/Down)
  PS_INC1,  PS_DEC1,              // Pitch +/- 1

  /* MMC */
  MMC_STOP=1, MMC_PLAY=2, MMC_FWD=4, MMC_REW=5, MMC_REC=6,

  /* UserDefined */
  // Size of CustomizedMessage, range of size 4*(n+1), n=0~7
  // (While the Max-size is 32 for index 0 that'll override index 1~7)
};

enum MidiAssignModeType {
  /* OneShot, Single(OnOff), Toggle */
  MODE_OS=0, MODE_SG, MODE_TG,

  /* ControlChange_Abs */
  // Foward:   abs_val = orig_val + (move_interval * m), m=1~8
  // Backward: abs_val = orig_val - (move_interval * m), m=1~8

  /* ControlChange_Rel */
  // Foward:   rel_val = 64 + (move_interval * m), m=1~8    [BinOffset] 65~127 -> +1~63
  // Backward: rel_val = 64 - (move_interval * m), m=1~8    [BinOffset] 63~0   -> -1~64

  VMUL1=0, VMUL2, VMUL3, VMUL4, VMUL5, VMUL6, VMUL7, VMUL8,
};

enum MidiAssignValueType {
  /* Curve
   *
   * BOOL,        value 0/127,    e.g. Keys
   * Linear_7bit, value 0~+127,   e.g. Joystick(Up,Right),  Accelerometer(Up,Right), Slider, Rotary, FSR
   *              value 0~-127,   e.g. Joystick(Down,Left), Accelerometer(Down,Left)
   * Linear_14bit value 0~+16383, e.g. Joystick(Up,Right)
   *              value 0~-16383, e.g. Joystick(Down,Left)
   */
           // BOOL           Linear_7bit       Linear_14bit
  CV_LH=0, // 0/1 -> 0/127,  0~127 -> 0~127,   0~16383 -> 0~16383
  CV_HL,   // 0/1 -> 127/0,  0~127 -> 127~0,   0~16383 -> 16383~0
  CV_ML,   // 0/1 -> 64/0,   0~127 -> 64~0,    0~16383 -> 8192~0
  CV_MH,   // 0/1 -> 64/127, 0~127 -> 64~127,  0~16383 -> 8192~16383

  /* ControlChange_Abs */
  // Config the initial value 0~127
};

typedef struct MidiAssignType {
  uint8_t msg_ch; // HighNibble Message, LowNibble Channel
  uint8_t number;
  uint8_t mode;
  uint8_t value;
} MidiAssign_t;

/**** Key Assign ****/
//  message: None/NT/CC/PC/PS/MC/UD
//  channel:
//      NT: (0~15) channel 1~16
//      CC: (0~15) channel 1~16
//      PC: (0~15) channel 1~16
//      PS: Unuse
//      MC: Unuse
//      UD: (0~7)  index
//  number:
//      NT: (0~127, 0x80) note number, fixed flag
//      CC: (0~127) controller number
//      PC: (0~127) patch number
//      PS: (0~7)   Index of how to increase / decrease the pitch
//      MC: MMC_xxx
//      UD: (0~7)   size 4*(n+1)
//  mode:
//      NT: SG
//      CC: OS/SG/TG
//      PC: OS
//      PS: OS
//      MC: OS
//      UD: OS
//  value:
//      NT: CV_xx
//      CC: CV_xx
//      PC: Unuse
//      PS: Unuse
//      MC: Unuse
//      UD: Unuse

/**** Pot Assign ****/
//  message: None/CC
//  channel:
//      CC: (0~15)  channel 1~16
//  number:
//      CC: (0~127) controller number
//  mode:
//      CC: Unuse
//  value:
//      CC: CV_xx

/**** FSR Assign ****/
//  message: None/CC/CP
//  channel:
//      CC: (0~15)  channel 1~16
//      CP: (0~15)  channel 1~16
//  number:
//      CC: (0~127) controller number
//      CP: Unuse
//  mode:
//      CC: Unuse
//      CP: Unuse
//  value:
//      CC: CV_xx
//      CP: CV_xx

/**** Joy Assign ****/
//  message: None/CC/PB
//  channel:
//      CC: (0~15)  channel 1~16
//      PB: (0~15)  channel 1~16
//  number:
//      CC: (0~127) controller number
//      PB: Unuse
//  mode:
//      CC: Unuse
//      PB: Unuse
//  value:
//      CC: CV_xx
//      PB: CV_xx

/**** Acc Assign ****/
//  message: None/CC
//  channel:
//      CC: (0~15)  channel 1~16
//  number:
//      CC: (0~127) controller number
//  mode:
//      CC: Unuse
//  value:
//      CC: CV_xx

/**** Wheel Assign ****/
//  message: None/AB/RE
//  channel:
//      AB: (0~15)  channel 1~16
//      RE: (0~15)  channel 1~16
//  number:
//      AB: (0~127) controller number
//      RE: (0~127) controller number
//  mode:
//      AB: (0~7)   multiple(n+1)
//      RE: (0~7)   multiple(n+1)
//  value:
//      AB: (0~127) Initial_value
//      RE: Unuse

enum MIDI_RGBLED_ModeType {
  MIDI_RGBLED_MODE0_TurnOff=0,           // Turn off
  MIDI_RGBLED_MODE1_TimingClock,         // Sync by TimingClock, one color(customize)
  MIDI_RGBLED_MODE2_NoteOnOff_DualColor, // Sync by NoteOn/Off, dual color(customize) change by fader L/R, DJ mode
};

enum MIDI_MONOLED_ModeType {
  MIDI_MONOLED_MODE0_TurnOff=0,          // Turn off
  MIDI_MONOLED_MODE1_Switch,             // Switch the LED1~20 On/Off
  MIDI_MONOLED_MODE2_SwitchHover,        // Switch the LED1~16 On/Off and one of the LED17~20 will be hover while button pressed
};



class MidiParam {
 public:
  MidiParam(int mode);

  uint8_t midi_mode;

  void load_default_parameters();


  /****************************************************************/
  /* Keys                                                         */
  /****************************************************************/
  /**
   * Delay between On and Off, Unit: 10ms
   */
  uint8_t key_hold_count;

  /**
   * Assign key function
   */
  MidiAssign_t key_assign[20];

  /**
   * User define message
   *  Refer to "USB Device Class Definition for MIDI Devices", chapter "4 USB-MIDI Event Packets" page 16
   *  Packet size 4 bytes for each
   */
  uint8_t key_userdefined_message[8][4];

  /**
   * Load key's parameters from memory
   * @param cnt key_hold_count
   * @param ass key_assign
   * @param cus key_custom_message
   */
  void load_key_param(const uint8_t cnt, const MidiAssign_t *ass, const uint8_t *cus);



  /****************************************************************/
  /* Potentiometer                                                */
  /****************************************************************/
  /**
   * Delay between On and Off, Unit: 10ms
   */
  uint8_t pot_hold_count;

  /**
   * Assign pot function
   */
  MidiAssign_t pot_assign;

  /**
   * Load pot's parameters from memory
   * @param cnt pot_hold_count
   * @param ass pot_assign
   */
  void load_pot_param(const uint8_t cnt, const MidiAssign_t *ass);



  /****************************************************************/
  /* Force sensing resistor                                       */
  /****************************************************************/
  /**
   * Delay between On and Off, Unit: 10ms
   */
  uint8_t fsr_hold_count;

  /**
   * Assign pot function
   */
  MidiAssign_t fsr_assign;

  /**
   * Load fsr's parameters from memory
   * @param cnt pot_hold_count
   * @param ass pot_assign
   */
  void load_fsr_param(const uint8_t cnt, const MidiAssign_t *ass);



  /****************************************************************/
  /* Joystick                                                     */
  /****************************************************************/
  /**
   * Delay between this and last time update, Unit: 10ms
   */
  uint8_t joy_hold_count;

  /**
   * Delay between button's On and Off, Unit: 10ms
   */
  uint8_t joybtn_hold_count;

  /**
   * Assign joy function
   */
  MidiAssign_t joy_assign[4];

  /**
   * Load pot's parameters from memory
   * @param cnt  joy_hold_count
   * @param cnt2 joybtn_hold_count
   * @param ass  joy_assign
   */
  void load_joy_param(const uint8_t cnt, const uint8_t cnt2, const MidiAssign_t *ass);



  /****************************************************************/
  /* Accelerometer                                                */
  /****************************************************************/
  /**
   * Delay between this and last time update, Unit: 10ms
   */
  uint8_t acc_hold_count;

  /**
   * Assign acc function
   */
  MidiAssign_t acc_assign[4];

  /**
   * Load pot's parameters from memory
   * @param cnt acc_hold_count
   * @param ass acc_assign
   */
  void load_acc_param(const uint8_t cnt, const MidiAssign_t *ass);



  /****************************************************************/
  /* Wheel                                                        */
  /****************************************************************/
  /**
   * Delay between this and last time update, Unit: 10ms
   */
  uint8_t whe_hold_count;

  /**
   * Assign whe function
   */
  MidiAssign_t whe_assign[2];

  /**
   * Load whe's parameters from memory
   * @param cnt whe_hold_count
   * @param ass whe_assign
   */
  void load_whe_param(const uint8_t cnt, const MidiAssign_t *ass);



  /****************************************************************/
  /* RGBLED                                                      */
  /****************************************************************/
  /**
   * RGBLED configuration (MIDI_RGBLED_ModeType)
   */
  uint8_t rgbled_mode;

  /**
   * RGBLED color1 color2, format: XX RR GG BB
   */
  uint32_t rgbled_color[2];

  /**
   * Load RGBLED's parameters from memory
   * @param mode rgbled_mode
   * @param color rgbled_color
   */
  void load_rgbled_param(const uint8_t mode, const uint32_t *color);



  /****************************************************************/
  /* MONOLED                                                      */
  /****************************************************************/
  /**
   * MONOLED configuration (MIDI_MONOLED_ModeType)
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



  /****************************************************************/
  /* Utility function                                             */
  /****************************************************************/
  /**
   * Convert data by selected curve
   * @param idx index of curve (CV_xx)
   * @param in input data
   * @return output data
   */
  uint8_t curve_bool(uint8_t idx, uint8_t in);

  /**
   * Convert data by selected curve
   * @param idx index of curve (CV_xx)
   * @param in input data
   * @return output data
   */
  uint8_t curve_linear7bit(uint8_t idx, uint8_t in);

  /**
   * Convert data by selected curve
   * @param idx index of curve (CV_xx)
   * @param in input data
   * @return output data
   */
  uint16_t curve_linear14bit(uint8_t idx, uint16_t in);
};

#endif