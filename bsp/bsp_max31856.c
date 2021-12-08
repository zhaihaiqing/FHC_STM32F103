
#include "main.h"
#include <math.h>

extern SPI_HandleTypeDef hspi3;


void MAX31865_Init(void)
{
	MX_SPI3_Init();
	
	MAX31865_Write_Reg(MAX31865_REG_CFG,0xD3);
	
	MAX31865_Write_Reg(MAX31865_REG_CFG,0xD3);
}


unsigned char SPI3_ReadWriteByte(unsigned char tx_data)
{
	unsigned char rx_data=0;
	HAL_SPI_TransmitReceive(&hspi3,&tx_data,&rx_data,1, 1000);	//先发送地址
	return rx_data;
}




void MAX31865_Write_Reg(unsigned char addr, unsigned char data)
{
	MAX_CS_L();
	
	SPI3_ReadWriteByte(addr|0x80);	//先发送地址
	
	SPI3_ReadWriteByte(data);	//发送数据
	
	MAX_CS_H();
}

unsigned char MAX31865_Read_Reg(unsigned char addr)
{
	unsigned char rx=0;
	MAX_CS_L();
	
	SPI3_ReadWriteByte(addr);	//先发送地址
	
	rx=SPI3_ReadWriteByte(0xff);	//发送数据
	
	MAX_CS_H();
	return rx;
}


//定义RTD参考电阻值
#define RTD_REF_R	402.0

float Max31865_get_temp(void)
{
	unsigned char msb=0;
	unsigned char lsb=0;
	unsigned short rtd_hex=0;
	float rtd=0.0;
	
	float Rt0 = 100;  //PT100
	float Z1,Z2,Z3,Z4,temp;
	float a = 3.9083e-3;
	float b = -5.775e-7;
	float rpoly;
	
	
	msb=MAX31865_Read_Reg(MAX31865_REG_RTD_MSB);		//log_info("MAX31865_REG_RTD_MSB:0x%x\r\n",msb);
	lsb=MAX31865_Read_Reg(MAX31865_REG_RTD_LSB);		//log_info("MAX31865_REG_RTD_LSB:0x%x\r\n",lsb);
	
	rtd_hex=msb<<8|lsb;
	rtd_hex >>= 1 ;
	
	rtd=rtd_hex*RTD_REF_R/32768.0;
	
	Z1 = -a;
	Z2 = a*a-4*b;
	Z3 = 4*b/Rt0;
	Z4 = 2*b;
	
	temp = Z2+Z3*rtd;
	temp = (sqrt(temp)+Z1)/Z4;
	
	if(temp>=0)
	{
		//log_info("rtd_hex:0x%x,rtd:%f,temp:%f\r\n",rtd_hex,rtd,temp);
		return temp;
	}
	
	rpoly = rtd;
	temp = -242.02;
	temp += 2.2228 * rpoly;
	rpoly *= rtd;  // square
	temp += 2.5859e-3 * rpoly;
	rpoly *= rtd;  // ^3
	temp -= 4.8260e-6 * rpoly;
	rpoly *= rtd;  // ^4
	temp -= 2.8183e-8 * rpoly;
	rpoly *= rtd;  // ^5
	temp += 1.5243e-10 * rpoly;
 
	//log_info("rtd_hex:0x%x,rtd:%f,temp:%f\r\n",rtd_hex,rtd,temp);
	
	return temp;
	
	
	
	
	
	
	
	
	
}

















