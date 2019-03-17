#ifndef MIDI_UI_H
#define MIDI_UI_H

#include "mbed.h"
#include "app_common.h"
#include "MidiParam.h"

void midi_ui_base(AppCtrl_t &ac, MidiParam *param, int mode);
void midi_ui_write(AppCtrl_t &ac, const char *fmt, ...); // ex. " NT 16 123", ch, num
void midi_ui_update(AppCtrl_t &ac, MidiParam *param, int keys);

#endif