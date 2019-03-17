#ifndef SETUPCOMMANDS_H
#define SETUPCOMMANDS_H

#include "mbed.h"
#include "USBMIDI.h"


#define SC_PKT_RX_MAX_SIZE (SYSEX_BUF_SIZE) // 64
#define SC_PKT_TX_MAX_SIZE (48)             // Raw_data(48bytes) -> USBMIDI_event_packet(64bytes)

#define SC_PKT_SYSEX 0xF0   // SysEx
#define SC_PKT_MID0  0x00   // Manufacturer SysEx ID
#define SC_PKT_MID1  0x7F
#define SC_PKT_MID2  0x7F
#define SC_PKT_RSV   0x00   // Reserved
#define SC_PKT_EOX   0xF7   // End of SysEx

#define SC_CMD_READ_DEVINF                 0x10
#define SC_CMD_READ_DEVINF_RX_LEN          7
#define SC_CMD_READ_DEVINF_TX_LEN          19
#define SC_CMD_READ_DEVINF_RX_MULTI_PKT    false
#define SC_CMD_READ_DEVINF_TX_MULTI_PKT    false
  
#define SC_CMD_READ_PARAM                  0x30
#define SC_CMD_READ_PARAM_RX_LEN           7
#define SC_CMD_READ_PARAM_TX_LEN           4103
#define SC_CMD_READ_PARAM_RX_MULTI_PKT     false
#define SC_CMD_READ_PARAM_TX_MULTI_PKT     true
  
#define SC_CMD_WRITE_PARAM                 0x31
#define SC_CMD_WRITE_PARAM_RX_LEN          4103
#define SC_CMD_WRITE_PARAM_TX_LEN          7
#define SC_CMD_WRITE_PARAM_RX_MULTI_PKT    true
#define SC_CMD_WRITE_PARAM_TX_MULTI_PKT    false
  
#define SC_CMD_TEST_LED                    0x42
#define SC_CMD_TEST_LED_RX_LEN             13
#define SC_CMD_TEST_LED_TX_LEN             7
#define SC_CMD_TEST_LED_RX_MULTI_PKT       false
#define SC_CMD_TEST_LED_TX_MULTI_PKT       false

#define SC_CMD_CALIB_JOYSTICK              0x53
#define SC_CMD_CALIB_JOYSTICK_RX_LEN       7
#define SC_CMD_CALIB_JOYSTICK_TX_LEN       15
#define SC_CMD_CALIB_JOYSTICK_RX_MULTI_PKT false
#define SC_CMD_CALIB_JOYSTICK_TX_MULTI_PKT false

#define SC_DATA_BUF_SIZE    2048

enum CommandParserStateMachine {
    STEP_WAITING_FOR_RX_FIRST_PACKET = 0,
    STEP_WAITING_FOR_RX_NEXT_PACKET,
    STEP_WAITING_FOR_TX_FIRST_PACKET,
    STEP_WAITING_FOR_TX_NEXT_PACKET,
};


class SetupCommands {
public:
  SetupCommands();

  int rxRequest(uint8_t *pkt, uint16_t length, bool hasNextPkt);

  int isCommandIn();

  int txResponse(uint8_t *pkt, uint16_t *length, bool *hasNextPkt);

  uint8_t   step;   // state-machine, enum CommandParserStateMachine
  uint8_t   cmd;    // command
  uint16_t  dlen;   // data length
  uint8_t  *dbuf;   // data buffer

private:
  void reset();

  bool verifyRxHeader(uint8_t *pkt);

  bool verifyRxCmdReadDevInf(uint16_t length, bool multi_pkt);

  bool verifyRxCmdReadParam(uint16_t length, bool multi_pkt);

  bool verifyRxCmdWriteParam(uint16_t length, bool multi_pkt);

  bool verifyRxCmdTestLed(uint16_t length, bool multi_pkt);

  bool verifyRxCmdCalibJoystick(uint16_t length, bool multi_pkt);

  void copyNibblesToBytes(uint8_t *nb, uint8_t *bt, uint16_t bt_count);

  void copyBytesToNibbles(uint8_t *bt, uint8_t *nb, uint16_t bt_count);
};

#endif