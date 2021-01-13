/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __ADC_INIT_H
#define __ADC_INIT_H

/* Includes ------------------------------------------------------------------------------*/
#include "ht32f5xxxx_gpio.h"
#include "ht32.h"
#include "ht32_board.h"
#include "ht32f5xxxx_ckcu.h"

extern u8 gADC_Result[3];
extern u8 gADC_CycleEndOfConversion;
extern vu32 gPotentiometerLevel;
extern volatile bool gADC_SingleEndOfConversion;

u16 get_battery_voltage(void);
// NOTE: 改善堵转电流检测算法
u16 check_motor_current(u16 protect_current);
u16 get_motor_current(void);
void adc_init(void);

#endif		/* __ADC_INIT_H --------------------------------------------------*/
