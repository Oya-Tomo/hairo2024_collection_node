#pragma once

#include "pico/stdlib.h"

// I2C context

typedef struct
{
    uint8_t memory[256];
    uint8_t address;
    bool written_address;
} i2c_context_t;

// System State Structures

const uint8_t IS_RUNNING_BYTE = 0x00;

const uint8_t BASE_ANGLE_BYTE = 0x00;
const uint8_t MID_ANGLE_BYTE = 0x01;
const uint8_t TIP_ANGLE_BYTE = 0x02;
const uint8_t ROTATE_BYTE = 0x03;
const uint8_t GRIPPER_SPEED_BYTE = 0x04;

typedef struct
{
    bool is_running; // 0: stop, 1: run
} system_state_t;

typedef struct
{
    float base_angle;
    float mid_angle;
    float tip_angle;
    float rotate;
    float gripper_speed;
} arm_state_t;

typedef struct
{

} collector_state_t;

system_state_t convert_to_system_state(uint8_t *data);

arm_state_t convert_to_arm_state(uint8_t *data);

collector_state_t convert_to_collector_state(uint8_t *data);