#include "hid_io.h"


//==============================================================================================
// HID In
//==============================================================================================
void hid_io_in(AppCtrl_t &ac, HidParam *param, HidManager *manager) {

  // TODO: Motor vibration

}



//==============================================================================================
// HID Out
//==============================================================================================
typedef struct HidOutDataInType {
  int   keys;
  float pot;
  float fsr;
  Vector2D joy_v2d;
  int      joy_dir;
  bool     joy_pressed;
  float acc_v3d[3];
  int jw[2];
} HidOutDataIn_t;

typedef struct HidOutDataOutType {
  int8_t x;   // x_axis
  int8_t y;   // y_axis
  int8_t t;   // throttle
  int8_t r;   // rudder
  uint32_t b; // buttons
  int8_t h;   // POV-hat,  0-8: N, NE, E, SE, S, SW, W, NW, Center
} HidOutDataOut_t;

static HidOutDataOut_t last_dout = {0, 0, 0, 0, 0, 8};

static bool isSwitchJoystickTrigger(HidOutDataIn_t &din, uint8_t ass) {
  if (ass >= CTRL_KEY_1_JOY_XY && ass <= CTRL_KEY_21_JOY_XY) {
    int switches = din.keys | ((din.joy_pressed ? 1 : 0) << 20);  // Bit0 ~ Bit20 --> SW1 ~ SW21
    int bit_n = ass - CTRL_KEY_1_JOY_XY;
    return (switches & (1 << bit_n)) ? 1 : 0;
  } else {
    return 0;
  }
}

static float dead_zone(float in, float rate) {
  float out;
  if      (in >  rate) out = map(in,  rate,  1.00f, 0,  1.00f);
  else if (in < -rate) out = map(in, -1.00f, -rate, -1.00f, 0);
  else                 out = 0;
  return out;
}

static int xy2dir(int x, int y) {
  int n = 0;
  if      (x == 0 && y >  0) n = 0;  // N
  else if (x >  0 && y >  0) n = 1;  // NE
  else if (x >  0 && y == 0) n = 2;  // E
  else if (x >  0 && y <  0) n = 3;  // SE
  else if (x == 0 && y <  0) n = 4;  // S
  else if (x <  0 && y <  0) n = 5;  // SW
  else if (x <  0 && y == 0) n = 6;  // W
  else if (x <  0 && y >  0) n = 7;  // NW
  return n;
}

static void get_xy(HidParam *param, HidManager *manager, HidOutDataIn_t &din, HidOutDataOut_t &dout) {
  uint8_t &ass_xy = param->p2l_assign.xy;
  uint8_t &ass_tr = param->p2l_assign.tr;
  uint8_t &ass_h  = param->p2l_assign.hat;

  if ((ass_xy == CTRL_JOY_XY) && (isSwitchJoystickTrigger(din, ass_tr) == false) && (isSwitchJoystickTrigger(din, ass_h) == false)) {
    float &xi = din.joy_v2d.x;
    float &yi = din.joy_v2d.y;
    int8_t &xo = dout.x;
    int8_t &yo = dout.y;
    xi = dead_zone(xi, 0.10f);  // Dead zone range: +/-10%
    yi = dead_zone(yi, 0.10f);  // Dead zone range: +/-10%
    xo = map(xi, -1.0f, 1.0f, -127, 127);
    yo = map(yi, -1.0f, 1.0f, 127, -127);
    xo = manager->joy_filter[0]->add(xo).average();
    yo = manager->joy_filter[1]->add(yo).average();

  } else if (ass_xy == CTRL_ACC_XY) {
    din.pot = (1.0f - din.pot); // Inverse
    float scale = map(din.pot, 0, 1.0f, 1.0f, 3.0f); // Scale range 1.0-3.0
    float &xi = din.acc_v3d[0];
    float &yi = din.acc_v3d[1];
    int8_t &xo = dout.x;
    int8_t &yo = dout.y;
    xi = dead_zone(xi, 0.10f);  // Dead zone range: +/-10%
    yi = dead_zone(yi, 0.10f);  // Dead zone range: +/-10%
    xo = map(xi*scale, -1.0f, 1.0f, -127, 127);
    yo = map(yi*scale, -1.0f, 1.0f, 127, -127);
    xo = manager->acc_filter[0]->add(xo).average();
    yo = manager->acc_filter[1]->add(yo).average();

  } else {
      dout.x = 0;
      dout.y = 0;
  }
}

static void get_tr(HidParam *param, HidManager *manager, HidOutDataIn_t &din, HidOutDataOut_t &dout) {
  uint8_t &ass_tr = param->p2l_assign.tr;
  uint8_t &ass_h  = param->p2l_assign.hat;
  uint8_t jog_gain = param->p2l_jog_gain + 1;

  if ((ass_tr == CTRL_JOY_XY) && (isSwitchJoystickTrigger(din, ass_h) == false)) {
    float &xi = din.joy_v2d.x;
    float &yi = din.joy_v2d.y;
    int8_t &xo = dout.t;
    int8_t &yo = dout.r;
    xi = dead_zone(xi, 0.10f);  // Dead zone range: +/-10%
    yi = dead_zone(yi, 0.10f);  // Dead zone range: +/-10%
    xo = map(yi, -1.0f, 1.0f, -127, 127);
    yo = map(xi, -1.0f, 1.0f, -127, 127);
    xo = manager->joy_filter[0]->add(xo).average();
    yo = manager->joy_filter[1]->add(yo).average();

  } else if (ass_tr == CTRL_ACC_XY) {
    din.pot = (1.0f - din.pot); // Inverse
    float scale = map(din.pot, 0, 1.0f, 1.0f, 3.0f); // Scale range 1.0-3.0
    float &xi = din.acc_v3d[0];
    float &yi = din.acc_v3d[1];
    int8_t &xo = dout.t;
    int8_t &yo = dout.r;
    xi = dead_zone(xi, 0.10f);  // Dead zone range: +/-10%
    yi = dead_zone(yi, 0.10f);  // Dead zone range: +/-10%
    xo = map(yi*scale, -1.0f, 1.0f, -127, 127);
    yo = map(xi*scale, -1.0f, 1.0f, -127, 127);
    xo = manager->acc_filter[0]->add(xo).average();
    yo = manager->acc_filter[1]->add(yo).average();

  } else if (ass_tr == CTRL_JOG_LR) {
    int &xi = din.jw[0];
    int &yi = din.jw[1];
    int8_t &xo = dout.t;
    int8_t &yo = dout.r;
    xi = (int)manager->whe_value[0] + (xi * jog_gain);
    yi = (int)manager->whe_value[1] + (yi * jog_gain);
    if (xi >  127) xi =  127;
    if (xi < -127) xi = -127;
    if (yi >  127) yi =  127;
    if (yi < -127) yi = -127;
    xi = xi / jog_gain * jog_gain;
    yi = yi / jog_gain * jog_gain;
    xo = manager->whe_value[0] = xi;
    yo = manager->whe_value[1] = yi;
    xo =  xo;
    yo = -yo;

  } else if (isSwitchJoystickTrigger(din, ass_tr)) {
    float &xi = din.joy_v2d.x;
    float &yi = din.joy_v2d.y;
    int8_t &xo = dout.t;
    int8_t &yo = dout.r;
    xi = dead_zone(xi, 0.10f);  // Dead zone range: +/-10%
    yi = dead_zone(yi, 0.10f);  // Dead zone range: +/-10%
    xo = map(yi, -1.0f, 1.0f, -127, 127);
    yo = map(xi, -1.0f, 1.0f, -127, 127);
    xo = manager->joy_filter[0]->add(xo).average();
    yo = manager->joy_filter[1]->add(yo).average();

  } else {
      dout.t = 0;
      dout.r = 0;
  }
}

static void get_b(HidParam *param, HidManager *manager, HidOutDataIn_t &din, HidOutDataOut_t &dout) {
  int switches = din.keys | ((din.joy_pressed ? 1 : 0) << 20);  // Bit0 ~ Bit20 --> SW1 ~ SW21

  for (int i=0; i<21; i++) {
    uint8_t ass_b = param->p2l_assign.btn[i];
    if (ass_b >= CTRL_KEY_1 && ass_b <= CTRL_KEY_21) {
      int bit_n = ass_b - CTRL_KEY_1;
      if (switches & (1 << bit_n)) {
        dout.b |= (1 << i);
      }
    }
  } // End of for loop
}

static void get_h(HidParam *param, HidManager *manager, HidOutDataIn_t &din, HidOutDataOut_t &dout) {
  uint8_t &ass_h = param->p2l_assign.hat;

  if (ass_h == CTRL_KEY_5_6_14_13_NESW) {
    uint8_t nesw = 0;
    if (din.keys & (1 << 4))  nesw |= 0x8;  // Is Key5 pressed ?
    if (din.keys & (1 << 5))  nesw |= 0x4;  // Is Key6 pressed ?
    if (din.keys & (1 << 13)) nesw |= 0x2;  // Is Key14 pressed ?
    if (din.keys & (1 << 12)) nesw |= 0x1;  // Is Key13 pressed ?
    switch (nesw) {
      case 0x8: dout.h = 0; break;  // N
      case 0xC: dout.h = 1; break;  // NE
      case 0x4: dout.h = 2; break;  // E
      case 0x6: dout.h = 3; break;  // SE
      case 0x2: dout.h = 4; break;  // S
      case 0x3: dout.h = 5; break;  // SW
      case 0x1: dout.h = 6; break;  // W
      case 0x9: dout.h = 7; break;  // NW
      default:  dout.h = 8; break;  // Center
    }

  } else if (isSwitchJoystickTrigger(din, ass_h)) {
    float &xi = din.joy_v2d.x;
    float &yi = din.joy_v2d.y;
    xi = dead_zone(xi, 0.10f);  // Dead zone range: +/-10%
    yi = dead_zone(yi, 0.10f);  // Dead zone range: +/-10%
    if (xi != 0 || yi != 0) {
      dout.h = (din.joy_dir == 0) ? 8 : (din.joy_dir - 1);
    } else {
      dout.h = 8;
    }

  } else {
    dout.h = 8;
  }
}

void hid_io_out(AppCtrl_t &ac, HidParam *param, HidManager *manager) {
  /* Input data */
  HidOutDataIn_t din;
  din.keys = ac.keys->read();
  din.pot = ac.pot->read();
  din.fsr = ac.fsr->read();

  din.joy_v2d = ac.joy->get_vector();
  din.joy_dir = ac.joy->get_direction();
  din.joy_pressed = ac.joy->is_pressed();

  ac.acc->get_vector(din.acc_v3d);

  din.jw[0] = ac.jw[0]->getPulses();
  din.jw[1] = ac.jw[1]->getPulses();
  ac.jw[0]->reset();
  ac.jw[1]->reset();

  /* Output data */
  HidOutDataOut_t dout = {0, 0, 0, 0, 0, 8};

  /* Process */
  get_xy(param, manager, din, dout);
  get_tr(param, manager, din, dout);
  get_b(param, manager, din, dout);
  get_h(param, manager, din, dout);

  /* USB Update */
  ac.usbjoy->update(dout.t, dout.r, dout.x, dout.y, dout.b, dout.h);

  /* Update OLED display */
  const char *direction[] = {
    " \x18 ",     // 0 N
    " \x18\x1A",  // 1 NE
    "  \x1A",     // 2 E
    " \x19\x1A",  // 3 SE
    " \x19 ",     // 4 S
    "\x1B\x19 ",  // 5 SW
    "\x1B  ",     // 6 W
    "\x1B\x18 ",  // 7 NW
  };

  if ((dout.x | dout.y) && (dout.x != last_dout.x || dout.y != last_dout.y)) {
    // X/Y
    int n = xy2dir(dout.x, -dout.y);
    hid_ui_write(ac, "X/Y %s", direction[n]);

  } else if ((dout.t | dout.r) && (dout.t != last_dout.t || dout.r != last_dout.r)) {
    // T/R
    uint8_t &ass_tr = param->p2l_assign.tr;
    if (ass_tr == CTRL_JOG_LR) {
      int8_t t = dout.t - last_dout.t;
      int8_t r = dout.r - last_dout.r;
      char ct;
      char cr;
      if      (t >  0) ct = '\x18'; // up
      else if (t <  0) ct = '\x19'; // down
      else             ct = ' ';
      if      (r >  0) cr = '\x18'; // up
      else if (r <  0) cr = '\x19'; // down
      else             cr = ' ';
      hid_ui_write(ac, "T/R %c %c", ct, cr);
    } else {
      int n = xy2dir(dout.r, dout.t);
      hid_ui_write(ac, "T/R %s", direction[n]);
    }

  } else if ((dout.b) && (dout.b != last_dout.b)) {
    // Button
    for(int i=0; i<21; i++) {
      if(dout.b & (1<<i)) {
        hid_ui_write(ac, "Btn %2d ", i+1);
        break;
      }
    }

  } else if ((dout.h != 8) && (dout.h != last_dout.h)) {
    // POV-hat
    hid_ui_write(ac, "POV %s", direction[dout.h]);

  }

  hid_ui_update(ac, param);
  memcpy(&last_dout, &dout, sizeof(HidOutDataOut_t));
}