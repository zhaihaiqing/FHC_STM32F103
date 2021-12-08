/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"


unsigned int test_data=0;

unsigned int sw_start_stop_flag=0;	//开始与停止标志位，==1，启动，==0停止；

unsigned int start_key_count=0;

unsigned int sw_time_flag=0;
unsigned int sw_mode_flag=0;		//工作模式，0=模式1,1=模式2,2=模式3
unsigned int sw_fan_flag=0;
unsigned int time_over_flag=0;		//倒计时结束

unsigned int five_sw_flag_ps=0;



extern RTC_HandleTypeDef hrtc;



int main(void)
{
	//uint16_t i=0;
	unsigned int mode1_count=0;
	unsigned int mode2_count=0;
	unsigned int mode3_count=0;
	unsigned int pd_count=0;
	
	HAL_Init();
	
	SystemClock_Config();
	MX_GPIO_Init();
	MX_DAC_Init();
	//MX_I2C1_Init();
	
	MX_RTC_Init();
	HAL_RTCEx_SetSecond_IT(&hrtc);
	
	//__HAL_RTC_SECOND_CLEAR_FLAG(hrtc, RTC_FLAG_SEC);
	
	//MX_TIM2_Init();
	MX_USART1_UART_Init();
	rt_show_version();
	get_RCCCLOCK();
	
	fhc_thread_pow_init();
	
	fhc_set_fan_speed(0.1);
	
//

	
	//pd_count=0;
	//rt_thread_mdelay(1000);
	
	while (1)
	{		

		if((sw_start_stop_flag==DEVICE_WORK_START) && time_over_flag)//设备处于开始模式，且倒计时结束
		{
			//log_info("Mode_start! mode=%d\r\n",sw_mode_flag);
			
			if(sw_mode_flag==D_MODE_1)//工作模式1，风机工作50分钟，加热丝工作10分钟，循环进行，手动结束
			{
				log_info("Mode1_start!\r\n");
				FHC_WORK_LED_ON();
				while(sw_start_stop_flag==DEVICE_WORK_START)
				{
					
					//1-1：风机工作50分钟
					log_info("FAN_Work!\r\n");
					mode1_count=0;
					FHC_FAN_LED_ON();		//开启风机
					rt_thread_mdelay(5);
					
					while(sw_start_stop_flag==DEVICE_WORK_START)
					{
						if(sw_fan_flag==0)fhc_set_fan_speed(FAN_LSPEED_VOL);		//低风速
						else if(sw_fan_flag==1)fhc_set_fan_speed(FAN_MSPEED_VOL);	//中风速
						else if(sw_fan_flag==2)fhc_set_fan_speed(FAN_HSPEED_VOL);	//高风速
						rt_thread_mdelay(100);
						mode1_count++;
						if(mode1_count>=FAN_WORK_TIME*10)break;	
					}
					FHC_FAN_LED_OFF();		//关闭风机
					fhc_set_fan_speed(FAN_0SPEED_VOL);
					log_info("FAN_STOP!\r\n");
					
					//1-2：加热工作10分钟
					log_info("HOT_Work!\r\n");
					mode1_count=0;
					if(sw_start_stop_flag==DEVICE_WORK_START)
					{
						FHC_HOT_LED_ON();		//开启加热
					}
					while(sw_start_stop_flag==DEVICE_WORK_START)
					{
						if(DP_PT100_temp<TEMP_L_LIMIT)	//如果温度低于110，开启加热
						{
							FHC_HOT_LED_ON();
						}
						if(DP_PT100_temp>TEMP_H_LIMIT)	//如果温度高于130，停止加热
						{
							FHC_HOT_LED_OFF();
						}
						rt_thread_mdelay(100);
						mode1_count++;
						if(mode1_count>=HOT_WORK_TIME*10)break;	
					}
					FHC_HOT_LED_OFF();		//关闭加热
					log_info("HOT_STOP!\r\n");
					
					rt_thread_mdelay(10);					
				}
				
				//1-3结束工作
				FHC_FAN_LED_OFF();
				FHC_HOT_LED_OFF();
				sw_mode_flag=D_MODE_F;//退出工作
				FHC_WORK_LED_OFF();
				pd_count=0;
				log_info("Mode1_stop!\r\n");
				
			}
			else if(sw_mode_flag==D_MODE_2)//工作模式2，风机单独工作，持续工作，手动停止
			{
				log_info("Mode2_start!\r\n");
				FHC_WORK_LED_ON();
				FHC_FAN_LED_ON();
				rt_thread_mdelay(5);
				
				
				while(sw_start_stop_flag==DEVICE_WORK_START)
				{
					if(sw_fan_flag==0)fhc_set_fan_speed(FAN_LSPEED_VOL);		//低风速
					else if(sw_fan_flag==1)fhc_set_fan_speed(FAN_MSPEED_VOL);	//中风速
					else if(sw_fan_flag==2)fhc_set_fan_speed(FAN_HSPEED_VOL);	//高风速
					rt_thread_mdelay(100);
				}
				
				fhc_set_fan_speed(FAN_0SPEED_VOL);//关闭风机DAC	//关闭风机
				FHC_FAN_LED_OFF();
				sw_mode_flag=D_MODE_F;
				FHC_WORK_LED_OFF();
				pd_count=0;
				log_info("Mode2_stop!\r\n");
				
			}
			else if(sw_mode_flag==D_MODE_3)//工作模式3，加热丝单独工作10分钟，自动停止，期间进行温度保护
			{
				//float temp_test=0;
				log_info("Mode3_start!\r\n");
				FHC_WORK_LED_ON();
				while(sw_start_stop_flag==DEVICE_WORK_START)
				{
					if(DP_PT100_temp<TEMP_L_LIMIT)
					{
						FHC_HOT_LED_ON();
					}
					if(DP_PT100_temp>TEMP_H_LIMIT)
					{
						FHC_HOT_LED_OFF();
					}
					rt_thread_mdelay(100);
					mode3_count++;
					if(mode3_count>=HOT_WORK_TIME*10)break;	
					//if(mode3_count>=1*60*10)break;
				}
				mode3_count=0;
				FHC_HOT_LED_OFF();	//停止加热
				log_info("Mode3_hot_stop!\r\n");
				
				
				//temp_test=255;
				//while(temp_test>=TEMP_S_LIMIT)	//温度大于50度，模式3未真正结束。
				while( (DP_PT100_temp>=TEMP_S_LIMIT) && (sw_start_stop_flag==DEVICE_WORK_START) )	//温度大于50度，模式3未真正结束。
				{
					rt_thread_mdelay(100);
					//temp_test -= 0.2;
				}
				
				sw_mode_flag=D_MODE_F;
				start_key_count=0;
				sw_start_stop_flag=start_key_count%2;	
				//time_over_flag=0;		//将倒计时完成标志位清零
				FHC_WORK_LED_OFF();		//停止工作
				pd_count=0;
				log_info("Mode3_work_stop,sw_start_stop_flag=%d!\r\n",sw_start_stop_flag);
				
				
			}
			else
			{
				rt_thread_mdelay(10);
			}
		}
		else if(sw_start_stop_flag==DEVICE_WORK_STOP)//处于停止模式下，且处于倒计时结束状态下，若5分钟未操作，则自动关机
		{
			rt_thread_mdelay(100);
			pd_count++;
			
			//log_info("pd_count:%d\r\n",pd_count);
		
			if(five_sw_flag_ps==1)
			{
				pd_count=0;
				five_sw_flag_ps=0;
			}
		
			if(pd_count>=5*60*10)
			{
				fhc_power_down();//执行关机程序
			}
		}
		else
		{
			rt_thread_mdelay(10);
		}
		
		
		
		//log_info("					test_data:%d\r\n",test_data);
	}
	
}



void fhc_power_down(void)
{
	log_info("pow OFF!\r\n");
	FHC_POW_HOLD_OFF();
	
	FHC_FANL_LED_OFF();
	FHC_FANM_LED_OFF();
	FHC_FANH_LED_OFF();
	
	FHC_MODE1_LED_OFF();
	FHC_MODE2_LED_OFF();
	FHC_MODE3_LED_OFF();
	
	FHC_FAN_LED_OFF();
	
	FHC_HOT_LED_OFF();
	FHC_WORK_LED_OFF();		//停止工作
	
	HC595SendData(0,0,0);
	
	FHC_POW_HOLD_OFF();
	
	while(1)						//执行关机程序，死循环，停止调度
	{
		;
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */




/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
