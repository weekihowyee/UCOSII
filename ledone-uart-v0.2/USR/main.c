#include "pbdata.h"

//ucosII�����ջ����
//�����������ȼ�
#define START_TASK_PRIO 9
#define LED1_TASK_PRIO 6

//���������ջ��С
#define START_STK_SIZE 64 // �ռ��С=64*4(�ֽ�)
#define LED1_STK_SIZE  64 // �ռ��С=64*4(�ֽ�)

//���������ջ�ռ�
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
	while(1)
	{
		printf("pr is ok!\n");
		delay_ms(800);
	}
	/*OSInit();//��ʼ��UCOS����ϵͳ

	OSTaskCreate(start_task,//ָ����������ָ��
				(void *)0,//����ʼִ��ʱ�����ݸ����������ָ��
				(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],//����������ջ��ջ��ָ��
				START_TASK_PRIO);//�������������ȼ�

	OSStart();//����ucos����ϵͳ*/
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

	OSStatInit();//��ʼ��ͳ������

	OS_ENTER_CRITICAL();//�����ٽ������ж��޷����
	OSTaskCreate(led1_task,//ָ����������ָ��
				(void *)0,//����ʼִ��ʱ�����ݸ����������ָ��
				(OS_STK *)&LED1_TASK_STK[LED1_STK_SIZE-1],//����������ջ��ջ��ָ��
				LED1_TASK_PRIO);//�������������ȼ�
	OSTaskSuspend(START_TASK_PRIO);//����ʼ����
	OS_EXIT_CRITICAL()//�˳��ٽ��� ���Ա��жϴ��
}
