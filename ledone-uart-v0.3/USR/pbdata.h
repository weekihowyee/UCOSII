#ifndef _pbdata_H
#define _pbdata_H

#include "stm32f10x.h"
#include "usr_app.h"
#include "includes.h"
#include "stm32f10x_usart.h"
#include "stdio.h"
#include "usart.h"
#include "misc.h"
#include "led.h"
#include "key.h"
/********************task config**************************/
//ucosII任务堆栈设置
//设置任务优先级
#define START_TASK_PRIO 9
#define LED1_TASK_PRIO 8
#define LED2_TASK_PRIO 7
#define KEY_TASK_PRIO 6

//设置任务堆栈大小
#define START_STK_SIZE 64 // 空间大小=64*4(字节)
#define LED1_STK_SIZE  64 // 空间大小=64*4(字节)
#define LED2_STK_SIZE  64
#define KEY_STK_SIZE  64

//创建任务堆栈空间
static OS_STK START_TASK_STK[START_STK_SIZE];
static OS_STK LED1_TASK_STK[LED1_STK_SIZE];
static OS_STK LED2_TASK_STK[LED2_STK_SIZE];
static OS_STK KEY_TASK_STK[KEY_STK_SIZE];

/**********************************************************/

//定义函数

void delay(u32 nCount);
void Delay_Init(void);
void delay_us(u32 nus);
void delay_ms(u16 nms);	 

#endif
