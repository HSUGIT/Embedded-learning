#include "stm32f10x.h"                  // Device header
#include "OLED.h"

int main()
{	
	OLED_Init();
	OLED_ShowNum(1, 1, 5, 16);
	OLED_ShowString(2, 1, "STRING");
	OLED_ShowChar(3, 1, 'a');
	OLED_ShowChar(4, 1, 'a');
	while (1)
	{
		
	}
}
