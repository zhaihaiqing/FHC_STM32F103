
#ifndef __BSP_I2C_H
#define __BSP_I2C_H


void MX_I2C1_Init(void);

////I2C ʱ����---PE11
//#define I2C_PORT_SCL_H()		GPIOE->BSRRL = GPIO_Pin_11
//#define I2C_PORT_SCL_L()		GPIOE->BSRRH = GPIO_Pin_11

////I2C ������---PE10
//#define I2C_PORT_SDA_H()		GPIOE->BSRRL = GPIO_Pin_10
//#define I2C_PORT_SDA_L()		GPIOE->BSRRH = GPIO_Pin_10
//#define I2C_PORT_READ_SDA() 	(GPIOE->IDR & GPIO_Pin_10)

//#define	I2C_PORT_SDA_NUM		10

//#define SET_I2C_PORT_SDA_IN()     GPIOB->MODER   &= 0xffcfffff;
//								//GPIOB->MODER   &= ~(3<<(I2C_PORT_SDA_NUM*2));
//								//GPIOB->PUPDR   &= 0xffcfffff;GPIOB->PUPDR   |= 0x00100000
//								//�������빦��
//								//��������
//#define SET_I2C_PORT_SDA_OUT()   {GPIOB->MODER   &= 0xffcfffff;GPIOB->MODER   |= 0x00100000;}

//								//GPIOB->OTYPER  &= 0xffffefff;GPIOB->OTYPER  |= 0x00001000;\
//								//GPIOB->OSPEEDR &= 0xfcffffff;GPIOB->OSPEEDR |= 0x03000000;\
//								//GPIOB->PUPDR   &= 0xfcffffff;GPIOB->PUPDR   |= 0x01000000
//								//����Ϊ�������
//								//�����������Ϊ��©
//								//��������ٶ�Ϊ40MHz
//								//��������
//								

//void i2c_port_init(void);
//void i2c_port_start(void);
//void i2c_port_stop(void);
//unsigned char i2c_port_wait_ack(void);
//void i2c_port_ack(void);
//void i2c_port_nack(void);
//void i2c_port_send_byte(unsigned char txd);
//unsigned char i2c_port_read_byte(unsigned char ack);



#endif


