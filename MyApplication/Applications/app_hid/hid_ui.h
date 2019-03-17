#ifndef HID_UI_H
#define HID_UI_H

#include "mbed.h"
#include "app_common.h"
#include "HidParam.h"

void hid_ui_base(AppCtrl_t &ac, HidParam *param);
void hid_ui_write(AppCtrl_t &ac, const char *fmt, ...); // ex. "Btn 32 ", num
void hid_ui_update(AppCtrl_t &ac, HidParam *param);

#endif