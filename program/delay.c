#include <stdio.h>   //Keil library
#include<intrins.h>
#include "STC8F.h"
//****************************************
//1ms延时子程序
//****************************************

void DelayMs(unsigned int k)  //延时N毫秒，0~65535ms
{
	while(--k)
	{
		unsigned char i, j;

		i = 24;
		j = 85;
		do
		{
			while (--j);
		} 
		while (--i);
	}
}

//****************************************
//1us延时子程序
//****************************************

void Delay1us(u8 k)		//@24.000MHz
{
	do
	{
		unsigned char i;
		i = 6;
		while (--i);
	} while (--k);
}