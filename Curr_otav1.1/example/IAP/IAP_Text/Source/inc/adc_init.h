/*
 * @Author: i4season.xiang
 * @Date: 2021-01-09 15:55:28
 * @LastEditTime: 2021-01-15 17:07:41
 * @FilePath: \Src_APc:\Users\24379\Desktop\Git_Svn\Curr_otav1.1\example\IAP\IAP_Text\Source\inc\adc_init.h
 */
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __ADC_INIT_H
#define __ADC_INIT_H

/* Includes ------------------------------------------------------------------------------*/
#include "ht32f5xxxx_gpio.h"
#include "ht32.h"
#include "ht32_board.h"
#include "ht32f5xxxx_ckcu.h"
#include "systick_init.h"

#define LOW_BATTERY     1.14
#define MEDIUM_BATTERY  1.47
#define HIGH_BATTERY    1.62

extern u8 gADC_Result[3];
extern u8 gADC_CycleEndOfConversion;
extern vu32 gPotentiometerLevel;
extern volatile bool gADC_SingleEndOfConversion;

u16 get_battery_voltage(void);
// NOTE: 改善堵转电流检测算法
u16 check_motor_current(u16 protect_current);
u16 get_motor_current(void);

u16 check_battery_level(void);
void adc_init(void);
void adc_del(void);

#endif		/* __ADC_INIT_H --------------------------------------------------*/
