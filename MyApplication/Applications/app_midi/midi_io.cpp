#include "midi_io.h"


//==============================================================================================
// MIDI In
//==============================================================================================
struct {
  RGBLed  *ctrl;
  uint8_t  mode;
  uint32_t color[2];
  uint8_t  count;
} usbmidiin_rgbled;

static void onUsbNoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
  // DEBUG_OUT("onUsbNoteOff(channel=%d, note=%d, velocity=%d)\n", channel, note, velocity);

  if (usbmidiin_rgbled.mode == MIDI_RGBLED_MODE2_NoteOnOff_DualColor) {
    if ((channel == 1 || channel == 2) && (note==0)) {
      // LED off
      usbmidiin_rgbled.ctrl->write(0);
    }
  }
}

static void onUsbNoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
  // DEBUG_OUT("onUsbNoteOn(channel=%d, note=%d, velocity=%d)\n", channel, note, velocity);

  if (usbmidiin_rgbled.mode == MIDI_RGBLED_MODE2_NoteOnOff_DualColor) {
    if ((channel == 1 || channel == 2) && (note==0)) {
      // LED on
      if (channel == 1) usbmidiin_rgbled.ctrl->write(usbmidiin_rgbled.color[0]);
      else              usbmidiin_rgbled.ctrl->write(usbmidiin_rgbled.color[1]);
    }
  }
}

static void onUsbPolyAftertouch(uint8_t channel, uint8_t note, uint8_t pressure) {
  // DEBUG_OUT("onUsbPolyAftertouch(channel=%d, note=%d, pressure=%d)\n", channel, note, pressure);
}

static void onUsbControlChange(uint8_t channel, uint8_t function, uint8_t value) {
  // DEBUG_OUT("onUsbControlChange(channel=%d, function=%d, value=%d)\n", channel, function, value);
}

static void onUsbProgramChange(uint8_t channel, uint8_t program) {
  // DEBUG_OUT("onUsbProgramChange(channel=%d, program=%d)\n", channel, program);
}

static void onUsbChannelAftertouch(uint8_t channel, uint8_t pressure) {
  // DEBUG_OUT("onUsbChannelAftertouch(channel=%d, pressure=%d)\n", channel, pressure);
}

static void onUsbPitchBend(uint8_t channel, uint16_t amount) {
  // DEBUG_OUT("onUsbPitchBend(channel=%d, amount=%d)\n", channel, amount);
}

static void onUsbSystemExclusive(uint8_t *pkt, uint16_t length, bool hasNextPkt) {
  // DEBUG_OUT("onUsbSystemExclusive(pkt=..., length=%d, hasNextPkt=%d)\n", length, hasNextPkt?1:0);
  // HEX_DUMP("pkt", pkt, length);
}

static void onUsbTimeCodeQuarterFrame(uint8_t timing) {
  // DEBUG_OUT("onUsbTimeCodeQuarterFrame(timing=%d)\n", timing);
}

static void onUsbSongPositionPointer(uint16_t position) {
  // DEBUG_OUT("onUsbSongPositionPointer(position=%d)\n", position);
}

static void onUsbSongSelect(uint8_t song) {
  // DEBUG_OUT("onUsbSongSelect(song=%d)\n", song);
}

static void onUsbTuneRequest() {
  // DEBUG_OUT("onUsbTuneRequest()\n");
}

static void onUsbTimingClock() {
  // DEBUG_OUT("onUsbTimingClock()\n");

  if (usbmidiin_rgbled.mode == MIDI_RGBLED_MODE1_TimingClock) {
    if        (usbmidiin_rgbled.count == 0) {
      usbmidiin_rgbled.ctrl->write(usbmidiin_rgbled.color[0]);  // LED on
    } else if (usbmidiin_rgbled.count == 12) {
      usbmidiin_rgbled.ctrl->write(0);                    // LED off
    }
    if (++usbmidiin_rgbled.count == 24) {
      usbmidiin_rgbled.count = 0;                         // clear count
    }
  }
}

static void onUsbStart() {
  // DEBUG_OUT("onUsbStart()\n");
}

static void onUsbContinue() {
  // DEBUG_OUT("onUsbContinue()\n");
}

static void onUsbStop() {
  // DEBUG_OUT("onUsbStop()\n");

  if (usbmidiin_rgbled.mode == MIDI_RGBLED_MODE1_TimingClock) {
    usbmidiin_rgbled.ctrl->write(0);  // LED off
    usbmidiin_rgbled.count = 0;       // clear count
  }
}

static void onUsbActiveSensing() {
  // DEBUG_OUT("onUsbActiveSensing()\n");
}

static void onUsbReset() {
    // DEBUG_OUT("onUsbReset()\n");
}


#if (MBED_CONF_APP_RUN_MODE == 0)

struct {
  USBMIDI  *ctrl;
} serialmidiin_usbmidiout;

static void onSerialNoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
  serialmidiin_usbmidiout.ctrl->sendNoteOff(channel, note, velocity);
}

static void onSerialNoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
  serialmidiin_usbmidiout.ctrl->sendNoteOn(channel, note, velocity);
}

static void onSerialPolyAftertouch(uint8_t channel, uint8_t note, uint8_t pressure) {
  serialmidiin_usbmidiout.ctrl->sendPolyAftertouch(channel, note, pressure);
}

static void onSerialControlChange(uint8_t channel, uint8_t function, uint8_t value) {
  serialmidiin_usbmidiout.ctrl->sendControlChange(channel, function, value);
}

static void onSerialProgramChange(uint8_t channel, uint8_t program) {
  serialmidiin_usbmidiout.ctrl->sendProgramChange(channel, program);
}

static void onSerialChannelAftertouch(uint8_t channel, uint8_t pressure) {
  serialmidiin_usbmidiout.ctrl->sendChannelAftertouch(channel, pressure);
}

static void onSerialPitchBend(uint8_t channel, uint16_t amount) {
  serialmidiin_usbmidiout.ctrl->sendPitchBend(channel, amount);
}

static void onSerialSystemExclusive(uint8_t *pkt, uint8_t length) {
  serialmidiin_usbmidiout.ctrl->sendSystemExclusive(pkt, length, false);
}

static void onSerialTimeCodeQuarterFrame(uint8_t timing) {
  serialmidiin_usbmidiout.ctrl->sendTimeCodeQuarterFrame(timing);
}

static void onSerialSongPositionPointer(uint16_t position) {
  serialmidiin_usbmidiout.ctrl->sendSongPositionPointer(position);
}

static void onSerialSongSelect(uint8_t song) {
  serialmidiin_usbmidiout.ctrl->sendSongSelect(song);
}

static void onSerialTuneRequest() {
  serialmidiin_usbmidiout.ctrl->sendTuneRequest();
}

static void onSerialTimingClock() {
  serialmidiin_usbmidiout.ctrl->sendTimingClock();
}

static void onSerialStart() {
  serialmidiin_usbmidiout.ctrl->sendStart();
}

static void onSerialContinue() {
  serialmidiin_usbmidiout.ctrl->sendContinue();
}

static void onSerialStop() {
  serialmidiin_usbmidiout.ctrl->sendStop();
}

static void onSerialActiveSensing() {
  serialmidiin_usbmidiout.ctrl->sendActiveSensing();
}

static void onSerialReset() {
  serialmidiin_usbmidiout.ctrl->sendReset();
}

#endif


void midi_io_in(AppCtrl_t &ac, MidiParam *param, MidiManager *manager) {
  /* USB MIDI-IN */
  USBMIDI *usbmidi = ac.usbmidi;
  usbmidi->attachNoteOff(callback(&onUsbNoteOff));
  usbmidi->attachNoteOn(callback(&onUsbNoteOn));
  usbmidi->attachPolyAftertouch(callback(&onUsbPolyAftertouch));
  usbmidi->attachControlChange(callback(&onUsbControlChange));
  usbmidi->attachProgramChange(callback(&onUsbProgramChange));
  usbmidi->attachChannelAftertouch(callback(&onUsbChannelAftertouch));
  usbmidi->attachPitchBend(callback(&onUsbPitchBend));
  usbmidi->attachSystemExclusive(callback(&onUsbSystemExclusive));
  usbmidi->attachTimeCodeQuarterFrame(callback(&onUsbTimeCodeQuarterFrame));
  usbmidi->attachSongPositionPointer(callback(&onUsbSongPositionPointer));
  usbmidi->attachSongSelect(callback(&onUsbSongSelect));
  usbmidi->attachTuneRequest(callback(&onUsbTuneRequest));
  usbmidi->attachTimingClock(callback(&onUsbTimingClock));
  usbmidi->attachStart(callback(&onUsbStart));
  usbmidi->attachContinue(callback(&onUsbContinue));
  usbmidi->attachStop(callback(&onUsbStop));
  usbmidi->attachActiveSensing(callback(&onUsbActiveSensing));
  usbmidi->attachReset(callback(&onUsbReset));
  usbmidiin_rgbled.ctrl     = ac.rgb;
  usbmidiin_rgbled.mode     = param->rgbled_mode;
  usbmidiin_rgbled.color[0] = param->rgbled_color[0];
  usbmidiin_rgbled.color[1] = param->rgbled_color[1];
  usbmidiin_rgbled.count    = 0;

#if (MBED_CONF_APP_RUN_MODE == 0)
  /* Serial MIDI-IN */
  SerialMIDI *serialmidi = ac.serialmidi;
  serialmidi->attachNoteOff(callback(&onSerialNoteOff));
  serialmidi->attachNoteOn(callback(&onSerialNoteOn));
  serialmidi->attachPolyAftertouch(callback(&onSerialPolyAftertouch));
  serialmidi->attachControlChange(callback(&onSerialControlChange));
  serialmidi->attachProgramChange(callback(&onSerialProgramChange));
  serialmidi->attachChannelAftertouch(callback(&onSerialChannelAftertouch));
  serialmidi->attachPitchBend(callback(&onSerialPitchBend));
  serialmidi->attachSystemExclusive(callback(&onSerialSystemExclusive));
  serialmidi->attachTimeCodeQuarterFrame(callback(&onSerialTimeCodeQuarterFrame));
  serialmidi->attachSongPositionPointer(callback(&onSerialSongPositionPointer));
  serialmidi->attachSongSelect(callback(&onSerialSongSelect));
  serialmidi->attachTuneRequest(callback(&onSerialTuneRequest));
  serialmidi->attachTimingClock(callback(&onSerialTimingClock));
  serialmidi->attachStart(callback(&onSerialStart));
  serialmidi->attachContinue(callback(&onSerialContinue));
  serialmidi->attachStop(callback(&onSerialStop));
  serialmidi->attachActiveSensing(callback(&onSerialActiveSensing));
  serialmidi->attachReset(callback(&onSerialReset));
  serialmidiin_usbmidiout.ctrl = ac.usbmidi;
#endif
}



//==============================================================================================
// MIDI Out
//==============================================================================================
static void key_nt(uint8_t kn, int8_t kv, AppCtrl_t &ac, MidiParam *param, MidiManager *manager, USBMIDI *usbmidi) {
  MidiAssign_t  &ka           = param->key_assign[kn];
  uint8_t       &hold_count   = param->key_hold_count;
  KeyManager_t  &km           = manager->key_manager[kn];
  int8_t        &pitch_shift  = manager->pitch_shift;

  while (1) {
    switch (km.step) {
      //--------------------------------------------------------------------------------------
      // Pressed
      case 0x00:{ // Check if the key is pressed
        if (kv > 0) {
          km.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
      } else {
          km.step = 0x00;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x01:{ // Send NoteOn
        uint8_t c = km.channel = (ka.msg_ch & 0x0F);
        uint8_t n = km.number  = (ka.number & NT_FIX) ? (ka.number & ~NT_FIX) : ((int8_t)ka.number + pitch_shift);
        uint8_t v              = param->curve_bool(ka.value, kv);
        usbmidi->sendNoteOn(c, n, v);
        midi_ui_write(ac, " NT %2d %3d", (c+1), n);
        km.count = 0;
        km.step = 0x02;
        goto DO_IT_NEXT_TIME;
      }
      case 0x02:{ // Check if the end of Note hold-time is arrival
        if (++km.count >= hold_count) {
          km.step = 0x10;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          km.step = 0x02;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

      //--------------------------------------------------------------------------------------
      // Released
      case 0x10:{ // Check if the key is released
        if (kv == 0) {
          km.step = 0x11;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          km.step = 0x10;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x11:{ // Send NoteOff
        uint8_t c = km.channel;
        uint8_t n = km.number;
        uint8_t v = 0;
        usbmidi->sendNoteOff(c, n, v);
        km.count = 0;
        km.step = 0x12;
        goto DO_IT_NEXT_TIME;
      }
      case 0x12:{ // Check if the end of Note hold-time is arrival
        if (++km.count >= hold_count) {
          km.step = 0x00;         // Yes
          goto DO_IT_RIGHT_NOW;
        }else {
          km.step = 0x12;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

static void key_cc(uint8_t kn, int8_t kv, AppCtrl_t &ac, MidiParam *param, MidiManager *manager, USBMIDI *usbmidi) {
  MidiAssign_t  &ka           = param->key_assign[kn];
  uint8_t       &hold_count   = param->key_hold_count;
  KeyManager_t  &km           = manager->key_manager[kn];

  while (1) {
    switch (km.step) {
      //--------------------------------------------------------------------------------------
      // Pressed
      case 0x00:{ // Check if the key is pressed
        if (kv > 0) {
          km.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
      } else {
          km.step = 0x00;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x01:{ // Check if the mode is toggle
        if (ka.mode == MODE_TG) {
          km.step = 0x02;         // Yes (TG)
          goto DO_IT_RIGHT_NOW;
      } else {
          km.step = 0x03;         // No  (OS, SG)
          goto DO_IT_RIGHT_NOW;
        }
      }
      case 0x02:{ // Check if the toggle phase is 0
        if (km.toggle == 0) {
          km.toggle = 1;
          km.step = 0x03;         // Yes
          goto DO_IT_RIGHT_NOW;
      } else {
          km.toggle = 0;
          km.step = 0x04;         // No
          goto DO_IT_RIGHT_NOW;
        }
      }
      case 0x03:{ // Send ControlChange start
        uint8_t c = km.channel = (ka.msg_ch & 0x0F);
        uint8_t n = km.number  = ka.number;
        uint8_t v              = param->curve_bool(ka.value, kv);
        usbmidi->sendControlChange(c, n, v);
        midi_ui_write(ac, " CC %2d %3d", (c+1), n);
        km.count = 0;
        km.step = 0x05;
        goto DO_IT_NEXT_TIME;
      }
      case 0x04:{ // Send ControlChange end
        uint8_t c = km.channel;
        uint8_t n = km.number;
        uint8_t v = 0;
        usbmidi->sendControlChange(c, n, v);
        midi_ui_write(ac, " CC %2d %3d", (c+1), n);
        km.count = 0;
        km.step = 0x05;
        goto DO_IT_NEXT_TIME;
      }
      case 0x05:{ // Check if the end of ControlChange hold-time is arrival
        if (++km.count >= hold_count) {
          km.step = 0x10;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          km.step = 0x05;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

      //--------------------------------------------------------------------------------------
      // Released
      case 0x10:{ // Check if the key is released
        if (kv == 0) {
          km.step = 0x11;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          km.step = 0x10;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x11:{ // Check if the mode is one-shot or toggle        
        if ((ka.mode == MODE_OS) || (ka.mode == MODE_TG)) {
          km.count = 0;
          km.step = 0x13;         // Yes (OS, TG)
          goto DO_IT_RIGHT_NOW;
        } else {
          km.step = 0x12;         // No  (SG)
          goto DO_IT_RIGHT_NOW;
        }
      }
      case 0x12:{ // Send ControlChange end
        uint8_t c = km.channel;
        uint8_t n = km.number;
        uint8_t v = 0;
        usbmidi->sendControlChange(c, n, v);
        km.count = 0;
        km.step = 0x13;
        goto DO_IT_NEXT_TIME;
      }
      case 0x13:{ // Check if the end of ControlChange hold-time is arrival
        if (++km.count >= hold_count) {
          km.step = 0x00;         // Yes
          goto DO_IT_RIGHT_NOW;
        }else {
          km.step = 0x13;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

static void key_pc(uint8_t kn, int8_t kv, AppCtrl_t &ac, MidiParam *param, MidiManager *manager, USBMIDI *usbmidi) {
  MidiAssign_t  &ka           = param->key_assign[kn];
  uint8_t       &hold_count   = param->key_hold_count;
  KeyManager_t  &km           = manager->key_manager[kn];

  while (1) {
    switch (km.step) {
      //--------------------------------------------------------------------------------------
      // Pressed
      case 0x00:{ // Check if the key is pressed
        if (kv > 0) {
          km.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
      } else {
          km.step = 0x00;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x01:{ // Send ProgramChange
        uint8_t c = km.channel = (ka.msg_ch & 0x0F);
        uint8_t n = km.number  = ka.number;
        usbmidi->sendProgramChange(c, n);
        midi_ui_write(ac, " PC %2d %3d", (c+1), n);
        km.count = 0;
        km.step = 0x02;
        goto DO_IT_NEXT_TIME;
      }
      case 0x02:{ // Check if the end of ProgramChange hold-time is arrival
        if (++km.count >= hold_count) {
          km.step = 0x10;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          km.step = 0x02;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

      //--------------------------------------------------------------------------------------
      // Released
      case 0x10:{ // Check if the key is released
        if (kv == 0) {
          km.step = 0x11;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          km.step = 0x10;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x11:{ // Check if the end of ProgramChange hold-time is arrival
        if (++km.count >= hold_count) {
          km.step = 0x00;         // Yes
          goto DO_IT_RIGHT_NOW;
        }else {
          km.step = 0x11;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

static void key_ps(uint8_t kn, uint8_t kv, AppCtrl_t &ac, MidiParam *param, MidiManager *manager, USBMIDI *usbmidi) {
  MidiAssign_t  &ka           = param->key_assign[kn];
  uint8_t       &hold_count   = param->key_hold_count;
  KeyManager_t  &km           = manager->key_manager[kn];
  int8_t        &pitch_shift  = manager->pitch_shift;

  while (1) {
    switch (km.step) {
      //--------------------------------------------------------------------------------------
      // Pressed
      case 0x00:{ // Check if the key is pressed
        if (kv > 0) {
          km.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
      } else {
          km.step = 0x00;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x01:{ // Pitch shift
        int n = 0;
        switch (ka.number) {
          case PS_C1:     pitch_shift = 0;  n = 36 + pitch_shift;   break;
          case PS_E2:     pitch_shift = 16; n = 36 + pitch_shift;   break;
          case PS_GS3:    pitch_shift = 32; n = 36 + pitch_shift;   break;
          case PS_C5:     pitch_shift = 48; n = 36 + pitch_shift;   break;
          case PS_INC12:  pitch_shift = (pitch_shift >=  60) ?  60 : (pitch_shift + 12);  n = +12;  break;
          case PS_DEC12:  pitch_shift = (pitch_shift <= -60) ? -60 : (pitch_shift - 12);  n = -12;  break;
          case PS_INC1:   pitch_shift = (pitch_shift >=  60) ?  60 : (pitch_shift + 1);   n = +1;   break;
          case PS_DEC1:   pitch_shift = (pitch_shift <= -60) ? -60 : (pitch_shift - 1);   n = -1;   break;
        }
        if (ka.number <= PS_C5) {
          midi_ui_write(ac, " PS    %3d", n);
        } else {
          midi_ui_write(ac, " PS    %+3d", n);
        }
        km.count = 0;
        km.step = 0x02;
        goto DO_IT_NEXT_TIME;
      }
      case 0x02:{ // Check if the end of PitchShift hold-time is arrival
        if (++km.count >= hold_count) {
          km.step = 0x10;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          km.step = 0x02;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

      //--------------------------------------------------------------------------------------
      // Released
      case 0x10:{ // Check if the key is released
        if (kv == 0) {
          km.step = 0x11;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          km.step = 0x10;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x11:{ // Check if the end of PitchShift hold-time is arrival
        if (++km.count >= hold_count) {
          km.step = 0x00;         // Yes
          goto DO_IT_RIGHT_NOW;
        }else {
          km.step = 0x11;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

static void key_mc(uint8_t kn, uint8_t kv, AppCtrl_t &ac, MidiParam *param, MidiManager *manager, USBMIDI *usbmidi) {
  MidiAssign_t  &ka             = param->key_assign[kn];
  uint8_t       &hold_count     = param->key_hold_count;
  KeyManager_t  &km             = manager->key_manager[kn];

  while (1) {
    switch (km.step) {
      //--------------------------------------------------------------------------------------
      // Pressed
      case 0x00:{ // Check if the key is pressed
        if (kv > 0) {
          km.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
      } else {
          km.step = 0x00;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x01:{ // MMC
        usbmidi->sendMMC(ka.number);
        const char *mmc[] = {"\xDB  ", "\x10  ", "\x10\x10 ", "\x11\x11 ", "\x07  "};
        int n = 0;
        switch (ka.number){
          case MMC_STOP: n = 0; break;
          case MMC_PLAY: n = 1; break;
          case MMC_FWD:  n = 2; break;
          case MMC_REW:  n = 3; break;
          case MMC_REC:  n = 4; break;
        }
        midi_ui_write(ac, "MMC    %s", mmc[n]);
        km.count = 0;
        km.step = 0x02;
        goto DO_IT_NEXT_TIME;
      }
      case 0x02:{ // Check if the end of MMC hold-time is arrival
        if (++km.count >= hold_count) {
          km.step = 0x10;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          km.step = 0x02;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

      //--------------------------------------------------------------------------------------
      // Released
      case 0x10:{ // Check if the key is released
        if (kv == 0) {
          km.step = 0x11;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          km.step = 0x10;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x11:{ // Check if the end of MMC hold-time is arrival
        if (++km.count >= hold_count) {
          km.step = 0x00;         // Yes
          goto DO_IT_RIGHT_NOW;
        }else {
          km.step = 0x11;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

static void key_ud(uint8_t kn, uint8_t kv, AppCtrl_t &ac, MidiParam *param, MidiManager *manager, USBMIDI *usbmidi) {
  MidiAssign_t  &ka             = param->key_assign[kn];
  uint8_t       &hold_count     = param->key_hold_count;
  KeyManager_t  &km             = manager->key_manager[kn];

  while (1) {
    switch (km.step) {
      //--------------------------------------------------------------------------------------
      // Pressed
      case 0x00:{ // Check if the key is pressed
        if (kv > 0) {
          km.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
      } else {
          km.step = 0x00;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x01:{ // UserDefined message
        uint8_t index    = (ka.msg_ch & 0x0F);
        uint8_t size     = 4 * (ka.number + 1);
        uint8_t *message = param->key_userdefined_message[index];
        usbmidi->sendUserDefined(message, size);
        midi_ui_write(ac, "USR    %3d", index);
        km.count = 0;
        km.step = 0x02;
        goto DO_IT_NEXT_TIME;
      }
      case 0x02:{ // Check if the end of Customized message hold-time is arrival
        if (++km.count >= hold_count) {
          km.step = 0x10;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          km.step = 0x02;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

      //--------------------------------------------------------------------------------------
      // Released
      case 0x10:{ // Check if the key is released
        if (kv == 0) {
          km.step = 0x11;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          km.step = 0x10;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x11:{ // Check if the end of UserDefined message hold-time is arrival
        if (++km.count >= hold_count) {
          km.step = 0x00;         // Yes
          goto DO_IT_RIGHT_NOW;
        }else {
          km.step = 0x11;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

static void pot_cc(uint8_t pv, AppCtrl_t &ac, MidiParam *param, MidiManager *manager, USBMIDI *usbmidi) {
  MidiAssign_t  &pa             = param->pot_assign;
  uint8_t       &hold_count     = param->pot_hold_count;
  PotManager_t  &pm             = manager->pot_manager;

  while (1) {
    switch (pm.step) {
      case 0x00:{ // Check if the value is changed
        if (pv != pm.value) {
          pm.value = pv;
          pm.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
      } else {
          pm.step = 0x00;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x01:{ // Send ControlChange
        uint8_t c = (pa.msg_ch & 0x0F);
        uint8_t n = pa.number;
        uint8_t v = param->curve_linear7bit(pa.value, pv);
        usbmidi->sendControlChange(c, n, v);
        pm.count = 0;
        pm.step = 0x02;
        goto DO_IT_NEXT_TIME;
      }
      case 0x02:{ // Check if the end of ControlChange hold-time is arrival
        if (++pm.count >= hold_count) {
          pm.step = 0x00;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          pm.step = 0x02;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

static void fsr_cc(uint8_t fv, AppCtrl_t &ac, MidiParam *param, MidiManager *manager, USBMIDI *usbmidi) {
  MidiAssign_t  &fa             = param->fsr_assign;
  uint8_t       &hold_count     = param->fsr_hold_count;
  FsrManager_t  &fm             = manager->fsr_manager;

  while (1) {
    switch (fm.step) {
      case 0x00:{ // Check if the value is changed
        if (fv != fm.value) {
          fm.value = fv;
          fm.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
      } else {
          fm.step = 0x00;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x01:{ // Send ControlChange
        uint8_t c = (fa.msg_ch & 0x0F);
        uint8_t n = fa.number;
        uint8_t v = param->curve_linear7bit(fa.value, fv);
        usbmidi->sendControlChange(c, n, v);
        fm.count = 0;
        fm.step = 0x02;
        goto DO_IT_NEXT_TIME;
      }
      case 0x02:{ // Check if the end of ControlChange hold-time is arrival
        if (++fm.count >= hold_count) {
          fm.step = 0x00;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          fm.step = 0x02;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

static void fsr_cp(uint8_t fv, AppCtrl_t &ac, MidiParam *param, MidiManager *manager, USBMIDI *usbmidi) {
  MidiAssign_t  &fa             = param->fsr_assign;
  uint8_t       &hold_count     = param->fsr_hold_count;
  FsrManager_t  &fm             = manager->fsr_manager;

  while (1) {
    switch (fm.step) {
      case 0x00:{ // Check if the value is changed
        if (fv != fm.value) {
          fm.value = fv;
          fm.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
      } else {
          fm.step = 0x00;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x01:{ // Send ControlChange
        uint8_t c = (fa.msg_ch & 0x0F);
        uint8_t v = param->curve_linear7bit(fa.value, fv);
        usbmidi->sendChannelAftertouch(c, v);
        fm.count = 0;
        fm.step = 0x02;
        goto DO_IT_NEXT_TIME;
      }
      case 0x02:{ // Check if the end of ControlChange hold-time is arrival
        if (++fm.count >= hold_count) {
          fm.step = 0x00;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          fm.step = 0x02;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

static void joy_cc(uint8_t jn, int8_t jv07, AppCtrl_t &ac, MidiParam *param, MidiManager *manager, USBMIDI *usbmidi) {
  MidiAssign_t  &ja             = param->joy_assign[jn];
  uint8_t       &hold_count     = param->joy_hold_count;
  JoyManager_t  &jm             = manager->joy_manager[jn];

  while (1) {
    switch (jm.step) {
      case 0x00:{ // Check if the value is changed
        if (jv07 != jm.value) {
          jm.value = jv07;
          jm.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
      } else {
          jm.step = 0x00;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x01:{ // Send ControlChange
        uint8_t c = (ja.msg_ch & 0x0F);
        uint8_t n = ja.number;
        uint8_t v = param->curve_linear7bit(ja.value, jv07);
        usbmidi->sendControlChange(c, n, v);
        jm.count = 0;
        jm.step = 0x02;
        goto DO_IT_NEXT_TIME;
      }
      case 0x02:{ // Check if the end of ControlChange hold-time is arrival
        if (++jm.count >= hold_count) {
          jm.step = 0x00;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          jm.step = 0x02;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

static void joy_pb(uint8_t jn, int16_t jv14, AppCtrl_t &ac, MidiParam *param, MidiManager *manager, USBMIDI *usbmidi) {
  MidiAssign_t  &ja             = param->joy_assign[jn];
  uint8_t       &hold_count     = param->joy_hold_count;
  JoyManager_t  &jm             = manager->joy_manager[jn];

  while (1) {
    switch (jm.step) {
      case 0x00:{ // Check if the value is changed
        if (jv14 != jm.value) {
          jm.value = jv14;
          jm.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          jm.step = 0x00;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x01:{ // Send PitchBend
        uint8_t  c = (ja.msg_ch & 0x0F);
        uint16_t p = param->curve_linear14bit(ja.value, jv14);
        usbmidi->sendPitchBend(c, p);
        jm.count = 0;
        jm.step = 0x02;
        goto DO_IT_NEXT_TIME;
      }
      case 0x02:{ // Check if the end of ControlChange hold-time is arrival
        if (++jm.count >= hold_count) {
          jm.step = 0x00;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          jm.step = 0x02;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

static void joy_btn(bool joy_pressed, AppCtrl_t &ac, MidiParam *param, MidiManager *manager) {
  uint8_t          &hold_count     = param->joybtn_hold_count;
  JoyBtnManager_t  &jbm            = manager->joybtn_manager;

  while (1) {
    switch (jbm.step) {
      //--------------------------------------------------------------------------------------
      // Pressed
      case 0x00:{ // Check if the button is pressed
        if (joy_pressed) {
          jbm.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
      } else {
          jbm.step = 0x00;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x01:{ // Toggle the acc_enable
        manager->acc_enable = !manager->acc_enable;
        midi_ui_write(ac, "ACC    %s", ((manager->acc_enable) ? "ON ":"OFF") );
        jbm.count = 0;
        jbm.step = 0x02;
        goto DO_IT_NEXT_TIME;
      }
      case 0x02:{ // Check if the end of pressed hold-time is arrival
        if (++jbm.count >= hold_count) {
          jbm.step = 0x10;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          jbm.step = 0x02;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

      //--------------------------------------------------------------------------------------
      // Released
      case 0x10:{ // Check if the key is released
        if (joy_pressed == 0) {
          jbm.step = 0x11;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          jbm.step = 0x10;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x11:{ // Check if the end of released hold-time is arrival
        if (++jbm.count >= hold_count) {
          jbm.step = 0x00;         // Yes
          goto DO_IT_RIGHT_NOW;
        }else {
          jbm.step = 0x11;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

static void acc_cc(uint8_t an, uint8_t av, AppCtrl_t &ac, MidiParam *param, MidiManager *manager, USBMIDI *usbmidi) {
  MidiAssign_t  &aa             = param->acc_assign[an];
  uint8_t       &hold_count     = param->acc_hold_count;
  AccManager_t  &am             = manager->acc_manager[an];

  while (1) {
    switch (am.step) {
      case 0x00:{ // Check if the value is changed
        if (av != am.value) {
          am.value = av;
          am.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
      } else {
          am.step = 0x00;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x01:{ // Send ControlChange
        uint8_t c = (aa.msg_ch & 0x0F);
        uint8_t n = aa.number;
        uint8_t v = param->curve_linear7bit(aa.value, av);
        usbmidi->sendControlChange(c, n, v);
        am.count = 0;
        am.step = 0x02;
        goto DO_IT_NEXT_TIME;
      }
      case 0x02:{ // Check if the end of ControlChange hold-time is arrival
        if (++am.count >= hold_count) {
          am.step = 0x00;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          am.step = 0x02;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

static void whe_ab(uint8_t wn, AppCtrl_t &ac, MidiParam *param, MidiManager *manager, USBMIDI *usbmidi) {
  MidiAssign_t  &wa             = param->whe_assign[wn];
  uint8_t       &hold_count     = param->whe_hold_count;
  WheManager_t  &wm             = manager->whe_manager[wn];
  QEI           *jw             = ac.jw[wn];

  while (1) {
    switch (wm.step) {
      case 0x00:{ // First time initial
        wm.value = wa.value;      // Load initial-value
        jw->reset();              // Drop out the first-time data
        wm.step = 0x01;
        goto DO_IT_NEXT_TIME;
      }
      case 0x01:{ // Check if the value is changed
        int v = jw->getPulses();
        jw->reset();
        v = (wn == 0) ? +v : -v;
        if (v != 0) {
          int m = wa.mode + 1;
          int n = (int)wm.value + (v * m);
          if (n > 127) n = 127;
          if (n <   0) n = 0;
          wm.value = n;
          wm.step = 0x02;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          wm.step = 0x01;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x02:{ // Send ControlChange
        uint8_t c = (wa.msg_ch & 0x0F);
        uint8_t n = wa.number;
        uint8_t v = wm.value;
        usbmidi->sendControlChange(c, n, v);
        wm.count = 0;
        wm.step = 0x03;
        goto DO_IT_NEXT_TIME;
      }
      case 0x03:{ // Check if the end of ControlChange hold-time is arrival
        if (++wm.count >= hold_count) {
          wm.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          wm.step = 0x03;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

static void whe_re(uint8_t wn, AppCtrl_t &ac, MidiParam *param, MidiManager *manager, USBMIDI *usbmidi) {
  MidiAssign_t  &wa             = param->whe_assign[wn];
  uint8_t       &hold_count     = param->whe_hold_count;
  WheManager_t  &wm             = manager->whe_manager[wn];
  QEI           *jw             = ac.jw[wn];

  while (1) {
    switch (wm.step) {
      case 0x00:{ // First time initial
        jw->reset();              // Drop out the first-time data
        wm.step = 0x01;
        goto DO_IT_NEXT_TIME;
      }
      case 0x01:{ // Check if the value is changed
        int v = jw->getPulses();
        jw->reset();
        v = (wn == 0) ? +v : -v;
        if (v != 0) {
          int m = wa.mode + 1;
          int n = 64 + (v * m);
          if (n > 127) n = 127;
          if (n <   0) n = 0;
          wm.value = n;
          wm.step = 0x02;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          wm.step = 0x01;         // No
          goto DO_IT_NEXT_TIME;
        }
      }
      case 0x02:{ // Send ControlChange
        uint8_t c = (wa.msg_ch & 0x0F);
        uint8_t n = wa.number;
        uint8_t v = wm.value;
        usbmidi->sendControlChange(c, n, v);
        wm.count = 0;
        wm.step = 0x03;
        goto DO_IT_NEXT_TIME;
      }
      case 0x03:{ // Check if the end of ControlChange hold-time is arrival
        if (++wm.count >= hold_count) {
          wm.step = 0x01;         // Yes
          goto DO_IT_RIGHT_NOW;
        } else {
          wm.step = 0x03;         // No
          goto DO_IT_NEXT_TIME;
        }
      }

    } // End of switch
DO_IT_RIGHT_NOW: while(0);
  } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

void midi_io_out(AppCtrl_t &ac, MidiParam *param, MidiManager *manager) {
// static Timer t;
// t.start();
// t.reset();

  /* Input data */
  int keys = ac.keys->read();
  float pot = ac.pot->read();
  float fsr = ac.fsr->read();

  Vector2D joy_v2d = ac.joy->get_vector();
  bool joy_pressed = ac.joy->is_pressed();

  float acc_v3d[3];
  ac.acc->get_vector(acc_v3d);

  /* Output data */
  USBMIDI *usbmidi = ac.usbmidi;

  /* Keys */
  for (int i = 0; i < 20; i++) {
    uint8_t msg = (param->key_assign[i].msg_ch & 0xF0);
    uint8_t kn = i;
    uint8_t kv = (keys & (1 << i)) ? 1 : 0;

    switch (msg) {
      case MSG_NO:                                              break; // None
      case MSG_NT: key_nt(kn, kv, ac, param, manager, usbmidi); break; // NoteOn, NoteOff
      case MSG_CC: key_cc(kn, kv, ac, param, manager, usbmidi); break; // ControlChange
      case MSG_PC: key_pc(kn, kv, ac, param, manager, usbmidi); break; // ProgramChange
      case MSG_PS: key_ps(kn, kv, ac, param, manager, usbmidi); break; // PitchShift
      case MSG_MC: key_mc(kn, kv, ac, param, manager, usbmidi); break; // MMC
      case MSG_UD: key_ud(kn, kv, ac, param, manager, usbmidi); break; // UserDefined
    }
  }

  /* Potentiometer */
  {
    uint8_t msg = (param->pot_assign.msg_ch & 0xF0);

    const  float threshold = 0.5f;
    static float last_pv = 0;
           float curr_pv = (1.0f - pot) * 127;
         uint8_t pv      = 0;
    if (abs(curr_pv - last_pv) > threshold){ // Update only if the difference value over threshold (Prevent data swing)
      last_pv = curr_pv;
      pv = (uint8_t)roundf(curr_pv);
    } else {
      pv = (uint8_t)roundf(last_pv);
    }
    pv = manager->pot_filter->add(pv).average();  // Filter

    switch (msg) {
      case MSG_NO:                                          break; // None
      case MSG_CC: pot_cc(pv, ac, param, manager, usbmidi); break; // ControlChange
    }
  }

  /* Force sensing resistor */
  {
    uint8_t msg = (param->fsr_assign.msg_ch & 0xF0);

    uint8_t fv = (uint8_t)roundf(fsr * 127);
    fv = manager->fsr_filter->add(fv).average();  // Filter

    switch (msg) {
      case MSG_NO:                                          break; // None
      case MSG_CC: fsr_cc(fv, ac, param, manager, usbmidi); break; // ControlChange
      case MSG_CP: fsr_cp(fv, ac, param, manager, usbmidi); break; // ChannelPressure
    }
  }

  /* Joystick */
  float &jx = joy_v2d.x;
  float &jy = joy_v2d.y;

  const float dead_zone_j = 0.10f;  // Dead zone range: ex. 0.10f ==> +/-10%

  if      (jx >  dead_zone_j) jx = map(jx,  dead_zone_j,  1.00f, 0,  1.00f);
  else if (jx < -dead_zone_j) jx = map(jx, -1.00f, -dead_zone_j, -1.00f, 0);
  else                        jx = 0;

  if      (jy >  dead_zone_j) jy = map(jy,  dead_zone_j,  1.00f, 0,  1.00f);
  else if (jy < -dead_zone_j) jy = map(jy, -1.00f, -dead_zone_j, -1.00f, 0);
  else                        jy = 0;

  for (int i = 0; i < 4; i++) {   // 0:X_Left 1:X_Right 2:Y_Down 3:Y_Up
    uint8_t msg = (param->joy_assign[i].msg_ch & 0xF0);
    uint8_t jn = i;
    uint8_t  jv07 = 0;
    uint16_t jv14 = 0;

    switch (i) {
      case 0:
        jv07 = (jx < 0) ? roundf(-jx * 127) : 0;
        jv14 = (jx < 0) ? roundf(map(-jx, 0, 1, 0, 16383)) : 0;
        break;
      case 1:
        jv07 = (jx > 0) ? roundf( jx * 127) : 0;
        jv14 = (jx > 0) ? roundf(map( jx, 0, 1, 0, 16383)) : 0;
        break;
      case 2:
        jv07 = (jy < 0) ? roundf(-jy * 127) : 0;
        jv14 = (jy < 0) ? roundf(map(-jy, 0, 1, 0, 16383)) : 0;
        break;
      case 3:
        jv07 = (jy > 0) ? roundf( jy * 127) : 0;
        jv14 = (jy > 0) ? roundf(map( jy, 0, 1, 0, 16383)) : 0;
        break;
    }

    if (msg == MSG_PB)  jv14 = manager->joy_filter[i]->add(jv14).average(); // Filter 14bit
    else                jv07 = manager->joy_filter[i]->add(jv07).average(); // Filter  7bit

    jv14 &= 0xFFF0; // Remove LSb 4bits

    switch (msg) {
      case MSG_NO:                                                break; // None
      case MSG_CC: joy_cc(jn, jv07, ac, param, manager, usbmidi); break; // ControlChange
      case MSG_PB: joy_pb(jn, jv14, ac, param, manager, usbmidi); break; // PitchBend
    }
  }

  joy_btn(joy_pressed, ac, param, manager);  // Joystick's button

  /* Accelerometer */
  if (manager->acc_enable) {
    float x = acc_v3d[0];   // Mapping physical-X to positive logical-X 
    float y = acc_v3d[1];   // Mapping physical-Y to positive logical-Y 

    const float dead_zone_a = 0.10f;  // Dead zone range: ex. 0.10f ==> +/-10%

    if      (x >  dead_zone_a) x = map(x,  dead_zone_a,  1.00f, 0,  1.00f);
    else if (x < -dead_zone_a) x = map(x, -1.00f, -dead_zone_a, -1.00f, 0);
    else                       x = 0;

    if      (y >  dead_zone_a) y = map(y,  dead_zone_a,  1.00f, 0,  1.00f);
    else if (y < -dead_zone_a) y = map(y, -1.00f, -dead_zone_a, -1.00f, 0);
    else                       y = 0;

    for (int i = 0; i < 4; i++) {   // 0:X_Left 1:X_Right 2:Y_Down 3:Y_Up
      uint8_t msg = (param->acc_assign[i].msg_ch & 0xF0);
      uint8_t an = i;
      uint8_t av = 0;

      switch (i) {
        case 0: av = (x < 0) ? (-x * 127) : 0;  break;
        case 1: av = (x > 0) ? ( x * 127) : 0;  break;
        case 2: av = (y < 0) ? (-y * 127) : 0;  break;
        case 3: av = (y > 0) ? ( y * 127) : 0;  break;
      }

      av = manager->acc_filter[i]->add(av).average();  // Filter

      switch (msg) {
        case MSG_NO:                                              break; // None
        case MSG_CC: acc_cc(an, av, ac, param, manager, usbmidi); break; // ControlChange
      }
    }
  }

  /* Wheel */
  for (int i = 0; i < 2; i++) {
    uint8_t msg = (param->whe_assign[i].msg_ch & 0xF0);
    uint8_t wn = i;

    switch (msg) {
      case MSG_NO:                                          break; // None
      case MSG_AB: whe_ab(wn, ac, param, manager, usbmidi); break; // ControlChange_Abs
      case MSG_RE: whe_re(wn, ac, param, manager, usbmidi); break; // ControlChange_Rel
    }
  }

#if (MBED_CONF_APP_RUN_MODE == 0)
  /* Serial MIDI-IN Parser */
  ac.serialmidi->parseRxMessageRoutine();
#endif

  /* Update OLED display and LEDs */
  midi_ui_update(ac, param, keys);

// t.stop();
// DEBUG_OUT("%d\n", t.read_us());
}