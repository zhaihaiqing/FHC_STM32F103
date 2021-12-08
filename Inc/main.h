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

功能及参数配置

*********************/
//#define	USE_TEST_MODE						//使用测试模式
#define USE_LONG_PRESS_TO_SW_ON_OFF			//使用长按开关机功能



/*****************************************************/
#define 	debug_log
#ifdef debug_log

			#define		log_info(...)	printf(__VA_ARGS__)

#else
			#define		log_info(...)
#endif



#define EMPTY 	0xFFFF


#ifdef USE_TEST_MODE				//测试模式
	
	#define D_MODE_1	0			//工作模式代码
	#define D_MODE_2	1
	#define D_MODE_3	2
	#define D_MODE_F	3

	#define DEVICE_WORK_START	1		//开始工作标志位
	#define DEVICE_WORK_STOP	0		//停止工作标志位
	
	#define TEMP_H_LIMIT	130			//温度上限
	#define TEMP_L_LIMIT	110			//温度下限

	#define FAN_WORK_TIME	5*60		//风机工作时间
	#define HOT_WORK_TIME	1*60		//加热器工作时间
	
	#define FAN_HSPEED_VOL	6.5			//高风速电压值
	#define FAN_MSPEED_VOL	5.5			//中风速电压值
	#define FAN_LSPEED_VOL	4.5			//低风速电压值
	#define FAN_0SPEED_VOL	0

#else								//正式模式
	
	#define D_MODE_1	0
	#define D_MODE_2	1
	#define D_MODE_3	2
	#define D_MODE_F	3

	#define DEVICE_WORK_START	1
	#define DEVICE_WORK_STOP	0
	
	#define TEMP_H_LIMIT	110				//温度上限标志
	#define TEMP_L_LIMIT	100				//温度下限标志
	#define TEMP_S_LIMIT	50				//模式3停止工作的温度限

	#define FAN_WORK_TIME	50*60
	#define HOT_WORK_TIME	10*60
	
	#define FAN_HSPEED_VOL	6.5
	#define FAN_MSPEED_VOL	5.5
	#define FAN_LSPEED_VOL	4.5
	#define FAN_0SPEED_VOL	0


#endif


//定义工作指示LED
#define	FHC_WORK_LED_ON()	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_13,GPIO_PIN_RESET)
#define	FHC_WORK_LED_OFF()	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_13,GPIO_PIN_SET)

//定义模式LED
#define	FHC_MODE1_LED_ON()	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)
#define	FHC_MODE1_LED_OFF()	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)

#define	FHC_MODE2_LED_ON()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET)
#define	FHC_MODE2_LED_OFF()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET)

#define	FHC_MODE3_LED_ON()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET)
#define	FHC_MODE3_LED_OFF()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET)


//定义风速LED
#define	FHC_FANH_LED_ON()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET)
#define	FHC_FANH_LED_OFF()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET)

#define	FHC_FANM_LED_ON()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET)
#define	FHC_FANM_LED_OFF()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET)

#define	FHC_FANL_LED_ON()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET)
#define	FHC_FANL_LED_OFF()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET)

//定义FAN、HOT状态输出LED
#define	FHC_FAN_LED_ON()	{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);}
#define	FHC_FAN_LED_OFF()	{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);}

#define	FHC_HOT_LED_ON()	{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);}
#define	FHC_HOT_LED_OFF()	{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);}

//定义FAN、HOT输出引脚
#define	FHC_FAN_ON()		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET)
#define	FHC_FAN_OFF()		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET)

#define	FHC_HOT_ON()		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET)
#define	FHC_HOT_OFF()		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET)

//定义功能按键

#define	FHC_READ_START_STOP()		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)				//key0
#define	FHC_READ_TIME_SET()			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)				//key1
#define	FHC_READ_MODE_SW()			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)				//key2
#define	FHC_READ_FAN_SW()			HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)				//key3

#define	FHC_READ_POW_SW()			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)				//PA0	开机按键


//定义开关机hold
#define	FHC_POW_HOLD_ON()		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)
#define	FHC_POW_HOLD_OFF()		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)



//定义SPI3_CS
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
