#include <stdio.h>
#include "pico/stdlib.h"

#include "features/connection/connection.h"
#include "features/arm/arm.h"
#include "features/collector/collector.h"
#include "features/ticker/ticker.h"

#define DEBUG

void irq_callback_register(uint gpio, uint32_t events)
{
    arm_callback_register(gpio, events);
}

void setup()
{
    stdio_init_all();

    connection_setup();
    arm_setup();
    collector_setup();

    gpio_set_irq_callback(irq_callback_register);
    irq_set_enabled(IO_IRQ_BANK0, true);
}

void tasks()
{
    system_state_t system_state = get_system_state();
    if (system_state.is_running)
    {
        arm_task();
        collector_task();
    }

#ifdef DEBUG
    arm_state_t arm_state = get_arm_state();
    collector_state_t collector_state = get_collector_state();

    stdio_printf("arm: %f %f %f %f %f | ", arm_state.base_angle, arm_state.mid_angle, arm_state.tip_angle, arm_state.rotate, arm_state.gripper_speed);
    stdio_printf("collector: %f %f \n", collector_state.belt_speed, collector_state.angle);
#endif
}

int main()
{
    setup();

    while (true)
    {
        tasks();

        frame_wait();
    }
    return 0;
}
