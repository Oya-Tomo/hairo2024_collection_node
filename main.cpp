#include <stdio.h>
#include "pico/stdlib.h"

#include "features/connection/connection.h"
#include "features/ticker/ticker.h"

void setup()
{
    stdio_init_all();

    connection_setup();
}

void tasks()
{
    connection_task();
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
