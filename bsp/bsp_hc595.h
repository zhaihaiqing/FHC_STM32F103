
#ifndef __BSP_HC595_H
#define __BSP_HC595_H


//定义74HC595功能引脚
#define	FHC_HC595_SCK_H()		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET)
#define	FHC_HC595_SCK_L()		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET)

#define	FHC_HC595_RCK_H()		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET)
#define	FHC_HC595_RCK_L()		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET)

#define	FHC_HC595_SER_H()		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET)
#define	FHC_HC595_SER_L()		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)




void HC595SendData(unsigned char dp_we,unsigned char dp_temp,unsigned char dp_time);



#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
