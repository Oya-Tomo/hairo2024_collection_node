#pragma once

#include "pico/stdlib.h"

const uint8_t IS_RUNNING_BYTE = 0x00;

typedef struct
{
    bool is_running; // 0: stop, 1: run
} system_state_t;

typedef struct
{
} arm_state_t;

typedef struct
{

} collector_state_t;

system_state_t convert_to_system_state(uint8_t mem[256]);

arm_state_t convert_to_arm_state(uint8_t mem[256]);

collector_state_t convert_to_collector_state(uint8_t mem[256]);