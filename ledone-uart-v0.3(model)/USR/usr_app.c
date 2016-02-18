#include "pbdata.h"

void led1_task(void *pdata)
{
	pdata=pdata;

	while(1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		delay_ms(1000);//1s
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		delay_ms(1000);//1s
	}
}

void led2_task(void *pdata)
{
	pdata=pdata;

	while(1)
	{
		
		if(OSTaskDelReq(OS_PRIO_SELF)==OS_ERR_TASK_DEL_REQ)
		{
				OSTaskDel(OS_PRIO_SELF);
		}
		
		GPIO_SetBits(GPIOD,GPIO_Pin_6);
		//printf("helloos!\n");
		delay_ms(1000);//1s
		GPIO_ResetBits(GPIOD,GPIO_Pin_6);
		delay_ms(1000);//1s
	}
}

void key_task(void *pdata)
{
	
	pdata=pdata;
	while(1)
	{
		
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==Bit_RESET)  //KEY1
		{
			OSTaskSuspend(LED1_TASK_PRIO);
		}
		
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)==Bit_RESET)	//KEY2
		{
			OSTaskResume(LED1_TASK_PRIO);
		}
		
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)==Bit_RESET)	//KEY3
		{
			OSTaskDelReq(LED2_TASK_PRIO);
		}
		
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==Bit_RESET)	//KEY4
		{
			OSTaskCreate(led2_task,//ָ����������ָ��
				(void *)0,//����ʼִ��ʱ�����ݸ����������ָ��
				(OS_STK *)&LED2_TASK_STK[LED2_STK_SIZE-1],//����������ջ��ջ��ָ��
				LED2_TASK_PRIO);//�������������ȼ�
		}
		
		delay_ms(10);
	}
}

