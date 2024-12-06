#include <stdio.h>

#include "features/connection/connection.h"

static i2c_context_t context;

static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event)
{
    switch (event)
    {
    case I2C_SLAVE_RECEIVE:
        if (!context.written_address)
        {
            context.address = i2c_read_byte_raw(i2c);
            context.written_address = true;
        }
        else
        {
            context.memory[context.address] = i2c_read_byte_raw(i2c);
            context.address++;
        }
        break;
    case I2C_SLAVE_REQUEST:
        i2c_write_byte_raw(i2c, context.memory[context.address]);
        context.address++;
        break;
    case I2C_SLAVE_FINISH:
        context.written_address = false;
        break;
    default:
        break;
    }
}

static const uint8_t I2C_SLAVE_ADDR = 0x30;
static const uint32_t I2C_BAUDRATE = 100 * 1000;

const uint I2C_SDA_PIN = 16;
const uint I2C_SCL_PIN = 17;

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
    return convert_to_system_state(context.memory);
}

arm_state_t get_arm_state()
{
    return convert_to_arm_state(context.memory);
}

collector_state_t get_collector_state()
{
    return convert_to_collector_state(context.memory);
}
