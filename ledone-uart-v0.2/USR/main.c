#include "pbdata.h"

//ucosII任务堆栈设置
//设置任务优先级
#define START_TASK_PRIO 9
#define LED1_TASK_PRIO 6

//设置任务堆栈大小
#define START_STK_SIZE 64 // 空间大小=64*4(字节)
#define LED1_STK_SIZE  64 // 空间大小=64*4(字节)

//创建任务堆栈空间
OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK LED1_TASK_STK[LED1_STK_SIZE];

void RCC_Configuration(void);
void start_task(void *pdata);

int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,(u8)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

int main(void)
{	
	Delay_Init();
	RCC_Configuration();
	LED_GPIO();
	usart_config();
	/*while(1)
	{
		printf("pr is ok!\n");
		delay_ms(800);
	}*/
	OSInit();//初始化UCOS操作系统

	OSTaskCreate(start_task,//指向任务代码的指针
				(void *)0,//任务开始执行时，传递给任务参数的指针
				(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],//分配给任务堆栈的栈顶指针
				START_TASK_PRIO);//分配给任务的优先级

	OSStart();//启动ucos操作系统
}

void RCC_Configuration(void)
{
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
}

void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;

	pdata=pdata;

	OSStatInit();//初始化统计任务

	OS_ENTER_CRITICAL();//进入临界区，中断无法打断
	OSTaskCreate(led1_task,//指向任务代码的指针
				(void *)0,//任务开始执行时，传递给任务参数的指针
				(OS_STK *)&LED1_TASK_STK[LED1_STK_SIZE-1],//分配给任务堆栈的栈顶指针
				LED1_TASK_PRIO);//分配给任务的优先级
	OSTaskSuspend(START_TASK_PRIO);//挂起开始任务
	OS_EXIT_CRITICAL()//退出临界区 可以被中断打断
}
