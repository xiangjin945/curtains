/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __MOTOR_INIT_H
#define __MOTOR_INIT_H

/* Includes ------------------------------------------------------------------------------*/
#include "pwm.h"
#include "ht32.h"
#include "ht32f5xxxx_tm.h"
#include "ht32_board_config.h"
#include "ht32f5xxxx_gpio.h"
#include "ht32f52231_41_libcfg.h"
#include "ht32f5xxxx_ckcu.h"
#include "ht32f52241_sk.h"

#define PWR_ON			1
#define PWR_OFF			0

#define NEGATIVE		0
#define POSITIVE		1

#define PB2_PWM_FREQ          10000 

//#define USE_GPIO_PB2_PB3
//#define USE_MCTM_PB2_PB3
#define USE_PWM_PB2_PB3

extern vu32 speed;

void motor_init(void);
void motor_pwr(u8 x);
void motor_stop(void);
void motor_dir(u8 x, u16 duty_cycle);
void open_curtain(void);
void close_curtain(void);
void enable_pb2_pwm(u16 duty_cycle,ControlStatus NewState);
void enable_pb3_pwm(u16 duty_cycle,ControlStatus NewState);

void clock_config(void);

#ifdef USE_GPIO_PB2_PB3
void motor_init(void);
#endif

#ifdef USE_MCTM_PB2_PB3
void motor_init_pb2(u16 duty_cycle);
void motor_init_pb3(u16 duty_cycle);
#endif

#ifdef USE_PWM_PB2_PB3
void  PB2_PWM_INIT(u16 duty_cycle,ControlStatus NewState);
void  PB3_PWM_INIT(u16 duty_cycle,ControlStatus NewState);
#endif

#endif		/* __MOTOR_INIT_H --------------------------------------------------*/
