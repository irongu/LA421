/*************************************************************************************************************
Vanilla�о���	  ��Ʒ
Vanilla Lab

HPDL8λС��ʱ��
�ͺ�LA421

DS3231���� ��ƬHPDL1414

2018-7-14 V0.2.1.1
��Ȩ���� ��ֹ�����κ���ҵ��;��

ע����������ds3231��������ļ����Ի����������о�����Ȩ������LA421.c,HPDL1414.c,HPDL1414.h
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
// ����IO��
//****************************************
sbit	SW1=P0^2;							//����
sbit	SW2=P0^3;

//extern u8 Time_Buffer[7];
u8 Time_Temp[7]={0x00,0x19,0x02,0x25,0x21,0x24,0x00};
const u8 Daylim[13]={0x00,0x31,0x28,0x31,0x30,0x31,0x30,0x31,0x31,0x30,0x31,0x30,0x31};
u8 DN,minute_last;//�����߱�ţ���һ�����������ʱ�ķ���
u8 bln_cnt=0;//��ʱ��2�����������������ڵ�ʱ��˸
bit TM2_flag=0;//��ʱ��2���������Ƿ�ﵽ�趨
u8 second_last;
bit sec_flag=0;
u8 flash1,flash2;//������˸����
//****************************************
// ��������ֵ
//****************************************

const u32 Divergence_MEM[]={571024,571015,523307,523299,456923,456914,456903,409431,409420,337187,334581,337337,337199,328403,000000,//0-14ΪAlpha��0.
														1130426,1130238,1130205,1129848,1130212,1130211,1130209,1130208,1130206,1130205,1048596,//15-25Ϊbeta
														1048599,1049326,1048728,//26-28λX
														2615074,//29λGamma
														3019430,3030493,3182879,3130238,3372329,3386019,3406288,3600104,3667293,//30-38λDelta
														4456441,4493624,4530805,4456442,4493623,4530806,4389117,//39-45λEpsilon
														275349,195284};//46-47λOmega

//****************************************
// ��������������Ķ�ʱ��
//****************************************														
void Timer1Init()		//2.7����@24.000MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x00;		//���ö�ʱ��ֵ
	TH1 = 0x00;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
}
														
//****************************************
// ���������
//****************************************

void RandomDivergence()//ȡ��ʱ��1��8λ�����ӣ��������������ֵ��ȫ�ֱ���DN
{
	u16 seed;
	Time_Handle();
	seed=(TH1<<8)+TL1;
	srand(seed);
	DN=rand()%47;
	minute_last=(Display_Time[3]<<4)+Display_Time[4];
}
														
//****************************************
// ��ʼ������
//****************************************

void initial()
{
	P0M0 = 0x00;								//����P0.0~P0.7Ϊ��������ģʽ
	P0M1 = 0x00;
	P1M0 = 0x00;								//����P1.0~P1.7Ϊ˫���ģʽ
	P1M1 = 0x00;
	P2M0 = 0x00;								//����P2.0~P2.7Ϊ˫���ģʽ
	P2M1 = 0x00;
	P3M0 = 0x00;								//����P3.0~P3.7Ϊ˫���ģʽ
	P3M1 = 0x00;
	WR1 = 1;
	WR2 = 1;
	SW1=1;
	SW2=1;
}

//****************************************
// ʱ��仯�жϣ�Ϊ��������ֵ�䶯��׼��
//****************************************

void ComparChange()
{
	Time_Handle();
	if(minute_last<Display_Time[0]+1)//�ж��Ƿ��Ѿ�������1����
	{	
		return;
	}
	RandomDivergence();//���Ѿ�����1�������������������
	return;
}
										 
//****************************************
// ʮ����ת��Ϊascii�룬���������±ߵ���ʾ����
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
// �����������ʾ������
//****************************************

void Divergence_change(int DN2)//��Ӧ�����ߵ���ʾ�������������0-47
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
	Bi0=DECtoASCII(Bi0);//��10������ת��Ϊascii��
	Bi1=DECtoASCII(Bi1);
	Bi2=DECtoASCII(Bi2);
	Bi3=DECtoASCII(Bi3);
	Bi4=DECtoASCII(Bi4);
	Bi5=DECtoASCII(Bi5);
	Bi6=DECtoASCII(Bi6);
	
	HPDLoutput_Databit(1,Bi0,'.',Bi1,Bi2);//��ʾ����
	HPDLoutput_Databit(2,Bi3,Bi4,Bi5,Bi6);
}

//****************************************		 
// �����ж�
//****************************************

u8 bothswitch()
{
	u8 returnout=0;
	if(SW1==0||SW2==0)
	{
		DelayMs(100);
		if(SW1==0&&SW2==0)
		{
			returnout=3;//������ͬʱ���£�����3
			//break;
		}
		else if(SW1==0&&SW2==1)
		{
			returnout=1;//����1���£�����1
			//break;
		}
		else if(SW1==1&&SW2==0)
		{
			returnout=2;//����2���£�����2
			//break;
		}
		else if(SW1==1&&SW2==1)
		{
			returnout=0;//�ް������£�����0
		}
		//break;
	}
	return returnout;
}

//****************************************
// ʮ���Ƶ�bcd
//****************************************
void DEC_TO_BCD(u8 *num)
{
	if(((*num)&0x0f)>9)
	(*num)+=6;
}

//****************************************
// ��ʱ
//****************************************
void time_change()
{
	u8 day;
	u8 Change_Time[6]={0x02,0x01,0x02,0x04,0x00,0x00};//ʱ����
	u8 Change_Date[6]={0x01,0x09,0x00,0x02,0x02,0x05};//������
	char count=0;	  
	DelayMs(500);

	if(SW2==0)										//�ٴ��ж��Ƿ�Ϊָ������
	{					
		while(1)											//����ʱ���趨
		{
			Time_Handle();
			
			Change_Time[0]=Display_Time[0];//ʱ
			Change_Time[1]=Display_Time[1];
			Change_Time[2]=Display_Time[3];//��
			Change_Time[3]=Display_Time[4];
			Change_Time[4]=Display_Time[6];//��
			Change_Time[5]=Display_Time[7];
			
			Change_Date[0]=Display_Time[2];//��
			Change_Date[1]=Display_Time[3];
			Change_Date[2]=Display_Time[5];//��
			Change_Date[3]=Display_Time[6];
			Change_Date[4]=Display_Time[8];//��
			Change_Date[5]=Display_Time[9];
			
			if(SW1==0||count==0)					 //��ʼ������
			{
				DelayMs(200);							  //���ڵ�Ƭ��ִ���ٶȽϿ죬��ֹ�Լ������࣬������ʱ	
				count++;
			}
			if(SW2 == 0)	//δ������
			{
				DelayMs(200);
				Time_Temp[count]++;
//				DEC_TO_BCD(&Time_Temp[count],0);
				/************************��Χ�޶�************************************/
					
				if((Time_Temp[count]>0x29)&&count==1)//��
					Time_Temp[count]=0x19;
				// if((Time_Temp[count]>0x07)&&count==2)//��
					// Time_Temp[count]=0x01;
				if((Time_Temp[count]>0x12)&&count==2)//��
					Time_Temp[count]=0x01;
				day=Daylim[Time_Temp[1]];
				if((Time_Temp[count]>day)&&count==3)//��
					Time_Temp[count]=0x01;
				if((Time_Temp[count]>0x23)&&count==4)//ʱ
					Time_Temp[count]=0x00;
				if((Time_Temp[count]>0x59)&&count>4)//��&��
					Time_Temp[count]=0x00;
				
			}
			//��������ʾ��������
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
			}				//���洢�˳�
			if(SW1 == 0 && SW2 == 0)		   //ͬʱ���� �洢�˳�
			{
				DS3231_Time_Init(&TimeValue);
				break;
			}
		}
	}
}

//****************************************
// ������
//****************************************
void main()
 {
	initial();
	Timer1Init();
	if(DS3231_Read_Byte(Address_control) & 0x80)  //EOSC����ֹ����ֹ���в�����
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
	/*if(DS3231_Read_Byte(Address_control_status) & 0x80)  //��������ֹͣ��������ʾ���ʧЧ
	{
		//DS3231_Write_Byte()
		HPDLoutput_Databit(1,'B','A','T',' ');
		HPDLoutput_Databit(2,'F','A','I','L');
		DelayMs(500);
		HPDLoutput_Databit(1,'R','E','P','L');
		HPDLoutput_Databit(2,' ','B','A','T');
		DelayMs(500);
	}*/
	RandomDivergence();							//�ϵ�ʱ������һ�������
	/*
	TimeValue.year=0x19;//����������в��򿪣���Щ��ʼʱ�����Ч	
	TimeValue.month=0x03;
	TimeValue.week=0x07;
	TimeValue.date=0x02;
	TimeValue.hour=0x22;
	TimeValue.minute=0x16;
	TimeValue.second=0x30;
	DS3231_Time_Init(&TimeValue);//�����ã�����ǿ��д��ʱ��ֵ
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
			time_change();//��ʱ
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
 