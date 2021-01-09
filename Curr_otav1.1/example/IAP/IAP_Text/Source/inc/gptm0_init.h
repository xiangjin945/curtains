/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __GPTM0_INIT_H
#define __GPTM0_INIT_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f5xxxx_gpio.h"
#include "ht32.h"
#include "ht32_board.h"
#include "gpio_init.h"
#include "uart0_init.h"
#include "usart0_init.h"

// 2021.1.7 低功耗专用计时标记(单位: ms) 
extern int gptm0_4low;

// 2020.11.21 time cost for calibration(unit: ms) 
extern int gptm0_4clbr;

extern int gptm0_cnt;
extern u8 gptm0_second;
extern u8 gptm0_minute;
extern u8 gptm0_hour;

void read_time(void);
void gptm0_onfiguration(void);

#endif /* __GPTM0_INIT_H ------------------------------------------------------------------------------------*/
