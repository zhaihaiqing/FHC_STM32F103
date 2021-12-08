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

unsigned int sw_start_stop_flag=0;	//��ʼ��ֹͣ��־λ��==1��������==0ֹͣ��

unsigned int start_key_count=0;

unsigned int sw_time_flag=0;
unsigned int sw_mode_flag=0;		//����ģʽ��0=ģʽ1,1=ģʽ2,2=ģʽ3
unsigned int sw_fan_flag=0;
unsigned int time_over_flag=0;		//����ʱ����

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

		if((sw_start_stop_flag==DEVICE_WORK_START) && time_over_flag)//�豸���ڿ�ʼģʽ���ҵ���ʱ����
		{
			//log_info("Mode_start! mode=%d\r\n",sw_mode_flag);
			
			if(sw_mode_flag==D_MODE_1)//����ģʽ1���������50���ӣ�����˿����10���ӣ�ѭ�����У��ֶ�����
			{
				log_info("Mode1_start!\r\n");
				FHC_WORK_LED_ON();
				while(sw_start_stop_flag==DEVICE_WORK_START)
				{
					
					//1-1���������50����
					log_info("FAN_Work!\r\n");
					mode1_count=0;
					FHC_FAN_LED_ON();		//�������
					rt_thread_mdelay(5);
					
					while(sw_start_stop_flag==DEVICE_WORK_START)
					{
						if(sw_fan_flag==0)fhc_set_fan_speed(FAN_LSPEED_VOL);		//�ͷ���
						else if(sw_fan_flag==1)fhc_set_fan_speed(FAN_MSPEED_VOL);	//�з���
						else if(sw_fan_flag==2)fhc_set_fan_speed(FAN_HSPEED_VOL);	//�߷���
						rt_thread_mdelay(100);
						mode1_count++;
						if(mode1_count>=FAN_WORK_TIME*10)break;	
					}
					FHC_FAN_LED_OFF();		//�رշ��
					fhc_set_fan_speed(FAN_0SPEED_VOL);
					log_info("FAN_STOP!\r\n");
					
					//1-2�����ȹ���10����
					log_info("HOT_Work!\r\n");
					mode1_count=0;
					if(sw_start_stop_flag==DEVICE_WORK_START)
					{
						FHC_HOT_LED_ON();		//��������
					}
					while(sw_start_stop_flag==DEVICE_WORK_START)
					{
						if(DP_PT100_temp<TEMP_L_LIMIT)	//����¶ȵ���110����������
						{
							FHC_HOT_LED_ON();
						}
						if(DP_PT100_temp>TEMP_H_LIMIT)	//����¶ȸ���130��ֹͣ����
						{
							FHC_HOT_LED_OFF();
						}
						rt_thread_mdelay(100);
						mode1_count++;
						if(mode1_count>=HOT_WORK_TIME*10)break;	
					}
					FHC_HOT_LED_OFF();		//�رռ���
					log_info("HOT_STOP!\r\n");
					
					rt_thread_mdelay(10);					
				}
				
				//1-3��������
				FHC_FAN_LED_OFF();
				FHC_HOT_LED_OFF();
				sw_mode_flag=D_MODE_F;//�˳�����
				FHC_WORK_LED_OFF();
				pd_count=0;
				log_info("Mode1_stop!\r\n");
				
			}
			else if(sw_mode_flag==D_MODE_2)//����ģʽ2��������������������������ֶ�ֹͣ
			{
				log_info("Mode2_start!\r\n");
				FHC_WORK_LED_ON();
				FHC_FAN_LED_ON();
				rt_thread_mdelay(5);
				
				
				while(sw_start_stop_flag==DEVICE_WORK_START)
				{
					if(sw_fan_flag==0)fhc_set_fan_speed(FAN_LSPEED_VOL);		//�ͷ���
					else if(sw_fan_flag==1)fhc_set_fan_speed(FAN_MSPEED_VOL);	//�з���
					else if(sw_fan_flag==2)fhc_set_fan_speed(FAN_HSPEED_VOL);	//�߷���
					rt_thread_mdelay(100);
				}
				
				fhc_set_fan_speed(FAN_0SPEED_VOL);//�رշ��DAC	//�رշ��
				FHC_FAN_LED_OFF();
				sw_mode_flag=D_MODE_F;
				FHC_WORK_LED_OFF();
				pd_count=0;
				log_info("Mode2_stop!\r\n");
				
			}
			else if(sw_mode_flag==D_MODE_3)//����ģʽ3������˿��������10���ӣ��Զ�ֹͣ���ڼ�����¶ȱ���
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
				FHC_HOT_LED_OFF();	//ֹͣ����
				log_info("Mode3_hot_stop!\r\n");
				
				
				//temp_test=255;
				//while(temp_test>=TEMP_S_LIMIT)	//�¶ȴ���50�ȣ�ģʽ3δ����������
				while( (DP_PT100_temp>=TEMP_S_LIMIT) && (sw_start_stop_flag==DEVICE_WORK_START) )	//�¶ȴ���50�ȣ�ģʽ3δ����������
				{
					rt_thread_mdelay(100);
					//temp_test -= 0.2;
				}
				
				sw_mode_flag=D_MODE_F;
				start_key_count=0;
				sw_start_stop_flag=start_key_count%2;	
				//time_over_flag=0;		//������ʱ��ɱ�־λ����
				FHC_WORK_LED_OFF();		//ֹͣ����
				pd_count=0;
				log_info("Mode3_work_stop,sw_start_stop_flag=%d!\r\n",sw_start_stop_flag);
				
				
			}
			else
			{
				rt_thread_mdelay(10);
			}
		}
		else if(sw_start_stop_flag==DEVICE_WORK_STOP)//����ֹͣģʽ�£��Ҵ��ڵ���ʱ����״̬�£���5����δ���������Զ��ػ�
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
				fhc_power_down();//ִ�йػ�����
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
	FHC_WORK_LED_OFF();		//ֹͣ����
	
	HC595SendData(0,0,0);
	
	FHC_POW_HOLD_OFF();
	
	while(1)						//ִ�йػ�������ѭ����ֹͣ����
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
