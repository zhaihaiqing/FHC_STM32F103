
#ifndef __BSP_MAX31865_H
#define __BSP_MAX31865_H


#define MAX31865_REG_CFG		0x00
#define MAX31865_REG_RTD_MSB	0x01
#define MAX31865_REG_RTD_LSB	0x02
#define MAX31865_REG_HIGH_MSB	0x03
#define MAX31865_REG_HIGH_LSB	0x04
#define MAX31865_REG_LOW_MSB	0x05
#define MAX31865_REG_LOW_LSB	0x06
#define MAX31865_REG_FS			0x07



void MAX31865_Init(void);
unsigned char SPI3_ReadWriteByte(unsigned char tx);
void MAX31865_Write_Reg(unsigned char addr, unsigned char data);
unsigned char MAX31865_Read_Reg(unsigned char addr);
float Max31865_get_temp(void);













#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
