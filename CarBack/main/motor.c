#include <stdio.h>
#include "motor.h"
#include "debug.h"

void init_controls() {
	car_controls.throttle = 0;
	car_controls.steering = 0;
}

int set_throttle(int throttle) {
    INFO("[MOTOR] Throttle set to %d\n", throttle);
	if (throttle < -100 || throttle > 100) {
		return 0;
	}

	car_controls.throttle = throttle;

	if ( throttle > 30) {
		brushed_motor_forward(MCPWM_UNIT_0, MCPWM_TIMER_0, (double)throttle);
	} else if ( throttle < -30 ){
        brushed_motor_backward(MCPWM_UNIT_0, MCPWM_TIMER_0, (double)(throttle*-1));
	} else {
		brushed_motor_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
	}

	return 1;
}

int set_steering(int steering) {
    INFO("[MOTOR] Steering set to %d\n", steering);
	if (steering < -100 || steering > 100) {
		return 0;
	}

	car_controls.steering = steering;

	if ( steering > 80) {
		brushed_motor_forward(MCPWM_UNIT_1, MCPWM_TIMER_1, (double)steering);
	} else if ( steering < -80 ){
        brushed_motor_backward(MCPWM_UNIT_1, MCPWM_TIMER_1, (double)(steering*-1));
	} else {
		brushed_motor_stop(MCPWM_UNIT_1, MCPWM_TIMER_1);
	}

	return 1;
}

void mcpwm_example_brushed_motor_control() {
	//1. mcpwm gpio initialization
	mcpwm_example_gpio_initialize();

	//2. initial mcpwm configuration
	printf("Configuring Initial Parameters of mcpwm...\n");
	mcpwm_config_t pwm_config;
	pwm_config.frequency = 1000;    //frequency = 500Hz,
	pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
	pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
	pwm_config.counter_mode = MCPWM_UP_COUNTER;
	pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
	mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure PWM0A & PWM0B with above settings
	mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_1, &pwm_config);    //Configure PWM0A & PWM0B with above settings
}

static void mcpwm_example_gpio_initialize()
{
    printf("initializing mcpwm gpio...\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, GPIO_PWM0B_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1A, GPIO_PWM1A_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1B, GPIO_PWM1B_OUT);
}

/**
 * @brief motor moves in forward direction, with duty cycle = duty %
 */
static void brushed_motor_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle)
{
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_B);
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, duty_cycle);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state
}

/**
 * @brief motor moves in backward direction, with duty cycle = duty %
 */
static void brushed_motor_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle)
{
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_A);
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_B, duty_cycle);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);  //call this each time, if operator was previously in low/high state
}

/**
 * @brief motor stop
 */
static void brushed_motor_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num)
{
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_A);
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_B);
}
