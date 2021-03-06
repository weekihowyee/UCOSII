

#include <includes.h>

static  OS_STK   App_TaskStartStk[APP_TASK_START_STK_SIZE];
//static  OS_STK   App_TaskUartStk[APP_TASK_UART_STK_SIZE];
static  OS_STK   App_TaskLEDStk[APP_TASK_LED_STK_SIZE];
//static  OS_STK   App_TaskKbdStk[APP_TASK_KBD_STK_SIZE];

//static  OS_EVENT *InfoSem;                      
//static  OS_EVENT *Disp_Box;			

static App_TaskStart(void *p_arg);

static App_TaskLED(void *p_arg);

/*int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,(u8)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}*/

int  main (void)
{
    CPU_INT08U  os_err;

    BSP_IntDisAll();                                            /* Disable all ints until we are ready to accept them.  */

    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel".         */

	BSP_Init();                                                 /* Initialize BSP functions.  */

  BSP_IntInit();//初始化各中断入口函数
	
 os_err = OSTaskCreate((void (*)(void *)) App_TaskStart,  /* Create the start task.                               */
                          (void          * ) 0,
                          (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                          (INT8U           ) APP_TASK_START_PRIO
                         );
                         
		OSTimeSet(0);
	  OSStart();           
	 return (0);
	}

static App_TaskStart(void *p_arg)
{
	 CPU_INT08U  os_err;

	OS_CPU_SysTickInit();                                       /* Initialize the SysTick. 		*/

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity.                              */
#endif


	os_err = OSTaskCreate((void (*)(void *)) App_TaskLED,
                          (void          * ) 0,
                          (OS_STK        * )&App_TaskLEDStk[APP_TASK_LED_STK_SIZE - 1],
                          (INT8U           ) APP_TASK_LED_PRIO
                         );
     OSTaskSuspend(APP_TASK_START_PRIO); 
/*while(1)
		{
	   OSTimeDlyHMSM(0, 0, 0, 800);	
	   BSP_LED_On(1);
	   OSTimeDlyHMSM(0, 0, 0, 800);
	   BSP_LED_Off(1);	 
    }*/
		
}

static App_TaskLED(void *p_arg)
{
	CPU_INT08U  os_err;

	OS_CPU_SysTickInit();                                       /* Initialize the SysTick. 		*/

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity.                              */
#endif
		 BSP_LED_Off(2);
		while(1)
		{
	   OSTimeDlyHMSM(0, 0, 0, 800);	
	   BSP_LED_On(1);
			printf("helloos\n");
	   OSTimeDlyHMSM(0, 0, 0, 800);
	   BSP_LED_Off(1);	 
    }
}

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














