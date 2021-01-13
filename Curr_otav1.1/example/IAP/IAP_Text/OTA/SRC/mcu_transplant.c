#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_configs.h"
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
#include "bftm_init.h"
#include "low_power_iniit.h"
#include "protocol.h"
#include "mcu_api.h"
int uart_transmit_output_fun(unsigned char value)
{
	
	USART_SendData(HT_UART0,value);
	while (USART_GetFlagStatus(HT_UART0, USART_FLAG_TXC) == RESET);
	
	USART_SendData(HT_USART0,value);
	while (USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);


}

void all_data_update_fun(void)
{
//	mcu_dp_enum_update(DPID_CONTROL,当前设备控制); //枚举型数据上报;
//    mcu_dp_value_update(DPID_PERCENT_CONTROL,当前开启百分比控制); //VALUE型数据上报;
//    mcu_dp_enum_update(DPID_MODE,当前工作模式); //枚举型数据上报;
//    mcu_dp_enum_update(DPID_CONTROL_BACK,当前电机反向); //枚举型数据上报;
//    mcu_dp_value_update(DPID_TIME_TOTAL,当前全程运行时间); //VALUE型数据上报;
//    mcu_dp_fault_update(DPID_FAULT,当前故障告警); //故障型数据上报;
//    mcu_dp_value_update(DPID_BATTERY_PERCENTAGE,当前电池电量); //VALUE型数据上报;
//    mcu_dp_enum_update(DPID_BORDER,当前设置限位); //枚举型数据上报;
//    mcu_dp_value_update(DPID_POSITION_BEST,当前最佳位置); //VALUE型数据上报;
//    mcu_dp_string_update(DPID_LED_SETTING,当前指示灯设置指针,当前指示灯设置数据长度); //STRING型数据上报;
}