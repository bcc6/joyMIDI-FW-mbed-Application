/**
 * This code refer to BLE_MIDI library at mbed by Kaoru Shoji
 */
#ifndef SERIALMIDI_H
#define SERIALMIDI_H

#include "mbed.h"
#include "BufferedSerial.h"


class SerialMIDI : public BufferedSerial {    
public:
    SerialMIDI(PinName tx, PinName rx, int baud, uint32_t buf_size, uint32_t tx_multiple);

    /* Serial Rx */
    inline void attachNoteOff(Callback<void(uint8_t, uint8_t, uint8_t)> fn)        { onNoteOff = fn; }
    inline void attachNoteOn (Callback<void(uint8_t, uint8_t, uint8_t)> fn)        { onNoteOn = fn; }
    inline void attachPolyAftertouch(Callback<void(uint8_t, uint8_t, uint8_t)> fn) { onPolyAftertouch = fn; }
    inline void attachControlChange(Callback<void(uint8_t, uint8_t, uint8_t)> fn)  { onControlChange = fn; }
    inline void attachProgramChange(Callback<void(uint8_t, uint8_t)> fn)           { onProgramChange = fn; }
    inline void attachChannelAftertouch(Callback<void(uint8_t, uint8_t)> fn)       { onChannelAftertouch = fn; }
    inline void attachPitchBend(Callback<void(uint8_t, uint16_t)> fn)              { onPitchBend = fn; }
    inline void attachSystemExclusive(Callback<void(uint8_t*, uint8_t)> fn) { onSystemExclusive = fn; }
    inline void attachTimeCodeQuarterFrame(Callback<void(uint8_t)> fn)      { onTimeCodeQuarterFrame = fn; }
    inline void attachSongPositionPointer(Callback<void(uint16_t)> fn)      { onSongPositionPointer = fn;}
    inline void attachSongSelect(Callback<void(uint8_t)> fn)                { onSongSelect = fn; }
    inline void attachTuneRequest(Callback<void()> fn)                      { onTuneRequest = fn; }
    inline void attachTimingClock(Callback<void()> fn)   { onTimingClock = fn; }
    inline void attachStart(Callback<void()> fn)         { onStart = fn; }
    inline void attachContinue(Callback<void()> fn)      { onContinue = fn; }
    inline void attachStop(Callback<void()> fn)          { onStop = fn; }
    inline void attachActiveSensing(Callback<void()> fn) { onActiveSensing = fn; }
    inline void attachReset(Callback<void()> fn)         { onReset = fn; }
    void parseRxMessageRoutine();

    /* Serial Tx */
    void sendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)        { sendMidiMessage(0x80 | (channel & 0xf), note, velocity); }
    void sendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)         { sendMidiMessage(0x90 | (channel & 0xf), note, velocity); }
    void sendPolyAftertouch(uint8_t channel, uint8_t note, uint8_t pressure) { sendMidiMessage(0xa0 | (channel & 0xf), note, pressure); }
    void sendControlChange(uint8_t channel, uint8_t function, uint8_t value) { sendMidiMessage(0xb0 | (channel & 0xf), function, value); }
    void sendProgramChange(uint8_t channel, uint8_t program)                 { sendMidiMessage(0xc0 | (channel & 0xf), program); }
    void sendChannelAftertouch(uint8_t channel, uint8_t pressure)            { sendMidiMessage(0xd0 | (channel & 0xf), pressure); }
    void sendPitchBend(uint8_t channel, uint16_t amount)                     { sendMidiMessage(0xe0 | (channel & 0xf), amount & 0x7f, (amount >> 7) & 0x7f); }
    void sendSystemExclusive(uint8_t *sysex, uint8_t length) { for (int i=0; i<length; i++) putc(sysex[i]); }
    void sendTimeCodeQuarterFrame(uint8_t timing)            { sendMidiMessage(0xf1, timing & 0x7f); }
    void sendSongPositionPointer(uint16_t position)          { sendMidiMessage(0xf2, position & 0x7f, (position >> 7) & 0x7f); }
    void sendSongSelect(uint8_t song)                        { sendMidiMessage(0xf3, song & 0x7f); }
    void sendTuneRequest()                                   { sendMidiMessage(0xf6); }
    void sendTimingClock()   { sendMidiMessage(0xf8); }
    void sendStart()         { sendMidiMessage(0xfa); }
    void sendContinue()      { sendMidiMessage(0xfb); }
    void sendStop()          { sendMidiMessage(0xfc); }
    void sendActiveSensing() { sendMidiMessage(0xfe); }
    void sendReset()         { sendMidiMessage(0xff); }

private:
    void sendMidiMessage(uint8_t data0) { putc(data0); }
    void sendMidiMessage(uint8_t data0, uint8_t data1) { putc(data0); putc(data1); }
    void sendMidiMessage(uint8_t data0, uint8_t data1, uint8_t data2) { putc(data0); putc(data1); putc(data2); }

    Callback<void(uint8_t, uint8_t, uint8_t)> onNoteOff;
    Callback<void(uint8_t, uint8_t, uint8_t)> onNoteOn;
    Callback<void(uint8_t, uint8_t, uint8_t)> onPolyAftertouch;
    Callback<void(uint8_t, uint8_t, uint8_t)> onControlChange;
    Callback<void(uint8_t, uint8_t)>          onProgramChange;
    Callback<void(uint8_t, uint8_t)>          onChannelAftertouch;
    Callback<void(uint8_t, uint16_t)>         onPitchBend;
    Callback<void(uint8_t *, uint8_t)>        onSystemExclusive;
    Callback<void(uint8_t)>                   onTimeCodeQuarterFrame;
    Callback<void(uint16_t)>                  onSongPositionPointer;
    Callback<void(uint8_t)>                   onSongSelect;
    Callback<void()>                          onTuneRequest;
    Callback<void()>                          onTimingClock;
    Callback<void()>                          onStart;
    Callback<void()>                          onContinue;
    Callback<void()>                          onStop;
    Callback<void()>                          onActiveSensing;
    Callback<void()>                          onReset;


    /**** MIDI Paser ****/

    enum MidiMessage {
        NoteOff               = 0x80,    // Channel Voice - Note Off : 3bytes
        NoteOn                = 0x90,    // Channel Voice - Note On : 3bytes
        PolyAfterTouch        = 0xA0,    // Channel Voice - Polyphonic AfterTouch : 3bytes
        ControlChange         = 0xB0,    // Channel Voice - Control Change / Channel Mode : 3bytes
        ProgramChange         = 0xC0,    // Channel Voice - Program Change : 2bytes
        ChannelAfterTouch     = 0xD0,    // Channel Voice - Channel (monophonic) AfterTouch : 2bytes
        PitchBend             = 0xE0,    // Channel Voice - Pitch Bend : 3bytes
        SystemExclusive       = 0xF0,    // System Common - System Exclusive : nbytes
        TimeCodeQuarterFrame  = 0xF1,    // System Common - MIDI Time Code Quarter Frame : 2bytes
        SongPosition          = 0xF2,    // System Common - Song Position Pointer : 3bytes
        SongSelect            = 0xF3,    // System Common - Song Select : 2bytes
        TuneRequest           = 0xF6,    // System Common - Tune Request : 1byte
        EOX                   = 0xF7,    // System Common - End of Exclusive : 1byte
        TimingClock           = 0xF8,    // System Real Time - Timing Clock : 1byte
        Start                 = 0xFA,    // System Real Time - Start : 1byte
        Continue              = 0xFB,    // System Real Time - Continue : 1byte
        Stop                  = 0xFC,    // System Real Time - Stop : 1byte
        ActiveSensing         = 0xFE,    // System Real Time - Active Sensing : 1byte
        Reset                 = 0xFF,    // System Real Time - System Reset : 1byte
    };

    enum MSG_STATE {
        MSG_STATE_STATUS_BYTE = 0,
        MSG_STATE_DATA_BYTE,
        MSG_STATE_SYSEX,
    };
    MSG_STATE msgState;
    uint8_t   msgBuf[64];
    uint8_t   msgBufPos;
    uint8_t   msgRunningStatus;

    void msg_reset();
};

#endif
