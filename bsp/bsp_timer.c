
#include "main.h"

TIM_HandleTypeDef htim2;


/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0x0;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}


///*******************************************************************************
//* Function Name  : Init_SysTick
//* Description    : Systick定时器
//* Input          : 
//* Output         : None
//* Return         : None
//*******************************************************************************/
////void Init_SysTick(void)
////{
////	RCC_ClocksTypeDef  RCC_Clocks;
////	
////	RCC_GetClocksFreq(&RCC_Clocks);
////	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
////	NVIC_SetPriority(SysTick_IRQn, 0x0);    //SysTick中断优先级设置
////}

///*******************************************************************************
//* Function Name  : Delay_us_Timer_Init
//* Description    : 亚毫秒定时功能初始化，采样timer7(基本定时器)
//* Input          : PCLK2频率（MHz）
//* Output         : None
//* Return         : None
//*******************************************************************************/
////void Delay_us_Timer_Init(unsigned char PCLK2_CLK)	
////{
////    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
////	//timer时钟来源，默认预分频系数，分频后的计数频率为Timer_CLK为PCLK1/PCLK2时钟频率的2倍
////    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
////      
////    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
////    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频因子，输入捕获时会用到
////    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式，向下计数
////    TIM_TimeBaseInitStruct.TIM_Period = 1000-1;			//自动装载值
////    TIM_TimeBaseInitStruct.TIM_Prescaler = PCLK2_CLK; //预分频系数，分频后的计数频率CNT_CLK为Timer_CLK/TIM_Prescaler
////    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
////     
////    while((TIM7->SR & TIM_FLAG_Update)!=SET);
////	TIM7->SR = (uint16_t)~TIM_FLAG_Update;
////}

///*******************************************************************************
//* Function Name  : Delay_us
//* Description    : 亚毫秒延时函数，延时范围（1-65536us）
//* Input          : 延时时间
//* Output         : None
//* Return         : None
//*******************************************************************************/
////void Delay_us(unsigned short us_cnt)
////{
////	TIM7->ARR = us_cnt;
////    TIM7->CNT = 0;						//装载值
////    TIM7->CR1 |= TIM_CR1_CEN;    				//使能定时器
////    while((TIM7->SR & TIM_FLAG_Update)!=SET);	//等待更新事件的发生
////    TIM7->SR = (uint16_t)~TIM_FLAG_Update;		//清除事件标志
////    TIM7->CR1 &= ~TIM_CR1_CEN;					//关闭定时器
////}


//void rt_hw_us_delay(rt_uint32_t us)
//{
//    rt_uint32_t delta;
//    /* 获得延时经过的 tick 数 */
//    us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
//    /* 获得当前时间 */
//    delta = SysTick->VAL;
//    /* 循环获得当前时间，直到达到指定的时间后退出循环 */
//    while (delta - SysTick->VAL< us);
//}








