#include "pbdata.h"

void RCC_Configuration(void);

int main(void)
{	
	Delay_Init();
	RCC_Configuration();

	while(1);
}

void RCC_Configuration(void)
{
	SystemInit();
}
