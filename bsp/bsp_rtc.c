
#include "main.h"

RTC_HandleTypeDef hrtc;

extern unsigned int test_data;

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

//void MX_RTC_Init(void)
//{

//  /* USER CODE BEGIN RTC_Init 0 */

//  /* USER CODE END RTC_Init 0 */

//  RTC_TimeTypeDef sTime = {0};
//  RTC_DateTypeDef DateToUpdate = {0};

//  /* USER CODE BEGIN RTC_Init 1 */

//  /* USER CODE END RTC_Init 1 */
//  /** Initialize RTC Only 
//  */
//  hrtc.Instance = RTC;
//  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
//  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
//  if (HAL_RTC_Init(&hrtc) != HAL_OK)
//  {
//    Error_Handler();
//  }

//  /* USER CODE BEGIN Check_RTC_BKUP */
//    
//  /* USER CODE END Check_RTC_BKUP */

//  /** Initialize RTC and set the Time and Date 
//  */
//  sTime.Hours = 0x0;
//  sTime.Minutes = 0x0;
//  sTime.Seconds = 0x0;

//  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
//  DateToUpdate.Month = RTC_MONTH_JANUARY;
//  DateToUpdate.Date = 0x1;
//  DateToUpdate.Year = 0x0;

//  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  
//   __HAL_RCC_CLEAR_RESET_FLAGS(); 
//  /* USER CODE BEGIN RTC_Init 2 */

//  /* USER CODE END RTC_Init 2 */

//}



void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc)
{
	test_data++;
}




