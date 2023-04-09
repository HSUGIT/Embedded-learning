#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

uint8_t RxData;

int main(void)
{
	OLED_Init();
	
	Serial_Init();
	
	while (1)
	{
		OLED_ShowNum(3, 1, 404, 3);
		if (Serial_GetRxFlag() == 1)
		{
			RxData = Serial_GetRxData();
			OLED_ShowHexNum(1, 1, RxData,2);
			//Serial_SendByte(RxData);
		}
		Serial_SendNumber(5,1);
	}
}
