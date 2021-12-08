
#include "main.h"



/**********************************************************************************
* Function Name  : HC595SendData
* 一次性向HC595中写入24个数据
*TEMP_tmp、Chansel_tmp、FCTRL_tmp：用bit7,bit6,bit5,bit4,bit3,bit2,bit1,bit0
* nTime:时间值(ms)
**********************************************************************************/
void HC595SendData(unsigned char dp_we,unsigned char dp_temp,unsigned char dp_time)
{
	unsigned char i;
	unsigned int Val;
	
	Val=((dp_time&0x000000ff)<<24)|((dp_temp&0x000000ff)<<16)|((dp_we&0x000000ff)<<8);
	for(i=0;i<24;i++)
	{
		if((Val<<i)&0x80000000) FHC_HC595_SER_H();
		else FHC_HC595_SER_L();
		FHC_HC595_SCK_L();
		__nop();
		__nop();
		FHC_HC595_SCK_H();
	}
	FHC_HC595_RCK_L();
	__nop();
	__nop();
	FHC_HC595_RCK_H();
	__nop();
	__nop();
	FHC_HC595_SCK_L();
	FHC_HC595_RCK_L();
}


















