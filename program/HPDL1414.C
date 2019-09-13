#include "HPDL1414.h"

//****************************************
// 1414������λѡ
//****************************************
void HPDLbit_select(u8 select)
{
	switch(select)
	{
		case 1://��һλ
		A1=1;
		A0=1;
		break;
		case 2://�ڶ�λ
		A1=1;
		A0=0;
		break;
		case 3://����λ
		A1=0;
		A0=1;
		break;
		case 4://����λ
		A1=0;
		A0=0;
		break;
	}
}


//****************************************
// 1414�������������λ
//****************************************
void HPDLoutput_Data(u8 word)	   				//����һ��8λ16���������ߵ�1λ��ȥ��ʣ�µ������ɸߵ���������D6D5D4D3D2D1D0
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
// 1414�����м����������Ϊ����
//****************************************
void HPDLoutput_Data1(u8 WR,u8 word[4])	   				//����1414������װ����ʾ���ݵ�4λ���飬��ʾ����Ϊ0x20��0x5f��ascii��
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
// 1414�����м����������Ϊ��������
//****************************************
void HPDLoutput_Databit(u8 WR,u8 word1,u8 word2,u8 word3,u8 word4)	   				//����1414������4λ����������ɵ���ʾ���ݣ���ʾ����Ϊ0x20��0x5f��ascii��
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