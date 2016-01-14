#ifndef DEBUG_H
#define DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../defines.h"

extern volatile bool in_debugger;

enum {
        DBG_USER,
        DBG_EXCEPTION,
        DBG_STEP,
        HIT_EXEC_BREAKPOINT,
        HIT_READ_BREAKPOINT,
        HIT_WRITE_BREAKPOINT,
        HIT_PORT_WRITE_BREAKPOINT,
        HIT_PORT_READ_BREAKPOINT
};

/* For Port Monitoring */
#define DBG_NO_HANDLE             0
#define DBG_PORT_READ             1
#define DBG_PORT_WRITE            2
#define DBG_PORT_FREEZE           4

/* For Memory Brakpoints */
#define DBG_READ_BREAKPOINT       1
#define DBG_WRITE_BREAKPOINT      2
#define DBG_EXEC_BREAKPOINT       4
#define DBG_STEP_OVER_BREAKPOINT  8

typedef struct {
    uint8_t *block;
    uint8_t *ports;
} debug_data_t;

typedef struct {        /* For debugging */
    int cpu_cycles;
    uint32_t stepOverAddress;
    uint32_t stepOutSPL;
    uint16_t stepOutSPS;
    debug_data_t data;
} debug_state_t;

/* Debugging */
extern debug_state_t debugger;

uint8_t debug_read_byte(uint32_t address);
uint16_t debug_read_short(uint32_t address);
uint32_t debug_read_long(uint32_t address);
uint32_t debug_read_word(uint32_t address, bool mode);
void debug_write_byte(uint32_t address, uint8_t value);
uint8_t debug_port_read_byte(uint32_t address);
void debug_port_write_byte(uint32_t address, uint8_t value);
void openDebugger(int reason, uint32_t address);

#ifdef __cplusplus
}
#endif

#endif
