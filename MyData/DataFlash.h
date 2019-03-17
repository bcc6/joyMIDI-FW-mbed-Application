#ifndef DATAFLASH_H
#define DATAFLASH_H

#include "mbed.h"
#include "my_debug.h"

#define DATAFLASH_PAGE_SIZE   2048                            // Page size 2KB
#define DATAFLASH_START_ADDR  (0x20000 - DATAFLASH_PAGE_SIZE) // Use the last page

#define DATAFLASH_ALL_DATA_OffSET     0
#define DATAFLASH_ALL_DATA_SIZE       2048

#define DATAFLASH_IMAGE_INTRO_OffSET        0
#define DATAFLASH_IMAGE_INTRO_SIZE          512
#define DATAFLASH_IMAGE_ICONS_OffSET        512
#define DATAFLASH_IMAGE_ICONS_SIZE          512
#define DATAFLASH_PARAM_MIDI_KB_OffSET      1024
#define DATAFLASH_PARAM_MIDI_KB_SIZE        256
#define DATAFLASH_PARAM_MIDI_PAD_OffSET     1280
#define DATAFLASH_PARAM_MIDI_PAD_SIZE       256
#define DATAFLASH_PARAM_MIDI_DJ_OffSET      1536
#define DATAFLASH_PARAM_MIDI_DJ_SIZE        256
#define DATAFLASH_PARAM_HID_JOYSTICK_OffSET 1792
#define DATAFLASH_PARAM_HID_JOYSTICK_SIZE   128
#define DATAFLASH_PARAM_OTHERS_OffSET       1920
#define DATAFLASH_PARAM_OTHERS_SIZE         128


class DataFlash : public FlashIAP {
 public:
  DataFlash();

  int readAllData(uint8_t *buf, uint16_t *len);

  int writeAllData(uint8_t *buf, uint16_t len);

  int readImageIntro(uint8_t *buf);

  int readImageIcons(uint8_t *buf);

  int readImageIconsN(uint8_t *buf, uint8_t n);

  int readParamMidiKeyboard(uint8_t *buf);
  
  int readParamMidiPad(uint8_t *buf);
  
  int readParamMidiDj(uint8_t *buf);

  int readParamHidJoystick(uint8_t *buf);

  int readParamOthersCalibJoystick(uint16_t &x, uint16_t &y);

 private:
  /** Load data to buffer from internal flash
   *  @param dest   Destination buffer
   *  @param offset Flash offset to read from
   *  @param size   Size to load to buffer in bytes, 4 bytes aligment
   *  @return       0 on success, negative error code on failure
   */
  int load(uint8_t *dest, uint16_t offset, uint16_t size);

  /** Save data to internal flash from buffer
   *  @param src    Source buffer
   *  @param offset Flash offset to write to
   *  @param size   Size to save to buffer in bytes, page size (2048 bytes)
   *  @return       0 on success, negative error code on failure
   */
  int save(uint8_t *src, uint16_t offset, uint16_t size);
};

#endif