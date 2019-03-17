#include "hid_ui.h"

void hid_ui_base(AppCtrl_t &ac, HidParam *param) {
  /* Clear screen */
  ac.oled->cls(NULL, 0);

  /* Icon 32x32 */
  uint8_t *icon = new uint8_t[128];
  ac.df->readImageIconsN(icon, 3);
  ac.oled->icon32x32((char *)icon);
  delete icon;

  /* HID info */
  ac.oled->locate(0, 5);
  ac.oled->printf("Msg Info");
  ac.oled->redraw();

  /* Mono LEDs */
  uint8_t  md = param->monoled_mode;
  uint32_t sw = param->monoled_switches;
  switch (md) {
  case HID_MONOLED_MODE0_TurnOff:     ac.leds->write(0);                        break;
  case HID_MONOLED_MODE1_Switch:      ac.leds->write(sw & 0xFFFFF);             break;
  }
}

void hid_ui_write(AppCtrl_t &ac, const char *fmt, ...) {
  ac.oled->locate(2, 5);

  char s[7+1]; // 7 charaters + '\n'
  va_list args;

  va_start(args, fmt);
  vsnprintf(s, 7+1, fmt, args);
  ac.oled->puts(s, false);
  va_end(args);
}

void hid_ui_update(AppCtrl_t &ac, HidParam *param) {
  /* Update OLED */
  ac.oled->redraw_partial(2, (5*8), 1, (7*8));
}
