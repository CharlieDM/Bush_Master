
#include "hal_uart3.h"
#include "stm32f10x_usart.h"
#include "core_hal.h"

#if USART3_FIFO_EN == 1

#define DMA_Rec_Len 200

typedef void (*Rxd3Func)(uint8_t *TBuf, uint8_t _usLen);

static void Usart3_NVICConfig(void);
static void Usart3_Send(uint8_t *TBuf, uint16_t _usLen);
static void Usart3Dummy(uint8_t *TBuf, uint8_t _usLen){;}	
static Rxd3Func Rxd3 = Usart3Dummy;

uint8_t  DMA_Rece_Buf[DMA_Rec_Len];	   //DMA接收串口数据缓冲区
	
static void Register(void *rxdFucn)
{
    Rxd3 = (Rxd3Func)rxdFucn;
}
	
void Hal_Usart3_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate            = USART3_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

  	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	
	USART_Cmd(USART3, ENABLE);		/* 使能串口 */
	USART_ClearFlag(USART3, USART_FLAG_TC);     
	
	Usart3_NVICConfig();
	
	//相应的DMA配置
	DMA_DeInit(DMA1_Channel3);   //将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_Rece_Buf;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
	DMA_InitStructure.DMA_BufferSize = DMA_Rec_Len;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	DMA_Cmd(DMA1_Channel3, ENABLE);  //正式驱动DMA传输
	
	Device.Usart5.Send = Usart3_Send;
	Device.Usart5.Register = Register;
}

static void Usart3_NVICConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/* 使能串口3中断 */
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
		while(USART_GetITStatus(USART3, USART_FLAG_TXE) == RESET);
	}
}

void USART3_IRQHandler(void)
{
	uint16_t  Usart3_Rec_Cnt=0;             //本帧数据长度	
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  
	{
		USART_ReceiveData(USART1);
		Usart3_Rec_Cnt = DMA_Rec_Len-DMA_GetCurrDataCounter(DMA1_Channel3);	
		
		Rxd3(DMA_Rece_Buf,Usart3_Rec_Cnt);
		
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);         
		
		DMA_Cmd(DMA1_Channel3, DISABLE );   
		DMA_SetCurrDataCounter(DMA1_Channel3,DMA_Rec_Len); 
		DMA_Cmd(DMA1_Channel3, ENABLE);   
	 } 
}

#endif





