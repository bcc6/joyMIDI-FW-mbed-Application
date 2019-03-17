#ifndef MIDI_IO_H
#define MIDI_IO_H

#include "MidiManager.h"
#include "MidiParam.h"
#include "app_common.h"
#include "midi_ui.h"
#include "mbed.h"

void midi_io_in(AppCtrl_t &ac, MidiParam *param, MidiManager *manager);
void midi_io_out(AppCtrl_t &ac, MidiParam *param, MidiManager *manager);

#endif