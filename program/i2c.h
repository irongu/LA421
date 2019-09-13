#ifndef _i2c_h
#define _i2c_h

#include "STC8F.h"
sbit scl=P2^5;
sbit sda=P2^4;

#define SCL_H() scl=1
#define SCL_L() scl=0

#define SDA_H() sda=1
#define SDA_L() sda=0

#define IN_SDA() sda

#define ds_a = 0 IN_SDA

void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(u8 a);
u8 IIC_Write_Byte(u8 dat);
u8 IIC_Read_Byte(void);

#endif
