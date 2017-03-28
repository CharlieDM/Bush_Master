
#include "hal_uart5.h"
#include "stm32f10x_usart.h"
#include "core_hal.h"

#if UART5_FIFO_EN == 1

typedef void (*Rxd5Func)(uint8_t data);

static void Uart5_NVICConfig(void);
static void Uart5_Send(uint8_t *TBuf, uint16_t _usLen);
static void Hal_RS485_IOInit(void);
static void Usart5Dummy(uint8_t data){;}	
static Rxd5Func Rxd5 = Usart5Dummy;

static void Register(void *rxdFucn)
{
    Rxd5 = (Rxd5Func)rxdFucn;
}
	
void Hal_Uart5_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* ����5 TX = PC12   RX = PD2 */
	/* ��1���� ����GPIO��UARTʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	/* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ��3������USART Rx��GPIO����Ϊ��������ģʽ
		����CPU��λ��GPIOȱʡ���Ǹ�������ģʽ���������������費�Ǳ����
		���ǣ��һ��ǽ�����ϱ����Ķ������ҷ�ֹ�����ط��޸���������ߵ����ò���
	*/
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	/* ��4���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate            = UART5_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);
	/*�򿪿����жϣ������յ�һ֡������֮����жϲŷ���*/
	USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);	/* ʹ�ܽ����ж� */
	/*�����������жϾͷ���*/
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(UART5, ENABLE);		/* ʹ�ܴ��� */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(UART5, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
	
	Hal_RS485_IOInit();
	Uart5_NVICConfig();
	
	Device.Usart5.Send = Uart5_Send;
	Device.Usart5.Register = Register;
}

static void Hal_RS485_IOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_RS485_TXEN, ENABLE);	  /* ��GPIOʱ�� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;	/* �������ģʽ */
	GPIO_InitStructure.GPIO_Pin   = PIN_RS485_TXEN;
	GPIO_Init(PORT_RS485_TXEN, &GPIO_InitStructure);
}

static void Uart5_NVICConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/* ʹ�ܴ���5�ж� */
	NVIC_InitStructure.NVIC_IRQChannel            = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void Uart5_Send(uint8_t *TBuf, uint16_t _usLen)
{
	uint8_t i_com=0;

	RS485_TX_EN(); 
	for(i_com=0;i_com<_usLen;i_com++)
	{
		USART_SendData(UART5,TBuf[i_com]);
		while(USART_GetITStatus(UART5, USART_FLAG_TXE) == RESET);
	}
	RS485_RX_EN();
}

void UART5_IRQHandler(void)
{
	uint8_t ch;
	/* ��������ж�  */
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		ch = USART_ReceiveData(UART5);	
		Rxd5(ch);		
	}
}
#endif





