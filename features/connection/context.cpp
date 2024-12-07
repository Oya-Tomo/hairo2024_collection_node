#include "features/connection/context.h"

#include <cmath>

system_state_t convert_to_system_state(uint8_t *data)
{
    system_state_t state;
    state.is_running = (bool)data[IS_RUNNING_BYTE];
    return state;
}

arm_state_t convert_to_arm_state(uint8_t *data)
{
    arm_state_t state;
    state.base_angle = (double)((int8_t)data[BASE_ANGLE_BYTE]) * M_PI / 128.0;
    state.mid_angle = (double)((int8_t)data[MID_ANGLE_BYTE]) * M_PI / 128.0;
    state.tip_angle = (double)((int8_t)data[TIP_ANGLE_BYTE]) * M_PI / 128.0;
    state.rotate = (double)((int8_t)data[ROTATE_BYTE]) * M_PI / 128.0;
    state.gripper_speed = (double)((int8_t)data[GRIPPER_SPEED_BYTE]) / 128.0;
    return state;
}

collector_state_t convert_to_collector_state(uint8_t *data)
{
    collector_state_t state;
    return state;
}