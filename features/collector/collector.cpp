#include "./collector.h"

#include "sclp/sclp.h"
#include "collector.h"

MotorDriver3Pins md_belt(1, 2);
MotorDriver3Pins::config_t md_belt_config = {
    .pwm_clkdiv = pwm_clkdiv_calc(10000, 1000),
    .pwm_wrap = 1000,
    .reverse = false,
};

Servo servo(0);
Servo::config_t servo_config = {
    .pwm_clkdiv = pwm_clkdiv_calc(50, 20000),
    .pwm_wrap = 20000,
    .min_pulse_us = 500,
    .min_angle = -135.0,
    .max_pulse_us = 2500,
    .max_angle = 135.0,
};

void collector_setup()
{
    md_belt.configure(md_belt_config);
    servo.configure(servo_config);
}

void collector_task()
{
    md_belt.set_duty_ratio(0.5);
}

void collector_reset()
{
}
