#include "features/arm/arm.h"
#include "features/connection/connection.h"
#include "features/ticker/ticker.h"

#include <cmath>
#include "arm.h"

MotorDriver3Pins turn_table_motor(10, 11);
MotorDriver3Pins::config_t turn_table_motor_config = {
    .pwm_clkdiv = pwm_clkdiv_calc(10 * 1000, 1000),
    .pwm_wrap = 1000,
    .reverse = false,
};

QEI turn_table_qei(20, 21);
QEI::config_t turn_table_qei_config = {
    .ppr = 48,
    .reverse = false,
};

PID turn_table_pid(PID::sPID);
PID::config_t turn_table_pid_config = {
    .Kp = 0.9,
    .Ki = 0.00,
    .Kd = 0.01,
    .Kf = 0,
    .guard = true,
    .min = -0.99,
    .max = 0.99,
};

MotorDriver3Pins hand_grip_motor(12, 13);
MotorDriver3Pins::config_t hand_grip_motor_config = {
    .pwm_clkdiv = pwm_clkdiv_calc(10 * 1000, 1000),
    .pwm_wrap = 1000,
    .reverse = false,
};

QEI hand_grip_qei(18, 19);
QEI::config_t hand_grip_qei_config = {
    .ppr = 48,
    .reverse = false,
};

const uint GRIP_LIMIT_SW_PIN = 14;

Servo base_servo(4);
Servo::config_t base_servo_config = {
    .pwm_clkdiv = pwm_clkdiv_calc(50, 20000),
    .pwm_wrap = 20000,
    .min_pulse_us = 500,
    .min_angle = -M_PI / 4.0 * 3.0,
    .max_pulse_us = 2500,
    .max_angle = M_PI / 4.0 * 3.0,
};

Servo mid_servo(6);
Servo::config_t mid_servo_config = {
    .pwm_clkdiv = pwm_clkdiv_calc(50, 20000),
    .pwm_wrap = 20000,
    .min_pulse_us = 500,
    .min_angle = -M_PI / 4.0 * 3.0,
    .max_pulse_us = 2500,
    .max_angle = M_PI / 4.0 * 3.0,
};

Servo tip_servo(8);
Servo::config_t tip_servo_config = {
    .pwm_clkdiv = pwm_clkdiv_calc(50, 20000),
    .pwm_wrap = 20000,
    .min_pulse_us = 500,
    .min_angle = -M_PI / 4.0 * 3.0,
    .max_pulse_us = 2500,
    .max_angle = M_PI / 4.0 * 3.0,
};

void arm_callback_register(uint gpio, uint32_t events)
{
    turn_table_qei.callback_register(gpio, events);
    hand_grip_qei.callback_register(gpio, events);
}

void arm_setup()
{
    turn_table_motor.configure(turn_table_motor_config);
    turn_table_qei.configure(turn_table_qei_config);
    turn_table_pid.configure(turn_table_pid_config);

    hand_grip_motor.configure(hand_grip_motor_config);
    hand_grip_qei.configure(hand_grip_qei_config);

    gpio_init(GRIP_LIMIT_SW_PIN);
    gpio_set_dir(GRIP_LIMIT_SW_PIN, GPIO_IN);
    gpio_pull_up(GRIP_LIMIT_SW_PIN);

    base_servo.configure(base_servo_config);
    mid_servo.configure(mid_servo_config);
    tip_servo.configure(tip_servo_config);
}

void arm_task()
{
    arm_state_t arm_state = get_arm_state();

    set_turn_table_angle(arm_state.rotate);
    set_hand_grip_move(arm_state.gripper_speed);
    set_joint_angle(arm_state.base_angle, arm_state.mid_angle, arm_state.tip_angle);
}

void set_turn_table_angle(float angle)
{
    static float turn_table_duty_ratio = 0.0;
    // motor duty state

    double taget_angle = angle;
    double current_angle = turn_table_qei.get_radians();

    turn_table_duty_ratio = turn_table_pid.calculate(taget_angle, current_angle, get_dt());
    turn_table_motor.set_duty_ratio(turn_table_duty_ratio);
}

void set_hand_grip_move(float dir)
{
    const float MAX_GRIP_ROTATE_RADIAN = M_PI * 8.0;

    bool limit_sw = gpio_get(GRIP_LIMIT_SW_PIN) == 0;
    if (limit_sw)
    {
        hand_grip_qei.reset();
    }

    float current_rotate = hand_grip_qei.get_radians();

    if (dir > 0 && current_rotate < MAX_GRIP_ROTATE_RADIAN)
    {
        hand_grip_motor.set_duty_ratio(dir);
    }
    else if (dir < 0 && !limit_sw)
    {
        hand_grip_motor.set_duty_ratio(dir);
    }
    else
    {
        hand_grip_motor.set_duty_ratio(0.0);
    }
}

void set_joint_angle(float base_angle, float mid_angle, float tip_angle)
{
    double root_angle = M_PI / 2.0;

    float base_angle_diff = base_angle - root_angle;
    float mid_angle_diff = mid_angle - base_angle;
    float tip_angle_diff = tip_angle - mid_angle;

    base_servo.set_angle(base_angle_diff);
    mid_servo.set_angle(mid_angle_diff);
    tip_servo.set_angle(tip_angle_diff);
}