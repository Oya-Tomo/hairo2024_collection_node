#pragma once

#include "pico/stdlib.h"

#include "sclp/sclp.h"

void arm_setup();

void arm_task();

void set_turn_table_angle(float angle);

void set_hand_grip_move(float dir);

void set_joint_angle(float base_angle, float mid_angle, float tip_angle);