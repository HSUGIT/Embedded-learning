#include "r_cg_macrodriver.h"
#include "r_cg_port.h"

uint16_t i;

void MySPI_W_SS(uint8_t BitValue)
{
	P5.0 = BitValue;
}

void MySPI_W_SCK(uint8_t BitValue)
{
	P1.0 = BitValue;
}

void MySPI_W_MOSI(uint8_t BitValue)
{
	P1.2 = ((BitValue == 0) ? 0 : 1);
}

uint8_t MySPI_R_MISO(void)
{
	return (1 & P1.1);
}

void MySPI_Init(void)
{
	R_PORT_Create();
	
	MySPI_W_SS(1);
	MySPI_W_SCK(0);
}

void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i, ByteReceive = 0x00;
	
	for (i = 0; i < 8; i ++)
	{
		MySPI_W_MOSI(ByteSend & (0x80 >> i));
		MySPI_W_SCK(1);
		if (MySPI_R_MISO() == 1){ByteReceive |= (0x80 >> i);}
		MySPI_W_SCK(0);
	}
	
	return ByteReceive;
}
