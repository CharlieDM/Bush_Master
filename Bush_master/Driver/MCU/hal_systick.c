
#include "stm32f10x.h"
#include "hal_systick.h"
#include "core_hal.h"
#include "debug.h"

#define Task_Num			(20)

typedef struct _TaskStruct
{
	uint8 Run;
	uint8 Timer;
	uint8 LvTimer;
	void (*FuncHander)();
}TaskStruct;

void Func(){};

static uint8 ucTaskPos = 0;	
static uint32 ulInc = 0;
static TaskStruct stTaskComps[Task_Num] = {{0,100,100,Func}};

static void RegisterSystick(uint8 timer, void(*Func)())
{
	CHECK_IF_RET_VOID(ucTaskPos>Task_Num-1, "Invalid Pos!!\n");
	
	stTaskComps[ucTaskPos].Run = 0;
	stTaskComps[ucTaskPos].Timer = timer;
	stTaskComps[ucTaskPos].LvTimer = timer;
	stTaskComps[ucTaskPos].FuncHander = Func;
	ucTaskPos++;
}

static void TaskUpdate(void)
{
    uint8 i;
    for (i=0; i<ucTaskPos; i++)          
    {
         if (stTaskComps[i].Timer)          
        {
            stTaskComps[i].Timer--;          
            if (stTaskComps[i].Timer == 0)        
            {
                 stTaskComps[i].Timer = stTaskComps[i].LvTimer;       
                 stTaskComps[i].Run = 1;           
            }
        }
   }
}

static void TaskProcess(void)
{
    uint8 i;
    for (i=0; i<ucTaskPos; i++)            
    {
         if (stTaskComps[i].Run)            
        {
             stTaskComps[i].FuncHander();         
             stTaskComps[i].Run = 0;           
        }
    }   
}

void SysTick_Handler(void)
{
	TaskUpdate();
	if(ulInc)
	{
		ulInc--; 
	}
}

void Delay_10ms(uint16_t nms)
{
	ulInc = nms;
	while(ulInc);
}

void Hal_SystickInit(void)
{
	SysTick_Config(SystemCoreClock / 100);
	Device.Systick.Register = RegisterSystick;
	Device.Systick.Run = TaskProcess;
	Device.Systick.Delay = Delay_10ms;
}


