#include "DataFlash.h"

DataFlash::DataFlash(): FlashIAP() {
    init();
}

int DataFlash::readAllData(uint8_t *buf, uint16_t *len) {
    *len = DATAFLASH_ALL_DATA_SIZE;
    load(&buf[0], DATAFLASH_ALL_DATA_OffSET, DATAFLASH_ALL_DATA_SIZE);
    return 0;
}

int DataFlash::writeAllData(uint8_t *buf, uint16_t len) {
    if (len != DATAFLASH_ALL_DATA_SIZE) return -1;
    save(&buf[0], DATAFLASH_ALL_DATA_OffSET, DATAFLASH_ALL_DATA_SIZE);
    return 0;
}

int DataFlash::readImageIntro(uint8_t *buf) {
    load(&buf[0], DATAFLASH_IMAGE_INTRO_OffSET, DATAFLASH_IMAGE_INTRO_SIZE);
    return 0;
}

int DataFlash::readImageIcons(uint8_t *buf) {
    load(&buf[0], DATAFLASH_IMAGE_ICONS_OffSET, DATAFLASH_IMAGE_ICONS_SIZE);
    return 0;
}

int DataFlash::readImageIconsN(uint8_t *buf, uint8_t n) {
    for(int i=0; i<4; i++) {
        load(&buf[i*32], (DATAFLASH_IMAGE_ICONS_OffSET + (n*32) + (i*128)), 32);
    }
    return 0;
}

int DataFlash::readParamMidiKeyboard(uint8_t *buf) {
    load(&buf[0], DATAFLASH_PARAM_MIDI_KB_OffSET, DATAFLASH_PARAM_MIDI_KB_SIZE);
    return 0;
}

int DataFlash::readParamMidiPad(uint8_t *buf) {
    load(&buf[0], DATAFLASH_PARAM_MIDI_PAD_OffSET, DATAFLASH_PARAM_MIDI_PAD_SIZE);
    return 0;
}

int DataFlash::readParamMidiDj(uint8_t *buf) {
    load(&buf[0], DATAFLASH_PARAM_MIDI_DJ_OffSET, DATAFLASH_PARAM_MIDI_DJ_SIZE);
    return 0;
}

int DataFlash::readParamHidJoystick(uint8_t *buf) {
    load(&buf[0], DATAFLASH_PARAM_HID_JOYSTICK_OffSET, DATAFLASH_PARAM_HID_JOYSTICK_SIZE);
    return 0;
}

int DataFlash::readParamOthersCalibJoystick(uint16_t &x, uint16_t &y) {
    uint16_t buf[2];
    load((uint8_t*)buf, DATAFLASH_PARAM_OTHERS_OffSET+0, 4);
    x = buf[0];
    y = buf[1];
    return 0;
}

int DataFlash::load(uint8_t *dest, uint16_t offset, uint16_t size) {
    read(&dest[0], (DATAFLASH_START_ADDR + offset), size);
    return 0;
#if 0
    uint8_t *page_buf = new uint8_t[DATAFLASH_PAGE_SIZE];
    if (page_buf == NULL) return -1;

    read(page_buf, DATAFLASH_START_ADDR, DATAFLASH_PAGE_SIZE);
    memcpy(dest, &(page_buf[offset]), size);

    delete page_buf;
    return 0;
#endif
}

int DataFlash::save(uint8_t *src, uint16_t offset, uint16_t size) {
    erase((DATAFLASH_START_ADDR + offset), size);
    program(&src[0], (DATAFLASH_START_ADDR + offset), size);
    return 0;
#if 0
    uint8_t *page_buf = new uint8_t[DATAFLASH_PAGE_SIZE];
    if (page_buf == NULL) return -1;

    // Save data by steps of Read/modify/write
    read(page_buf, DATAFLASH_START_ADDR, DATAFLASH_PAGE_SIZE);
    memcpy(&page_buf[offset], src, size);
    erase(DATAFLASH_START_ADDR, DATAFLASH_PAGE_SIZE);
    program(page_buf, DATAFLASH_START_ADDR, DATAFLASH_PAGE_SIZE);

    delete page_buf;
    return 0;
#endif
}
