#include "pbdata.h"

#define START_TASK_PRIO 9

#define LED1_TASK_PRIO 6

#define START_STK_SIZE 64
#define LED1_STK_SIZE 64

OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK LED1_TASK_STK[LED1_STK_SIZE];

void RCC_Configuration(void);
void start_task(void *pdata);


int main(void)
{	
	Delay_Init();
	RCC_Configuration();
	LED_GPIO();
	
	OSInit();
	
	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE],START_TASK_PRIO);
	
	OSStart();
	/**/
}

void RCC_Configuration(void)
{
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
}

void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	
	OSStatInit();
	
	OS_ENTER_CRITICAL();
	
	OSTaskCreate(led1_task,(void *)0,(OS_STK *)&LED1_TASK_STK[LED1_STK_SIZE],LED1_TASK_PRIO);
	
	OSTaskSuspend(START_TASK_PRIO);
	
	OS_EXIT_CRITICAL();
	
}
