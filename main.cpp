#include "mbed.h"
#include "my_debug.h"
#include "my_test.h"
#include "app_common.h"
#include "app_midi.h"
#include "app_hid.h"
#include "app_setup_param.h"

DEBUG_SERIAL(PA_2, PA_3, 115200);

DataFlash df;

USBMIDI     *usbmidi;
USBJoystick *usbjoy;

SerialMIDI  *serialmidi;  // Share the I/O pin with DEBUG_SERIAL

AnalogIn    slide_rotary_pot(PB_1);
SlideRotary pot(slide_rotary_pot);

AnalogIn    fsr_in(PB_4);
FSR         fsr(fsr_in);

AnalogIn    joystick_x(PB_3);
AnalogIn    joystick_y(PB_2);
DebounceIn  joystick_sw(PB_6);
Joystick    joy(joystick_x, joystick_y, joystick_sw);

PwmOut rgbled_r(PC_3);
PwmOut rgbled_g(PC_2);
PwmOut rgbled_b(PC_1);
RGBLed rgb(rgbled_r, rgbled_g, rgbled_b);

DigitalOut leds_sh(PE_10);
DigitalOut leds_st(PE_11);
DigitalOut leds_ds(PE_12);
LEDs leds(leds_sh, leds_st, leds_ds);

// DigitalOut keys_ce(PB_7);
DigitalOut keys_ck(PD_1);
DigitalOut keys_pl(PD_2);
DigitalIn  keys_si(PD_3);
Keys keys(keys_ck, keys_pl, keys_si);

QEI jwa(PA_0, PA_1, NC, 12, QEI::X2_ENCODING);  // Jog-Wheel Left
QEI jwb(PD_0, PB_5, NC, 12, QEI::X2_ENCODING);  // Jog-Wheel Right

class I2C2 : public I2C {
 public:
  I2C2(PinName sda, PinName scl) : I2C(sda, scl) {
    frequency(400000);
    start();
  };
};
I2C2          i2c(PE_0, PC_4);
BMA253        accel(i2c, (0x18 << 1));
SSD1306       oled(i2c, 0x78);

// SystemReport *sys_state;  // for debug

//=================================================================================
// Event

//=================================================================================
// Redefine
void mbed_die(void) {
  // Call flow:　error() -> exit(1) -> mbed_die()
  while (1) {
    for (int i = 0; i < 4; ++i) {
      rgb.write(COLOR_RED);
      wait_ms(150);
      rgb.write(0);
      wait_ms(150);
    }
    for (int i = 0; i < 4; ++i) {
      rgb.write(COLOR_RED);
      wait_ms(400);
      rgb.write(0);
      wait_ms(400);
    }
  }
}

//=================================================================================
// Main
int main(void) {
  DEBUG_OUT("\n%s\n", MBED_CONF_APP_WELCOME_STRING);
  DEBUG_OUT("mbed-os-%d.%d.%d\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
  DEBUG_OUT("fw-%d.%d\n\n", MBED_CONF_APP_FIRMWARE_VERSION_MAJOR, MBED_CONF_APP_FIRMWARE_VERSION_MINOR);

  // sys_state = new SystemReport(5000);
  // sys_state->report_state();

  /* Test functions */
#if (MBED_CONF_APP_RUN_MODE == 2)
  test_console(usbmidi, usbjoy, &pot, &fsr, &joy, &rgb, &leds, &keys, &jwa, &jwb, &accel, &oled);
  while(1);
#endif

  /* Load parameters of calibration */
  {
    uint16_t x, y;
    df.readParamOthersCalibJoystick(x, y);
    float fx = ((float)x)/1000;
    float fy = ((float)y)/1000;
    joy.calibrate(0.0f, fx, 1.0f, 0.0f, fy, 1.0f);
  }

  /* Display intro image & application icons */
  {
    oled.init();
    uint8_t *img;
    img = new uint8_t[DATAFLASH_IMAGE_INTRO_SIZE];
    df.readImageIntro(&img[0]);
    oled.cls((char *)img, 1);
    delete img;
    wait(3);

    img = new uint8_t[DATAFLASH_IMAGE_ICONS_SIZE];
    df.readImageIcons(&img[0]);
    oled.cls((char *)img, 1);
    delete img;
  }

  /* Select to run application */
  AppCtrl_t appctrl;
  appctrl.df = &df;
  appctrl.usbmidi = usbmidi;
  appctrl.usbjoy = usbjoy;
  appctrl.serialmidi = serialmidi;
  appctrl.pot = &pot;
  appctrl.fsr = &fsr;
  appctrl.joy = &joy;
  appctrl.rgb = &rgb;
  appctrl.leds = &leds;
  appctrl.keys = &keys;
  appctrl.jw[0] = &jwa;
  appctrl.jw[1] = &jwb;
  appctrl.acc = &accel;
  appctrl.oled = &oled;

  leds.write((1<<10) | (1<<0) | (1<<1) | (1<<2) | (1<<3));  // led {11,1,2,3,4}

  while (1) {
    int k = keys.read();

    switch (k) {

      case KEY_SW(11):  // key11, Setup parameters
        DEBUG_OUT("param\n");
        rgb.write(COLOR_WHITE);
        wait(0.2);
        while(keys.read());
        rgb.write(0);
        app_setup_param(appctrl);
        break;

      case KEY_SW(1):   // key1, Left-Key Keyboard
        DEBUG_OUT("kb\n");
        rgb.write(COLOR_RED);
        wait(0.2);
        while(keys.read());
        rgb.write(0);
        app_midi(appctrl, APP_MIDI_KB);
        break;

      case KEY_SW(2):   // key2, Up-Key Pad
        DEBUG_OUT("pad\n");
        rgb.write(COLOR_GREEN);
        wait(0.2);
        while(keys.read());
        rgb.write(0);
        app_midi(appctrl, APP_MIDI_PAD);
        break;

      case KEY_SW(3):   // key3, Right-Key DJ
        DEBUG_OUT("dj\n");
        rgb.write(COLOR_BLUE);
        wait(0.2);
        while(keys.read());
        rgb.write(0);
        app_midi(appctrl, APP_MIDI_DJ);
        break;

      case KEY_SW(4):   // key4, Down-Key Joystick
        DEBUG_OUT("app_hid\n");
        rgb.write(COLOR_YELLOW);
        wait(0.2);
        while(keys.read());
        rgb.write(0);
        app_hid(appctrl);
        break;
    }
  } // End of while loop
}
