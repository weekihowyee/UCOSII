#ifndef _pbdata_H
#define _pbdata_H

#include "stm32f10x.h"
#include "includes.h"
#include "misc.h"
#include "led.h"

//���庯��

void delay(u32 nCount);
void Delay_Init(void);
void delay_us(u32 nus);
void delay_ms(u16 nms);	 

#endif
