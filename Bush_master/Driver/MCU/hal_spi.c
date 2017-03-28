
#include "hal_spi.h"
#include "core_hal.h"

static void Hal_Spi_Write_Byte_L(uint8_t _ucByte);
static uint8_t Hal_Spi_Read_Byte_L(void);	
static void Hal_Spi_Write_Byte_H(uint8_t _ucByte);
static uint8_t Hal_Spi_Read_Byte_H(void);	

void Hal_Spi_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_SCK | RCC_MOSI | RCC_MISO, ENABLE);	

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Pin = PIN_SCK;
	GPIO_Init(PORT_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_MOSI;
	GPIO_Init(PORT_MOSI, &GPIO_InitStructure);	

	/* SPI片选脚，软件控制 */
  GPIO_InitStructure.GPIO_Pin = PIN_NSS;	
  GPIO_Init(PORT_NSS, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		/* MISO 设置为输入上拉 */
	GPIO_InitStructure.GPIO_Pin = PIN_MISO;
	GPIO_Init(PORT_MISO, &GPIO_InitStructure);
	
	Device.Spi.ReadHigh = Hal_Spi_Read_Byte_H;
	Device.Spi.WriteHigh = Hal_Spi_Write_Byte_H;
	Device.Spi.ReadLow = Hal_Spi_Read_Byte_L;
	Device.Spi.WriteLow = Hal_Spi_Write_Byte_L;
}

static void Hal_Spi_Deay(void)
{
	uint32_t i;

	for (i = 0; i < 5; i++);
}

static void Hal_Spi_Write_Byte_L(uint8_t _ucByte)
{
	uint8_t i;

	for(i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			MOSI_1();
		}
		else
		{
			MOSI_0();
		}
		Hal_Spi_Deay();
		SCK_1();
		_ucByte <<= 1;
		Hal_Spi_Deay();
		SCK_0();
	}
	Hal_Spi_Deay();
}

static uint8_t Hal_Spi_Read_Byte_L(void)
{
	uint8_t read = 0;
	uint8_t i;
	
	for (i = 0; i < 8; i++)
	{
		read = read<<1;

		if (MISO_IS_HIGH())
		{
			read++;
		}
		SCK_1();
		Hal_Spi_Deay();
		SCK_0();
		Hal_Spi_Deay();
	}
  return read;
}

static void Hal_Spi_Write_Byte_H(uint8_t _ucByte)
{
	uint8_t i;

	for(i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			MOSI_1();
		}
		else
		{
			MOSI_0();
		}
		SCK_0();
		_ucByte <<= 1;
		Hal_Spi_Deay();
		SCK_1();				/* SCK上升沿采集数据, SCK空闲时为高电平 */
		Hal_Spi_Deay();
	}
}

static uint8_t Hal_Spi_Read_Byte_H(void)
{
	uint8_t read = 0;
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		SCK_0();
		Hal_Spi_Deay();
		read = read << 1;
		if (MISO_IS_HIGH())
		{
			read++;
		}
		SCK_1();
		Hal_Spi_Deay();
	}
	return read;
}

void Hal_Spi_clk_Set(uint8_t _data)
{
	if (_data == 0)
	{
		SCK_0();
	}
	else
	{
		SCK_1();
	}
}







