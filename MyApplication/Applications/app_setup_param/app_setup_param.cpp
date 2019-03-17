#include "app_setup_param.h"


static SetupCommands *sc;

static void onControlChange(uint8_t channel, uint8_t function, uint8_t value) {
  // DEBUG_OUT("onControlChange(channel=%d, function=%d, value=%d)\n", channel, function, value);
}

static void onSystemExclusive(uint8_t *pkt, uint16_t length, bool hasNextPkt) {
  // DEBUG_OUT("onSystemExclusive(pkt=..., length=%d, hasNextPkt=%d)\n", length, hasNextPkt?1:0);
  // HEX_DUMP("sysex", pkt, length);

  // Rx request
  sc->rxRequest(pkt, length, hasNextPkt);
}

static void readDevInf(AppCtrl_t &ac) {
  struct DevInf { uint32_t sn; uint8_t major; uint8_t minor; } devinf;
  sc->dlen = sizeof(DevInf);

  SYS_UnlockReg();
  FMC_ENABLE_ISP();
  devinf.sn = FMC_Read(DEVICE_SERIAL_NUMBER_ADDR);
  FMC_DISABLE_ISP();
  SYS_LockReg();

  devinf.major = MBED_CONF_APP_FIRMWARE_VERSION_MAJOR;
  devinf.minor = MBED_CONF_APP_FIRMWARE_VERSION_MINOR;
  memcpy(sc->dbuf, (uint8_t *)&devinf, sc->dlen);

  /* Display OK */
  ac.oled->locate(3, 6);
  ac.oled->printf(" OK ");
  ac.oled->redraw();
}

static void readParam(AppCtrl_t &ac) {
  ac.df->readAllData(sc->dbuf, &sc->dlen);
}

static void writeParam(AppCtrl_t &ac) {
  ac.df->writeAllData(sc->dbuf, sc->dlen);
}

static void testLed(AppCtrl_t &ac) {
  uint8_t *p = sc->dbuf;
  ac.rgb->write(p[0], p[1], p[2]);
}

static void calibJoystick(AppCtrl_t &ac) {
  struct FixedPointXY { uint16_t x; uint16_t y; } fpxy;

  Vector2D v2d = ac.joy->get_raw();
  fpxy.x = (uint16_t)(v2d.x * 1000);
  fpxy.y = (uint16_t)(v2d.y * 1000);
  sc->dlen = sizeof(FixedPointXY);
  memcpy(sc->dbuf, (uint8_t *)&fpxy, sc->dlen);

  // DEBUG_OUT("X(0.%3d),Y(0.%3d)\n", fpxy.x, fpxy.y);
}

void app_setup_param(AppCtrl_t &ac) {
  /* Init */
  sc = new SetupCommands();

  /* Display */
  ac.oled->cls(NULL, 0);
  ac.oled->locate(1, 1);
  ac.oled->printf("Connect to PC");
  ac.oled->locate(3, 6);
  ac.oled->printf("----");
  ac.oled->redraw();

  /* Setup USB */
  ac.usbmidi = new USBMIDI();
  ac.usbmidi->attachControlChange(callback(&onControlChange));      // Handle CC123 All note off
  ac.usbmidi->attachSystemExclusive(callback(&onSystemExclusive));  // Mainly commands parser

  /* Main loop */
  while (1) {

    // Check if there are any commands
    if (sc->isCommandIn()) {
      // Do command
      switch (sc->cmd) {
        case SC_CMD_READ_DEVINF:    readDevInf(ac);     break;
        case SC_CMD_READ_PARAM:     readParam(ac);      break;
        case SC_CMD_WRITE_PARAM:    writeParam(ac);     break;
        case SC_CMD_TEST_LED:       testLed(ac);        break;
        case SC_CMD_CALIB_JOYSTICK: calibJoystick(ac);  break;
      }
      sc->step = STEP_WAITING_FOR_TX_FIRST_PACKET;

      // Tx response
      uint8_t  pkt[SC_PKT_TX_MAX_SIZE];
      uint16_t length;
      bool     hasNextPkt;
      do {
        sc->txResponse(pkt, &length, &hasNextPkt);
        ac.usbmidi->sendSystemExclusive(pkt, length, hasNextPkt);
      } while (hasNextPkt);
    }
  } // End of while loop

}