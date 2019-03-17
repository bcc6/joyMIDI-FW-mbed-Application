#ifndef APP_SETUP_PARAM_H
#define APP_SETUP_PARAM_H

#include "mbed.h"
#include "app_common.h"
#include "SetupCommands.h"

#define DEVICE_SERIAL_NUMBER_ADDR   0x00100FFC

void app_setup_param(AppCtrl_t &ac);

#endif