#ifndef _HPDL1414_H_
#define _HPDL1414_H_

#include <stdio.h>   //Keil library
//#include "intrins.h"
#include "delay.h"

//****************************************
// 定义IO口
//****************************************
#define D0	PAout(11)							//定义数据对应IO
#define	D1	PAout(12)
#define	D2	PAout(15)
#define	D3	PBout(3)
#define	D4	PDout(1)
#define	D5	PDout(0)
#define	D6	PBout(4)
#define	A0	PAout(2)					//1414位选
#define	A1	PAout(1)
#define	WR1	PBout(7)					//1414片选,0开启1关闭
#define	WR2	PAout(0)
#define	WR3	PAout(3)

//****************************************
// 1414单根管位选
//****************************************
void HPDLbit_select(u8 select);				//传入1到4的数字

//****************************************
// 1414数据最终输出到位
//****************************************
void HPDLoutput_Data(u8 word);				//传入一个8位16进制数，高第1位舍去，剩下的数据由高到低依次是D6D5D4D3D2D1D0


void HPDLoutput_Databit(u8 WR,u8 word1,u8 word2,u8 word3,u8 word4);
//****************************************
// 1414数据输出处理
//****************************************
void HPDLoutput_Data1(u8 WR,u8 word[4]); //传入依次为片选，第一个到第四个字符，直接传入ascll码
#endif