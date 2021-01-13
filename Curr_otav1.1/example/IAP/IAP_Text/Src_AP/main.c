/*********************************************************************************************************/ /**
 * @file    IAP/IAP_Text/Src_AP/main.c
 * @version $Rev:: 4724         $
 * @date    $Date:: 2020-04-07 #$
 * @brief   The main program of IAP example.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
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
#include "system.h"
#include "main_callfun.h"

unsigned char versions[3];
void version_fun(void)
{
	versions[0] = MCU_VERSION_1;
	versions[1] = MCU_VERSION_2;
	versions[2] = MCU_VERSION_3;
}

int main(void)
{
	NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, IAP_APFLASH_START);

	printf("VERSION 1.1.1!\n");
	version_fun();
	bt_protocol_init(); //Э�鴮�ڵĳ�ʼ��

	clock_config(); //ʱ������
	uart0_init();	//B7.B8, TYBN1
	usart0_init();	//C4.C5, ������

	key_init();			  //������ʼ��
	gpio_init();		  //LED��ʼ��
	adc_init();			  //ADC��ʼ��
	gptm0_onfiguration(); //��ʱ����ʼ��
	bftm_onfiguration();
	rtc_init(); //RTC��ʼ��
	systick_init();
	low_power_init(); //�͹��ĳ�ʼ��
	bt_send_mcu_ver();

	variable.full_running_time = rw(FLASH_55K_GLOBAL_DATA);
	variable.global_percent = rw(FLASH_55K_GLOBAL_DATA + 4);
	if (variable.full_running_time == 0xFFFFFFFF)//���״���������ʱ�����һ�θ�ȫ�ֱ�������ֵ�������Ǵ�flash��ȡֵ���if���ڽ��롣
	{
		u32 i;
		for (i = FLASH_55K_GLOBAL_DATA; i < FLASH_55K_GLOBAL_DATA + 1024; i += 1024)
		{
			FLASH_ErasePage(i);
		}		
		variable.global_percent = 100;
		variable.full_running_time = 0;
		FLASH_ProgramWordData(FLASH_55K_GLOBAL_DATA, variable.full_running_time);
    FLASH_ProgramWordData(FLASH_55K_GLOBAL_DATA+4, variable.global_percent);
	
	}
#if 1
	mcu_state = 1; //��һ���ϵ�
	//heart_beat();							//��һ������
	//motor_pwr(PWR_ON);
	motor_pwr(PWR_OFF);
	SET_LED(SET);

	curtain_mode = 0;
	calibration_status = 0;
	calibration_operation = 0;
	motor_current = 0;
#endif

	low_power_event_flag = 0;
	while (1)
	{

		motor_current_test(); //�������
		motor_turn();		  //����ת
		calibration_test();	  //У��

		bt_uart_service(); //ͿѻЭ�鴦��

		if ((gptm0_4low > 30000) && (low_power_event_flag == 0))
		{
			gptm0_4low = 0;
			motor_stop();
			low_power_event_flag = 1;
			tybn1_join_sleep_mode();

			EXTI_ClearWakeupFlag(EXTI_CHANNEL_1);
			EXTI_ClearWakeupFlag(EXTI_CHANNEL_2);
			EXTI_ClearWakeupFlag(EXTI_CHANNEL_5);
			EXTI_ClearWakeupFlag(EXTI_CHANNEL_14);
			EXTI_ClearWakeupFlag(EXTI_CHANNEL_15);
			EXTI_WakeupEventIntConfig(ENABLE);

			ADC_Cmd(HT_ADC, DISABLE);
			//SET_LED(RESET);
			printf("enter slepp!\n");
			PWRCU_DeepSleep1(PWRCU_SLEEP_ENTRY_WFI);

			printf("REBOOT slepp!\n");
			adc_init();
			//bt_send_mcu_ver();
		}
		//��������
		//if(motor_current>0x190)		//400mA
		if (motor_current > protect_current)
		{
			motor_current = 0;
			motor_stop();
		}
	}
}
