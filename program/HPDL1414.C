#include "HPDL1414.h"

//****************************************
// 1414单根管位选
//****************************************
void HPDLbit_select(u8 select)
{
	switch(select)
	{
		case 1://第一位
		A1=1;
		A0=1;
		break;
		case 2://第二位
		A1=1;
		A0=0;
		break;
		case 3://第三位
		A1=0;
		A0=1;
		break;
		case 4://第四位
		A1=0;
		A0=0;
		break;
	}
}


//****************************************
// 1414数据最终输出到位
//****************************************
void HPDLoutput_Data(u8 word)	   				//传入一个8位16进制数，高第1位舍去，剩下的数据由高到低依次是D6D5D4D3D2D1D0
{
	D3=(word&(0x01<<3));
	D2=(word&(0x01<<2));
	D1=(word&(0x01<<1));
	D0=(word&(0x01<<0));
	D6=(word&(0x01<<6));
	D5=(word&(0x01<<5));
	D4=(word&(0x01<<4));
}

//****************************************
// 1414数据中间输出，传入为数组
//****************************************
void HPDLoutput_Data1(u8 WR,u8 word[4])	   				//传入1414段数和装入显示内容的4位数组，显示内容为0x20到0x5f的ascii码
{
	switch(WR)
	{
		case 1:
		
		HPDLbit_select(1);
		HPDLoutput_Data(word[0]);
		WR1=0;
		Delay1us(1);
		WR1=1;

	
		HPDLbit_select(2);
		HPDLoutput_Data(word[1]);
		WR1=0;
		Delay1us(1);
		WR1=1;
	
		HPDLbit_select(3);
		HPDLoutput_Data(word[2]);
		WR1=0;
		Delay1us(1);
		WR1=1;
	
		HPDLbit_select(4);
		HPDLoutput_Data(word[3]);
		WR1=0;
		Delay1us(1);
		WR1=1;
		
		break;
		
		case 2:
		
		HPDLbit_select(1);
		HPDLoutput_Data(word[0]);
		WR2=0;
		Delay1us(1);
		WR2=1;
			
		HPDLbit_select(2);
		HPDLoutput_Data(word[1]);
		WR2=0;
		Delay1us(1);
		WR2=1;
	
		HPDLbit_select(3);
		HPDLoutput_Data(word[2]);
		WR2=0;
		Delay1us(1);
		WR2=1;
		
		HPDLbit_select(4);
		HPDLoutput_Data(word[3]);
		WR2=0;
		Delay1us(1);
		WR2=1;
		
		break;
	}
	
	// if(WR==0)
	// {
	// HPDLbit_select(1);
	// HPDLoutput_Data(word[0]);
	// WR1=0;
	// Delay1us(1);
	// WR1=1;

	
	// HPDLbit_select(2);
	// HPDLoutput_Data(word[1]);
	// WR1=0;
	// Delay1us(1);
	// WR1=1;
	
	// HPDLbit_select(3);
	// HPDLoutput_Data(word[2]);
	// WR1=0;
	// Delay1us(1);
	// WR1=1;
	
	// HPDLbit_select(4);
	// HPDLoutput_Data(word[3]);
	// WR1=0;
	// Delay1us(1);
	// WR1=1;
	
	// }
	// else if(WR==1)
	// {
	// HPDLbit_select(1);
	// HPDLoutput_Data(word[0]);
	// WR2=0;
	// Delay1us(1);
	// WR2=1;
	
	// HPDLbit_select(2);
	// HPDLoutput_Data(word[1]);
	// WR2=0;
	// Delay1us(1);
	// WR2=1;
	
	// HPDLbit_select(3);
	// HPDLoutput_Data(word[2]);
	// WR2=0;
	// Delay1us(1);
	// WR2=1;
	
	// HPDLbit_select(4);
	// HPDLoutput_Data(word[3]);
	// WR2=0;
	// Delay1us(1);
	// WR2=1;
	
	// }

}

//****************************************
// 1414数据中间输出，传入为独立数据
//****************************************
void HPDLoutput_Databit(u8 WR,u8 word1,u8 word2,u8 word3,u8 word4)	   				//传入1414段数和4位单独的数组成的显示内容，显示内容为0x20到0x5f的ascii码
{
	switch(WR)
	{
		case 1:
		
		HPDLbit_select(1);
		HPDLoutput_Data(word1);
		WR1=0;
		Delay1us(1);
		WR1=1;

	
		HPDLbit_select(2);
		HPDLoutput_Data(word2);
		WR1=0;
		Delay1us(1);
		WR1=1;
	
		HPDLbit_select(3);
		HPDLoutput_Data(word3);
		WR1=0;
		Delay1us(1);
		WR1=1;
	
		HPDLbit_select(4);
		HPDLoutput_Data(word4);
		WR1=0;
		Delay1us(1);
		WR1=1;
		
		break;
		
		case 2:
		
		HPDLbit_select(1);
		HPDLoutput_Data(word1);
		WR2=0;
		Delay1us(1);
		WR2=1;
			
		HPDLbit_select(2);
		HPDLoutput_Data(word2);
		WR2=0;
		Delay1us(1);
		WR2=1;
	
		HPDLbit_select(3);
		HPDLoutput_Data(word3);
		WR2=0;
		Delay1us(1);
		WR2=1;
		
		HPDLbit_select(4);
		HPDLoutput_Data(word4);
		WR2=0;
		Delay1us(1);
		WR2=1;
		
		break;
	}
	
	// if(WR==0)
	// {
	// HPDLbit_select(1);
	// HPDLoutput_Data(word1);
	// WR1=0;
	// Delay1us(1);
	// WR1=1;

	
	// HPDLbit_select(2);
	// HPDLoutput_Data(word2);
	// WR1=0;
	// Delay1us(1);
	// WR1=1;
	
	// HPDLbit_select(3);
	// HPDLoutput_Data(word3);
	// WR1=0;
	// Delay1us(1);
	// WR1=1;
	
	// HPDLbit_select(4);
	// HPDLoutput_Data(word4);
	// WR1=0;				   
	// Delay1us(1);
	// WR1=1;
	
	// }
	// else if(WR==1)
	// {
	// HPDLbit_select(1);
	// HPDLoutput_Data(word1);
	// WR2=0;
	// Delay1us(1);
	// WR2=1;
	
	// HPDLbit_select(2);
	// HPDLoutput_Data(word2);
	// WR2=0;
	// Delay1us(1);
	// WR2=1;
	
	// HPDLbit_select(3);
	// HPDLoutput_Data(word3);
	// WR2=0;
	// Delay1us(1);
	// WR2=1;
	
	// HPDLbit_select(4);
	// HPDLoutput_Data(word4);
	// WR2=0;
	// Delay1us(1);
	// WR2=1;
	
	// }

}