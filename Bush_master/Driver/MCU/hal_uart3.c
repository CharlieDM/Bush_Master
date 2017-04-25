
#include "hal_uart3.h"
#include "stm32f10x_usart.h"
#include "core_hal.h"

#if USART3_FIFO_EN == 1

#define DMA_Rec_Len 200

typedef void (*Rxd3Func)(uint8_t _data);

static void Usart3_NVICConfig(void);
static void Usart3_Send(uint8_t *TBuf, uint16_t _usLen);
static void Usart3Dummy(uint8_t _data){;}	
static Rxd3Func Rxd3 = Usart3Dummy;

	
static void Register(void *rxdFucn)
{
    Rxd3 = (Rxd3Func)rxdFucn;
}
	
void Hal_Usart3_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate            = USART3_BAUD;	/* ²¨ÌØÂÊ */
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

  	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	
	USART_Cmd(USART3, ENABLE);		
	USART_ClearFlag(USART3, USART_FLAG_TC);     
	
	Usart3_NVICConfig();
	
	Device.Usart3.Send = Usart3_Send;
	Device.Usart3.Register = Register;
}

static void Usart3_NVICConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel            = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void Usart3_Send(uint8_t *TBuf, uint16_t _usLen)
{
	uint8_t i_com=0;

	for(i_com=0;i_com<_usLen;i_com++)
	{
		USART_SendData(USART3,TBuf[i_com]);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	}
}

void USART3_IRQHandler(void)
{
	uint8_t ch;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		ch = USART_ReceiveData(USART3);	
		Rxd3(ch);		
	}
}


#endif





