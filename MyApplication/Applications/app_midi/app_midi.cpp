#include "app_midi.h"

#if 0
static void debug_show_param(MidiParam *p) {
  /* key */
  DEBUG_OUT("\nkey:\n");
  DEBUG_OUT("cnt=%d\n", p->key_hold_count);
  HEX_DUMP("ass", &p->key_assign[0], 80);
  HEX_DUMP("ud", &p->key_userdefined_message[0], 32);
  /* pot */
  DEBUG_OUT("\npot:\n");
  DEBUG_OUT("cnt=%d\n", p->pot_hold_count);
  HEX_DUMP("ass", &p->pot_assign, 4);
  /* fsr */
  DEBUG_OUT("\nfsr:\n");
  DEBUG_OUT("cnt=%d\n", p->fsr_hold_count);
  HEX_DUMP("ass", &p->fsr_assign, 4);
  /* joy */
  DEBUG_OUT("\njoy:\n");
  DEBUG_OUT("cntj,cntb=%d,%d\n", p->joy_hold_count, p->joybtn_hold_count);
  HEX_DUMP("ass", &p->joy_assign[0], 16);
  /* acc */
  DEBUG_OUT("\nacc:\n");
  DEBUG_OUT("cnt=%d\n", p->acc_hold_count);
  HEX_DUMP("ass", &p->acc_assign[0], 16);
  /* whe */
  DEBUG_OUT("\nwhe:\n");
  DEBUG_OUT("cnt=%d\n", p->whe_hold_count);
  HEX_DUMP("ass", &p->whe_assign[0], 8);
  /* rgbled */
  DEBUG_OUT("\nrgb:\n");
  DEBUG_OUT("mode=%d color=%06X %06X\n", p->rgbled_mode, p->rgbled_color[0], p->rgbled_color[1]);
  /* monoled */
  DEBUG_OUT("\nmono:\n");
  DEBUG_OUT("mode=%d switches=%05X\n", p->monoled_mode, p->monoled_switches);
}
#endif

// // extern SystemReport *sys_state;
// static void debug_midi_stats() {
//   // sys_state->report_heap_stats();
//   // sys_state->report_thread_stats();
//   // DEBUG_OUT("\n");
//   DEBUG_OUT("oz123\n");
// }

static int load_user_parameters(AppCtrl_t &ac, MidiParam *param) {
  uint8_t *buf;
  buf = new uint8_t[256];

  switch (param->midi_mode) {
    case APP_MIDI_KB:   ac.df->readParamMidiKeyboard(buf); break;
    case APP_MIDI_PAD:  ac.df->readParamMidiPad(buf);      break;
    case APP_MIDI_DJ:   ac.df->readParamMidiDj(buf);       break;
  }
  param->load_key_param(buf[0], (MidiAssign_t *)&buf[1], &buf[81]);
  param->load_pot_param(buf[113], (MidiAssign_t *)&buf[114]);
  param->load_fsr_param(buf[118], (MidiAssign_t *)&buf[119]);
  param->load_joy_param(buf[123], buf[124], (MidiAssign_t *)&buf[125]);
  param->load_acc_param(buf[141], (MidiAssign_t *)&buf[142]);
  param->load_whe_param(buf[158], (MidiAssign_t *)&buf[159]);
  param->load_rgbled_param(buf[167], (uint32_t *)&buf[168]);
  param->load_monoled_param(buf[176], *((uint32_t *)&buf[177]));

  delete buf;
  return 0;
}

void app_midi(AppCtrl_t &ac, int mode) {
  /* Get parameter and manager */
  MidiParam   *param   = new MidiParam(mode);
  MidiManager *manager = new MidiManager();

  /* Load user parameters */
#if 1
  load_user_parameters(ac, param);
  // debug_show_param(param);
#else
  if (load_user_parameters(ac, param) < 0) {
    param->load_default_parameters();
  }
  // debug_show_param(param);
#endif

  /* Display base framework */
  midi_ui_base(ac, param, mode);

#if (MBED_CONF_APP_RUN_MODE == 0)
  /* Setup Serial-MIDI */
  ac.serialmidi = new SerialMIDI(NC, PA_3, 31250, 64, 1);  // rx_buf(64), tx_buf(64*1)
#endif

  /* Setup USB-MIDI */
  ac.usbmidi = new USBMIDI();
  // DEBUG_OUT("USB Pluged\n");  // This spend too much time and lead the iOS connect fail

  /* Setup MIDI-in */
  midi_io_in(ac, param, manager);

  /* Setup MIDI-out */
  EventQueue *eq = mbed_event_queue();                  // Request the shared queue
  eq->call_every(10, midi_io_out, ac, param, manager);  // Call every 10ms
  // eq->call_every(5000, debug_midi_stats);               // Call every 5000ms
  eq->dispatch_forever();
}
