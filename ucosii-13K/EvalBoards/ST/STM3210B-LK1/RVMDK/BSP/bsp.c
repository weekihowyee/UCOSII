/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-LK1 Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.10
* Programmer(s) : BH3NVN
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_MODULE
#include <bsp.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
                                                                
#define  GPIO_RxPin                              GPIO_Pin_10
#define  GPIO_TxPin                              GPIO_Pin_9

#define  GPIO_UART2RxPin                              GPIO_Pin_3
#define  GPIO_UART2TxPin                              GPIO_Pin_2


#define  BSP_GPIOC_LED1                           DEF_BIT_08
#define  BSP_GPIOC_LED2                           DEF_BIT_09


#define  BSP_GPIOA_PB_KEY1                        DEF_BIT_00
#define  BSP_GPIOB_PB_KEY2                        DEF_BIT_09

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  BSP_ADC_Init     (void);

static  void  BSP_LED_Init     (void);

static  void  BSP_PB_Init      (void);

static  void GPIO_Configuration(void);

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/*******************************************************************************
* Function Name  : PUTCHAR_PROTOTYPE
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
PUTCHAR_PROTOTYPE
{
  /* Write a character to the USART */
  USART_SendData(USART1, (u8) ch);

  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {
  }
    return ch;
}

void NVIC_Configuration(void)
{
 NVIC_InitTypeDef NVIC_InitStructure; 

  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   

  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure); 

  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
  USART_ClearITPendingBit(USART2,USART_IT_RXNE); 
}



void UART2_int(void)
{
	  USART_InitTypeDef USART_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;

  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /* Configure USARTx_Tx as alternate function push-pull */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_UART2TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USARTx_Rx as input floating */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_UART2RxPin; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
	USART_Init(USART2,&USART_InitStructure); 
	USART_Cmd(USART2, ENABLE);  
	NVIC_Configuration();	
	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	

	
}
/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
  USART_InitTypeDef USART_InitStructure;
  /* USARTx configuration ------------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    RCC_WaitForHSEStartUp();

    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLK2Config(RCC_HCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    FLASH_SetLatency(FLASH_Latency_2);
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);

    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
        ;
    }

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    while (RCC_GetSYSCLKSource() != 0x08) {
        ;
    }


/*
SCU_AHBPeriphClockConfig(__VIC,ENABLE);

//VIC_DeInit();
VIC_Config(UART1_ITLine, VIC_IRQ, 15);
VIC_ITCmd(UART1_ITLine, ENABLE);
*/


    BSP_ADC_Init();                                             /* Initialize the I/Os for the ADC      controls.       */
    BSP_LED_Init();                                             /* Initialize the I/Os for the LED      controls.       */
	BSP_PB_Init();                                              		/* Initialize the I/Os for the PB       control.        */
   // BSP_Joystick_Init();                                        /* Initialize the I/Os for the Joystick control. 		 */
	GPIO_Configuration();
	USART_Init(USART1,&USART_InitStructure); 
	USART_Cmd(USART1, ENABLE);   
	UART2_int();
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure USARTx_Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USARTx_Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

}

/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/


CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;


    RCC_GetClocksFreq(&rcc_clocks);

    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                         OS CORTEX-M3 FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         OS_CPU_SysTickClkFreq()
*
* Description : Get system tick clock frequency.
*
* Argument(s) : none.
*
* Return(s)   : Clock frequency (of system tick).
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

INT32U  OS_CPU_SysTickClkFreq (void)
{
    INT32U  freq;


    freq = BSP_CPU_ClkFreq();
    return (freq);
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                              ADC FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             BSP_ADC_Init()
*
* Description : Initialize the board's ADC
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_ADC_Init (void)
{
    ADC_InitTypeDef   adc_init;
    GPIO_InitTypeDef  gpio_init;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_StructInit(&gpio_init);
    gpio_init.GPIO_Pin  = GPIO_Pin_0;
    gpio_init.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &gpio_init);

    adc_init.ADC_Mode               = ADC_Mode_Independent;
    adc_init.ADC_ScanConvMode       = DISABLE;
    adc_init.ADC_ContinuousConvMode = ENABLE;
    adc_init.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
    adc_init.ADC_DataAlign          = ADC_DataAlign_Right;
    adc_init.ADC_NbrOfChannel       = 1;
    ADC_Init(ADC1, &adc_init);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_13Cycles5);
    ADC_Cmd(ADC1, ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


/*
*********************************************************************************************************
*                                           BSP_ADC_GetStatus()
*
* Description : This function initializes the board's ADC
*
* Argument(s) : adc             ID of the ADC to probe.  For this board, the only legitimate value is 1.
*
* Return(s)   : The numerator of the binary fraction representing the result of the latest ADC conversion.
*               This value will be a 12-bit value between 0x0000 and 0x0FFF, inclusive.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT16U  BSP_ADC_GetStatus (CPU_INT08U  adc)
{
    CPU_INT16U  result;


    result = 0;

    if (adc == 1) {
        result = ADC_GetConversionValue(ADC1);
    }

    return (result);
}
/*
*********************************************************************************************************
*********************************************************************************************************
*                                               PB FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              BSP_PB_Init()
*
* Description : Initialize the board's PB.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
//key
static  void  BSP_PB_Init (void)
{
    GPIO_InitTypeDef  gpio_init;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_StructInit(&gpio_init);
    gpio_init.GPIO_Pin  = BSP_GPIOA_PB_KEY1;
    gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &gpio_init);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_StructInit(&gpio_init);
    gpio_init.GPIO_Pin  = BSP_GPIOB_PB_KEY2;
    gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &gpio_init);

}

/*
*********************************************************************************************************
*                                           BSP_PB_GetStatus()
*
* Description : Get the status of a push button on the board.
*
* Argument(s) : pb      The ID of the push button to probe
*
*                       1    probe the user push button
*
* Return(s)   : DEF_FALSE   if the push button is pressed.
*               DEF_TRUE    if the push button is not pressed.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_PB_GetStatus (CPU_INT08U pb)
{
    CPU_BOOLEAN  status;
    CPU_INT32U   pin;

    status = DEF_FALSE;

    switch (pb) {
        case BSP_PB_ID_KEY1:
             pin = GPIO_ReadInputDataBit(GPIOA, BSP_GPIOA_PB_KEY1);
             if (pin == 0) {
                 status = 1;
             }
             break;

        case BSP_PB_ID_KEY2:
             pin = GPIO_ReadInputDataBit(GPIOB, BSP_GPIOB_PB_KEY2);
             if (pin == 0) {
                 status = 2;
             }
             break;

        default:
             break;
    }

    return (status);
}




/*
*********************************************************************************************************
*********************************************************************************************************
*                                              LED FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             BSP_LED_Init()
*
* Description : Initialize the I/O for the LEDs
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_LED_Init (void)
{
    GPIO_InitTypeDef  gpio_init;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    gpio_init.GPIO_Pin   = BSP_GPIOC_LED1 | BSP_GPIOC_LED2 ;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &gpio_init);
}

/*
*********************************************************************************************************
*                                             BSP_LED_On()
*
* Description : Turn ON any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turn ON all LEDs on the board
*                       1    turn ON LED 1
*                       2    turn ON LED 2
*                       3    turn ON LED 3
*                       4    turn ON LED 4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_On (CPU_INT08U led)
{
    switch (led) {
        case 0:
             GPIO_SetBits(GPIOB, BSP_GPIOC_LED1 | BSP_GPIOC_LED2 );
             break;

        case 1:
             GPIO_SetBits(GPIOB, BSP_GPIOC_LED1);
             break;

        case 2:
             GPIO_SetBits(GPIOB, BSP_GPIOC_LED2);
             break;

        default:
             break;
    }
}

/*
*********************************************************************************************************
*                                              BSP_LED_Off()
*
* Description : Turn OFF any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turn OFF all LEDs on the board
*                       1    turn OFF LED 1
*                       2    turn OFF LED 2
*                       3    turn OFF LED 3
*                       4    turn OFF LED 4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Off (CPU_INT08U led)
{
    switch (led) {
        case 0:
             GPIO_ResetBits(GPIOB, BSP_GPIOC_LED1 | BSP_GPIOC_LED2 );
             break;

        case 1:
             GPIO_ResetBits(GPIOB, BSP_GPIOC_LED1);
             break;

        case 2:
             GPIO_ResetBits(GPIOB, BSP_GPIOC_LED2);
             break;
        default:
             break;
    }
}

/*
*********************************************************************************************************
*                                            BSP_LED_Toggle()
*
* Description : TOGGLE any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    TOGGLE all LEDs on the board
*                       1    TOGGLE LED 1
*                       2    TOGGLE LED 2
*                       3    TOGGLE LED 3
*                       4    TOGGLE LED 4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Toggle (CPU_INT08U led)
{
    CPU_INT32U  pins;


    pins = GPIO_ReadOutputData(GPIOB);

    switch (led) {
        case 0:
             BSP_LED_Toggle(1);
             BSP_LED_Toggle(2);       
             break;

        case 1:
             if ((pins & BSP_GPIOC_LED1) == 0) {
                 GPIO_SetBits(  GPIOB, BSP_GPIOC_LED1);
             } else {
                 GPIO_ResetBits(GPIOB, BSP_GPIOC_LED1);
             }
            break;

        case 2:
             if ((pins & BSP_GPIOC_LED2) == 0) {
                 GPIO_SetBits(  GPIOB, BSP_GPIOC_LED2);
             } else {
                 GPIO_ResetBits(GPIOB, BSP_GPIOC_LED2);
             }
            break;
        default:
             break;
    }
}

