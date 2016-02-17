#include "pbdata.h"

void LED_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void led1_task(void *pdata)
{
	pdata=pdata;
		while(1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		delay_ms(1000);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		delay_ms(1000);//0.5s
	}
}
