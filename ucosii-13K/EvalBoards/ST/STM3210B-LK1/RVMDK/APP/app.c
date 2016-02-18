/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
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
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-LK1 Evaluation Board
*
* Filename      : app.c
* Version       : V1.10
* Programmer(s) : BH3NVN
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
                                

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  OS_STK   App_TaskStartStk[APP_TASK_START_STK_SIZE];
static  OS_STK   App_TaskUartStk[APP_TASK_UART_STK_SIZE];
static  OS_STK   App_TaskLEDStk[APP_TASK_LED_STK_SIZE];
static  OS_STK   App_TaskKbdStk[APP_TASK_KBD_STK_SIZE];

static  OS_EVENT *InfoSem;                      
static  OS_EVENT *Disp_Box;						
/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static  void  App_TaskCreate       (void);
static  void  App_DispScr_SignOn   (void);

static  void  App_TaskStart        (void *p_arg);
static  void  App_TaskUart        (void *p_arg);
static  void  App_TaskKbd          (void *p_arg);
static  void  App_TaskLED 				 (void *p_arg);

/*
static Printf("a")


USART_SendData(USART2,'O');
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != RESET);
*/
/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

int  main (void)
{
    CPU_INT08U  os_err;

    BSP_IntDisAll();                                            /* Disable all ints until we are ready to accept them.  */

    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel".         */

	BSP_Init();                                                 /* Initialize BSP functions.  */

  BSP_IntInit();//初始化各中断入口函数    
	 
	App_DispScr_SignOn();                            
	printf("OS Init OK！\r\n");
    os_err = OSTaskCreate((void (*)(void *)) App_TaskStart,  /* Create the start task.                               */
                          (void          * ) 0,
                          (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                          (INT8U           ) APP_TASK_START_PRIO
                         );
	printf("Creat App_TaskStart！\r\n");
#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif

	InfoSem = OSSemCreate(0); 
	Disp_Box = OSMboxCreate((void*)0);               
	OSTimeSet(0);
    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */

    return (0);
}

/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : The startup task.  The uC/OS-II ticker should only be initialize once multitasking starts.
*
* Argument(s) : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_TaskStart (void *p_arg)
{
    CPU_INT32U  i;
    CPU_INT32U  j;
    CPU_INT32U  dly;


    (void)p_arg;

	OS_CPU_SysTickInit();                                       /* Initialize the SysTick. 		*/

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity.                              */
#endif

	App_TaskCreate();

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
		for (j = 0; j < 4; j++) {
            for (i = 1; i <= 4; i++) {
                BSP_LED_On(i);
                dly = (BSP_ADC_GetStatus(1) >> 4) + 2;
                OSTimeDlyHMSM(0, 0, 0, dly);
                BSP_LED_Off(i);
                dly = (BSP_ADC_GetStatus(1) >> 4) + 2;
                OSTimeDlyHMSM(0, 0, 0, dly);
            }

            for (i = 3; i >= 2; i--) {
                BSP_LED_On(i);
                dly = (BSP_ADC_GetStatus(1) >> 4) + 2;
                OSTimeDlyHMSM(0, 0, 0, dly);
                BSP_LED_Off(i);
                dly = (BSP_ADC_GetStatus(1) >> 4) + 2;
                OSTimeDlyHMSM(0, 0, 0, dly);
            }
        }

        for (i = 0; i < 4; i++) {
            BSP_LED_On(0);
            dly = (BSP_ADC_GetStatus(1) >> 4) + 2;
            OSTimeDlyHMSM(0, 0, 0, dly * 3);
            BSP_LED_Off(0);
            dly = (BSP_ADC_GetStatus(1) >> 4) + 2;
            OSTimeDlyHMSM(0, 0, 0, dly * 3);
        }
    }
}

static  void  App_TaskUart       (void *p_arg)
{
	u8 *ptr,*ptr1;
	u8 i,flag = 0,lenth = 0;
	ptr1 = ptr;
	while(1);
	{		
		do{
			*ptr = USART_ReceiveData(USART2);
			ptr++;
			lenth ++ ;
		  }while(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == 1);
		flag++;
		if(flag == 4)
			{
				for(i = 0;i<lenth;i++)
				{
					ptr = ptr1;
					USART_SendData(USART2,*ptr1);
	      	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != RESET);
	    	}
	    	ptr1 = ptr;
	    	flag = 0;
	    	lenth = 0;
			}
		
	}
	
}



/*
*********************************************************************************************************
*                                            App_TaskCreate()
*
* Description : Create the application tasks.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TaskStart().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_TaskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = OSTaskCreate((void (*)(void *)) App_TaskLED,
                          (void          * ) 0,
                          (OS_STK        * )&App_TaskLEDStk[APP_TASK_LED_STK_SIZE - 1],
                          (INT8U           ) APP_TASK_LED_PRIO
                         );
	printf("Creat App_TaskLED！\r\n");
#if (OS_TASK_NAME_SIZE >= 9)
    OSTaskNameSet(APP_TASK_LED_PRIO, "LED", &os_err);
#endif

os_err = OSTaskCreate((void (*)(void *)) App_TaskUart,
                          (void          * ) 0,
                          (OS_STK        * )&App_TaskUartStk[APP_TASK_UART_STK_SIZE],
                          (INT8U           ) APP_TASK_UART_PRIO
                         );
	printf("Creat App_TaskUart！\r\n");
#if (OS_TASK_NAME_SIZE >= 9)
    OSTaskNameSet(APP_TASK_UART_PRIO, "USART", &os_err);
#endif


    os_err = OSTaskCreate((void (*)(void *)) App_TaskKbd,
                          (void          * ) 0,
                          (OS_STK        * )&App_TaskKbdStk[APP_TASK_KBD_STK_SIZE - 1],
                          (INT8U           ) APP_TASK_KBD_PRIO
                          );
	printf("Creat App_TaskKbd！\r\n");
		
#if (OS_TASK_NAME_SIZE >= 9)
    OSTaskNameSet(APP_TASK_KBD_PRIO, "KeyBoard", &os_err);
#endif

}
/*
*********************************************************************************************************
*                                            App_TaskLCD()
*
* Description : The LCD Task
*
* Argument(s) : p_arg       Argument passed to 'App_TaskLCD()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  App_TaskLED (void *p_arg)
{
	while(1)
		{
	  OSTimeDlyHMSM(0, 0, 0, 800);	
	  BSP_LED_On(1);
	  OSTimeDlyHMSM(0, 0, 0, 800);
	  BSP_LED_Off(1);	 
	  //USART_SendData(USART2, 'O');
   // while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
    }
}

/*
*********************************************************************************************************
*                                            App_TaskKbd()
*
* Description : Monitor the state of the push buttons
*
* Argument(s) : p_arg       Argument passed to 'App_TaskKbd()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_TaskKbd (void *p_arg)
{
    INT8U  b1,b2;
	//INT8U  err;
    (void)p_arg;
	 
    while (DEF_TRUE) {
	    //OSSemPend(InfoSem,0,&err);
        b1 = BSP_PB_GetStatus(BSP_PB_ID_KEY1);
		b2 = BSP_PB_GetStatus(BSP_PB_ID_KEY2);
		if (b1 == 1)	{
		App_DispScr_SignOn();
		} 
		if (b2 == 2)	{
		App_DispScr_SignOn();//OSMboxPost(Disp_Box,(void*)0);
		}
		OSTimeDlyHMSM(0, 0, 0, 20); 
    }
}

/*
*********************************************************************************************************
*                                            App_TaskJoystick()
*
* Description : Monitor the state of the Joystick
*
* Argument(s) : p_arg       Argument passed to 'App_TaskKbd()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          App_DispScr_SignOn()
*
* Description : Display uC/OS-II system information on the USART.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TaskKbd().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_DispScr_SignOn (void)
{


    printf("\r\n  Micrium uC/OS-II  \r\n");
    printf("  ST STM32 (Cortex-M3)\r\n\r\n");

    printf("  uC/OS-II:  V%ld.%ld%ld\r\n",OSVersion()/100,(OSVersion() % 100) / 10,(OSVersion() % 10));
    printf("  TickRate: %ld  \r\n",OS_TICKS_PER_SEC);
    printf("  CPU Usage: %ld%    \r\n",OSCPUUsage);
    printf("  CPU Speed:%ld MHz  \r\n",BSP_CPU_ClkFreq() / 1000000L);
    printf("  #Ticks: %ld  \r\n",OSTime);
    printf("  #CtxSw: %ld  \r\n\r\n",OSCtxSwCtr);

}
/*
*********************************************************************************************************
*********************************************************************************************************
*                                          uC/OS-II APP HOOKS
*********************************************************************************************************
*********************************************************************************************************
*/

#if (OS_APP_HOOKS_EN > 0)
/*
*********************************************************************************************************
*                                      TASK CREATION HOOK (APPLICATION)
*
* Description : This function is called when a task is created.
*
* Argument(s) : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void  App_TaskCreateHook (OS_TCB *ptcb)
{

}

/*
*********************************************************************************************************
*                                    TASK DELETION HOOK (APPLICATION)
*
* Description : This function is called when a task is deleted.
*
* Argument(s) : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void  App_TaskDelHook (OS_TCB *ptcb)
{
    (void)ptcb;
}

/*
*********************************************************************************************************
*                                      IDLE TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskIdleHook(), which is called by the idle task.  This hook
*               has been added to allow you to do such things as STOP the CPU to conserve power.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 251
void  App_TaskIdleHook (void)
{
}
#endif

/*
*********************************************************************************************************
*                                        STATISTIC TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskStatHook(), which is called every second by uC/OS-II's
*               statistics task.  This allows your application to add functionality to the statistics task.
*
* Argument(s) : none.
*********************************************************************************************************
*/

void  App_TaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                        TASK SWITCH HOOK (APPLICATION)
*
* Description : This function is called when a task switch is performed.  This allows you to perform other
*               operations during a context switch.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*
*               (2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                   will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                  task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

#if OS_TASK_SW_HOOK_EN > 0
void  App_TaskSwHook (void)
{

}
#endif

/*
*********************************************************************************************************
*                                     OS_TCBInit() HOOK (APPLICATION)
*
* Description : This function is called by OSTCBInitHook(), which is called by OS_TCBInit() after setting
*               up most of the TCB.
*
* Argument(s) : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 204
void  App_TCBInitHook (OS_TCB *ptcb)
{
    (void)ptcb;
}
#endif

#endif
