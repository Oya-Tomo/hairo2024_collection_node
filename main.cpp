#include <stdio.h>
#include "pico/stdlib.h"

#include "features/connection/connection.h"
#include "features/arm/arm.h"
#include "features/collector/collector.h"
#include "features/ticker/ticker.h"

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
