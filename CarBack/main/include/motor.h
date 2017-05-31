#ifndef _MOTOR_H_
#define _MOTOR_H_
/*
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"
*/
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

#define GPIO_PWM0A_OUT 2   //Set GPIO 15 as PWM0A
#define GPIO_PWM0B_OUT 4   //Set GPIO 16 as PWM0B
#define GPIO_PWM1A_OUT 17   //Set GPIO 15 as PWM0A
#define GPIO_PWM1B_OUT 18   //Set GPIO 16 as PWM0B


// IO2, IO4, IO17, IO18

struct Controls {
  int throttle; // -100 full reverse, 0 still, 100 full forward
  int steering;     // -100 full left, 0 straigt, 100 full right
};

struct Controls car_controls;

void init_controls();
int set_throttle(int throttle);
int set_steering(int steering);
void execute_controls();
void mcpwm_example_brushed_motor_control();
static void mcpwm_example_gpio_initialize();
static void brushed_motor_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);
static void brushed_motor_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);
static void brushed_motor_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num);

#endif
