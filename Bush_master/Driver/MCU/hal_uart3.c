
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

uint8_t  DMA_Rece_Buf[DMA_Rec_Len];	   //DMA���մ������ݻ�����
	
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

	USART_InitStructure.USART_BaudRate            = USART3_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

  	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	
	USART_Cmd(USART3, ENABLE);		/* ʹ�ܴ��� */
	USART_ClearFlag(USART3, USART_FLAG_TC);     
	
	Usart3_NVICConfig();
	
	//��Ӧ��DMA����
	DMA_DeInit(DMA1_Channel3);   //��DMA��ͨ��5�Ĵ�������Ϊȱʡֵ  ����1��Ӧ����DMAͨ��5
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_Rece_Buf;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
	DMA_InitStructure.DMA_BufferSize = DMA_Rec_Len;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	DMA_Cmd(DMA1_Channel3, ENABLE);  //��ʽ����DMA����
	
	Device.Usart5.Send = Usart3_Send;
	Device.Usart5.Register = Register;
}

static void Usart3_NVICConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/* ʹ�ܴ���3�ж� */
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
	uint16_t  Usart3_Rec_Cnt=0;             //��֡���ݳ���	
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





