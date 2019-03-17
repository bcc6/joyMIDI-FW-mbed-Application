#include "midi_ui.h"

void midi_ui_base(AppCtrl_t &ac, MidiParam *param, int mode) {
  /* Clear screen */
  ac.oled->cls(NULL, 0);

  /* Icon 32x32 */
  uint8_t *icon = new uint8_t[128];
  ac.df->readImageIconsN(icon, mode);
  ac.oled->icon32x32((char *)icon);
  delete icon;

  /* MIDI info */
  ac.oled->locate(0, 5);
  ac.oled->printf("Msg Ch Num");
  ac.oled->redraw();

  /* Mono LEDs */
  uint8_t  md = param->monoled_mode;
  uint32_t sw = param->monoled_switches;
  switch (md) {
  case MIDI_MONOLED_MODE0_TurnOff:     ac.leds->write(0);                        break;
  case MIDI_MONOLED_MODE1_Switch:      ac.leds->write(sw & 0xFFFFF);             break;
  case MIDI_MONOLED_MODE2_SwitchHover: ac.leds->write((sw & 0xFFFF0) | 0x00001); break;
  }
}

void midi_ui_write(AppCtrl_t &ac, const char *fmt, ...) {
  ac.oled->locate(2, 5);

  char s[10+1]; // 10 charaters + '\n'
  va_list args;

  va_start(args, fmt);
  vsnprintf(s, 10+1, fmt, args);
  ac.oled->puts(s, false);
  va_end(args);
}

void midi_ui_update(AppCtrl_t &ac, MidiParam *param, int keys) {
  /* Update OLED */
  ac.oled->redraw_partial(2, (5*8), 1, (10*8));

  /* Update LEDs */
  uint8_t  md = param->monoled_mode;
  uint32_t sw = param->monoled_switches;
  static uint8_t last_hover_4keys = 0;
         uint8_t temp_hover_4keys = keys & 0xF;
         uint8_t this_hover_4keys = 0;

  if (md == MIDI_MONOLED_MODE2_SwitchHover) {
    for (int i=0; i<4; i++) {
      if (temp_hover_4keys & (1<<i)) {
        this_hover_4keys = (1<<i);
        break;
      }
    }
    if ((this_hover_4keys != 0) && (this_hover_4keys != last_hover_4keys)) {
      last_hover_4keys = this_hover_4keys;
      ac.leds->write((sw & 0xFFFF0) | this_hover_4keys);
    }
  }
}
