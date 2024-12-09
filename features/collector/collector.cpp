#include "features/collector/collector.h"
#include "features/connection/connection.h"

#include "sclp/sclp.h"

#include <cmath>

MotorDriver3Pins belt_motor(1, 2);
MotorDriver3Pins::config_t belt_motor_config = {
    .pwm_clkdiv = pwm_clkdiv_calc(10000, 1000),
    .pwm_wrap = 1000,
    .reverse = false,
};

Servo servo(0);
Servo::config_t servo_config = {
    .pwm_clkdiv = pwm_clkdiv_calc(50, 20000),
    .pwm_wrap = 20000,
    .min_pulse_us = 500,
    .min_angle = -M_PI * 3 / 4,
    .max_pulse_us = 2500,
    .max_angle = M_PI * 3 / 4,
};

void collector_setup()
{
    belt_motor.configure(belt_motor_config);
    servo.configure(servo_config);
}

void collector_task()
{
    collector_state_t collector_state = get_collector_state();
    belt_motor.set_duty_ratio(collector_state.belt_speed);
    servo.set_angle(collector_state.angle);
}

void collector_reset()
{
}
