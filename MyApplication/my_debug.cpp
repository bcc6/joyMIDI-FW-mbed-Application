#include "my_debug.h"


#define HEX_DUMP_NUM_IN_LINE 8   //8, 16


void hex_dump(const char *desc, void *addr, uint32_t len) {
    uint32_t i;
    uint8_t buff[HEX_DUMP_NUM_IN_LINE+1];     // stores the ASCII data
    uint8_t *p = (uint8_t *)addr;     // cast to make the code cleaner.

    // Output description if given.
    if(desc != NULL)
        DEBUG_OUT("%s:\n", desc);

    // Process every byte in the data.
    for(i = 0; i < len; i++){
        // Multiple of 16 means new line (with line offset).
        if((i % HEX_DUMP_NUM_IN_LINE) == 0) {
            // Just don't print ASCII for the zeroth line.
            if(i != 0)
                DEBUG_OUT("  %s\n", buff);

            // Output the offset.
            DEBUG_OUT("  %04X ", i);
        }

        // Now the hex code for the specific character.
        DEBUG_OUT(" %02X", p[i]);

        // And store a printable ASCII character for later.
        if((p[i] < 0x20) || (p[i] > 0x7e))
            buff[i % HEX_DUMP_NUM_IN_LINE] = '.';
        else
            buff[i % HEX_DUMP_NUM_IN_LINE] = p[i];
        buff[(i % HEX_DUMP_NUM_IN_LINE) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % HEX_DUMP_NUM_IN_LINE) != 0) {
        DEBUG_OUT("   ");
        i++;
    }

    // And print the final ASCII bit.
    DEBUG_OUT("  %s\n", buff);
}
