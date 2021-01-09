#include "service_logic_manage.h"

global_variable variable;

u8 monday = 0;
u8 tuesday = 0;
u8 wednesday = 0;
u8 thursday = 0;
u8 friday = 0;
u8 saturday = 0;
u8 sunday = 0;
u8 count_down_action = 0;
volatile u16 protect_current = 0x140;

//����ģʽ
volatile u8 curtain_mode = 0;	//0=����, 1=�Ҵ���
//У׼״̬
volatile u8 calibration_status = 0;	//0=δ�趨, 1=��ʼ, 2=���
//У׼����
volatile u8 calibration_operation = 0;	//0=����, 1=ֹͣ, 2=����
//ȫ������ʱ��
//volatile u32 variable.full_running_time = 0;
//У׼���� AutoMode
volatile u8 calibration_operation_auto = 0;	//0=auto operation status, 1=user operation status

struct week_task week_task;
struct dp_data_buf_t dp_data_buf_t;
struct tlv_type tlv_type = {0};

//dp_id=0x01=�����������ؿ���
void curtain_control_01_pack(u8 *data_buf)
{
	int i = 0;
	
	//usart0_send(data_buf,5);
	dp_data_buf_t.dp_id = data_buf[0];
	dp_data_buf_t.dp_type = data_buf[1];
	dp_data_buf_t.dp_data_len = data_buf[2];
	dp_data_buf_t.dp_data_len = (dp_data_buf_t.dp_data_len<<8)&data_buf[3];
	dp_data_buf_t.dp_data_value[0] = data_buf[4];
	
	for(i=0;i<dp_data_buf_t.dp_data_len;i++)
	{
		//dp_data_buf_t.dp_data_value[0] = data_buf[i+4];
	}
	
#ifdef DEBUG_STATE_LED
	SET_LED(RESET);			//����
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,RESET);
#endif
	
	//if(data_buf[4]==0x01)
	if(dp_data_buf_t.dp_id==0x01)
	{
		//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,RESET);
		//SET_LED(RESET);			//����
		if(dp_data_buf_t.dp_data_value[0]==0x00)
		{
#ifdef DEBUG_STATE_LED
			SET_LED(RESET);			//����
			GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,RESET);
#endif
			open_curtain();
			uart0_len = 0;
			memset(uart0_buf,0,100);
		}
		if(dp_data_buf_t.dp_data_value[0]==0x01)
		{
#ifdef DEBUG_STATE_LED
			SET_LED(RESET);			//����
			GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,RESET);
#endif
			motor_pwr(PWR_OFF);
			motor_stop();
			uart0_len = 0;
			memset(uart0_buf,0,100);
		}
		if(dp_data_buf_t.dp_data_value[0]==0x02)
		{
#ifdef DEBUG_STATE_LED
			SET_LED(RESET);			//����
			GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,RESET);
#endif
			close_curtain();
			uart0_len = 0;
			memset(uart0_buf,0,100);
		}
	}
}

//dp_id=0x02=�ٷֱȿ���
void currain_percentage_02_pack(u8 *data_buf)
{

}

//dp_id=0x0C=12=���ϸ澯
void fault_alarm_0C_pack(u8 *data_buf)
{

}

//dp_id=0x0D=13=��ص���
void battery_level_0D_pack(u8 *data_buf)
{

}

//dp_id=0x65=101=�Զ�����ģʽ
void auto_open_close_65_pack(u8 *data_buf)
{
	
}

//dp_id=0x66=102=����ʱ
void count_down_66_pack(void)
{
	//02 08 00 01 FF FF 41 00 00 01
	if(tlv_type.value[0])
	{
		alarm_second = (tlv_type.value[0]*60*60);
	}
	if(tlv_type.value[1])
	{
		alarm_second += (tlv_type.value[1]*60);
	}
	
	count_down_action = tlv_type.value[5];

	setup_interrupt(alarm_second);
	rtc_interrupt = 0;
}

//dp_id=0x66=102=��ʱ
void timing_control_66_pack(void)
{
	//01 08 00 01 FF FF 00 00 00 01
	int i = 0;
	int date[7] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40};
	
	if(tlv_type.value[4]==0x00)	//����
	{
		//tlv_type.value[0]*60*60
		//tlv_type.value[0] = current_time-tlv_type.value[0];
		count_down_66_pack();	//����ʱ
	}
	else						//�ظ�
	{
		for(i=0;i<7;i++)
		{
			switch(tlv_type.value[4]&date[i])
			{
				case 0x01:		//monday
					week_task.monday_timed_task.hour = tlv_type.value[0];
					week_task.monday_timed_task.minute = tlv_type.value[1];
					week_task.monday_timed_task.action = tlv_type.value[5];
					break;
				case 0x02:		//tuesday
					week_task.tuesday_timed_task.hour = tlv_type.value[0];
					week_task.tuesday_timed_task.minute = tlv_type.value[1];
					week_task.tuesday_timed_task.action = tlv_type.value[5];
					break;
				case 0x04:		//wednesday
					week_task.wednesday_timed_task.hour = tlv_type.value[0];
					week_task.wednesday_timed_task.minute = tlv_type.value[1];
					week_task.wednesday_timed_task.action = tlv_type.value[5];
					break;
				case 0x08:		//thursday
					week_task.thursday_timed_task.hour = tlv_type.value[0];
					week_task.thursday_timed_task.minute = tlv_type.value[1];
					week_task.thursday_timed_task.action = tlv_type.value[5];
					break;
				case 0x10:		//friday
					week_task.friday_timed_task.hour = tlv_type.value[0];
					week_task.friday_timed_task.minute = tlv_type.value[1];
					week_task.friday_timed_task.action = tlv_type.value[5];
					break;
				case 0x20:		//saturday
					week_task.saturday_timed_task.hour = tlv_type.value[0];
					week_task.saturday_timed_task.minute = tlv_type.value[1];
					week_task.saturday_timed_task.action = tlv_type.value[5];
					break;
				case 0x40:		//sunday
					week_task.sunday_timed_task.hour = tlv_type.value[0];
					week_task.sunday_timed_task.minute = tlv_type.value[1];
					week_task.sunday_timed_task.action = tlv_type.value[5];
					break;
			}
		}
	}
}

//dp_id=0x66=102=��ʱ����
void timed_tasks_66_pack(u8 *data_buf)
{
	//66 00 00 0A 01 08 06 1E FF FF 41 00 00 01
	//01����ʱ��02--����ʱ
	//08������
	//061E=6��30
	//FFFF��ȱʡ
	//41:�ظ����ڣ�����--00
	//00������01-��
	//00����������
	//01: 
	int i = 0;
	
	dp_data_buf_t.dp_id = data_buf[0];
	dp_data_buf_t.dp_type = data_buf[1];
	dp_data_buf_t.dp_data_len = data_buf[2];
	dp_data_buf_t.dp_data_len = (dp_data_buf_t.dp_data_len<<8)|data_buf[3];
	
	for(i=0;i<dp_data_buf_t.dp_data_len;i++)
	{
		dp_data_buf_t.dp_data_value[i] = data_buf[i+4];
	}
	
	tlv_type.tag = dp_data_buf_t.dp_data_value[0];
	tlv_type.len = dp_data_buf_t.dp_data_value[1];
	for(i=0;i<tlv_type.len;i++)
	{
		tlv_type.value[i] = dp_data_buf_t.dp_data_value[i+2];
	}
	
	if(tlv_type.tag==0x01)	//��ʱ
	{
		timing_control_66_pack();
	}
	if(tlv_type.tag==0x02)	//����ʱ
	{
		count_down_66_pack();
	}
	
	speed = ((u32)data_buf[4])<<24;
	speed |= ((u32)data_buf[5])<<16;
	speed |= ((u32)data_buf[6])<<8;
	speed |= (u32)data_buf[7];
}

//dp_id=0x67=103=����ģʽ
void curtain_mode_67_pack(const u8 *data_buf)
{
	//67 04 00 01 00
	int i = 0;
	struct dp_data_buf_t dp_data_buf_t_01;
	
	dp_data_buf_t_01.dp_id = data_buf[0];
	dp_data_buf_t_01.dp_type = data_buf[1];
	dp_data_buf_t_01.dp_data_len = data_buf[2];
	dp_data_buf_t_01.dp_data_len = (dp_data_buf_t_01.dp_data_len<<8)|data_buf[3];
	
	for(i=0;i<dp_data_buf_t_01.dp_data_len;i++)
	{
		dp_data_buf_t_01.dp_data_value[i] = data_buf[i+4];
	}
	
	if(dp_data_buf_t_01.dp_data_value[0]==0)
	{
		curtain_mode = 0;
	}
	else
	{
		curtain_mode = 1;
	}
}
//dp_id=0x6C=108=����ٶ�����
void motor_speed_setting_pack(const unsigned char value[])
{
	//6C 02 00 04 00 00 00 33
	struct dp_data_buf_t dp_data_buf_t_01;
	
	dp_data_buf_t_01.dp_id = value[0];
	dp_data_buf_t_01.dp_type = value[1];
	dp_data_buf_t_01.dp_data_len = value[2];
	dp_data_buf_t_01.dp_data_len = (dp_data_buf_t_01.dp_data_len<<8)|value[3];
	speed = ((u32)value[4])<<24;
	speed |= ((u32)value[5])<<16;
	speed |= ((u32)value[6])<<8;
	speed |= (u32)value[7];
}

void dp_download_curtain(const unsigned char value[])
{ 
    //STRING�������ݴ���
    unsigned char string_data;
    
    string_data = value[0];


	if(string_data == 0)
	{
		curtain_mode = 0;
	}
	if(string_data == 1)
	{
		curtain_mode = 1;
	}
     
}
void dp_DPID_CALIBRATION_STATUS(const unsigned char value[])
{ 
    //STRING�������ݴ���
    unsigned char string_data;
    
    string_data = value[0];


	if(string_data == 0)
	{
		calibration_status = 0;
	}
	if(string_data == 1)
	{
		calibration_status = 1;
		//open_curtain();

	}
	if(string_data == 2)
	{
		calibration_status = 2;
		//close_curtain();
	}
     
}

////dp_id=0x68=1034У׼״̬
void calibration_state_68_pack(u8 *data_buf)
{}
//{
//	//68 04 00 01 00
//	int i = 0;
//	struct dp_data_buf_t dp_data_buf_t_01;
//	
//    usart0_send(data_buf,5);
//    
//	dp_data_buf_t_01.dp_id = data_buf[0];
//	dp_data_buf_t_01.dp_type = data_buf[1];
//	dp_data_buf_t_01.dp_data_len = data_buf[2];
//	dp_data_buf_t_01.dp_data_len = (dp_data_buf_t_01.dp_data_len<<8)|data_buf[3];
//	
//	for(i=0;i<dp_data_buf_t_01.dp_data_len;i++)
//	{
//		dp_data_buf_t_01.dp_data_value[i] = data_buf[i+4];
//	}
//	
//	if(dp_data_buf_t_01.dp_data_value[0]==0)
//	{
//		calibration_status = 0;		//δ�趨	
//	}
//	if(dp_data_buf_t_01.dp_data_value[0]==1)
//	{
//		calibration_status = 1;		//��ʼ
//		//calibration_operation = 1;			//ֹͣ
//	}
//	if(dp_data_buf_t_01.dp_data_value[0]==2)
//	{
//		calibration_status = 2;		//ֹͣ
//	}
//}

//dp_id=0x69=105=LED����
void control_led_69_pack(u8 *data_buf)
{
	
}

//dp_id=0x6A=106=��ص������㱨��
void battery_level_6A_pack(u8 *data_buf)
{
	//65 01 00 01 01
	struct dp_data_buf_t dp_data_buf_t_01;
	
	dp_data_buf_t_01.dp_id = data_buf[0];
	dp_data_buf_t_01.dp_type = data_buf[1];
	dp_data_buf_t_01.dp_data_len = data_buf[2];
	dp_data_buf_t_01.dp_data_len = (dp_data_buf_t_01.dp_data_len<<8)|data_buf[3];
	speed = ((u32)data_buf[4])<<24;
	speed |= ((u32)data_buf[5])<<16;
	speed |= ((u32)data_buf[6])<<8;
	speed |= (u32)data_buf[7];
}

//dp_id=0x6B=107=����ģʽ
void fast_mode_6B_pack(u8 *data_buf)
{
	
}

//dp_id=0x6C=108=����ٶ�����
void motor_speed_setting_6C_pack(u8 *data_buf)
{
	//6C 02 00 04 00 00 00 33
	struct dp_data_buf_t dp_data_buf_t_01;
	
	dp_data_buf_t_01.dp_id = data_buf[0];
	dp_data_buf_t_01.dp_type = data_buf[1];
	dp_data_buf_t_01.dp_data_len = data_buf[2];
	dp_data_buf_t_01.dp_data_len = (dp_data_buf_t_01.dp_data_len<<8)|data_buf[3];
	speed = ((u32)data_buf[4])<<24;
	speed |= ((u32)data_buf[5])<<16;
	speed |= ((u32)data_buf[6])<<8;
	speed |= (u32)data_buf[7];
}

//dp_id=0x6D=109=У׼����
void calibration_operation_6D_pack(u8 *data_buf)
{
	//6D 04 00 01 00
	int i = 0;
	struct dp_data_buf_t dp_data_buf_t_01;
	
	dp_data_buf_t_01.dp_id = data_buf[0];
	dp_data_buf_t_01.dp_type = data_buf[1];
	dp_data_buf_t_01.dp_data_len = data_buf[2];
	dp_data_buf_t_01.dp_data_len = (dp_data_buf_t_01.dp_data_len<<8)|data_buf[3];
	
	for(i=0;i<dp_data_buf_t_01.dp_data_len;i++)
	{
		dp_data_buf_t_01.dp_data_value[i] = data_buf[i+4];
	}
	
	if(dp_data_buf_t_01.dp_data_value[0]==0)
	{
		calibration_operation = 0;			//����
	}
	if(dp_data_buf_t_01.dp_data_value[0]==1)
	{
		calibration_operation = 1;			//ֹͣ
	}
	if(dp_data_buf_t_01.dp_data_value[0]==2)
	{
		calibration_operation = 2;			//����
	}
	// 2020.11.20 When operation CMD received, set auto mode false
	calibration_operation_auto = 0;
}
void dp_DPID_CALIBRATION_operation(const unsigned char value[])
{ 
    //STRING�������ݴ���
    unsigned char string_data;
    
    string_data = value[0];


	if(string_data == 0)
	{
		calibration_operation = 0;			//����
	}
	if(string_data == 1)
	{
		calibration_operation = 1;			//ֹͣ
	}
	if(string_data == 2)
	{
		calibration_operation = 2;			//����
	}
	calibration_operation_auto = 0;
     
}


//dp_id=0x78=120=���õ����������
void set_motor_current_78(u8 *data_buf)
{
	//78 02 00 04 00 00 02 F4 81
	// 55 AA 00 07 00 08 | 78 02 00 04 00 00 01 90 1D
	int i = 0;
	struct dp_data_buf_t dp_data_buf_t_01;
	
	usart0_send(data_buf,8);
	
	dp_data_buf_t_01.dp_id = data_buf[0];
	dp_data_buf_t_01.dp_type = data_buf[1];
	dp_data_buf_t_01.dp_data_len = data_buf[2];
	dp_data_buf_t_01.dp_data_len = (dp_data_buf_t_01.dp_data_len<<8)|data_buf[3];
	
	for(i=0;i<dp_data_buf_t_01.dp_data_len;i++)
	{
		dp_data_buf_t_01.dp_data_value[i] = data_buf[i+4];
	}
	
	protect_current = data_buf[6]&0xFFFF;
	protect_current = (protect_current<<8)|(data_buf[7]&0xFFFF);
	//protect_current = dp_data_buf_t_01.dp_data_value[2]&0xFFFF;
	//protect_current = (protect_current<<8)|(dp_data_buf_t_01.dp_data_value[3]&0xFFFF);
	
	//USART_SendData(HT_USART0,(protect_current>>8));
	//USART_SendData(HT_USART0,(protect_current&0xFF));
	
	//USART_SendData(HT_USART0,protect_current);
	//while(USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);
}