#include "./format.h"

system_state_t convert_to_system_state(uint8_t mem[256])
{
    system_state_t state;
    state.is_running = mem[IS_RUNNING_BYTE];
    return state;
}

arm_state_t convert_to_arm_state(uint8_t mem[256])
{
    arm_state_t state;
    return state;
}

collector_state_t convert_to_collector_state(uint8_t mem[256])
{
    collector_state_t state;
    return state;
}