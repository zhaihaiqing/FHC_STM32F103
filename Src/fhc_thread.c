#include "main.h"


ALIGN(RT_ALIGN_SIZE)


float DP_PT100_temp=0.0;



static unsigned char SET_time_hour;
static unsigned char DP_time_hour;
static unsigned char DP_time_min;

static struct rt_timer td_timer1;					//创建一个软件定时器


//电源按键扫描线程
static struct rt_thread pow_thread;				//按键扫描线程控制块
static char pow_thread_stack[512];				//线程堆栈
#define POW_THREAD_PRIORITY         8			//线程优先级，按键扫描为最高优先级
#define POW_THREAD_TIMESLICE        100			//线程的时间片大小

//功能按键扫描线程
static struct rt_thread key_thread;				//按键扫描线程控制块
static char key_thread_stack[512];				//线程堆栈
#define KEY_THREAD_PRIORITY         10			//线程优先级，按键扫描为最高优先级
#define KEY_THREAD_TIMESLICE        100			//线程的时间片大小

//数码管显示线程
static struct rt_thread dp_thread;				//数码管扫描线程控制块
static char dp_thread_stack[512];				//线程堆栈
#define DP_THREAD_PRIORITY          9			//线程优先级，按键扫描为最高优先级
#define DP_THREAD_TIMESLICE         100			//线程的时间片大小

//温度采集线程
static struct rt_thread temp_thread;			//温度采集线程控制块
static char temp_thread_stack[512];				//线程堆栈
#define TEMP_THREAD_PRIORITY          11		//线程优先级，按键扫描为最高优先级
#define TEMP_THREAD_TIMESLICE         100		//线程的时间片大小

//倒计时线程
static struct rt_thread time_thread;			//倒计时线程控制块
static char time_thread_stack[512];				//线程堆栈
#define TIME_THREAD_PRIORITY          12		//线程优先级，按键扫描为最高优先级
#define TIME_THREAD_TIMESLICE         100		//线程的时间片大小

//加热控制线程
static struct rt_thread hot_thread;				//加热控制线程控制块
static char hot_thread_stack[512];				//线程堆栈
#define HOT_THREAD_PRIORITY          13			//线程优先级，按键扫描为最高优先级
#define HOT_THREAD_TIMESLICE         100		//线程的时间片大小

static struct rt_thread fan_thread;				//风机控制线程控制块
static char fan_thread_stack[512];				//线程堆栈
#define FAN_THREAD_PRIORITY          14			//线程优先级，按键扫描为最高优先级
#define FAN_THREAD_TIMESLICE         100		//线程的时间片大小

int fhc_thread_pow_init(void)					//创建开机检测线程
{
	rt_thread_init(&pow_thread,					//线程控制块
                   "pow_thread",				//线程控制块名字
                   pow_thread_entry,			//线程入口函数
                   RT_NULL,						//线程入口函数的参数
                   &pow_thread_stack[0],		//线程栈起始地址
                   sizeof(pow_thread_stack),	//线程栈大小
                   POW_THREAD_PRIORITY, 		//线程优先级
				   POW_THREAD_TIMESLICE);		//线程时间片大小
	rt_thread_startup(&pow_thread); 
				   
	return 0;
}

int fhc_thread_init(void)
{	
	rt_timer_init(&td_timer1, "td_timer1",  /* 定时器名字是 timer1 */
                    soft_timeout1, /* 超时时回调的处理函数 */
                    RT_NULL, /* 超时函数的入口参数 */
                    1000, /* 定时长度，以 OS Tick 为单位，即 10 个 OS Tick */
                    RT_TIMER_FLAG_PERIODIC); /* 周期性定时器 */
	
	
	rt_thread_init(&key_thread,					//线程控制块
                   "key_thread",				//线程控制块名字
                   key_thread_entry,			//线程入口函数
                   RT_NULL,						//线程入口函数的参数
                   &key_thread_stack[0],		//线程栈起始地址
                   sizeof(key_thread_stack),	//线程栈大小
                   KEY_THREAD_PRIORITY, 		//线程优先级
				   KEY_THREAD_TIMESLICE);		//线程时间片大小
				   
	rt_thread_init(&dp_thread,					//线程控制块
                   "dp_thread",					//线程控制块名字
                   dp_thread_entry,				//线程入口函数
                   RT_NULL,						//线程入口函数的参数
                   &dp_thread_stack[0],			//线程栈起始地址
                   sizeof(dp_thread_stack),		//线程栈大小
                   DP_THREAD_PRIORITY, 			//线程优先级
				   DP_THREAD_TIMESLICE);		//线程时间片大小
				   
	rt_thread_init(&temp_thread,					//线程控制块
                   "temp_thread",					//线程控制块名字
                   temp_thread_entry,				//线程入口函数
                   RT_NULL,							//线程入口函数的参数
                   &temp_thread_stack[0],			//线程栈起始地址
                   sizeof(temp_thread_stack),		//线程栈大小
                   TEMP_THREAD_PRIORITY, 			//线程优先级
				   TEMP_THREAD_TIMESLICE);			//线程时间片大小	
				   
	rt_thread_init(&time_thread,					//线程控制块
                   "time_thread",					//线程控制块名字
                   time_thread_entry,				//线程入口函数
                   RT_NULL,							//线程入口函数的参数
                   &time_thread_stack[0],			//线程栈起始地址
                   sizeof(time_thread_stack),		//线程栈大小
                   TIME_THREAD_PRIORITY, 			//线程优先级
				   TIME_THREAD_TIMESLICE);		//线程时间片大小
				   
	
				   
	
					   
	rt_thread_startup(&key_thread); 
    rt_thread_startup(&dp_thread); 
	rt_thread_startup(&temp_thread); 
	rt_thread_startup(&time_thread); 
	
	
				   
	rt_timer_start(&td_timer1);
					
	return 0;
}

//创建并启动风机散热线程
int fhc_fan_thread_init(void)
{
	rt_thread_init(&fan_thread,					//线程控制块
                   "fan_thread",					//线程控制块名字
                   fan_thread_entry,				//线程入口函数
                   RT_NULL,							//线程入口函数的参数
                   &fan_thread_stack[0],			//线程栈起始地址
                   sizeof(fan_thread_stack),		//线程栈大小
                   FAN_THREAD_PRIORITY, 			//线程优先级
				   FAN_THREAD_TIMESLICE);		//线程时间片大小
				   
	rt_thread_startup(&fan_thread);
	return 0;
}

//创建并启动加热线程
int fhc_hot_thread_init(void)
{
	rt_thread_init(&hot_thread,					//线程控制块
                   "hot_thread",					//线程控制块名字
                   hot_thread_entry,				//线程入口函数
                   RT_NULL,							//线程入口函数的参数
                   &hot_thread_stack[0],			//线程栈起始地址
                   sizeof(hot_thread_stack),		//线程栈大小
                   HOT_THREAD_PRIORITY, 			//线程优先级
				   HOT_THREAD_TIMESLICE);		//线程时间片大小
				   
	rt_thread_startup(&hot_thread); 
	return 0;
}




void soft_timeout1(void* parameter)
{
    ;;
	//rt_kprintf("one shot timer is timeout\n");
}


#ifdef	USE_LONG_PRESS_TO_SW_ON_OFF
void pow_thread_entry(void *par)					//开机检测线程，在该线程中实现长按开机、长按关机
{
	log_info("pow_thread startup!\r\n");
	uint8_t pow_status=0;
	uint32_t pow_count=0;
	while(1)										//每隔10tick，扫描一遍开机检测按键，实现连续长按开机、长按关机
	{
		rt_thread_mdelay(10);
		if(!FHC_READ_POW_SW())
		{
			five_sw_flag_ps=1;
			pow_count++;
			if(pow_count>200)						//连续长按3S，
			{
				if(pow_status==0)					//如果状态标志为0，执行开机
				{
					log_info("pow ON!\r\n");
					FHC_FANL_LED_ON();
					FHC_FANM_LED_ON();
					FHC_FANH_LED_ON();
					FHC_MODE1_LED_ON();
					FHC_MODE2_LED_ON();
					FHC_MODE3_LED_ON();
					FHC_POW_HOLD_ON();
					while(!FHC_READ_POW_SW())		//在此处待机直到松开按键
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
					
					fhc_power_down();//执行关机程序
					
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
void pow_thread_entry(void *par)					//开机检测线程，在该线程中实现长按开机、长按关机，临时使用
{
	log_info("pow_thread startup!\r\n");
	
	FHC_POW_HOLD_ON();
	fhc_thread_init();
	
	while(1)										//每隔10tick，扫描一遍开机检测按键，实现连续长按开机、长按关机
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
	
	FHC_MODE1_LED_ON();//默认模式1，
	FHC_FANL_LED_ON();//默认使用最低风速
	
	fan_key_count=3;	//设置默认值
	mode_key_count=3;	//设置默认值
	sw_fan_flag=0;
	sw_mode_flag=0;
	
	
	while(1)										//每隔10tick，扫描一遍按键
	{
		
		if(!FHC_READ_START_STOP())//按下启动按键
		{
			five_sw_flag_ps=1;
			
			if(sw_start_stop_flag==0)//说明由stop切换为start
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
			else	//说明是由start切换为stop
			{
				while(!FHC_READ_START_STOP())
				{
					rt_thread_mdelay(5);
					start_key_lps_count++;
					if(start_key_lps_count>=600)//长按
					{
						FHC_WORK_LED_OFF();
						
						start_key_count++;
						sw_start_stop_flag=start_key_count%2;
						SET_time_hour=0;
						DP_time_hour=0;
						DP_time_min=0;
						time_over_flag=0;		//将倒计时完成标志位清零
						
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
//				if(start_key_lps_count>=600)//长按
//				{
//					if(sw_start_stop_flag==0)	//如果按下STOP,
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
//					if(sw_start_stop_flag==0)	//如果按下STOP,
//					{
//						SET_time_hour=0;
//						DP_time_hour=0;
//						DP_time_min=0;
//						time_over_flag=0;		//将倒计时完成标志位清零
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
//			if(sw_start_stop_flag==0)	//如果按下STOP,
//			{
//				SET_time_hour=0;
//				DP_time_hour=0;
//				DP_time_min=0;
//				time_over_flag=0;		//将倒计时完成标志位清零
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
		
		
		if(!FHC_READ_FAN_SW())//按下风速按键
		{
			five_sw_flag_ps=1;
			while(!FHC_READ_FAN_SW())							//下降沿触发
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
		
		
		if(sw_start_stop_flag==DEVICE_WORK_STOP)//在停止模式下才能设置
		{
		
			
			if(!FHC_READ_MODE_SW())//按下模式按键
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
			
			
			if(!FHC_READ_TIME_SET())//按下时间按键，预约时间设置按键
			{
				unsigned int time_key_count=0;
				unsigned char long_time_key_flag=0;
				
				five_sw_flag_ps=1;
				while(!FHC_READ_TIME_SET())
				{
					rt_thread_mdelay(10);
					time_key_count++;
					if(time_key_count>=25)	//实现长按连续加1
					{
						time_key_count=0;
						long_time_key_flag=1;
						SET_time_hour++;
						if(SET_time_hour==25)SET_time_hour=0;
						DP_time_hour=SET_time_hour;
						DP_time_min=0;
					}
				}
				
				if(long_time_key_flag==0)	//如果没有长按，就单次加1
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

//数据显示线程
void dp_thread_entry(void *par)
{
	unsigned char dp_delay=5;
	unsigned char code_SMG_ND=0x00;													//段码code，不显示
	unsigned char code_SMG_N=0x02;													//段码code，负号
	unsigned char code_SMG[10]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6};	//段码code
	unsigned char code_SWG[4]={0x10,0x20,0x40,0x80};								//位选code
	uint64_t ix=0;
	
	unsigned char buf[4]={0};		//buf[3]用来标记数据类型(1为温度，2为时间)，buf[2]暂未使用，buf[1]:温度的MSB/时间的hour，buf[0]:温度的LSB/时间的min,,温度数据位uint16_t，X10保留1位小数
	
	log_info("dp_thread startup!\r\n");
	
	DP_PT100_temp=0;
	DP_time_hour=0;
	DP_time_min=0;
	
	while(1)										//温度显示范围-99.9-999.9℃，时间范围
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

//温度数据获取线程
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

//时间倒计时线程
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
		
		if(sw_start_stop_flag==DEVICE_WORK_START)	//如果设备启动
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
				
				if(min>=SET_time_hour*60)//如果计时到达，发送倒计时完成标志位，开始工作
				{
					if(time_over_flag==0)
					{
						//DP_time_hour=(SET_time_hour*60-min)/60;
						DP_time_min =0;
					}
					time_over_flag=1;			//倒计时已经结束
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
		else						//设备停止
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
//		if(sw_start_stop_flag==DEVICE_WORK_START)	//如果设备启动
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
//			if(min>=SET_time_hour*60)//如果计时到达，发送倒计时完成标志位，开始工作
//			{
//				time_over_flag=1;			//倒计时已经结束
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
//		else						//设备停止
//		{
//			sec=60;
//			rt_thread_mdelay(10);
//		}
//	}
//}


//加热控制线程
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

//风机控制线程
void fan_thread_entry(void *par)
{		
	rt_thread_mdelay(200);
	log_info("fan_thread startup!\r\n");
	
	//1:打开风机电源，和LED指示
	FHC_FAN_LED_ON();
	//FHC_FAN_ON();
	
	//2：延时200ms
	rt_thread_mdelay(200);
	
	//3：设置DAC值
	
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




























