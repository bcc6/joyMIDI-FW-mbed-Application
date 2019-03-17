#include "SetupCommands.h"

SetupCommands::SetupCommands() {
  dbuf = new uint8_t[SC_DATA_BUF_SIZE];
  reset();
}

int SetupCommands::rxRequest(uint8_t *pkt, uint16_t length, bool hasNextPkt) {

  switch (step) {

    //--------------------------------------------------------------------------
    case STEP_WAITING_FOR_RX_FIRST_PACKET: {
      if (verifyRxHeader(&pkt[0])) {

        switch (pkt[5]) {

          case SC_CMD_READ_DEVINF:
            if (verifyRxCmdReadDevInf(length, hasNextPkt)) {
              cmd = SC_CMD_READ_DEVINF;
            } else goto PARSE_NG;
          break;

          case SC_CMD_READ_PARAM:
            if (verifyRxCmdReadParam(length, hasNextPkt)) {
              cmd = SC_CMD_READ_PARAM;
            } else goto PARSE_NG;
          break;

          case SC_CMD_WRITE_PARAM:
            if (verifyRxCmdWriteParam(length, hasNextPkt)) {
              cmd = SC_CMD_WRITE_PARAM;
              dlen = (SC_PKT_RX_MAX_SIZE - 6) / 2;
              copyNibblesToBytes(&pkt[6], &dbuf[0], dlen);
              step = STEP_WAITING_FOR_RX_NEXT_PACKET;
            } else goto PARSE_NG;
          break;

          case SC_CMD_TEST_LED:
            if (verifyRxCmdTestLed(length, hasNextPkt)) {
              cmd = SC_CMD_TEST_LED;
              dlen = 3;
              copyNibblesToBytes(&pkt[6], &dbuf[0], dlen);
            } else goto PARSE_NG;
          break;

          case SC_CMD_CALIB_JOYSTICK:
            if (verifyRxCmdCalibJoystick(length, hasNextPkt)) {
              cmd = SC_CMD_CALIB_JOYSTICK;
            } else goto PARSE_NG;
          break;

          default: goto PARSE_NG;
        }

      } else goto PARSE_NG;
    } break;

    //--------------------------------------------------------------------------
    case STEP_WAITING_FOR_RX_NEXT_PACKET: {

      switch (cmd) {
          
        case SC_CMD_WRITE_PARAM:
          if (length == SC_PKT_RX_MAX_SIZE && hasNextPkt == true) {
            // Not last packet
            uint16_t cnt = SC_PKT_RX_MAX_SIZE / 2;
            if ((dlen + cnt) > SC_DATA_BUF_SIZE) goto PARSE_NG;
            copyNibblesToBytes(&pkt[0], &dbuf[dlen], cnt);
            dlen += cnt;
          } else if (hasNextPkt == false) {
            // Last packet
            uint16_t cnt = (length - 1) / 2;
            if ((dlen + cnt) != SC_DATA_BUF_SIZE) goto PARSE_NG;
            copyNibblesToBytes(&pkt[0], &dbuf[dlen], cnt);
            dlen += cnt;
          } else goto PARSE_NG;
        break;

        default: goto PARSE_NG;
      }

    } break;

  }

  return 0;

PARSE_NG:
  reset();
  return -1;
}

int SetupCommands::isCommandIn() {
  int result = 0;

  switch (cmd) {
    case SC_CMD_READ_DEVINF:
    case SC_CMD_READ_PARAM:
    case SC_CMD_TEST_LED:
    case SC_CMD_CALIB_JOYSTICK:
      result = 1;
      break;
    case SC_CMD_WRITE_PARAM:
      result = (dlen == 2048) ? 1 : 0;
      break;
  }
  return result;
}

int SetupCommands::txResponse(uint8_t *pkt, uint16_t *length, bool *hasNextPkt) {

  switch (step) {

    //--------------------------------------------------------------------------
    case STEP_WAITING_FOR_TX_FIRST_PACKET: {

      pkt[0] = SC_PKT_SYSEX;
      pkt[1] = SC_PKT_MID0;
      pkt[2] = SC_PKT_MID1;
      pkt[3] = SC_PKT_MID2;
      pkt[4] = SC_PKT_RSV;
      pkt[5] = cmd;

      switch (cmd) {

        case SC_CMD_READ_DEVINF: {
          copyBytesToNibbles(&dbuf[0], &pkt[6], dlen);
          pkt[SC_CMD_READ_DEVINF_TX_LEN - 1] = SC_PKT_EOX;
          *length = SC_CMD_READ_DEVINF_TX_LEN;
          *hasNextPkt = SC_CMD_READ_DEVINF_TX_MULTI_PKT;
        } break;

        case SC_CMD_READ_PARAM: {
          uint16_t cnt = (SC_PKT_TX_MAX_SIZE - 6) / 2;
          copyBytesToNibbles(&dbuf[0], &pkt[6], cnt);
          dlen -= cnt;
          *length = SC_PKT_TX_MAX_SIZE;
          *hasNextPkt = SC_CMD_READ_PARAM_TX_MULTI_PKT;
          step = STEP_WAITING_FOR_TX_NEXT_PACKET;
        } break;

        case SC_CMD_WRITE_PARAM: {
          pkt[SC_CMD_WRITE_PARAM_TX_LEN - 1] = SC_PKT_EOX;
          *length = SC_CMD_WRITE_PARAM_TX_LEN;
          *hasNextPkt = SC_CMD_WRITE_PARAM_TX_MULTI_PKT;
        } break;

        case SC_CMD_TEST_LED: {
          pkt[SC_CMD_TEST_LED_TX_LEN - 1] = SC_PKT_EOX;
          *length = SC_CMD_TEST_LED_TX_LEN;
          *hasNextPkt = SC_CMD_TEST_LED_TX_MULTI_PKT;
        } break;

        case SC_CMD_CALIB_JOYSTICK: {
          copyBytesToNibbles(&dbuf[0], &pkt[6], dlen);
          pkt[SC_CMD_CALIB_JOYSTICK_TX_LEN - 1] = SC_PKT_EOX;
          *length = SC_CMD_CALIB_JOYSTICK_TX_LEN;
          *hasNextPkt = SC_CMD_CALIB_JOYSTICK_TX_MULTI_PKT;
        } break;
      }
    } break;

    //--------------------------------------------------------------------------
    case STEP_WAITING_FOR_TX_NEXT_PACKET: {

      switch (cmd) {
          
        case SC_CMD_READ_PARAM: {
          if ((dlen * 2 + 1) > SC_PKT_TX_MAX_SIZE ) {
            // Not last packet
            uint16_t cnt = SC_PKT_TX_MAX_SIZE / 2;
            copyBytesToNibbles(&dbuf[SC_DATA_BUF_SIZE - dlen], &pkt[0], cnt);
            dlen -= cnt;
            *length = SC_PKT_TX_MAX_SIZE;
            *hasNextPkt = true;
          } else {
            // Last packet
            uint16_t cnt = dlen;
            copyBytesToNibbles(&dbuf[SC_DATA_BUF_SIZE - dlen], &pkt[0], cnt);
            dlen -= cnt;
            pkt[cnt * 2] = SC_PKT_EOX;
            *length = cnt * 2 + 1;
            *hasNextPkt = false;
          }
        } break;
      }
    } break;

  }

  if (*hasNextPkt == false) {
    reset();
  }

  return 0;
}

void SetupCommands::reset() {
  cmd = dlen = step = 0;
}

bool SetupCommands::verifyRxHeader(uint8_t *pkt) {
  return (pkt[0] == SC_PKT_SYSEX && pkt[1] == SC_PKT_MID0 && 
          pkt[2] == SC_PKT_MID1  && pkt[3] == SC_PKT_MID2);
}

bool SetupCommands::verifyRxCmdReadDevInf(uint16_t length, bool multi_pkt) {
  return (length == SC_CMD_READ_DEVINF_RX_LEN && multi_pkt == SC_CMD_READ_DEVINF_RX_MULTI_PKT);
}

bool SetupCommands::verifyRxCmdReadParam(uint16_t length, bool multi_pkt) {
  return (length == SC_CMD_READ_PARAM_RX_LEN && multi_pkt == SC_CMD_READ_PARAM_RX_MULTI_PKT);
}

bool SetupCommands::verifyRxCmdWriteParam(uint16_t length, bool multi_pkt) {
  return (length == SC_PKT_RX_MAX_SIZE/*SC_CMD_WRITE_PARAM_RX_LEN*/ && multi_pkt == SC_CMD_WRITE_PARAM_RX_MULTI_PKT);
}

bool SetupCommands::verifyRxCmdTestLed(uint16_t length, bool multi_pkt) {
  return (length == SC_CMD_TEST_LED_RX_LEN && multi_pkt == SC_CMD_TEST_LED_RX_MULTI_PKT);
}

bool SetupCommands::verifyRxCmdCalibJoystick(uint16_t length, bool multi_pkt) {
  return (length == SC_CMD_CALIB_JOYSTICK_RX_LEN && multi_pkt == SC_CMD_CALIB_JOYSTICK_RX_MULTI_PKT);
}

void SetupCommands::copyNibblesToBytes(uint8_t *nb, uint8_t *bt, uint16_t bt_count) {
  for (uint16_t i = 0; i < bt_count; i++) {
    bt[i] = ((nb[i*2+0] & 0x0F) << 4) | (nb[i*2+1] & 0x0F);
  }
}

void SetupCommands::copyBytesToNibbles(uint8_t *bt, uint8_t *nb, uint16_t bt_count) {
  for (uint16_t i = 0; i < bt_count; i++) {
    nb[i*2+0] = (bt[i] & 0xF0) >> 4;
    nb[i*2+1] = (bt[i] & 0x0F);
  }
}