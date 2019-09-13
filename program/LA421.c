/*************************************************************************************************************
Vanilla研究所	  出品
Vanilla Lab

HPDL8位小型时钟
型号LA421

DS3231方案 两片HPDL1414

2018-7-14 V0.2.1.1
版权所有 禁止用于任何商业用途！

注：本程序中ds3231相关驱动文件来自互联网，本研究所版权仅限于LA421.c,HPDL1414.c,HPDL1414.h
*************************************************************************************************************/

#include "STC8F.h"
#include <stdio.h>   //Keil library
#include<intrins.h>
#include "HPDL1414.h"
#include "delay.h"
#include"ds3231.h"
#include "stdlib.h"

#define u8 unsigned char
#define u16  unsigned int
#define u32  unsigned long int

//****************************************
// 定义IO口
//****************************************
sbit	SW1=P0^2;							//按键
sbit	SW2=P0^3;

//extern u8 Time_Buffer[7];
u8 Time_Temp[7]={0x00,0x19,0x02,0x25,0x21,0x24,0x00};
const u8 Daylim[13]={0x00,0x31,0x28,0x31,0x30,0x31,0x30,0x31,0x31,0x30,0x31,0x30,0x31};
u8 DN,minute_last;//世界线编号，上一次生成随机数时的分钟
u8 bln_cnt=0;//定时器2触发次数计数，用于调时闪烁
bit TM2_flag=0;//定时器2触发次数是否达到设定
u8 second_last;
bit sec_flag=0;
u8 flash1,flash2;//用于闪烁缓存
//****************************************
// 世界线数值
//****************************************

const u32 Divergence_MEM[]={571024,571015,523307,523299,456923,456914,456903,409431,409420,337187,334581,337337,337199,328403,000000,//0-14为Alpha补0.
														1130426,1130238,1130205,1129848,1130212,1130211,1130209,1130208,1130206,1130205,1048596,//15-25为beta
														1048599,1049326,1048728,//26-28位X
														2615074,//29位Gamma
														3019430,3030493,3182879,3130238,3372329,3386019,3406288,3600104,3667293,//30-38位Delta
														4456441,4493624,4530805,4456442,4493623,4530806,4389117,//39-45位Epsilon
														275349,195284};//46-47位Omega

//****************************************
// 用于生成随机数的定时器
//****************************************														
void Timer1Init()		//2.7毫秒@24.000MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x00;		//设置定时初值
	TH1 = 0x00;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
}
														
//****************************************
// 随机数生成
//****************************************

void RandomDivergence()//取定时器1低8位做种子，生成随机数，赋值给全局变量DN
{
	u16 seed;
	Time_Handle();
	seed=(TH1<<8)+TL1;
	srand(seed);
	DN=rand()%47;
	minute_last=(Display_Time[3]<<4)+Display_Time[4];
}
														
//****************************************
// 初始化程序
//****************************************

void initial()
{
	P0M0 = 0x00;								//设置P0.0~P0.7为高阻输入模式
	P0M1 = 0x00;
	P1M0 = 0x00;								//设置P1.0~P1.7为双向口模式
	P1M1 = 0x00;
	P2M0 = 0x00;								//设置P2.0~P2.7为双向口模式
	P2M1 = 0x00;
	P3M0 = 0x00;								//设置P3.0~P3.7为双向口模式
	P3M1 = 0x00;
	WR1 = 1;
	WR2 = 1;
	SW1=1;
	SW2=1;
}

//****************************************
// 时间变化判断，为世界线数值变动做准备
//****************************************

void ComparChange()
{
	Time_Handle();
	if(minute_last<Display_Time[0]+1)//判断是否已经经过了1分钟
	{	
		return;
	}
	RandomDivergence();//若已经超过1分钟则重新生成随机数
	return;
}
										 
//****************************************
// 十进制转换为ascii码，仅适用于下边的显示函数
//****************************************

u8 DECtoASCII(u8 DEC)
{
	u8 ASCII2;
	if(DEC==10)
	{
		ASCII2=0x20;
	}
	else
	{
		ASCII2=0x30+DEC;
	}
	return ASCII2;
}

//****************************************
// 根据随机数显示世界线
//****************************************

void Divergence_change(int DN2)//对应世界线的显示函数，输入参数0-47
{
	int Bi0,Bi1,Bi2,Bi3,Bi4,Bi5,Bi6;
	if(DN2<=14)
	{
	Bi0=0;
	Bi1=Divergence_MEM[DN2]/100000;
	Bi2=Divergence_MEM[DN2]%100000/10000;
	Bi3=Divergence_MEM[DN2]%10000/1000;
	Bi4=Divergence_MEM[DN2]%1000/100;
	Bi5=Divergence_MEM[DN2]%100/10;
	Bi6=Divergence_MEM[DN2]%10;
	}
	else if(DN2<=45)
	{
	Bi0=Divergence_MEM[DN2]/1000000;
	Bi1=Divergence_MEM[DN2]%1000000/100000;
	Bi2=Divergence_MEM[DN2]%100000/10000;
	Bi3=Divergence_MEM[DN2]%10000/1000;
	Bi4=Divergence_MEM[DN2]%1000/100;
	Bi5=Divergence_MEM[DN2]%100/10;
	Bi6=Divergence_MEM[DN2]%10;
	}
	else if(DN2>45)
	{
	Bi0=10;
	Bi1=Divergence_MEM[DN2]/100000;
	Bi2=Divergence_MEM[DN2]%100000/10000;
	Bi3=Divergence_MEM[DN2]%10000/1000;
	Bi4=Divergence_MEM[DN2]%1000/100;
	Bi5=Divergence_MEM[DN2]%100/10;
	Bi6=Divergence_MEM[DN2]%10;
	}
	
	//glow_tube_drv(Bi0,10,Bi1,Bi2,Bi3,Bi4,Bi5,Bi6);
	Bi0=DECtoASCII(Bi0);//将10进制数转换为ascii码
	Bi1=DECtoASCII(Bi1);
	Bi2=DECtoASCII(Bi2);
	Bi3=DECtoASCII(Bi3);
	Bi4=DECtoASCII(Bi4);
	Bi5=DECtoASCII(Bi5);
	Bi6=DECtoASCII(Bi6);
	
	HPDLoutput_Databit(1,Bi0,'.',Bi1,Bi2);//显示部分
	HPDLoutput_Databit(2,Bi3,Bi4,Bi5,Bi6);
}

//****************************************		 
// 按键判断
//****************************************

u8 bothswitch()
{
	u8 returnout=0;
	if(SW1==0||SW2==0)
	{
		DelayMs(100);
		if(SW1==0&&SW2==0)
		{
			returnout=3;//两按键同时按下，返回3
			//break;
		}
		else if(SW1==0&&SW2==1)
		{
			returnout=1;//按键1按下，返回1
			//break;
		}
		else if(SW1==1&&SW2==0)
		{
			returnout=2;//按键2按下，返回2
			//break;
		}
		else if(SW1==1&&SW2==1)
		{
			returnout=0;//无按键按下，返回0
		}
		//break;
	}
	return returnout;
}

//****************************************
// 十进制到bcd
//****************************************
void DEC_TO_BCD(u8 *num)
{
	if(((*num)&0x0f)>9)
	(*num)+=6;
}

//****************************************
// 调时
//****************************************
void time_change()
{
	u8 day;
	u8 Change_Time[6]={0x02,0x01,0x02,0x04,0x00,0x00};//时分秒
	u8 Change_Date[6]={0x01,0x09,0x00,0x02,0x02,0x05};//年月日
	char count=0;	  
	DelayMs(500);

	if(SW2==0)										//再次判断是否为指定按键
	{					
		while(1)											//进入时间设定
		{
			Time_Handle();
			
			Change_Time[0]=Display_Time[0];//时
			Change_Time[1]=Display_Time[1];
			Change_Time[2]=Display_Time[3];//分
			Change_Time[3]=Display_Time[4];
			Change_Time[4]=Display_Time[6];//秒
			Change_Time[5]=Display_Time[7];
			
			Change_Date[0]=Display_Time[2];//年
			Change_Date[1]=Display_Time[3];
			Change_Date[2]=Display_Time[5];//月
			Change_Date[3]=Display_Time[6];
			Change_Date[4]=Display_Time[8];//日
			Change_Date[5]=Display_Time[9];
			
			if(SW1==0||count==0)					 //初始设置年
			{
				DelayMs(200);							  //由于单片机执行速度较快，防止自加数过多，加入延时	
				count++;
			}
			if(SW2 == 0)	//未做消抖
			{
				DelayMs(200);
				Time_Temp[count]++;
//				DEC_TO_BCD(&Time_Temp[count],0);
				/************************范围限定************************************/
					
				if((Time_Temp[count]>0x29)&&count==1)//年
					Time_Temp[count]=0x19;
				// if((Time_Temp[count]>0x07)&&count==2)//周
					// Time_Temp[count]=0x01;
				if((Time_Temp[count]>0x12)&&count==2)//月
					Time_Temp[count]=0x01;
				day=Daylim[Time_Temp[1]];
				if((Time_Temp[count]>day)&&count==3)//日
					Time_Temp[count]=0x01;
				if((Time_Temp[count]>0x23)&&count==4)//时
					Time_Temp[count]=0x00;
				if((Time_Temp[count]>0x59)&&count>4)//分&秒
					Time_Temp[count]=0x00;
				
			}
			//添加你的显示函数在这
			if(count==1)
			{
				Change_Date[0]=Time_Temp[count]>>4;
				Change_Date[1]=Time_Temp[count];
				HPDLoutput_Databit(1,'2','0',flash1,flash2);
				HPDLoutput_Databit(2,Change_Date[2],Change_Date[3],Change_Date[4],Change_Date[5]);
			}
			if(count==2)
			{
				Change_Date[2]=Time_Temp[count]>>4;
				Change_Date[3]=Time_Temp[count];
				HPDLoutput_Databit(1,'2','0',Change_Date[0],Change_Date[1]);
				HPDLoutput_Databit(2,flash1,flash2,Change_Date[4],Change_Date[5]);
			}
			if(count==3)
			{
				Change_Date[4]=Time_Temp[count]>>4;
				Change_Date[5]=Time_Temp[count];
				HPDLoutput_Databit(1,'2','0',Change_Date[0],Change_Date[1]);
				HPDLoutput_Databit(2,Change_Date[2],Change_Date[3],flash1,flash2);
			}
			if(count==4)
			{
				Change_Time[0]=Time_Temp[count]>>4;
				Change_Time[1]=Time_Temp[count];
				HPDLoutput_Databit(1,flash1,flash2,':',Change_Time[2]);
				HPDLoutput_Databit(2,Change_Time[3],':',Change_Time[4],Change_Time[5]);
			}
			if(count==5)
			{
				Change_Time[2]=Time_Temp[count]>>4;
				Change_Time[3]=Time_Temp[count];
				HPDLoutput_Databit(1,Change_Time[0],Change_Time[1],':',flash1);
				HPDLoutput_Databit(2,flash2,':',Change_Time[4],Change_Time[5]);
			}
			if(count==6)
			{
				Change_Time[4]=Time_Temp[count]>>4;
				Change_Time[5]=Time_Temp[count];
				HPDLoutput_Databit(1,Change_Time[0],Change_Time[1],':',Change_Time[2]);
				HPDLoutput_Databit(2,Change_Time[3],':',flash1,flash2);
			}

			if(count==7)
			{
				break;	
			}				//不存储退出
			if(SW1 == 0 && SW2 == 0)		   //同时按下 存储退出
			{
				DS3231_Time_Init(&TimeValue);
				break;
			}
		}
	}
}

//****************************************
// 主程序
//****************************************
void main()
 {
	initial();
	Timer1Init();
	if(DS3231_Read_Byte(Address_control) & 0x80)  //EOSC被禁止，终止运行并报错
	{
		while(1)
		{
			HPDLoutput_Databit(1,'C','L','K',' ');
			HPDLoutput_Databit(2,'E','R','R',' ');
			DelayMs(300);
			HPDLoutput_Databit(1,'C','H','K',' ');
			HPDLoutput_Databit(2,'3','2','3','1');
			DelayMs(300);
		}
	}			
	/*if(DS3231_Read_Byte(Address_control_status) & 0x80)  //晶振曾经停止工作，提示电池失效
	{
		//DS3231_Write_Byte()
		HPDLoutput_Databit(1,'B','A','T',' ');
		HPDLoutput_Databit(2,'F','A','I','L');
		DelayMs(500);
		HPDLoutput_Databit(1,'R','E','P','L');
		HPDLoutput_Databit(2,' ','B','A','T');
		DelayMs(500);
	}*/
	RandomDivergence();							//上电时先生成一次随机数
	/*
	TimeValue.year=0x19;//如果下面那行不打开，这些初始时间均无效	
	TimeValue.month=0x03;
	TimeValue.week=0x07;
	TimeValue.date=0x02;
	TimeValue.hour=0x22;
	TimeValue.minute=0x16;
	TimeValue.second=0x30;
	DS3231_Time_Init(&TimeValue);//调试用，程序强制写入时间值
	*/
	
	while(1)
	{
		Time_Handle();
		if(sec_flag==1)
		{
			HPDLoutput_Databit(1,Display_Time[0],Display_Time[1],Display_Time[2],Display_Time[3]);
			HPDLoutput_Databit(2,Display_Time[4],Display_Time[5],Display_Time[6],Display_Time[7]);
		}
		if(bothswitch()==1)
		{
			HPDLoutput_Databit(1,Display_Date[0],Display_Date[1],Display_Date[2],'9');
			HPDLoutput_Databit(2,Display_Date[5],Display_Date[6],Display_Date[8],Display_Date[9]);
			while(SW1==0);
		}
		if(bothswitch()==2)
		{
			time_change();//调时
		}
		if(bothswitch()==3)
		{
			ComparChange();
			Divergence_change(DN);
			DelayMs(300);
			while(SW1==0||SW2==0);
			DelayMs(1000);
		}		
	}
 }
 