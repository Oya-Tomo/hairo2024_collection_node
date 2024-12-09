#include <stdio.h>
#include "pico/stdlib.h"

#include "features/connection/connection.h"
#include "features/arm/arm.h"
#include "features/collector/collector.h"
#include "features/ticker/ticker.h"

void setup()
{
    stdio_init_all();

    connection_setup();
    arm_setup();
    collector_setup();
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
