/**
  ******************************************************************************
  * @file    Project/BSP/bsp_i2c_port.c 
  * @author  zhaihaiqing
  * @version V1.0.0
  * @date    5-April-2020
  * @brief   使用
  ******************************************************************************
  */
#include "main.h"
//#include "bsp_i2c_port.h"

//void i2c_port_init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOB时钟

//	//GPIOE11,E10初始化设置
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
//	I2C_PORT_SCL_H();
//	I2C_PORT_SDA_H();
//}

////产生IIC起始信号
//void i2c_port_start(void)
//{
//	SET_I2C_PORT_SDA_OUT();     //sda线输出
//	I2C_PORT_SDA_H();	  	  
//	I2C_PORT_SCL_H();
//	rt_hw_us_delay(4);
// 	I2C_PORT_SDA_L();//START:when CLK is high,DATA change form high to low 
//	rt_hw_us_delay(4);
//	I2C_PORT_SCL_L();//钳住I2C总线，准备发送或接收数据 
//}
////产生IIC停止信号
//void i2c_port_stop(void)
//{
//	SET_I2C_PORT_SDA_OUT();//sda线输出
//	I2C_PORT_SCL_L();
//	I2C_PORT_SDA_L();//STOP:when CLK is high DATA change form low to high
// 	rt_hw_us_delay(4);
//	I2C_PORT_SCL_H();
//	I2C_PORT_SDA_H();//发送I2C总线结束信号
//	rt_hw_us_delay(4);							   	
//}
////等待应答信号到来
////返回值：1，接收应答失败
////        0，接收应答成功
//unsigned char i2c_port_wait_ack(void)
//{
//	u8 ucErrTime=0;
//	SET_I2C_PORT_SDA_IN();      //SDA设置为输入  
//	I2C_PORT_SDA_H();rt_hw_us_delay(1);	   
//	I2C_PORT_SCL_H();rt_hw_us_delay(1);	 
//	while(I2C_PORT_READ_SDA())
//	{
//		ucErrTime++;
//		if(ucErrTime>250)
//		{
//			i2c_port_stop();
//			return 1;
//		}
//	}
//	I2C_PORT_SCL_L();//时钟输出0 	   
//	return 0;  
//} 
////产生ACK应答
//void i2c_port_ack(void)
//{
//	I2C_PORT_SCL_L();
//	SET_I2C_PORT_SDA_OUT();
//	I2C_PORT_SDA_L();
//	rt_hw_us_delay(2);
//	I2C_PORT_SCL_H();
//	rt_hw_us_delay(2);
//	I2C_PORT_SCL_L();
//}
////不产生ACK应答		    
//void i2c_port_nack(void)
//{
//	I2C_PORT_SCL_L();
//	SET_I2C_PORT_SDA_OUT();
//	I2C_PORT_SDA_H();
//	rt_hw_us_delay(2);
//	I2C_PORT_SCL_H();
//	rt_hw_us_delay(2);
//	I2C_PORT_SCL_L();
//}					 				     
////IIC发送一个字节
////返回从机有无应答
////1，有应答
////0，无应答			  
//void i2c_port_send_byte(unsigned char txd)
//{                        
//    unsigned char t;   
//	SET_I2C_PORT_SDA_OUT();	    
//    I2C_PORT_SCL_L();//拉低时钟开始数据传输
//    for(t=0;t<8;t++)
//    {              
//        if((txd&0x80)>>7)I2C_PORT_SDA_H();
//		else	I2C_PORT_SDA_L();
//        txd<<=1; 	  
//		rt_hw_us_delay(2);   //对TEA5767这三个延时都是必须的
//		I2C_PORT_SCL_H();
//		rt_hw_us_delay(2); 
//		I2C_PORT_SCL_L();
//		rt_hw_us_delay(2);
//    }	 
//} 	    
////读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
//unsigned char i2c_port_read_byte(unsigned char ack)
//{
//	unsigned char i,receive=0;
//	SET_I2C_PORT_SDA_IN();//SDA设置为输入
//    for(i=0;i<8;i++ )
//	{
//        I2C_PORT_SCL_L(); 
//        rt_hw_us_delay(2);
//		I2C_PORT_SCL_H();
//        receive<<=1;
//        if(I2C_PORT_READ_SDA())receive++;   
//		rt_hw_us_delay(2); 
//    }					 
//    if (!ack)
//        i2c_port_nack();//发送nACK
//    else
//        i2c_port_ack(); //发送ACK   
//    return receive;
//}



I2C_HandleTypeDef hi2c1;

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}
















