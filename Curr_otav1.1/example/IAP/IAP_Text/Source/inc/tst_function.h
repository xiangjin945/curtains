/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __TEST_FUNCTION_H
#define __TEST_FUNCTION_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_config.h"
#include "gpio_init.h"
#include "uart0_init.h"
#include "usart0_init.h"
#include "string.h"
#include "motor_init.h"
#include "ht32_board_config.h"
#include "gptm0_init.h"
#include "tybn1_init.h"
#include "ht32f5xxxx_rstcu.h"
#include "rtc_init.h"
#include "systick_init.h"
#include "save_all_setting.h"
#include "adc_init.h"
#include "tst_function.h"
#include "service_logic_manage.h"

extern u16 motor_current;

void update_motor_current(u32 current_data);
void update_protect_current(u32 current_data);
void gptm0_test(void);
void motor_current_test(void);
void rtc_test(void);
void motor_turn(void);
void motor_10_test(void);
void calibration_test(void);
void tybn1_test(void);
void set_motor_current_78(u8 *data_buf);
void global_arg_fun(void);

#endif /* __TEST_FUNCTION_H */
