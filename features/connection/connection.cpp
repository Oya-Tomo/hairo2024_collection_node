#include <stdio.h>

#include "./connection.h"
#include "connection.h"

#include "hardware/spi.h"

static const uint I2C_SLAVE_ADDR = 0x20;
static const uint I2C_BAUDRATE = 100000;

uint I2C_SDA_PIN = 16;
uint I2C_SCL_PIN = 17;

static i2c_context_t context;

uint BAUD_RATE = 500 * 1000;

static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event)
{
    switch (event)
    {
    case I2C_SLAVE_RECEIVE: // master has written some data
        if (!context.mem_address_written)
        {
            // writes always start with the memory address
            context.mem_address = i2c_read_byte_raw(i2c);
            context.mem_address_written = true;
        }
        else
        {
            // save into memory
            context.mem[context.mem_address] = i2c_read_byte_raw(i2c);
            context.mem_address++;
        }
        break;
    case I2C_SLAVE_REQUEST: // master is requesting data
        // load from memory
        i2c_write_byte_raw(i2c, context.mem[context.mem_address]);
        context.mem_address++;
        break;
    case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
        context.mem_address_written = false;
        break;
    default:
        break;
    }
}

void connection_setup()
{
    gpio_init(I2C_SDA_PIN);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);

    gpio_init(I2C_SCL_PIN);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL_PIN);

    i2c_init(i2c0, I2C_BAUDRATE);
    i2c_slave_init(i2c0, I2C_SLAVE_ADDR, &i2c_slave_handler);
}

void connection_task()
{
}

void connection_print()
{
}

system_state_t get_system_state()
{
    return convert_to_system_state(context.mem);
}

arm_state_t get_arm_state()
{
    return convert_to_arm_state(context.mem);
}

collector_state_t get_collector_state()
{
    return convert_to_collector_state(context.mem);
}
