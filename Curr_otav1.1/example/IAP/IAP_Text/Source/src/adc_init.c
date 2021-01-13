#include "adc_init.h"

u16 battery_voltage[100] = {0};
u16 tmp_voltage[100] = {0};
//vu32 gPotentiometerLevel;
//u8 gADC_Result[3] = {0};
//u8 gADC_CycleEndOfConversion;
//volatile bool gADC_SingleEndOfConversion;

u16 get_battery_voltage(void)
{
	int i,j,n,min,max;
	u16 average = 0;
	
	for(i=0;i<10;i++)
	{
		battery_voltage[i] = (HT_ADC->DR[0] & 0x0FFF);
	}
	
	n = 0;
	max = min = battery_voltage[0];
	for(i=0;i<10;i++)
	{
		for(j=1;j<10;j++)
		{
			if(battery_voltage[j]>max)
			{
				n++;
				max = battery_voltage[j];
			}
			if(battery_voltage[j]<min)
			{
				n++;
				min = battery_voltage[j];
			}
		}
	}
	for(i=0;i<10;i++)
	{
		if((tmp_voltage[j]==max)||(tmp_voltage[j]==min))
		{
			tmp_voltage[j] = 0;
		}
	}
	for(i=0;i<(10-n);i++)
	{
		average += tmp_voltage[i];
	}
	average = average/(10-n);
	
	return average;
}


//adc_val/4096*Vref=Value			// NOTE: 12.9 比较堵转电流阈值的检测电流采样算法
u16 check_motor_current(u16 protect_current)
{
	int i;
	int t=0;
	int k=0;
	float real_volt = 0;
	u16 average_volt = 0;
	
	//ms_delay_systick(3);
	for(i=0;i<10;i++)
	{
		
		tmp_voltage[i] = (HT_ADC->DR[0] & 0x0FFF);
		// NOTE: 每次采样值均转化为电流值
		real_volt = ((float)tmp_voltage[i]/4095)*3.3;//这里的类型
		real_volt = real_volt*1000;
		
		if(real_volt >= protect_current) //获取到real_volt的值为 1E 2E 2D 值都比 64小 
		{
			USART_SendData(HT_USART0,0xab);
			t++;
			average_volt = average_volt + real_volt;	
		}
		else
		{
			k++;
			average_volt = average_volt + real_volt;
		}
		if(t>=8)
		{
			USART_SendData(HT_USART0,0x0f);
			average_volt = average_volt/t;
			USART_SendData(HT_USART0,average_volt>>8);
			USART_SendData(HT_USART0,average_volt);
			
			USART_SendData(HT_USART0,0x0f);
			return average_volt;
		}
	}
	
	// NOTE: 仅当10次采样值不含3次以上超过阈值，则取平均值为当前电流
	average_volt = average_volt/k;
	return average_volt;
}

//adc_val/4096*Vref=Value			// NOTE: 取平均值的检测电流采样算法
u16 get_motor_current(void)
{
	int i,j,n,min,max;
	u16 average = 0;
	
	for(i=0;i<10;i++)
	{
		tmp_voltage[i] = (HT_ADC->DR[0] & 0x0FFF);
	}
	
	n = 0;
	max = min = tmp_voltage[0];
	for(i=0;i<10;i++)
	{
		for(j=1;j<10;j++)
		{
			if(tmp_voltage[j]>max)
			{
				n++;
				max = tmp_voltage[j];
			}
			if(tmp_voltage[j]<min)
			{
				n++;
				min = tmp_voltage[j];
			}
		}
	}
	for(i=0;i<10;i++)
	{
		if((tmp_voltage[j]==max)||(tmp_voltage[j]==min))
		{
			tmp_voltage[j] = 0;
		}
	}
	for(i=0;i<(10-n);i++)
	{
		average += tmp_voltage[i];
	}
	average = average/(10-n);
	
	return average;
}

void adc_init(void)
{
	/* Config AFIO mode as ADC function                                                                       */
	//AFIO_GPxConfig(GPIO_PA, AFIO_PIN_4, AFIO_FUN_ADC);
	//配置指定引脚的GPIO模式
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_ADC);

	/* Continuous Mode, Length 1, SubLength 1                
	配置常规组的转换模式(连续和单次)和列表队列的长度。                                                 */
	ADC_RegularGroupConfig(HT_ADC, CONTINUOUS_MODE, 1, 1);

	/* ADC Channel n, Rank 0, Sampling clock is (1.5 + 0) ADCLK
	Conversion time = (sampling clock + 12.5) / ADCLK = 12.4 uS */
	//ADC_RegularChannelConfig(HT_ADC, ADC_CH_6, 0);
	//在定序器中为常规频道配置相应的等级
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_7, 0);

	/* Use Software Trigger as ADC trigger source 
	配置ADC触发源以进行常规通道转换。                                                            */
	ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_SOFTWARE);

	/* Enable ADC single end of conversion interrupt,
	The ADC ISR will store the ADC result into global variable gPotentiometerLevel.
	使能ADC单端转换中断，ADC ISR将ADC结果存储到全局变量gPotentiometerLevel */
	ADC_IntConfig(HT_ADC, ADC_INT_SINGLE_EOC, ENABLE);//启用或禁用ADC中断

	ADC_Cmd(HT_ADC, ENABLE);//启用或禁用指定的ADC。

	/* Software trigger to start continuous mode                                                              */
	ADC_SoftwareStartConvCmd(HT_ADC, ENABLE);//启用或禁用选定的ADC软件启动常规通道转换。
}

void adc_init_2(void)
{
	/* Configure specify IO's AFIO mode as ADC function */
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_ADC);

	/* ADCLK frequency is set to CK_AHB / 64 */
	CKCU_SetADCPrescaler(CKCU_ADCPRE_DIV64);

	/* One Shot Mode, Conversion/Sequence Length 3, SubLength 0 */
	ADC_RegularGroupConfig(HT_ADC, ONE_SHOT_MODE, 3, 0);

	/* Set sampling time as 1.5 + 36 ADCCLK, Conversion = 12.5 + 1.5 + 36 = 50 ADCCLK */
	ADC_SamplingTimeConfig(HT_ADC, 36);

	/* Set ADC Channel of Sequence */
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_7, 0);

	/* Enable ADC single/cycle end of conversion interrupt */
	ADC_IntConfig(HT_ADC, ADC_INT_SINGLE_EOC | ADC_INT_CYCLE_EOC, ENABLE);

	/* Enable the ADC Interrupts */
	NVIC_EnableIRQ(ADC_IRQn);

	/* Use GPTM0 CH3O as ADC trigger source */
	ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_GPTM0_CH3O);
	
	ADC_Cmd(HT_ADC, ENABLE);
}

void adc_init_1(void)
{
#if 0
	/* Enable the ADC Interrupts */
	NVIC_EnableIRQ(ADC_IRQn);
	
	/* ADCLK frequency is set to 72/64 MHz = 1.125MHz */
	CKCU_SetADCPrescaler(CKCU_ADCPRE_DIV64);
	
	/* Config AFIO mode as ADC function */
	// AFIO_GPxConfig(ADC_800_AFIO_PROT, ADC_800_AFIO_PIN, AFIO_FUN_ADC);
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_ADC);
	/* Continuous Mode, Length 6, SubLength 1 */
	ADC_RegularGroupConfig(HT_ADC, CONTINUOUS_MODE, 4, 1);
	
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_7, 0);
#else
	/* Enable the ADC interrupts */
	NVIC_EnableIRQ(ADC_IRQn);
	
	/* Config AFIO mode as ADC function */
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_ADC);

	/* Continuous Mode, Length 1, SubLength 1 */
	ADC_RegularGroupConfig(HT_ADC, CONTINUOUS_MODE, 1, 1);

	/* ADC Channel n, Rank 0, Sampling clock is (1.5 + 0) ADCLK
	Conversion time = (sampling clock + 12.5) / ADCLK = 12.4 uS */
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_7, 0);

	/* Use Software Trigger as ADC trigger source */
	ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_SOFTWARE);

	/* Enable ADC single end of conversion interrupt,
	The ADC ISR will store the ADC result into global variable gPotentiometerLevel. */
	ADC_IntConfig(HT_ADC, ADC_INT_SINGLE_EOC, ENABLE);

	ADC_Cmd(HT_ADC, ENABLE);

	/* Software trigger to start continuous mode */
	ADC_SoftwareStartConvCmd(HT_ADC, ENABLE);
	
	USART_SendData(HT_USART0,0xAA);
	while(USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);
#endif
}
