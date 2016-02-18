#include "pbdata.h"

void KEY_GPIO(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;	
		
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
		GPIO_Init(GPIOC,&GPIO_InitStructure);
}


