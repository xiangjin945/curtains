/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __GPIO_INIT_H
#define __GPIO_INIT_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f5xxxx_gpio.h"
//#include "ht32_board_config.h"
#include "ht32.h"
#include "ht32_board.h"
#include "motor_init.h"

#define SET_LED(x)		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_4,x);
#define LED_FLICKER		HT_GPIOB->DOUTR ^= HT_GPIOB.GPIO_PIN_2;
#define SET_KEY()		GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_5)
#define LEFT_KEY()		GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_2)
#define RIGHT_KEY()		GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_1)

void led_blink(u8 on_off);
void key_init(void);
void gpio_init(void);

#endif /* __IAP_GPIO_INIT_H ------------------------------------------------------------------------------------*/
