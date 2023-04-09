#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
#include "oled_font.h"

uint8_t  end_oled_tx;
uint8_t  end_oled_rx;
uint8_t rx_buf[20];
#define addr 0x78

void OLED_WriteCommand(uint8_t Command)
{
	uint8_t cmd_buf[2];	
	cmd_buf[0] = 0x00;	
	cmd_buf[1] = Command;	
	R_IIC01_Master_Send(addr, cmd_buf, 2);	
	while(!end_oled_tx);		
	end_oled_tx = 0;		
	R_IIC01_Master_Receive(addr,rx_buf,1);	
	while(!end_oled_rx);	
	end_oled_rx = 0;		
}

void OLED_WriteData(uint8_t Data)
{
	uint8_t dat_buf[2];	
	dat_buf[0] = 0x40;
	dat_buf[1] = Data;
	R_IIC01_Master_Send(addr, dat_buf, 2);	
	while(!end_oled_tx);
	end_oled_tx = 0;
	R_IIC01_Master_Receive(addr,rx_buf,1);
	while(!end_oled_rx);
	end_oled_rx = 0;
}


void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);			
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	
	OLED_WriteCommand(0x00 | (X & 0x0F));		
}


void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}


void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		
	}
}


void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}


uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}


void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}


void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}


void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}


void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

void OLED_Init(void)
{
	uint32_t i, j;
	end_oled_tx = 0;
	end_oled_rx = 0;	
	
	for (i = 0; i < 1000; i++)
	{
		for (j = 0; j < 1000; j++);
	}
	
	R_IIC01_Create();		
	
	OLED_WriteCommand(0xAE);	
	
	OLED_WriteCommand(0xD5);	
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	
	
	OLED_WriteCommand(0xA1);	
	
	OLED_WriteCommand(0xC8);	

	OLED_WriteCommand(0xDA);	
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	

	OLED_WriteCommand(0xA6);	

	OLED_WriteCommand(0x8D);	
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	
		
	OLED_Clear();			
}
