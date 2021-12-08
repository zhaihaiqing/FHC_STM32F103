
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
//* Description    : Systick��ʱ��
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
////	NVIC_SetPriority(SysTick_IRQn, 0x0);    //SysTick�ж����ȼ�����
////}

///*******************************************************************************
//* Function Name  : Delay_us_Timer_Init
//* Description    : �Ǻ��붨ʱ���ܳ�ʼ��������timer7(������ʱ��)
//* Input          : PCLK2Ƶ�ʣ�MHz��
//* Output         : None
//* Return         : None
//*******************************************************************************/
////void Delay_us_Timer_Init(unsigned char PCLK2_CLK)	
////{
////    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
////	//timerʱ����Դ��Ĭ��Ԥ��Ƶϵ������Ƶ��ļ���Ƶ��ΪTimer_CLKΪPCLK1/PCLK2ʱ��Ƶ�ʵ�2��
////    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
////      
////    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
////    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;		//ʱ�ӷ�Ƶ���ӣ����벶��ʱ���õ�
////    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ�����¼���
////    TIM_TimeBaseInitStruct.TIM_Period = 1000-1;			//�Զ�װ��ֵ
////    TIM_TimeBaseInitStruct.TIM_Prescaler = PCLK2_CLK; //Ԥ��Ƶϵ������Ƶ��ļ���Ƶ��CNT_CLKΪTimer_CLK/TIM_Prescaler
////    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
////     
////    while((TIM7->SR & TIM_FLAG_Update)!=SET);
////	TIM7->SR = (uint16_t)~TIM_FLAG_Update;
////}

///*******************************************************************************
//* Function Name  : Delay_us
//* Description    : �Ǻ�����ʱ��������ʱ��Χ��1-65536us��
//* Input          : ��ʱʱ��
//* Output         : None
//* Return         : None
//*******************************************************************************/
////void Delay_us(unsigned short us_cnt)
////{
////	TIM7->ARR = us_cnt;
////    TIM7->CNT = 0;						//װ��ֵ
////    TIM7->CR1 |= TIM_CR1_CEN;    				//ʹ�ܶ�ʱ��
////    while((TIM7->SR & TIM_FLAG_Update)!=SET);	//�ȴ������¼��ķ���
////    TIM7->SR = (uint16_t)~TIM_FLAG_Update;		//����¼���־
////    TIM7->CR1 &= ~TIM_CR1_CEN;					//�رն�ʱ��
////}


//void rt_hw_us_delay(rt_uint32_t us)
//{
//    rt_uint32_t delta;
//    /* �����ʱ������ tick �� */
//    us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
//    /* ��õ�ǰʱ�� */
//    delta = SysTick->VAL;
//    /* ѭ����õ�ǰʱ�䣬ֱ���ﵽָ����ʱ����˳�ѭ�� */
//    while (delta - SysTick->VAL< us);
//}








