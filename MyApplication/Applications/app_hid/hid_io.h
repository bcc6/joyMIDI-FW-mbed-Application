#ifndef HID_IO_H
#define HID_IO_H

#include "HidManager.h"
#include "HidParam.h"
#include "app_common.h"
#include "Hid_ui.h"
#include "mbed.h"

void hid_io_in(AppCtrl_t &ac, HidParam *param, HidManager *manager);
void hid_io_out(AppCtrl_t &ac, HidParam *param, HidManager *manager);

#endif