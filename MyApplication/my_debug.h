#ifndef MY_DEBUG_H
#define MY_DEBUG_H

#include "mbed.h"

#if     (MBED_CONF_APP_RUN_MODE == 0)

    /* Disable serial debug */
    #define DEBUG_SERIAL(...)               // Empty
    #define DEBUG_OUT(...)                  do{}while(0)
    #define DEBUG_IN(...)                   do{}while(0)
    #define PRESS(c)                        do{}while(0)
    #define PRESS_ANY_KEY_TO(s)             do{}while(0)
    #define PRESS_ANY_KEY_TO_BREAK_LOOP()   do{}while(0)
    #define ASSERT(expr)                    do{}while(0)
    #define HEX_DUMP(desc, addr, len)       do{}while(0)

#elif   (MBED_CONF_APP_RUN_MODE == 1) || (MBED_CONF_APP_RUN_MODE == 2)

    /* Enable serial debug */
    void hex_dump(const char *desc, void *addr, uint32_t len);
    #define DEBUG_SERIAL(...)               FileHandle *mbed::mbed_override_console(int) { static UARTSerial s(__VA_ARGS__); return &s; }
    #define DEBUG_OUT(...)                  printf(__VA_ARGS__)
    #define DEBUG_IN(...)                   scanf (__VA_ARGS__)
    #define PRESS(c)                        while( getc(stdin) != c)
    #define PRESS_ANY_KEY_TO(s)             printf("Press any key to %s >\n", s); while(!(readable() && getc(stdin)))
    #define PRESS_ANY_KEY_TO_BREAK_LOOP()   if(readable() && getc(stdin)) break
    #define ASSERT(expr) { \
        if(!(expr)) { \
            printf("ERROR: %s ,file %s, line %d \n", #expr, __FILE__, __LINE__); \
            while(1); \
        } \
    }
    #define HEX_DUMP(desc, addr, len)       hex_dump(desc, addr, len)

#endif



// NOTE: SystemReport class is copied and modified from mbed's stat_report.h
//       Add below configuration option to enable.
//
//      "platform.all-stats-enabled": true
//      "platform.heap-stats-enabled": true,
//      "platform.stack-stats-enabled": true
//      "platform.thread-stats-enabled": true
//      "platform.sys-stats-enabled": true
//      "platform.cpu-stats-enabled": true

/**
 *  System Reporting library. Provides runtime information on device:
 *      - CPU sleep, idle, and wake times
 *      - Heap and stack usage
 *      - Thread information
 *      - Static system information
 */
class SystemReport {
    mbed_stats_heap_t   heap_stats;
    mbed_stats_cpu_t    cpu_stats;
    mbed_stats_sys_t    sys_stats;

    mbed_stats_thread_t *thread_stats;
    uint8_t   thread_count;
    uint8_t   max_thread_count;
    uint32_t  sample_time_ms;

public:
    /**
     *  SystemReport - Sample rate in ms is required to handle the CPU percent awake logic
     */
    SystemReport(uint32_t sample_rate) : max_thread_count(8), sample_time_ms(sample_rate)
    {
        thread_stats = new mbed_stats_thread_t[max_thread_count];

        // Collect the static system information
        mbed_stats_sys_get(&sys_stats);

        DEBUG_OUT("==== SYSTEM INFO ====\n");
        DEBUG_OUT("Mbed OS Version: %ld \n", sys_stats.os_version);
        DEBUG_OUT("CPU ID: 0x%lx \n", sys_stats.cpu_id);
        DEBUG_OUT("Compiler ID: %d \n", sys_stats.compiler_id);
        DEBUG_OUT("Compiler Version: %ld \n", sys_stats.compiler_version);

        for (int i = 0; i < MBED_MAX_MEM_REGIONS; i++) {
            if (sys_stats.ram_size[i] != 0) {
                DEBUG_OUT("RAM%d: Start 0x%lx Size: 0x%lx \n", i, sys_stats.ram_start[i], sys_stats.ram_size[i]);
            }
        }
        for (int i = 0; i < MBED_MAX_MEM_REGIONS; i++) {
            if (sys_stats.rom_size[i] != 0) {
                DEBUG_OUT("ROM%d: Start 0x%lx Size: 0x%lx \n", i, sys_stats.rom_start[i], sys_stats.rom_size[i]);
            }
        }
    }

    ~SystemReport(void)
    {
        free(thread_stats);
    }

    /**
     *  Report on each Mbed OS Platform stats API
     */
    void report_state(void)
    {
        report_cpu_stats();
        report_heap_stats();
        report_thread_stats();

        // Clear next line to separate subsequent report logs
        DEBUG_OUT("\n");
    }

    /**
     *  Report CPU idle and awake time in terms of percentage
     */
    void report_cpu_stats(void)
    {
        static uint64_t prev_idle_time = 0;

        DEBUG_OUT("==== CPU STATS ====\n");

        // Collect and print cpu stats
        mbed_stats_cpu_get(&cpu_stats);

        uint64_t diff = (cpu_stats.idle_time - prev_idle_time);
        uint8_t idle = (diff * 100) / (sample_time_ms * 1000);  // usec;
        uint8_t usage = 100 - ((diff * 100) / (sample_time_ms * 1000));  // usec;;
        prev_idle_time = cpu_stats.idle_time;

        DEBUG_OUT("Idle: %d%% Usage: %d%% \n", idle, usage);
    }

    /**
     *  Report current heap stats. Current heap refers to the current amount of
     *  allocated heap. Max heap refers to the highest amount of heap allocated
     *  since reset.
     */
    void report_heap_stats(void)
    {
        DEBUG_OUT("==== HEAP STATS ====\n");

        // Collect and print heap stats
        mbed_stats_heap_get(&heap_stats);

        DEBUG_OUT("Current heap: %lu\n", heap_stats.current_size);
        DEBUG_OUT("Max heap size: %lu\n", heap_stats.max_size);
    }

    /**
     *  Report active thread stats
     */
    void report_thread_stats(void)
    {
        DEBUG_OUT("==== THREAD STATS ====\n");

        // Collect and print running thread stats
        int count = mbed_stats_thread_get_each(thread_stats, max_thread_count);

        for (int i = 0; i < count; i++) {
            DEBUG_OUT("ID: 0x%lx \n",        thread_stats[i].id);
            DEBUG_OUT("Name: %s \n",         thread_stats[i].name);
            DEBUG_OUT("State: %ld \n",       thread_stats[i].state);
            DEBUG_OUT("Priority: %ld \n",    thread_stats[i].priority);
            DEBUG_OUT("Stack Size: %ld \n",  thread_stats[i].stack_size);
            DEBUG_OUT("Stack Space: %ld \n", thread_stats[i].stack_space);
        }
    }
};

#endif
