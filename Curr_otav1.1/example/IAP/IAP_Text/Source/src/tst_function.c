#include "tst_function.h"

volatile u8 run_state = 0;			//����״̬
volatile u8 rtc_motor_state = 0;
volatile u8 key_flag = 0;			//key_flag=1--���, key_flag=2--�Ҽ�
volatile u8 motor_status = 0;		//motor_status=1--��������, motor_status=2--����
u16 motor_current = 0;

void delay(int i)
{
	for(;i>0;i--){;;;}
}

void update_battery_value(u32 current_data)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x07;
    dp_data_buf[4] = 0x00;	//���ݳ���
    dp_data_buf[5] = 0x08;	//���ݳ���
	dp_data_buf[6] = 0x0D;
	dp_data_buf[7] = 0x02;
	dp_data_buf[8] = 0x00;
	dp_data_buf[9] = 0x04;
	dp_data_buf[10] = (current_data>>24);  //����
	dp_data_buf[11] = (current_data>>16);  //����
	dp_data_buf[12] = (current_data>>8);  //����
	dp_data_buf[13] = (current_data&0xFF);  //����
	crc8 = get_check_sum(dp_data_buf,14);
	dp_data_buf[14] = crc8;
	uart0_send(dp_data_buf,15);
	//usart0_send(dp_data_buf,15);
}

void update_protect_current(u32 current_data)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x07;
    dp_data_buf[4] = 0x00;	//���ݳ���
    dp_data_buf[5] = 0x08;	//���ݳ���
	dp_data_buf[6] = 0X78;
	dp_data_buf[7] = 0x02;
	dp_data_buf[8] = 0x00;
	dp_data_buf[9] = 0x04;
	dp_data_buf[10] = (current_data>>24);  //����
	dp_data_buf[11] = (current_data>>16);  //����
	dp_data_buf[12] = (current_data>>8);  //����
	dp_data_buf[13] = (current_data&0xFF);  //����
	crc8 = get_check_sum(dp_data_buf,14);
	dp_data_buf[14] = crc8;
	uart0_send(dp_data_buf,15);
	//usart0_send(dp_data_buf,15);
}
	
void update_motor_current(u32 current_data)
{
	u8 crc8 = 0;
	
	//55 AA 00 07 00 06 6E 00 00 02 00 64
    //dp_id=110 -- ��ת��������
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x07;
    dp_data_buf[4] = 0x00;	//���ݳ���
    dp_data_buf[5] = 0x08;	//���ݳ���
	dp_data_buf[6] = 0X6E;
	dp_data_buf[7] = 0x02;
	dp_data_buf[8] = 0x00;
	dp_data_buf[9] = 0x04;
	dp_data_buf[10] = (current_data>>24);  //����
	dp_data_buf[11] = (current_data>>16);  //����
	dp_data_buf[12] = (current_data>>8);  //����
	dp_data_buf[13] = (current_data&0xFF);  //����
	crc8 = get_check_sum(dp_data_buf,14);
	dp_data_buf[14] = crc8;
	uart0_send(dp_data_buf,15);
	//usart0_send(dp_data_buf,15);
}
	
//gptm0���Ե������ת
void gptm0_test(void)
{
    u8 state = 0;
    
    //open_curtain();
    if(gptm0_second<3)
    {
        if(state==0)
        {
            state =1;
            SET_LED(RESET);
            open_curtain();
       }
    }
    if(gptm0_second>2)
    {
        if(state==1)
        {
            state = 2;
            SET_LED(SET);
            close_curtain();
       }
    }
    if(gptm0_second>5)
    {
        state = 0;
        gptm0_second = 0;
    }
}

//ADC����
void motor_current_test(void)
{
	float tmp_voltage = 0;
	
    //gPotentiometerLevel = (HT_ADC->DR[0] & 0x0FFF);
    if(gptm0_second > 2)
    {
        gptm0_cnt = 0;
        gptm0_second = 0;  
		motor_current = check_motor_current(protect_current);
        // USART_SendData(HT_USART0,0xdd);
    
        //  USART_SendData(HT_USART0,(motor_current<<8));
        // while(USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET){;}
        // USART_SendData(HT_USART0,(u8)motor_current);
        // while(USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET){;}
        // 	USART_SendData(HT_USART0,0xdd);
		
		update_motor_current(motor_current);//�ϱ���ǰ����ֵ
		update_protect_current(protect_current);  
		update_battery_value(33);		// �ϱ�ĿǰΪ�ٷֱȣ���ģ��ֵ(33%) 33
    }
}

//RTC����ʱ
void rtc_test(void)
{
#if 1
    if(rtc_interrupt>0)
    {
        rtc_interrupt = 0;
        rtc_motor_state = 1;
        gptm0_second = 0;
        RTC_Cmd(DISABLE);
        
        if(count_down_action==0)
        {
            if(curtain_mode==0)
                open_curtain();		//��ת
            else
                close_curtain();	//��ת
        }
        else
        {
            if(curtain_mode==0)
                close_curtain();	//��ת
            else
                open_curtain();		//��ת
        }
        
        GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,RESET);
        USART_SendData(HT_USART0,rtc_interrupt);
        while(USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);
    }
    if((rtc_motor_state==1)&&(gptm0_second>9))
    {
        rtc_motor_state = 0;
        motor_stop();
        
        GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,SET);
        USART_SendData(HT_USART0,rtc_interrupt);
        while(USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);
    }
#else
    if(rtc_interrupt<2)
    {
        if(motor_status==0)
        {
            GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,RESET);
            motor_status = 1;
            open_curtain();
        }
    }
    if(rtc_interrupt>4)
    {
        if(motor_status==1)
        {
            GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,SET);
            motor_status = 2;
            close_curtain();
        }
    }
    if(rtc_interrupt>6)
    {
        motor_stop();
    }
    if(rtc_interrupt>8)
    {
        motor_status = 0;
        rtc_interrupt = 0;
    }
#endif
}
//u32 crc32=0;
//�������ת����
void motor_turn(void)
{
    if(!SET_KEY())
    {
        SET_LED(RESET);
        motor_stop();
        key_flag = 0;
        motor_pwr(PWR_OFF);
    }
    else
    {
        SET_LED(SET);
    }
    
    if(!LEFT_KEY())
    {
        SET_LED(RESET);
        key_flag = 1;
    }
    if(key_flag==1) // TouchGo�������: �����豸���� - �����ת
    {
        tybn1_out_sleep_mode();
		gptm0_4low = 0;
        key_flag = 0;
        motor_pwr(PWR_ON);
        motor_stop();
        delay(100);
        
        //if(curtain_mode==0)
        //    open_curtain();		//��ת
        //else
        /*1��6 ������ת��ת����*/
        //close_curtain();	//��ת
        open_curtain();
    }
    
    if(!RIGHT_KEY())
    {
        SET_LED(RESET);
        key_flag = 2;
    }
    if(key_flag==2) // TouchGo�Ҽ�����: �����豸���� - �����ת
    {
        tybn1_out_sleep_mode();
		gptm0_4low = 0;
        key_flag = 0;
        motor_pwr(PWR_ON);
        motor_stop();
        delay(100);
        //if(curtain_mode==0)
        //    close_curtain();	//��ת
        //else
        /*1��6 ������ת��ת����*/
        //open_curtain();		//��ת
        close_curtain();
    }
}    

//��ת10�룬��ת10��
void motor_10_test(void)
{
    if(!SET_KEY())
    {
        SET_LED(RESET);
        motor_stop();
        key_flag = 0;
        motor_pwr(PWR_OFF);
    }
    else
    {
        SET_LED(SET);
    }
    
    if(!LEFT_KEY())
    {
        SET_LED(RESET);
        gptm0_cnt = 0;
        gptm0_second = 0;
        key_flag = 1;
        motor_status = 1;
    }
    if(key_flag==1)
    {
        if(gptm0_second<9)
        {
            if(motor_status==1)
            {
                motor_status = 2;
                motor_pwr(PWR_ON);
                motor_stop();
                delay(100);
                
                if(curtain_mode==0)
                    open_curtain();		//��ת
                else
                    close_curtain();	//��ת
            }
        }
        if(gptm0_second>9)
        {
            if(motor_status==2)
            {
                motor_status = 3;
                motor_pwr(PWR_ON);
                motor_stop();
                delay(100);
                if(curtain_mode==0)
                    close_curtain();	//��ת
                else
                    open_curtain();		//��ת
            }
        }
        if(gptm0_second>19)
        {
            motor_status = 1;
            gptm0_cnt = 0;
            gptm0_second = 0;
        }
    }
    
    if(!RIGHT_KEY())
    {
        SET_LED(RESET);
        gptm0_cnt = 0;
        gptm0_second = 0;
        key_flag = 2;
        motor_status =1;
    }
    if(key_flag==2)
    {
        if(gptm0_second<9)
        {
            if(motor_status==1)
            {
                motor_status = 2;
                motor_pwr(PWR_ON);
                motor_stop();
                delay(100);
                if(curtain_mode==0)
                    close_curtain();	//��ת
                else
                    open_curtain();		//��ת
            }
        }
        if(gptm0_second>9)
        {
            if(motor_status==2)
            {
                motor_status = 3;
                motor_pwr(PWR_ON);
                motor_stop();
                delay(100);
                
                if(curtain_mode==0)
                    open_curtain();		//��ת
                else
                    close_curtain();	//��ת
            }
        }
        if(gptm0_second>19)
        {
            motor_status = 1;
            gptm0_cnt = 0;
            gptm0_second = 0;
        }
    }
}


void stop_fun(void)
{
	
			motor_stop();			        //ֹͣ

			if(run_state==1)    			//����
			{	
				//0=����, 1=�Ҵ���
				if(curtain_mode==0)     //0=����
				{			
					variable.full_running_time = variable.full_running_time + gptm0_4clbr;
					gptm0_4clbr = 0;									// ��ʱ����: ��ʱ�����							
					
				}
				if(curtain_mode==1)     //1=�Ҵ���
				{
					variable.full_running_time = variable.full_running_time - gptm0_4clbr;
					gptm0_4clbr = 0;									// ��ʱ����: ��ʱ�����
				}
			}
			
			if(run_state==2)    			//����
			{
				//0=����, 1=�Ҵ���
				if(curtain_mode==0)     //0=����
				{
					variable.full_running_time -= gptm0_4clbr;
					gptm0_4clbr = 0;									// ��ʱ����: ��ʱ�����
					/*variable.full_running_time -= gptm0_second*1000;
					variable.full_running_time -= gptm0_cnt;
					gptm0_cnt = 0;
					gptm0_second = 0;*/
				}
				
				if(curtain_mode==1)      //1=�Ҵ���
				{
					variable.full_running_time += gptm0_4clbr;
					gptm0_4clbr = 0;									// ��ʱ����: ��ʱ�����
					/*variable.full_running_time += gptm0_second*1000;
					variable.full_running_time += gptm0_cnt;
					gptm0_cnt = 0;
					gptm0_second = 0;*/
				}
			}
			
			run_state = 3;
			//TODO: upload DP - calibration_operation: 1=ֹͣ
}

//У׼
void calibration_test(void)
{
    /*
    У׼ģʽ��
    �����ʼ -- �����������ʼ��ʱ
    ������ -- ֹͣ���������У��ʱ��
    ����
    ���ֹͣ -- ֹͣ��������浱ǰʱ��󸶳�ֵ
    ������� -- ��ת�����¼�ʱ���������Ҵ���ģʽ������-�ۼӵ��ϴ�ʱ�䣬�Ҵ���-��ȥ����ʱ��
    ������� -- ��ת�����¼�ʱ���������Ҵ���ģʽ���Ҵ���-�ۼӵ��ϴ�ʱ�䣬����-��ȥ����ʱ��
    ���ж�ת����  Ҳ�Ǹ�"ֹͣ"��һ��
    */
	//У׼״̬ calibration_status: 0 =δ�趨, 1=��ʼ, 2=���
	//calibration_operation: 0 =����, 1=ֹͣ, 2=����
	//run_state 0=��ʼУ׼ 1=����  2=����  3=ֹͣ
	//calibration_operation_auto	1:�Զ�У׼ģʽ  0:�ֶ�У׼ģʽ(ģ���·�-У׼����DPЭ����)
	//gptm0_4clbr		У׼ר�ü�ʱ����
	
	if(calibration_status==1)		//��ʼУ׼
	{
		if(run_state==0)			//��һ�ο�ʼ
		{
			USART_SendData(HT_USART0,0x22);
			//run_state = 1;
			//0=����, 1=�Ҵ���
			if(curtain_mode==0){
				USART_SendData(HT_USART0,curtain_mode);
				
				open_curtain();		//�򿪴���
				ms_delay_systick(100);

//				ms_delay_systick(100);
//				ms_delay_systick(100);
//				ms_delay_systick(100);
			  	run_state = 1;		//����
				//TODO: upload DP - calibration_operation: 0=����
			} else {
				close_curtain();	//�رմ���
				run_state = 2;		//����
				//TODO: upload DP - calibration_operation: 2=����
			}			
			
			calibration_operation_auto = 1;		// �Զ�У׼ģʽ: true
			variable.full_running_time=0;
			gptm0_4clbr = 0;									// У׼��ʱ��ʼ
			//gptm0_cnt = 0;
			//gptm0_second = 0;
		}
		
		//0=����, 2=����
		//���� && �ֶ�У׼ģʽ && ����У׼����������״̬�����������ת.���ü�ʱ
		if(calibration_operation==0 && calibration_operation_auto==0 && run_state == 3)	// �������ƶ�״̬�£������ظ����룬����ᵼ�¼�ʱ���ã������ۼӴ���
		{
			run_state = 1;
			
			open_curtain();		//��ת����
			gptm0_4clbr = 0;									// ��ʱ����: �µ����ƴ���
			ms_delay_systick(100);
			ms_delay_systick(100);
			ms_delay_systick(100);
			//gptm0_cnt = 0;
			//gptm0_second = 0;
		}
		//���� && �ֶ�У׼ģʽ && ����У׼����������״̬�����������ת.���ü�ʱ
		if(calibration_operation==2  && calibration_operation_auto==0 && run_state == 3)	// �������ƶ�״̬�£������ظ����룬����ᵼ�¼�ʱ���ã������ۼӴ���
		{
			run_state = 2;
			
			close_curtain();	//��ת����
			gptm0_4clbr = 0;	// ��ʱ����: �µ����ƴ���
			ms_delay_systick(100);
			ms_delay_systick(100);
			ms_delay_systick(100);
			//gptm0_cnt = 0;
			//gptm0_second = 0;
		}
		// a. ��У׼����.ֹͣ ���� �ֶ�У׼״̬ --> ����ֹͣ�������
		// b. ������ת.ǿ��ͣת 							--> ����ֹͣ�������
		/*��������*/ /*ֹͣ*/ 
		if((motor_current>protect_current) || (calibration_operation==1 && calibration_operation_auto==0))
		{
			calibration_operation=1;
			stop_fun();
			
		}
		

	}
	
	if(calibration_status==2)	      //���У׼
	{
		USART_SendData(HT_USART0,0xcc);
		// 2020.11.20 ������Ȼ����ת��״̬��ͣת����: ���¼�ʱ����
		if (run_state==1 || run_state==2)
		{	
			
			motor_stop();	
			if(run_state==1)    			//����
			{				
				//0=����, 1=�Ҵ���
				if(curtain_mode==0)     //0=����
				{
					variable.full_running_time += gptm0_4clbr;
					gptm0_4clbr = 0;									// ��ʱ����: ��ʱ�����
					/*variable.full_running_time += gptm0_second*1000;
					variable.full_running_time += gptm0_cnt;
					gptm0_cnt = 0;
					gptm0_second = 0;*/
				}
				if(curtain_mode==1)     //1=�Ҵ���
				{
					variable.full_running_time -= gptm0_4clbr;
					gptm0_4clbr = 0;									// ��ʱ����: ��ʱ�����
					/*variable.full_running_time -= gptm0_second*1000;
					variable.full_running_time -= gptm0_cnt;
					gptm0_cnt = 0;
					gptm0_second = 0;*/
				}
			}
			
			if(run_state==2)    			//����
			{
				//0=����, 1=�Ҵ���
				if(curtain_mode==0)     //0=����
				{
					variable.full_running_time -= gptm0_4clbr;
					gptm0_4clbr = 0;									// ��ʱ����: ��ʱ�����
					/*variable.full_running_time -= gptm0_second*1000;
					variable.full_running_time -= gptm0_cnt;
					gptm0_cnt = 0;
					gptm0_second = 0;*/
				}
				
				if(curtain_mode==1)      //1=�Ҵ���
				{
					variable.full_running_time += gptm0_4clbr;
					gptm0_4clbr = 0;									// ��ʱ����: ��ʱ�����
					/*variable.full_running_time += gptm0_second*1000;
					variable.full_running_time += gptm0_cnt;
					gptm0_cnt = 0;
					gptm0_second = 0;*/
				}
			}
		}		
		
		run_state = 0;
		//ֹͣ
		calibration_status = 0;
		full_running_time_0A_pack(variable.full_running_time);

        int i;
        for(i=FLASH_55K_GLOBAL_DATA;i<FLASH_55K_GLOBAL_DATA+1024;i+=1024)
		{
			FLASH_ErasePage(i);
		}
        FLASH_ProgramWordData(FLASH_55K_GLOBAL_DATA, variable.full_running_time);
        FLASH_ProgramWordData(FLASH_55K_GLOBAL_DATA+4, variable.global_percent);
		//variable.full_running_time = 0;
	}
}



//TYBN1����
void tybn1_test(void)
{
    if(uart0_len>6)
    {
        ms_delay_systick(100);
        
        if((uart0_buf[0]==0x55)&&(uart0_buf[1]==0xAA))
        {
			tybn1_out_sleep_mode();
            tybn1_ctrl();
            mcu_state = 1;
        }
        uart0_len = 0;
        memset(uart0_buf,0,100);
    }
    
    if(gptm0_second>9)
    {
        //heart_beat();
        //gptm0_second = 0;
    }
}