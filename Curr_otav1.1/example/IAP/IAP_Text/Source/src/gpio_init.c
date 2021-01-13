#include "gpio_init.h"

void led_blink(u8 on_off)
{
	if(on_off==0)
	{
		//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_2,~GPIO_ReadOutBit(HT_GPIOB, GPIO_PIN_2));
		//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,~GPIO_ReadOutBit(HT_GPIOB, GPIO_PIN_3));
		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_4,RESET);
	}
	if(on_off==1)
	{
		//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_2,~GPIO_ReadOutBit(HT_GPIOB, GPIO_PIN_2));
		//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,~GPIO_ReadOutBit(HT_GPIOB, GPIO_PIN_3));
		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_4,SET);
	}
}

void key_init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct = {0};

    //PC1 ????????
    AFIO_GPxConfig(GPIO_PC,            AFIO_PIN_1,  AFIO_FUN_DEFAULT);
    GPIO_DirectionConfig(HT_GPIOC     ,GPIO_PIN_1,  GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOC,  GPIO_PIN_1,  GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOC,         GPIO_PIN_1,  ENABLE);
    /* Connect Button EXTI Channel to Button GPIO Pin */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_1, AFIO_ESS_PC);
    /* Configure button EXTI Line on falling edge */
    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_1;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_POSITIVE_EDGE;
    EXTI_Init(&EXTI_InitStruct);
    /* Enable Button EXTI Interrupt */
    EXTI_IntConfig(EXTI_CHANNEL_1, ENABLE);
    NVIC_EnableIRQ(EXTI0_1_IRQn);

    //PC2 ????????
    AFIO_GPxConfig(GPIO_PC, AFIO_PIN_2, AFIO_FUN_DEFAULT);
    GPIO_DirectionConfig(HT_GPIOC ,GPIO_PIN_2, GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_2, GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOC, GPIO_PIN_2, ENABLE);
    /* Connect Button EXTI Channel to Button GPIO Pin */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_2, AFIO_ESS_PC);
    /* Configure button EXTI Line on falling edge */
    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_2;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_POSITIVE_EDGE;
    EXTI_Init(&EXTI_InitStruct);
    /* Enable Button EXTI Interrupt */
    EXTI_IntConfig(EXTI_CHANNEL_2, ENABLE);
    NVIC_EnableIRQ(EXTI2_3_IRQn);

    //PB5 ????????
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_5, AFIO_FUN_DEFAULT);
    GPIO_DirectionConfig(HT_GPIOB    ,GPIO_PIN_5,  GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOB,  GPIO_PIN_5,  GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOB,         GPIO_PIN_5,  ENABLE);
    /* Connect Button EXTI Channel to Button GPIO Pin                                                       */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_5, AFIO_ESS_PB);
    /* Configure button EXTI Line on falling edge                                                           */
    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_5;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_POSITIVE_EDGE;
    EXTI_Init(&EXTI_InitStruct);
    /* Enable Button EXTI Interrupt                                                                         */
    EXTI_IntConfig(EXTI_CHANNEL_5, ENABLE);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
#if 0
    //PA14 ????????
    AFIO_GPxConfig(GPIO_PA,            AFIO_PIN_14,  AFIO_FUN_DEFAULT);
    GPIO_DirectionConfig(HT_GPIOA    ,GPIO_PIN_14,  GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOA,  GPIO_PIN_14,  GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOA,         GPIO_PIN_14,  ENABLE);
    /* Connect Button EXTI Channel to Button GPIO Pin                                                       */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_14, AFIO_ESS_PA);
    /* Configure button EXTI Line on falling edge                                                           */
    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_14;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_POSITIVE_EDGE;
    EXTI_Init(&EXTI_InitStruct);
    /* Enable Button EXTI Interrupt                                                                         */
    EXTI_IntConfig(EXTI_CHANNEL_14, ENABLE);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
#endif
//PA15 ???????? - ??????????MCU		TODO: ???¡À¡Á???PA15 GPIO????????????¡À???PA15??????????¡¤¡À????MCU
//    AFIO_GPxConfig(GPIO_PA,            AFIO_PIN_15,  AFIO_FUN_DEFAULT);
//    GPIO_DirectionConfig(HT_GPIOA    ,GPIO_PIN_15,  GPIO_DIR_IN);
//    GPIO_PullResistorConfig(HT_GPIOA,  GPIO_PIN_15,  GPIO_PR_UP);
//    GPIO_InputConfig(HT_GPIOA,         GPIO_PIN_15,  ENABLE);
//    /* Connect Button EXTI Channel to Button GPIO Pin                                                       */
//    AFIO_EXTISourceConfig(AFIO_EXTI_CH_15, AFIO_ESS_PA);
//    /* Configure button EXTI Line on falling edge                                                           */
//    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_15;
//    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
//    EXTI_InitStruct.EXTI_DebounceCnt = 0;
//    EXTI_InitStruct.EXTI_IntType = EXTI_POSITIVE_EDGE;
//    EXTI_Init(&EXTI_InitStruct);
//    /* Enable Button EXTI Interrupt                                                                         */
//    EXTI_IntConfig(EXTI_CHANNEL_15, ENABLE);
//    NVIC_EnableIRQ(EXTI4_15_IRQn);
#if 0
	CKCU_PeripClockConfig_TypeDef CKCUClock = {0};
	CKCUClock.Bit.PB = 1;
	CKCUClock.Bit.AFIO = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	CKCUClock.Bit.PC = 1;
	CKCUClock.Bit.AFIO = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
#endif
#if 0
	//KEY1=SET
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_5, AFIO_MODE_1);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_5, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_5, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_5, ENABLE);
	
	//KEY2=left_key
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_1, AFIO_MODE_1);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_1, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_1, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_1, ENABLE);
	
	//KEY3=right_key
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_2, AFIO_MODE_1);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_2, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_2, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_2, ENABLE);
#endif
}

void gpio_init(void)
{
#if 0
	CKCU_PeripClockConfig_TypeDef CKCUClock = {0};
	CKCUClock.Bit.PA = 1;
	CKCUClock.Bit.AFIO = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	CKCUClock.Bit.PB = 1;
	CKCUClock.Bit.AFIO = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
#endif
	
	//LED
	AFIO_GPxConfig(GPIO_PB, GPIO_PIN_4, AFIO_FUN_GPIO);//LED
	HT_GPIOB->RR = GPIO_PIN_4;
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_4, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOB, GPIO_PIN_4, GPIO_DV_16MA);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_4, GPIO_DIR_OUT);
	//GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_4,SET);
	//GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_4,RESET);
	
	// AFIO_GPxConfig(GPIO_PC, GPIO_PIN_3, AFIO_FUN_GPIO);//LED
	// HT_GPIOC->RR = GPIO_PIN_3;
	// GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_3, GPIO_PR_DISABLE);
	// GPIO_DriveConfig(HT_GPIOC, GPIO_PIN_3, GPIO_DV_16MA);
	// GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_3, GPIO_DIR_OUT);
	// GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,SET);
	
	//TYBN1==GPIO_11=????¡Á¨¦??????????????
	AFIO_GPxConfig(GPIO_PA, GPIO_PIN_14, AFIO_FUN_GPIO);
	HT_GPIOB->RR = GPIO_PIN_14;
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_14, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOA, GPIO_PIN_14, GPIO_DV_16MA);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_14, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_14,RESET);
	
	//TYBN1==GPIO_14=????¡Á¨¦????MCU????
	AFIO_GPxConfig(GPIO_PA, GPIO_PIN_15, AFIO_MODE_1);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_15, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_15, GPIO_PR_DISABLE);
	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_15, ENABLE);
	
	
	AFIO_GPxConfig(GPIO_PA, GPIO_PIN_15, AFIO_MODE_1);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_15, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_15, GPIO_PR_DISABLE);
    GPIO_InputConfig(HT_GPIOA, GPIO_PIN_15, ENABLE);
	
    //DC5V_EN
	AFIO_GPxConfig(GPIO_PB, GPIO_PIN_0, AFIO_FUN_GPIO);
	HT_GPIOB->RR = GPIO_PIN_0;
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_0, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOB, GPIO_PIN_0, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_0, GPIO_DIR_OUT);
	
    //TM8837_EN
	AFIO_GPxConfig(GPIO_PB, GPIO_PIN_1, AFIO_FUN_GPIO);
	HT_GPIOB->RR = GPIO_PIN_1;
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_1, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOB, GPIO_PIN_1, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_1, GPIO_DIR_OUT);

	//pc0
	AFIO_GPxConfig(GPIO_PC, GPIO_PIN_0, AFIO_FUN_GPIO);
	HT_GPIOC->RR = GPIO_PIN_0;
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_0, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOC, GPIO_PIN_0, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_0, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_0,SET);
	
	motor_pwr(PWR_OFF);
}
