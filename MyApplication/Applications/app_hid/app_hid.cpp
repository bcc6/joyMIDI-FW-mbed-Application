#include "app_hid.h"

#if 0
static void debug_show_param(HidParam *p) {

  /* p2l */
  DEBUG_OUT("\np2l:\n");
  HEX_DUMP("ass", &p->p2l_assign, 24);
  DEBUG_OUT("jog_gain=%d\n", p->p2l_jog_gain);

  /* monoled */
  DEBUG_OUT("\nmono:\n");
  DEBUG_OUT("mode=%d switches=%05X\n", p->monoled_mode, p->monoled_switches);
}
#endif

static int load_user_parameters(AppCtrl_t &ac, HidParam *param) {
  uint8_t *buf;
  buf = new uint8_t[128];

  ac.df->readParamHidJoystick(buf);
  param->load_p2l_param((HidAssign_t *)&buf[0], buf[24]);
  param->load_monoled_param(buf[25], *((uint32_t *)&buf[26]));

  delete buf;
  return 0;
}

void app_hid(AppCtrl_t &ac) {
  /* Get parameter and manager */
  HidParam   *param   = new HidParam();
  HidManager *manager = new HidManager();

  /* Load user parameters */
#if 1
  load_user_parameters(ac, param);
  // debug_show_param(param);
#else
  if (load_user_parameters(ac, param) < 0) {
    param->load_default_parameters();
  }
  // debug_show_param(param);
#endif

  /* Display base framework */
  hid_ui_base(ac, param);

  /* Setup USB */
  ac.usbjoy = new USBJoystick();
  // DEBUG_OUT("USB Pluged\n");  // This spend too much time and lead the iOS connect fail

  /* Setup HID-in */
  hid_io_in(ac, param, manager);

  /* Setup HID-out */
  EventQueue *eq = mbed_event_queue();                // Request the shared queue
  eq->call_every(10, hid_io_out, ac, param, manager); // Call every 10ms
  eq->dispatch_forever();
}
