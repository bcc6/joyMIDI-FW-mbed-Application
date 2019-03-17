#include "SerialMIDI.h"


SerialMIDI::SerialMIDI(PinName tx, PinName rx, int baud, uint32_t buf_size, uint32_t tx_multiple) :
    BufferedSerial(tx, rx, baud, buf_size, tx_multiple),
    msgState(MSG_STATE_STATUS_BYTE), msgBuf(), msgBufPos(0), msgRunningStatus(0)
{ }

void SerialMIDI::parseRxMessageRoutine() {
    uint8_t step = 0;
    uint8_t byte = 0;

    while (1) {
        
        switch (step) {

            //--------------------------------------------------------------------------------------
            // Get data
            case 0x00: {    // Check if any data available
                if (readable()) {           // Yes, extract one byte data
                    byte = getc();
                    step = 0x01;
                    goto DO_IT_RIGHT_NOW;
                } else {                    // No, exit
                    step = 0x00;
                    goto DO_IT_NEXT_TIME;   
                }
            }

            case 0x01: {    // Branch
                if (byte >= 0x80) {

                    /* Status byte (0x80 - 0xFF) */                    
                    if        (byte>=0x80 && byte<=0xEF) { // Channel Voice / Channel Mode Messages
                        msgRunningStatus = byte;
                        step = 0x10;
                    } else if (byte>=0xF0 && byte<=0xF7) { // System Common Messages
                        msgRunningStatus = 0;
                        step = 0x20;
                    } else if (byte>=0xF8              ) { // System RealTime Messages
                        step = 0x30;
                    }

                 } else {

                    /* Data byte (0x00 - 0x7F) */
                    if        (msgBuf[0]>=0x80 && msgBuf[0]<=0xEF) {  // Channel Voice/Mode Messages
                        step = 0x11;
                    } else if (msgBuf[0]>=0xF0 && msgBuf[0]<=0xF7) {  // System Common Messages
                        step = 0x21;
                    } else if (msgBuf[0]==0x00 && msgRunningStatus) { // Channel Voice/Mode Messages (Running Status)
                        step = 0x11;
                    } else {
                        step = 0x00;
                    }

                 }
                 goto DO_IT_RIGHT_NOW;
            }

            //--------------------------------------------------------------------------------------
            // Channel Voice / Channel Mode Messages
            // - 0x80 ~ 0xEF
            // - Included channel information
            // - Support Running-status
            // - NoteOn with velocity zero is the same as NoteOff
            case 0x10:{    // Process Status-Byte
                if (msgState == MSG_STATE_STATUS_BYTE) {

                    msgBuf[msgBufPos++] = byte;
                    switch (msgBuf[0] & 0xF0) {
                        case NoteOff:               // Status 1 byte, Data 2 byte
                        case NoteOn:                // Status 1 byte, Data 2 byte
                        case PolyAfterTouch:        // Status 1 byte, Data 2 byte
                        case ControlChange:         // Status 1 byte, Data 2 byte
                        case ProgramChange:         // Status 1 byte, Data 1 byte
                        case ChannelAfterTouch:     // Status 1 byte, Data 1 byte
                        case PitchBend:             // Status 1 byte, Data 2 byte
                            msgState = MSG_STATE_DATA_BYTE;
                            break;
                        default:
                            msg_reset();
                            break;
                    }

                } else if (msgState == MSG_STATE_DATA_BYTE) {
                
                    msg_reset();

                } else { // (msgState == MSG_STATE_SYSEX)

                    msg_reset();

                }
                step = 0x00;
                goto DO_IT_RIGHT_NOW;
            }

            case 0x11:{    // Process Data-Byte
                if (msgState == MSG_STATE_STATUS_BYTE) {

                    /* Running status */
                    msgBuf[msgBufPos++] = msgRunningStatus; // StatusByte
                    msgState = MSG_STATE_DATA_BYTE;
                    step = 0x11;
                    goto DO_IT_RIGHT_NOW;

                } else if (msgState == MSG_STATE_DATA_BYTE) {
                
                    msgBuf[msgBufPos++] = byte;
                    switch (msgBuf[0] & 0xF0) {
                        case NoteOff:               // Status 1 byte, Data 2 byte
                            if (msgBufPos >= 3) {
                                onNoteOff(msgBuf[0] & 0x0F, msgBuf[1], msgBuf[2]);
                                msg_reset();
                            }
                            break;
                        case NoteOn:                // Status 1 byte, Data 2 byte
                            if (msgBufPos >= 3) {
                                if (msgBuf[2] == 0) { // Velocity = 0
                                    onNoteOff(msgBuf[0] & 0x0F, msgBuf[1], msgBuf[2]);
                                } else {
                                    onNoteOn(msgBuf[0] & 0x0F, msgBuf[1], msgBuf[2]);
                                }
                                msg_reset();
                            }
                            break;
                        case PolyAfterTouch:        // Status 1 byte, Data 2 byte
                            if (msgBufPos >= 3) {
                                onPolyAftertouch(msgBuf[0] & 0x0F, msgBuf[1], msgBuf[2]);
                                msg_reset();
                            }
                            break;
                        case ControlChange:        // Status 1 byte, Data 2 byte
                            if (msgBufPos >= 3) {
                                onControlChange(msgBuf[0] & 0x0F, msgBuf[1], msgBuf[2]);
                                msg_reset();
                            }
                            break;
                        case ProgramChange:         // Status 1 byte, Data 1 byte
                            if (msgBufPos >= 2) {
                                onProgramChange(msgBuf[0] & 0x0F, msgBuf[1]);
                                msg_reset();
                            }
                            break;
                        case ChannelAfterTouch:     // Status 1 byte, Data 1 byte
                            if (msgBufPos >= 2) {
                                onChannelAftertouch(msgBuf[0] & 0x0F, msgBuf[1]);
                                msg_reset();
                            }
                            break;
                        case PitchBend:             // Status 1 byte, Data 2 byte
                            if (msgBufPos >= 3) {
                                onPitchBend(msgBuf[0] & 0x0F, (msgBuf[1] & 0x7f) | ((msgBuf[2] & 0x7f) << 7));
                                msg_reset();
                            }
                            break;
                        default:
                            msg_reset();
                            break;
                    }

                } else { // (msgState == MSG_STATE_SYSEX)

                    msg_reset();

                }
                step = 0x00;
                goto DO_IT_RIGHT_NOW;
            }

            //--------------------------------------------------------------------------------------
            // System Common Messages
            // - 0xF0 ~ 0xF7
            // - Include long message SystemExclusive(0xF0) + Data... + EOX(0xF7)
            case 0x20:{    // Process Status-Byte
                if (msgState == MSG_STATE_STATUS_BYTE) {

                    msgBuf[msgBufPos++] = byte;
                    switch (msgBuf[0]) {
                        case SystemExclusive:       // Status 1 byte, Data n byte, EOX 1 byte
                            msgState = MSG_STATE_SYSEX;
                            break;
                        case TimeCodeQuarterFrame:  // Status 1 byte, Data 1 byte
                        case SongPosition:          // Status 1 byte, Data 2 byte
                        case SongSelect:            // Status 1 byte, Data 1 byte
                            msgState = MSG_STATE_DATA_BYTE;
                            break;
                        case TuneRequest:           // Status 1 byte, Data 0 byte
                            onTuneRequest();
                            //msg_reset();
                            //break;
                        default:
                            msg_reset();
                            break;
                    }

                } else if (msgState == MSG_STATE_DATA_BYTE) {

                    msg_reset();

                } else { // (msgState == MSG_STATE_SYSEX)

                    if (byte == EOX) {
                        onSystemExclusive(msgBuf, msgBufPos);   // No included EOX byte
                    }
                    msg_reset();

                }
                step = 0x00;
                goto DO_IT_RIGHT_NOW;
            }

            case 0x21:{    // Process Data-Byte
                if (msgState == MSG_STATE_STATUS_BYTE) {

                    msg_reset();

                } else if (msgState == MSG_STATE_DATA_BYTE) {

                    msgBuf[msgBufPos++] = byte;
                    switch (msgBuf[0]) {
                        case TimeCodeQuarterFrame:  // Status 1 byte, Data 1 byte
                            if (msgBufPos >= 2) {
                                onTimeCodeQuarterFrame(msgBuf[1]);
                                msg_reset();
                            }
                            break;
                        case SongSelect:            // Status 1 byte, Data 1 byte
                            if (msgBufPos >= 2) {
                                onSongSelect(msgBuf[1]);
                                msg_reset();
                            }
                            break;
                        case SongPosition:          // Status 1 byte, Data 2 byte
                            if (msgBufPos >= 3) {
                                onSongPositionPointer((msgBuf[1] & 0x7f) | ((msgBuf[2] & 0x7f) << 7));
                                msg_reset();
                            }
                            break;
                        default:
                            msg_reset();
                            break;
                    }

                } else { // (msgState == MSG_STATE_SYSEX)

                    msgBuf[msgBufPos++] = byte;
                    if (msgBufPos >= sizeof(msgBuf)) {
                        msg_reset();    // Buffer overflow
                    }

                }
                step = 0x00;
                goto DO_IT_RIGHT_NOW;
            }

            //--------------------------------------------------------------------------------------
            // System RealTime Messages
            // - 0xF8 ~ 0xFF
            // - May interspersed with another message
            // - Should be processed immediately
            case 0x30:{    // Process Status-Byte, no Data-Byte
                switch (byte) {
                    case TimingClock:   onTimingClock();    break;
                    case Start:         onStart();          break;
                    case Continue:      onContinue();       break;
                    case Stop:          onStop();           break;
                    case ActiveSensing: onActiveSensing();  break;
                    case Reset:         onReset();          break;
                    default:            msg_reset();        break;
                }
                step = 0x00;
                goto DO_IT_RIGHT_NOW;
            }

        } // End of switch
DO_IT_RIGHT_NOW: while(0);

    } // End of while-loop
DO_IT_NEXT_TIME: while(0);
}

void SerialMIDI::msg_reset() {
    msgState = MSG_STATE_STATUS_BYTE;
    msgBuf[0]  = 0;
    msgBufPos  = 0;
}






