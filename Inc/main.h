/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#include <stdio.h>

#include "rtthread.h"


#include "bsp_clock.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "bsp_timer.h"
#include "bsp_i2c.h"
#include "bsp_spi.h"
#include "bsp_dac.h"
#include "bsp_rtc.h"
#include "bsp_timer.h"

#include "bsp_max31865.h"
#include "bsp_hc595.h"


#include "fhc_thread.h"


/*****************

���ܼ���������

*********************/
//#define	USE_TEST_MODE						//ʹ�ò���ģʽ
#define USE_LONG_PRESS_TO_SW_ON_OFF			//ʹ�ó������ػ�����



/*****************************************************/
#define 	debug_log
#ifdef debug_log

			#define		log_info(...)	printf(__VA_ARGS__)

#else
			#define		log_info(...)
#endif



#define EMPTY 	0xFFFF


#ifdef USE_TEST_MODE				//����ģʽ
	
	#define D_MODE_1	0			//����ģʽ����
	#define D_MODE_2	1
	#define D_MODE_3	2
	#define D_MODE_F	3

	#define DEVICE_WORK_START	1		//��ʼ������־λ
	#define DEVICE_WORK_STOP	0		//ֹͣ������־λ
	
	#define TEMP_H_LIMIT	130			//�¶�����
	#define TEMP_L_LIMIT	110			//�¶�����

	#define FAN_WORK_TIME	5*60		//�������ʱ��
	#define HOT_WORK_TIME	1*60		//����������ʱ��
	
	#define FAN_HSPEED_VOL	6.5			//�߷��ٵ�ѹֵ
	#define FAN_MSPEED_VOL	5.5			//�з��ٵ�ѹֵ
	#define FAN_LSPEED_VOL	4.5			//�ͷ��ٵ�ѹֵ
	#define FAN_0SPEED_VOL	0

#else								//��ʽģʽ
	
	#define D_MODE_1	0
	#define D_MODE_2	1
	#define D_MODE_3	2
	#define D_MODE_F	3

	#define DEVICE_WORK_START	1
	#define DEVICE_WORK_STOP	0
	
	#define TEMP_H_LIMIT	110				//�¶����ޱ�־
	#define TEMP_L_LIMIT	100				//�¶����ޱ�־
	#define TEMP_S_LIMIT	50				//ģʽ3ֹͣ�������¶���

	#define FAN_WORK_TIME	50*60
	#define HOT_WORK_TIME	10*60
	
	#define FAN_HSPEED_VOL	6.5
	#define FAN_MSPEED_VOL	5.5
	#define FAN_LSPEED_VOL	4.5
	#define FAN_0SPEED_VOL	0


#endif


//���幤��ָʾLED
#define	FHC_WORK_LED_ON()	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_13,GPIO_PIN_RESET)
#define	FHC_WORK_LED_OFF()	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_13,GPIO_PIN_SET)

//����ģʽLED
#define	FHC_MODE1_LED_ON()	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)
#define	FHC_MODE1_LED_OFF()	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)

#define	FHC_MODE2_LED_ON()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET)
#define	FHC_MODE2_LED_OFF()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET)

#define	FHC_MODE3_LED_ON()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET)
#define	FHC_MODE3_LED_OFF()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET)


//�������LED
#define	FHC_FANH_LED_ON()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET)
#define	FHC_FANH_LED_OFF()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET)

#define	FHC_FANM_LED_ON()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET)
#define	FHC_FANM_LED_OFF()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET)

#define	FHC_FANL_LED_ON()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET)
#define	FHC_FANL_LED_OFF()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET)

//����FAN��HOT״̬���LED
#define	FHC_FAN_LED_ON()	{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);}
#define	FHC_FAN_LED_OFF()	{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);}

#define	FHC_HOT_LED_ON()	{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);}
#define	FHC_HOT_LED_OFF()	{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);}

//����FAN��HOT�������
#define	FHC_FAN_ON()		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET)
#define	FHC_FAN_OFF()		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET)

#define	FHC_HOT_ON()		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET)
#define	FHC_HOT_OFF()		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET)

//���幦�ܰ���

#define	FHC_READ_START_STOP()		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)				//key0
#define	FHC_READ_TIME_SET()			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)				//key1
#define	FHC_READ_MODE_SW()			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)				//key2
#define	FHC_READ_FAN_SW()			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)				//key3

#define	FHC_READ_POW_SW()			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)				//PA0	��������


//���忪�ػ�hold
#define	FHC_POW_HOLD_ON()		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)
#define	FHC_POW_HOLD_OFF()		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)



//����SPI3_CS
#define MAX_CS_H()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET)
#define MAX_CS_L()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET)



extern unsigned int test_data;


extern unsigned int sw_start_stop_flag;

extern unsigned int start_key_count;
	
extern unsigned int sw_time_flag;
extern unsigned int sw_mode_flag;
extern unsigned int sw_fan_flag;
extern unsigned int time_over_flag;

extern unsigned int five_sw_flag_ps;

void fhc_power_down(void);
void Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
