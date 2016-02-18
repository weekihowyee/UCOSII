#include "pbdata.h"

void RCC_Configuration(void);
void GPIO_Configuration(void);

int main(void)
{
   RCC_Configuration();	//系统时钟初始化
   GPIO_Configuration();//端口初始化
   
	while(1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		delay(6000000);//0.5s
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		delay(6000000);//0.5s
	}
}

void RCC_Configuration(void)
{
    SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
}

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

