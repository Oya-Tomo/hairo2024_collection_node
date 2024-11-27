#pragma once

#include "hardware/i2c.h"
#include "pico/i2c_slave.h"
#include "pico/stdlib.h"
#include "./format.h"

typedef struct
{
    uint8_t mem[256];
    uint8_t mem_address;
    bool mem_address_written;
} i2c_context_t;

static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event);

void connection_setup();

void connection_task();

void connection_print();

system_state_t get_system_state();

arm_state_t get_arm_state();

collector_state_t get_collector_state();