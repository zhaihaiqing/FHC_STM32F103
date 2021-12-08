#include "main.h"


ALIGN(RT_ALIGN_SIZE)


float DP_PT100_temp=0.0;



static unsigned char SET_time_hour;
static unsigned char DP_time_hour;
static unsigned char DP_time_min;

static struct rt_timer td_timer1;					//����һ�������ʱ��


//��Դ����ɨ���߳�
static struct rt_thread pow_thread;				//����ɨ���߳̿��ƿ�
static char pow_thread_stack[512];				//�̶߳�ջ
#define POW_THREAD_PRIORITY         8			//�߳����ȼ�������ɨ��Ϊ������ȼ�
#define POW_THREAD_TIMESLICE        100			//�̵߳�ʱ��Ƭ��С

//���ܰ���ɨ���߳�
static struct rt_thread key_thread;				//����ɨ���߳̿��ƿ�
static char key_thread_stack[512];				//�̶߳�ջ
#define KEY_THREAD_PRIORITY         10			//�߳����ȼ�������ɨ��Ϊ������ȼ�
#define KEY_THREAD_TIMESLICE        100			//�̵߳�ʱ��Ƭ��С

//�������ʾ�߳�
static struct rt_thread dp_thread;				//�����ɨ���߳̿��ƿ�
static char dp_thread_stack[512];				//�̶߳�ջ
#define DP_THREAD_PRIORITY          9			//�߳����ȼ�������ɨ��Ϊ������ȼ�
#define DP_THREAD_TIMESLICE         100			//�̵߳�ʱ��Ƭ��С

//�¶Ȳɼ��߳�
static struct rt_thread temp_thread;			//�¶Ȳɼ��߳̿��ƿ�
static char temp_thread_stack[512];				//�̶߳�ջ
#define TEMP_THREAD_PRIORITY          11		//�߳����ȼ�������ɨ��Ϊ������ȼ�
#define TEMP_THREAD_TIMESLICE         100		//�̵߳�ʱ��Ƭ��С

//����ʱ�߳�
static struct rt_thread time_thread;			//����ʱ�߳̿��ƿ�
static char time_thread_stack[512];				//�̶߳�ջ
#define TIME_THREAD_PRIORITY          12		//�߳����ȼ�������ɨ��Ϊ������ȼ�
#define TIME_THREAD_TIMESLICE         100		//�̵߳�ʱ��Ƭ��С

//���ȿ����߳�
static struct rt_thread hot_thread;				//���ȿ����߳̿��ƿ�
static char hot_thread_stack[512];				//�̶߳�ջ
#define HOT_THREAD_PRIORITY          13			//�߳����ȼ�������ɨ��Ϊ������ȼ�
#define HOT_THREAD_TIMESLICE         100		//�̵߳�ʱ��Ƭ��С

static struct rt_thread fan_thread;				//��������߳̿��ƿ�
static char fan_thread_stack[512];				//�̶߳�ջ
#define FAN_THREAD_PRIORITY          14			//�߳����ȼ�������ɨ��Ϊ������ȼ�
#define FAN_THREAD_TIMESLICE         100		//�̵߳�ʱ��Ƭ��С

int fhc_thread_pow_init(void)					//������������߳�
{
	rt_thread_init(&pow_thread,					//�߳̿��ƿ�
                   "pow_thread",				//�߳̿��ƿ�����
                   pow_thread_entry,			//�߳���ں���
                   RT_NULL,						//�߳���ں����Ĳ���
                   &pow_thread_stack[0],		//�߳�ջ��ʼ��ַ
                   sizeof(pow_thread_stack),	//�߳�ջ��С
                   POW_THREAD_PRIORITY, 		//�߳����ȼ�
				   POW_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
	rt_thread_startup(&pow_thread); 
				   
	return 0;
}

int fhc_thread_init(void)
{	
	rt_timer_init(&td_timer1, "td_timer1",  /* ��ʱ�������� timer1 */
                    soft_timeout1, /* ��ʱʱ�ص��Ĵ����� */
                    RT_NULL, /* ��ʱ��������ڲ��� */
                    1000, /* ��ʱ���ȣ��� OS Tick Ϊ��λ���� 10 �� OS Tick */
                    RT_TIMER_FLAG_PERIODIC); /* �����Զ�ʱ�� */
	
	
	rt_thread_init(&key_thread,					//�߳̿��ƿ�
                   "key_thread",				//�߳̿��ƿ�����
                   key_thread_entry,			//�߳���ں���
                   RT_NULL,						//�߳���ں����Ĳ���
                   &key_thread_stack[0],		//�߳�ջ��ʼ��ַ
                   sizeof(key_thread_stack),	//�߳�ջ��С
                   KEY_THREAD_PRIORITY, 		//�߳����ȼ�
				   KEY_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
				   
	rt_thread_init(&dp_thread,					//�߳̿��ƿ�
                   "dp_thread",					//�߳̿��ƿ�����
                   dp_thread_entry,				//�߳���ں���
                   RT_NULL,						//�߳���ں����Ĳ���
                   &dp_thread_stack[0],			//�߳�ջ��ʼ��ַ
                   sizeof(dp_thread_stack),		//�߳�ջ��С
                   DP_THREAD_PRIORITY, 			//�߳����ȼ�
				   DP_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
				   
	rt_thread_init(&temp_thread,					//�߳̿��ƿ�
                   "temp_thread",					//�߳̿��ƿ�����
                   temp_thread_entry,				//�߳���ں���
                   RT_NULL,							//�߳���ں����Ĳ���
                   &temp_thread_stack[0],			//�߳�ջ��ʼ��ַ
                   sizeof(temp_thread_stack),		//�߳�ջ��С
                   TEMP_THREAD_PRIORITY, 			//�߳����ȼ�
				   TEMP_THREAD_TIMESLICE);			//�߳�ʱ��Ƭ��С	
				   
	rt_thread_init(&time_thread,					//�߳̿��ƿ�
                   "time_thread",					//�߳̿��ƿ�����
                   time_thread_entry,				//�߳���ں���
                   RT_NULL,							//�߳���ں����Ĳ���
                   &time_thread_stack[0],			//�߳�ջ��ʼ��ַ
                   sizeof(time_thread_stack),		//�߳�ջ��С
                   TIME_THREAD_PRIORITY, 			//�߳����ȼ�
				   TIME_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
				   
	
				   
	
					   
	rt_thread_startup(&key_thread); 
    rt_thread_startup(&dp_thread); 
	rt_thread_startup(&temp_thread); 
	rt_thread_startup(&time_thread); 
	
	
				   
	rt_timer_start(&td_timer1);
					
	return 0;
}

//�������������ɢ���߳�
int fhc_fan_thread_init(void)
{
	rt_thread_init(&fan_thread,					//�߳̿��ƿ�
                   "fan_thread",					//�߳̿��ƿ�����
                   fan_thread_entry,				//�߳���ں���
                   RT_NULL,							//�߳���ں����Ĳ���
                   &fan_thread_stack[0],			//�߳�ջ��ʼ��ַ
                   sizeof(fan_thread_stack),		//�߳�ջ��С
                   FAN_THREAD_PRIORITY, 			//�߳����ȼ�
				   FAN_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
				   
	rt_thread_startup(&fan_thread);
	return 0;
}

//���������������߳�
int fhc_hot_thread_init(void)
{
	rt_thread_init(&hot_thread,					//�߳̿��ƿ�
                   "hot_thread",					//�߳̿��ƿ�����
                   hot_thread_entry,				//�߳���ں���
                   RT_NULL,							//�߳���ں����Ĳ���
                   &hot_thread_stack[0],			//�߳�ջ��ʼ��ַ
                   sizeof(hot_thread_stack),		//�߳�ջ��С
                   HOT_THREAD_PRIORITY, 			//�߳����ȼ�
				   HOT_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
				   
	rt_thread_startup(&hot_thread); 
	return 0;
}




void soft_timeout1(void* parameter)
{
    ;;
	//rt_kprintf("one shot timer is timeout\n");
}


#ifdef	USE_LONG_PRESS_TO_SW_ON_OFF
void pow_thread_entry(void *par)					//��������̣߳��ڸ��߳���ʵ�ֳ��������������ػ�
{
	log_info("pow_thread startup!\r\n");
	uint8_t pow_status=0;
	uint32_t pow_count=0;
	while(1)										//ÿ��10tick��ɨ��һ�鿪����ⰴ����ʵ���������������������ػ�
	{
		rt_thread_mdelay(10);
		if(!FHC_READ_POW_SW())
		{
			five_sw_flag_ps=1;
			pow_count++;
			if(pow_count>200)						//��������3S��
			{
				if(pow_status==0)					//���״̬��־Ϊ0��ִ�п���
				{
					log_info("pow ON!\r\n");
					FHC_FANL_LED_ON();
					FHC_FANM_LED_ON();
					FHC_FANH_LED_ON();
					FHC_MODE1_LED_ON();
					FHC_MODE2_LED_ON();
					FHC_MODE3_LED_ON();
					FHC_POW_HOLD_ON();
					while(!FHC_READ_POW_SW())		//�ڴ˴�����ֱ���ɿ�����
					{
						rt_thread_mdelay(5);
					}
					rt_thread_mdelay(1200);
					FHC_FANL_LED_OFF();
					FHC_FANM_LED_OFF();
					FHC_FANH_LED_OFF();
					fhc_thread_init();
					pow_count=0;
					pow_status=1;
				}
				else
				{
					pow_status=0;
					
					fhc_power_down();//ִ�йػ�����
					
					pow_count=0;
					pow_status=0;
				}
			}
		}
		else
		{
			pow_count=0;			
		}
	}
}
#else
void pow_thread_entry(void *par)					//��������̣߳��ڸ��߳���ʵ�ֳ��������������ػ�����ʱʹ��
{
	log_info("pow_thread startup!\r\n");
	
	FHC_POW_HOLD_ON();
	fhc_thread_init();
	
	while(1)										//ÿ��10tick��ɨ��һ�鿪����ⰴ����ʵ���������������������ػ�
	{
		rt_thread_mdelay(10);
	}
}
#endif



void key_thread_entry(void *par)
{
	log_info("key_thread startup!\r\n");
	unsigned int fan_key_count=0;
	unsigned int mode_key_count=0;
	
	unsigned int start_key_lps_count=0;
	
	log_info("fan_count:%d,sw_fan_flag:%d\r\n",fan_key_count,sw_fan_flag);	
	
	FHC_FANL_LED_OFF();
	FHC_FANM_LED_OFF();
	FHC_FANH_LED_OFF();
	FHC_MODE1_LED_OFF();
	FHC_MODE2_LED_OFF();
	FHC_MODE3_LED_OFF();
	
	FHC_MODE1_LED_ON();//Ĭ��ģʽ1��
	FHC_FANL_LED_ON();//Ĭ��ʹ����ͷ���
	
	fan_key_count=3;	//����Ĭ��ֵ
	mode_key_count=3;	//����Ĭ��ֵ
	sw_fan_flag=0;
	sw_mode_flag=0;
	
	
	while(1)										//ÿ��10tick��ɨ��һ�鰴��
	{
		
		if(!FHC_READ_START_STOP())//������������
		{
			five_sw_flag_ps=1;
			
			if(sw_start_stop_flag==0)//˵����stop�л�Ϊstart
			{
				//FHC_WORK_LED_ON();
				while(!FHC_READ_START_STOP())
				{
					rt_thread_mdelay(5);
				}
				start_key_lps_count=0;
				start_key_count++;
				sw_start_stop_flag=start_key_count%2;
				
				sw_mode_flag=mode_key_count%3;
				FHC_MODE1_LED_OFF();
				FHC_MODE2_LED_OFF();
				FHC_MODE3_LED_OFF();
				if(sw_mode_flag==0)FHC_MODE1_LED_ON();
				else if(sw_mode_flag==1)FHC_MODE2_LED_ON();
				else if(sw_mode_flag==2)FHC_MODE3_LED_ON();
				
				log_info("start_key_count:%d,sw_start_stop_flag:%d\r\n",start_key_count,sw_start_stop_flag);
			}
			else	//˵������start�л�Ϊstop
			{
				while(!FHC_READ_START_STOP())
				{
					rt_thread_mdelay(5);
					start_key_lps_count++;
					if(start_key_lps_count>=600)//����
					{
						FHC_WORK_LED_OFF();
						
						start_key_count++;
						sw_start_stop_flag=start_key_count%2;
						SET_time_hour=0;
						DP_time_hour=0;
						DP_time_min=0;
						time_over_flag=0;		//������ʱ��ɱ�־λ����
						
						while(!FHC_READ_START_STOP())
						{
							rt_thread_mdelay(5);
						}
						start_key_lps_count=0;
						
						log_info("start_key_count:%d,sw_start_stop_flag:%d\r\n",start_key_count,sw_start_stop_flag);
					}
				}				
			}
			
			
			
			
			
			
//			while(!FHC_READ_START_STOP())
//			{
//				rt_thread_mdelay(5);
//				start_key_lps_count++;
//				if(start_key_lps_count>=600)//����
//				{
//					if(sw_start_stop_flag==0)	//�������STOP,
//					{
//						FHC_WORK_LED_ON();
//					}
//					else
//					{
//						FHC_WORK_LED_OFF();
//					}
//					while(!FHC_READ_START_STOP())
//					{
//						rt_thread_mdelay(5);
//					}
//					
//					start_key_lps_count=0;
//					start_key_count++;
//					sw_start_stop_flag=start_key_count%2;
//					if(sw_start_stop_flag==0)	//�������STOP,
//					{
//						SET_time_hour=0;
//						DP_time_hour=0;
//						DP_time_min=0;
//						time_over_flag=0;		//������ʱ��ɱ�־λ����
//					}
//					else
//					{
//						sw_mode_flag=mode_key_count%3;
//						FHC_MODE1_LED_OFF();
//						FHC_MODE2_LED_OFF();
//						FHC_MODE3_LED_OFF();
//						if(sw_mode_flag==0)FHC_MODE1_LED_ON();
//						else if(sw_mode_flag==1)FHC_MODE2_LED_ON();
//						else if(sw_mode_flag==2)FHC_MODE3_LED_ON();
//					}
//					log_info("start_key_count:%d,sw_start_stop_flag:%d\r\n",start_key_count,sw_start_stop_flag);
//				}
//			}
			
//			start_key_count++;
//			sw_start_stop_flag=start_key_count%2;
//			if(sw_start_stop_flag==0)	//�������STOP,
//			{
//				SET_time_hour=0;
//				DP_time_hour=0;
//				DP_time_min=0;
//				time_over_flag=0;		//������ʱ��ɱ�־λ����
//			}
//			else
//			{
//				sw_mode_flag=mode_key_count%3;
//				FHC_MODE1_LED_OFF();
//				FHC_MODE2_LED_OFF();
//				FHC_MODE3_LED_OFF();
//				if(sw_mode_flag==0)FHC_MODE1_LED_ON();
//				else if(sw_mode_flag==1)FHC_MODE2_LED_ON();
//				else if(sw_mode_flag==2)FHC_MODE3_LED_ON();
//			}
//			log_info("start_key_count:%d,sw_start_stop_flag:%d\r\n",start_key_count,sw_start_stop_flag);			
		}
		
		
		if(!FHC_READ_FAN_SW())//���·��ٰ���
		{
			five_sw_flag_ps=1;
			while(!FHC_READ_FAN_SW())							//�½��ش���
			{
				rt_thread_mdelay(5);
			}
			fan_key_count++;
			
			sw_fan_flag=fan_key_count%3;
			FHC_FANL_LED_OFF();
			FHC_FANM_LED_OFF();
			FHC_FANH_LED_OFF();
			
			if(sw_fan_flag==0)FHC_FANL_LED_ON();
			else if(sw_fan_flag==1)FHC_FANM_LED_ON();
			else if(sw_fan_flag==2)FHC_FANH_LED_ON();
			
			time_over_flag=0;
			
			log_info("fan_count:%d,sw_fan_flag:%d\r\n",fan_key_count,sw_fan_flag);			
		}
		
		
		if(sw_start_stop_flag==DEVICE_WORK_STOP)//��ֹͣģʽ�²�������
		{
		
			
			if(!FHC_READ_MODE_SW())//����ģʽ����
			{
				five_sw_flag_ps=1;
				while(!FHC_READ_MODE_SW())
				{
					rt_thread_mdelay(5);
				}
				mode_key_count++;
				sw_mode_flag=mode_key_count%3;
				FHC_MODE1_LED_OFF();
				FHC_MODE2_LED_OFF();
				FHC_MODE3_LED_OFF();
				if(sw_mode_flag==0)FHC_MODE1_LED_ON();
				else if(sw_mode_flag==1)FHC_MODE2_LED_ON();
				else if(sw_mode_flag==2)FHC_MODE3_LED_ON();
				
				time_over_flag=0;
				
				log_info("mode_key_count:%d,sw_mode_flag:%d\r\n",mode_key_count,sw_mode_flag);			
			}
			
			
			if(!FHC_READ_TIME_SET())//����ʱ�䰴����ԤԼʱ�����ð���
			{
				unsigned int time_key_count=0;
				unsigned char long_time_key_flag=0;
				
				five_sw_flag_ps=1;
				while(!FHC_READ_TIME_SET())
				{
					rt_thread_mdelay(10);
					time_key_count++;
					if(time_key_count>=25)	//ʵ�ֳ���������1
					{
						time_key_count=0;
						long_time_key_flag=1;
						SET_time_hour++;
						if(SET_time_hour==25)SET_time_hour=0;
						DP_time_hour=SET_time_hour;
						DP_time_min=0;
					}
				}
				
				if(long_time_key_flag==0)	//���û�г������͵��μ�1
				{
					SET_time_hour++;
					if(SET_time_hour==25)SET_time_hour=0;	
					DP_time_hour=SET_time_hour;
					DP_time_min=0;
				}
				
				time_over_flag=0;
				
				log_info("SET_time_hour:%d\r\n",SET_time_hour);			
			}
		}		
		rt_thread_mdelay(10);
	}
}

//������ʾ�߳�
void dp_thread_entry(void *par)
{
	unsigned char dp_delay=5;
	unsigned char code_SMG_ND=0x00;													//����code������ʾ
	unsigned char code_SMG_N=0x02;													//����code������
	unsigned char code_SMG[10]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6};	//����code
	unsigned char code_SWG[4]={0x10,0x20,0x40,0x80};								//λѡcode
	uint64_t ix=0;
	
	unsigned char buf[4]={0};		//buf[3]���������������(1Ϊ�¶ȣ�2Ϊʱ��)��buf[2]��δʹ�ã�buf[1]:�¶ȵ�MSB/ʱ���hour��buf[0]:�¶ȵ�LSB/ʱ���min,,�¶�����λuint16_t��X10����1λС��
	
	log_info("dp_thread startup!\r\n");
	
	DP_PT100_temp=0;
	DP_time_hour=0;
	DP_time_min=0;
	
	while(1)										//�¶���ʾ��Χ-99.9-999.9�棬ʱ�䷶Χ
	{
		ix++;
		rt_thread_mdelay(dp_delay);
		
		HC595SendData(0x00,0x00,0x00);
		if(DP_PT100_temp<0)
		{
			HC595SendData(code_SWG[3],code_SMG_N,code_SMG[(DP_time_hour%100)/10]);
		}
		else
		{
			HC595SendData(code_SWG[3],code_SMG[(((int)DP_PT100_temp)%1000)/100],code_SMG[(DP_time_hour%100)/10]);
		}
		rt_thread_mdelay(dp_delay);
		HC595SendData(0x00,0x00,0x00);
		if((DP_time_hour*3600+DP_time_min*60>0) && (sw_start_stop_flag==DEVICE_WORK_START))
		{
			if((ix/(125/dp_delay))%2 )
			{
				HC595SendData(code_SWG[2],code_SMG[((((unsigned int)DP_PT100_temp)%100)/10)],code_SMG[DP_time_hour%10]);
			}
			else
			{
				HC595SendData(code_SWG[2],code_SMG[((((unsigned int)DP_PT100_temp)%100)/10)],code_SMG[DP_time_hour%10]|0x01);
			}
		}
		else
		{
			HC595SendData(code_SWG[2],code_SMG[((((unsigned int)DP_PT100_temp)%100)/10)],code_SMG[DP_time_hour%10]|0x01);
		}
		
		rt_thread_mdelay(dp_delay);
		HC595SendData(0x00,0x00,0x00);
		HC595SendData(code_SWG[1],code_SMG[((unsigned int)DP_PT100_temp)%10]|0x01,code_SMG[(DP_time_min%100)/10]);
		
		rt_thread_mdelay(dp_delay);
		HC595SendData(0x00,0x00,0x00);
		if((DP_time_hour*3600+DP_time_min*60>0) && (sw_start_stop_flag==DEVICE_WORK_START))
		{
			if((ix/(125/dp_delay))%2 )
			{
				HC595SendData(code_SWG[0],code_SMG[((unsigned int)(DP_PT100_temp*10))%10],code_SMG[DP_time_min%10]);
			}
			else
			{
				HC595SendData(code_SWG[0],code_SMG[((unsigned int)(DP_PT100_temp*10))%10],code_SMG[DP_time_min%10]|0x01);
			}
		}
		else
		{
			HC595SendData(code_SWG[0],code_SMG[((unsigned int)(DP_PT100_temp*10))%10],code_SMG[DP_time_min%10]|0x01);
		}		
	}
}

//�¶����ݻ�ȡ�߳�
void temp_thread_entry(void *par)
{	
	rt_thread_mdelay(100);
	
	MAX31865_Init();
	
	rt_thread_mdelay(100);
	
	Max31865_get_temp();
	rt_thread_mdelay(100);
	Max31865_get_temp();
	rt_thread_mdelay(100);
	
	log_info("temp_thread startup!\r\n");
	
	while(1)										
	{
		rt_thread_mdelay(333);
		//log_info("MAX31865_REG_CFG:0x%x\r\n",MAX31865_Read_Reg(MAX31865_REG_CFG));
		DP_PT100_temp=Max31865_get_temp();
	}
}

//ʱ�䵹��ʱ�߳�
void time_thread_entry(void *par)
{	
	unsigned int time_d=0;
	unsigned char hour=12;
	unsigned int min=0;
	
	unsigned int sec=0;
	
	rt_thread_mdelay(200);
	
	log_info("time_thread startup!\r\n");
	
	while(1)										
	{
		
		if(sw_start_stop_flag==DEVICE_WORK_START)	//����豸����
		{
			if(SET_time_hour==0)
			{
				time_over_flag=1;
			}
			else
			{
				rt_thread_mdelay(1000);
				sec++;
				if(sec>=60)
				{
					sec=0;
					min++;
				}
				
				if(min>=SET_time_hour*60)//�����ʱ������͵���ʱ��ɱ�־λ����ʼ����
				{
					if(time_over_flag==0)
					{
						//DP_time_hour=(SET_time_hour*60-min)/60;
						DP_time_min =0;
					}
					time_over_flag=1;			//����ʱ�Ѿ�����
					//log_info("time_in=0\r\n");
				}
				else
				{
					//sec+=1;
					DP_time_hour=(SET_time_hour*60-min)/60;
					DP_time_min =(SET_time_hour*60-min)%60;
					time_over_flag=0;
					//log_info("time--\r\n");
				}
			}
		}
		else						//�豸ֹͣ
		{
			sec= 0;
			min= 0;
			rt_thread_mdelay(10);
		}
	}
}

//void time_thread_entry(void *par)
//{	
//	unsigned int time_d=0;
//	unsigned char hour=12;
//	unsigned int min=0;
//	
//	unsigned int sec=0;
//	
//	rt_thread_mdelay(200);
//	
//	log_info("time_thread startup!\r\n");
//	
//	while(1)										
//	{
//		
//		if(sw_start_stop_flag==DEVICE_WORK_START)	//����豸����
//		{
//			rt_thread_mdelay(1000);
//			sec++;
//			
//			if(sec>=60)
//			{
//				sec=0;
//				min++;
//			}
//			
//			if(min>=SET_time_hour*60)//�����ʱ������͵���ʱ��ɱ�־λ����ʼ����
//			{
//				time_over_flag=1;			//����ʱ�Ѿ�����
//				//log_info("time_in=0\r\n");
//			}
//			else
//			{
//				//sec+=1;
//				DP_time_hour=(SET_time_hour*3600-sec)/3600;
//				DP_time_min =((SET_time_hour*3600-sec)%3600)/60;
//				time_over_flag=0;
//				//log_info("time--\r\n");
//			}
//		}
//		else						//�豸ֹͣ
//		{
//			sec=60;
//			rt_thread_mdelay(10);
//		}
//	}
//}


//���ȿ����߳�
void hot_thread_entry(void *par)
{	
	rt_thread_mdelay(200);
	FHC_HOT_LED_ON();
	
	log_info("hot_thread startup!\r\n");
	
	while(1)										
	{
		rt_thread_mdelay(1000);
	}
	
	FHC_HOT_LED_OFF();
}

//��������߳�
void fan_thread_entry(void *par)
{		
	rt_thread_mdelay(200);
	log_info("fan_thread startup!\r\n");
	
	//1:�򿪷����Դ����LEDָʾ
	FHC_FAN_LED_ON();
	//FHC_FAN_ON();
	
	//2����ʱ200ms
	rt_thread_mdelay(200);
	
	//3������DACֵ
	
	//set_dac_val(2048);
	
	while(1)										
	{
		rt_thread_mdelay(1000);
	}
	
	FHC_FAN_LED_OFF();
	//FHC_FAN_OFF();
	
}


void fhc_set_fan_speed(float vol)
{
	unsigned short vol_code;
	
	
	vol_code=vol*4096.0/(3.3*(1.0+100.0/24.9));
	
	//log_info("vol:%f,vol_code:%d\r\n",vol,vol_code);
	
	if(vol_code>=4095)vol_code=4095;
	
	set_dac_val(vol_code);
	
}




























